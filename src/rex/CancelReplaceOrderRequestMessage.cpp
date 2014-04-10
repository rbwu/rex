/*
 * CancelReplaceOrderRequestMessage.cpp
 *
 *  Created on: Jan 5, 2010
 *      Author: ben
 */
#include <boost/shared_ptr.hpp>
#include "quickfix/Session.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "Config.h"
#include "CancelReplaceOrderRequestMessage.h"
#include "CancelOrderRejectMessage.h"
#include "Exchange.h"
#include "FixEngine.h"
#include "FixMessageHandler.h"
#include "FixTypeConverter.h"
#include "Order.h"
#include "OrderBook.h"
#include "OutgoingSessionManager.h"

using boost::shared_ptr;

namespace rex {

log4cxx::LoggerPtr CancelReplaceOrderRequestMessage::logger_(log4cxx::Logger::getLogger("rex.CancelReplaceOrderRequestMessage"));

std::string CancelReplaceOrderRequestMessage::getSymbol() const {
  FIX::Symbol symbol;
  fixMsg_.get(symbol);
  return symbol.getValue();
}

std::string CancelReplaceOrderRequestMessage::getSenderCompId() const {
  FIX::SenderCompID senderCompId;
  fixMsg_.getHeader().get(senderCompId);
  return senderCompId.getValue();
}


std::string CancelReplaceOrderRequestMessage::getClOrdId() const {
  FIX::ClOrdID clOrdId;
  fixMsg_.get(clOrdId);
  return clOrdId.getValue();
}

std::string CancelReplaceOrderRequestMessage::getOrigClOrdId() const {
  FIX::OrigClOrdID origClOrdId;
  fixMsg_.get(origClOrdId);
  return origClOrdId.getValue();
}

std::string CancelReplaceOrderRequestMessage::getOrderId() const {
  FIX::OrderID orderId;
  fixMsg_.get(orderId);
  return orderId.getValue();
}

void CancelReplaceOrderRequestMessage::process(OrderBook &orderBook) const {
  FixMessageHandler *handler = exchange_->getFixEngine()->getFixMessageHandler();

  FIX::Side side;
  fixMsg_.get(side);

  shared_ptr<Order> order = orderBook.remove(FixTypeConverter::convert(side), getOrigClOrdId(), getSenderCompId());
  if(order.get() != 0) {
    FIX::Price price;
    fixMsg_.get(price);

    FIX::OrderQty orderQty;
    fixMsg_.get(orderQty);

    /* In this implementation, we are modifying open quantity,
     * there's a possibility of in-flight fill
     */
    order->setPrice(price);
    order->setOpenQuantity(static_cast<long>(orderQty.getValue()));
    order->setClOrdId(getClOrdId());
    shared_ptr<Order> orderCopy(order->clone());
    handler->sendOrderResponse(orderCopy, Order::ORDER_STATUS_REPLACED);

    // now match the order
    orderBook.match(order);
    if(order->isOpen()) {
      // order is not completely filled. let's add it to the order book
      orderBook.add(order);
    }
  }
  else {
    // issue reject response
    reject("Order not found.");
  }
}

void CancelReplaceOrderRequestMessage::reject(const std::string &reason) const {
  LOG4CXX_INFO(logger_, "Sending an order replace reject response");

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
