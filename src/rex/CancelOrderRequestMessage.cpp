/*
 * CancelOrderRequestMessage.cpp
 *
 *  Created on: Dec 8, 2009
 *      Author: ben
 */

#include <boost/shared_ptr.hpp>
#include "quickfix/Session.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "CancelOrderRequestMessage.h"
#include "CancelOrderRejectMessage.h"
#include "Config.h"
#include "Exchange.h"
#include "FixEngine.h"
#include "FixMessageHandler.h"
#include "FixTypeConverter.h"
#include "Order.h"
#include "OrderBook.h"
#include "CancelOrderRejectMessage.h"
#include "OutgoingSessionManager.h"

using boost::shared_ptr;

namespace rex {

log4cxx::LoggerPtr CancelOrderRequestMessage::logger_(log4cxx::Logger::getLogger("rex.CancelOrderRequestMessage"));

std::string CancelOrderRequestMessage::getSymbol() const {
  FIX::Symbol symbol;
  fixMsg_.get(symbol);
  return symbol.getValue();
}

std::string CancelOrderRequestMessage::getSenderCompId() const {
  FIX::SenderCompID senderCompId;
  fixMsg_.getHeader().get(senderCompId);
  return senderCompId.getValue();
}


std::string CancelOrderRequestMessage::getClOrdId() const {
  FIX::ClOrdID clOrdId;
  fixMsg_.get(clOrdId);
  return clOrdId.getValue();
}

std::string CancelOrderRequestMessage::getOrigClOrdId() const {
  FIX::OrigClOrdID origClOrdId;
  fixMsg_.get(origClOrdId);
  return origClOrdId.getValue();
}

std::string CancelOrderRequestMessage::getOrderId() const {
  FIX::OrderID orderId;
  fixMsg_.get(orderId);
  return orderId.getValue();
}

void CancelOrderRequestMessage::process(OrderBook &orderBook) const {
  FixMessageHandler *handler = exchange_->getFixEngine()->getFixMessageHandler();

  FIX::Side side;
  fixMsg_.get(side);

  shared_ptr<Order> removedOrder = orderBook.remove(FixTypeConverter::convert(side), getOrigClOrdId(), getSenderCompId());
  if(removedOrder.get() != 0) {
    // update clOrdId and issue cancel response
    removedOrder->setClOrdId(getClOrdId());
    shared_ptr<Order> orderCopy(removedOrder->clone());
    handler->sendOrderResponse(orderCopy, Order::ORDER_STATUS_CANCELED);
  }
  else {
    // issue reject response
    reject("Order not found.");
  }
}

void CancelOrderRequestMessage::reject(const std::string &reason) const {
  LOG4CXX_INFO(logger_, "Sending an order cancel reject response");

  FIX42::OrderCancelReject cancelReject(
    FIX::OrderID(getOrderId()),
    FIX::ClOrdID(getClOrdId()),
    FIX::OrigClOrdID(getOrigClOrdId()),
    FIX::OrdStatus(FIX::OrdStatus_REJECTED),
    FIX::CxlRejResponseTo_ORDER_CANCEL_REQUEST);

  CancelOrderRejectMessage *msg = new CancelOrderRejectMessage(exchange_, cancelReject);
  if(exchange_->getOutgoingSessionManager()->send(msg, getSenderCompId()) == -1) {
    delete msg;
    msg = 0;
  }
}

}
