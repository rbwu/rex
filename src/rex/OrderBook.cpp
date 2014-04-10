/*
 * OrderBook.cpp
 *
 *  Created on: Sep 8, 2009
 *      Author: ben
 */

#include <exception>

#include "OrderBook.h"
#include "FixEngine.h"
#include "FixMessageHandler.h"
#include "Exchange.h"
#include "Message.h"
#include "Exceptions.h"

using std::string;
using boost::shared_ptr;

namespace rex {

log4cxx::LoggerPtr OrderBook::logger_(log4cxx::Logger::getLogger("rex.OrderBook"));

OrderBook::OrderBook(const std::string &symbol, Exchange *exchange)
  : symbol_(symbol), exchange_(exchange) {
}

OrderBook::~OrderBook() {
}

void OrderBook::match(boost::shared_ptr<Order> newOrder) {
  if(newOrder->getSide() == Order::SIDE_BUY) {
    while (true) {
      if (askOrders_.empty() || !newOrder->isOpen())
        return;

      AskOrders::iterator it = askOrders_.begin();
      shared_ptr<Order> bookOrder = it->second;
      if(newOrder->getPrice() < bookOrder->getPrice())
        return;
      else {
        match(newOrder, bookOrder);
        if(!bookOrder->isOpen()) {
          askOrders_.erase(it);
        }
      }
    }
  }
  else {
    while (true) {
      if (bidOrders_.empty() || !newOrder->isOpen())
        return;

      BidOrders::iterator it = bidOrders_.begin();
      shared_ptr<Order> bookOrder = it->second;
      if(newOrder->getPrice() > bookOrder->getPrice())
        return;
      else {
        match(newOrder, bookOrder);
        if(!bookOrder->isOpen()) {
          bidOrders_.erase(it);
        }
      }
    }
  }
}

/**
 * Adds an order to the order book
 *
 * The match function must be called first prior to calling this
 */
void OrderBook::add(boost::shared_ptr<Order> newOrder) {
  if(newOrder->isOpen()) {
    if(newOrder->getSide() == Order::SIDE_BUY) {
      bidOrders_.insert(BidOrders::value_type(newOrder->getPrice(), newOrder));
    }
    else {
      askOrders_.insert(AskOrders::value_type(newOrder->getPrice(), newOrder));
    }
  }
}

void OrderBook::match(boost::shared_ptr<Order> newOrder, boost::shared_ptr<Order> bookOrder) {
  if(newOrder->getSide() == bookOrder->getSide())
    return;
  else {
    if((newOrder->getSide() == Order::SIDE_BUY && newOrder->getPrice() >= bookOrder->getPrice()) ||
       (newOrder->getSide() == Order::SIDE_SELL && newOrder->getPrice() <= bookOrder->getPrice())) {

      double fillPrice = bookOrder->getPrice();
      long fillQuantity = (newOrder->getOpenQuantity() < bookOrder->getOpenQuantity()) ?
          newOrder->getOpenQuantity() : bookOrder->getOpenQuantity();
      bookOrder->fill(fillPrice, fillQuantity);
      newOrder->fill(fillPrice, fillQuantity);

      bookOrder->log();
      newOrder->log();

      FixMessageHandler *handler = exchange_->getFixEngine()->getFixMessageHandler();

      Order::Status status1 = bookOrder->getExecutedQuantity() < bookOrder->getOpenQuantity()
        ? Order::ORDER_STATUS_PARTIALLY_FILLED : Order::ORDER_STATUS_FILLED;
      shared_ptr<Order> order1(bookOrder->clone());
      handler->sendOrderResponse(order1, status1);

      Order::Status status2 = newOrder->getExecutedQuantity() < newOrder->getOpenQuantity()
        ? Order::ORDER_STATUS_PARTIALLY_FILLED : Order::ORDER_STATUS_FILLED;
      shared_ptr<Order> order2(newOrder->clone());
      handler->sendOrderResponse(order2, status2);
    }
  }
}

boost::shared_ptr<Order> OrderBook::remove(Order::Side side, const std::string &orderId) {
  shared_ptr<Order> removedOrder;
  if(side == Order::SIDE_BUY) {
    for(BidOrders::iterator it = bidOrders_.begin(); it != bidOrders_.end(); ++it) {
      if(it->second->getOrderId() == orderId) {
        removedOrder = it->second;
        bidOrders_.erase(it);
        break;
      }
    }
  }
  else {
    for(AskOrders::iterator it = askOrders_.begin(); it != askOrders_.end(); ++it) {
      if(it->second->getOrderId() == orderId) {
        removedOrder = it->second;
        askOrders_.erase(it);
        break;
      }
    }
  }

  return removedOrder;
}

boost::shared_ptr<Order> OrderBook::remove(Order::Side side, const std::string &clOrdId, const std::string &senderCompId) {
  shared_ptr<Order> removedOrder;
  if(side == Order::SIDE_BUY) {
    for(BidOrders::iterator it = bidOrders_.begin(); it != bidOrders_.end(); ++it) {
      if(it->second->getClOrdId() == clOrdId && it->second->getSenderCompId() == senderCompId) {
        removedOrder = it->second;
        bidOrders_.erase(it);
        break;
      }
    }
  }
  else {
    for(AskOrders::iterator it = askOrders_.begin(); it != askOrders_.end(); ++it) {
      if(it->second->getClOrdId() == clOrdId && it->second->getSenderCompId() == senderCompId) {
        removedOrder = it->second;
        askOrders_.erase(it);
        break;
      }
    }
  }

  return removedOrder;
}

void OrderBook::display() const {
  LOG4CXX_INFO(logger_, "Displaying order book for " << symbol_);
  for(BidOrders::const_iterator it = bidOrders_.begin(); it != bidOrders_.end(); ++it) {
    LOG4CXX_INFO(logger_, "symbol=" << symbol_ << ", side=bid, price="
        << it->second->getPrice() << ", quantity=" << it->second->getOpenQuantity());
  }

  for(AskOrders::const_iterator it = askOrders_.begin(); it != askOrders_.end(); ++it) {
    LOG4CXX_INFO(logger_, "symbol=" << symbol_ << ", side=ask, price="
        << it->second->getPrice() << ", quantity=" << it->second->getOpenQuantity());
  }
}

int OrderBook::svc() {
  LOG4CXX_INFO(logger_, "OrderBook thread now running for " << symbol_);
  for (ACE_Message_Block *mblk; getq (mblk) != -1; ) {
    try {
      Message *msg = reinterpret_cast<Message *> (mblk->rd_ptr ());
      mblk->release();
      msg->process(*this);
      delete msg;
      msg = 0;
    } catch(const RexException &e) {
      LOG4CXX_ERROR(logger_, "RexException thrown: " << e.what());
    } catch(const std::exception &e) {
      LOG4CXX_ERROR(logger_, "STD Exception thrown :" << e.what());
    }
  }
  LOG4CXX_INFO(logger_, "OrderBook thread exited for " << symbol_);
  return 0;
}

}
