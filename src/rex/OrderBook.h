/*
 * OrderBook.h
 *
 *  Created on: Sep 8, 2009
 *      Author: ben
 */

#ifndef ORDERBOOK_H_
#define ORDERBOOK_H_

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "log4cxx/logger.h"
#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Block.h"

#include "Order.h"

namespace rex {

class Exchange;
class Message;

class OrderBook : public ACE_Task<ACE_MT_SYNCH> {
public:
  enum { MAX_THREADS = 1 };

private:
  typedef std::multimap<double, boost::shared_ptr<Order>, std::greater<double> > BidOrders;
  typedef std::multimap<double, boost::shared_ptr<Order>, std::less<double> > AskOrders;

public:
  explicit OrderBook(const std::string &symbol, Exchange *exchange = 0);
  ~OrderBook();

  const std::string &getSymbol() const { return symbol_; }
  void setExchange(Exchange *exchange) { exchange_ = exchange; }
  void match(boost::shared_ptr<Order> newOrder);
  void add(boost::shared_ptr<Order> newOrder);
  boost::shared_ptr<Order> remove(Order::Side side, const std::string &orderId);
  boost::shared_ptr<Order> remove(Order::Side side, const std::string &clOrdId, const std::string &senderCompId);
  void display() const;

  // ACE_Task overrides
  virtual int open(void * = 0) {
    return activate (THR_NEW_LWP, MAX_THREADS);
  }

  virtual int put(ACE_Message_Block *mblk, ACE_Time_Value *timeout = 0) {
    return putq(mblk, timeout);
  }

  virtual int svc();
private:
  void match(boost::shared_ptr<Order> newOrder, boost::shared_ptr<Order> bookOrder);

private:
  std::string symbol_;
  BidOrders bidOrders_;
  AskOrders askOrders_;
  Exchange *exchange_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* ORDERBOOK_H_ */
