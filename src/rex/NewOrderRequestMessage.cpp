/*
 * NewOrderRequestMessage.cpp
 *
 *  Created on: Dec 7, 2009
 *      Author: ben
 */

#include <boost/shared_ptr.hpp>
#include "quickfix/Session.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/Values.h"
#include "Config.h"
#include "Exchange.h"
#include "FixTypeConverter.h"
#include "FixEngine.h"
#include "FixMessageHandler.h"
#include "NewOrderRequestMessage.h"
#include "ExecutionReportMessage.h"
#include "OrderBook.h"
#include "IdGenerator.h"
#include "OrderBookManager.h"
#include "OutgoingSessionManager.h"

using boost::shared_ptr;

namespace rex {

log4cxx::LoggerPtr NewOrderRequestMessage::logger_(log4cxx::Logger::getLogger("rex.NewOrderRequestMessage"));

std::string NewOrderRequestMessage::getSymbol() const {
  FIX::Symbol symbol;
  fixMsg_.get(symbol);
  return symbol.getValue();
}

std::string NewOrderRequestMessage::getSenderCompId() const {
  FIX::SenderCompID senderCompId;
  fixMsg_.getHeader().get(senderCompId);
  return senderCompId.getValue();
}

std::string NewOrderRequestMessage::getClOrdId() const {
  FIX::ClOrdID clOrdId;
  fixMsg_.get(clOrdId);
  return clOrdId.getValue();
}

std::string NewOrderRequestMessage::getOrigClOrdId() const {
  return "";
}

FIX::Side NewOrderRequestMessage::getSide() const {
  FIX::Side side;
  fixMsg_.get(side);
  return side;
}

void NewOrderRequestMessage::process(OrderBook& orderBook) const {

  LOG4CXX_INFO(logger_, "Handing new order request");

  FIX::SenderCompID senderCompId;
  fixMsg_.getHeader().get(senderCompId);

  FIX::TargetCompID targetCompId;
  fixMsg_.getHeader().get(targetCompId);

  FIX::ClOrdID clOrdId;
  fixMsg_.get(clOrdId);

  FIX::Symbol symbol;
  fixMsg_.get(symbol);

  FIX::Side side;
  fixMsg_.get(side);

  FIX::OrdType orderType;
  fixMsg_.get(orderType);

  FIX::Price price;
  if (orderType == FIX::OrdType_LIMIT)
    fixMsg_.get(price);

  FIX::OrderQty orderQty;
  fixMsg_.get(orderQty);

  FIX::TimeInForce timeInForce(FIX::TimeInForce_DAY);
  if(fixMsg_.isSetField(timeInForce))
    fixMsg_.get(timeInForce);

  // let's do our validations here
  if(!exchange_->getOrderBookManager()->symbolExists(symbol.getString())) {
    LOG4CXX_INFO(logger_, "Symbol " << symbol.getString() << " not found. Rejecting this order.");
    reject("Security not found");
    return;
  }


  shared_ptr<Order> order(new Order(
      exchange_->getIdGenerator()->nextOrderId(),
      clOrdId.getValue(),
      senderCompId.getValue(),
      symbol.getValue(),
      FixTypeConverter::convert(side),
      (long)orderQty,
      price.getValue(),
      FixTypeConverter::convert(orderType),
      FixTypeConverter::convert(timeInForce)));

  // order is accepted, send order confirmation
  LOG4CXX_INFO(logger_, "New order accepted");
  exchange_->getFixEngine()->getFixMessageHandler()->sendOrderResponse(order, Order::ORDER_STATUS_NEW);

  // now match the order
  orderBook.match(order);
  if(order->isOpen()) {
    // order is not completely filled. let's add it to the order book
    orderBook.add(order);
  }
}

void NewOrderRequestMessage::reject(const std::string &reason) const {
  LOG4CXX_INFO(logger_, "Sending an order reject response");

  FIX42::ExecutionReport execReport(
    FIX::OrderID(getClOrdId()),
    FIX::ExecID(exchange_->getIdGenerator()->nextExecutionId()),
    FIX::ExecTransType(FIX::ExecTransType_NEW),
    FIX::ExecType(FIX::ExecType_REJECTED),
    FIX::OrdStatus(FIX::ExecType_REJECTED),
    FIX::Symbol(getSymbol()), getSide(), FIX::LeavesQty(0), FIX::CumQty(0), FIX::AvgPx(0));

  execReport.set(FIX::ClOrdID(getClOrdId()));
  execReport.set(FIX::Text(reason));

  ExecutionReportMessage *msg = new ExecutionReportMessage(exchange_, execReport);
  if(exchange_->getOutgoingSessionManager()->send(msg, getSenderCompId()) == -1) {
    delete msg;
    msg = 0;
  }
}

} // namespace rex
