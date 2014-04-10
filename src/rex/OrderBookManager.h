/*
 * OrderBookManager.h
 *
 *  Created on: Nov 28, 2009
 *      Author: ben
 */

#ifndef ORDERBOOKMANAGER_H_
#define ORDERBOOKMANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "log4cxx/logger.h"

namespace rex {

class Exchange;
class Message;
class OrderBook;

class OrderBookManager {
public:
  typedef boost::shared_ptr<OrderBook> OrderBookPtr;
  typedef std::map<std::string, OrderBookPtr> OrderBooks;

public:
  OrderBookManager(Exchange *exchange);
  ~OrderBookManager();

  void init(const std::vector<std::string> &securities);
  void fini();
  int process(Message *message);

  void display() const;

  bool symbolExists(const std::string &symbol) const;

private:
  OrderBooks orderBooks_;
  Exchange* exchange_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* ORDERBOOKMANAGER_H_ */
