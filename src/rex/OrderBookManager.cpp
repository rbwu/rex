/*
 * OrderBookManager.cpp
 *
 *  Created on: Nov 28, 2009
 *      Author: ben
 */

#include <exception>
#include "ace/Message_Block.h"
#include "OrderBookManager.h"
#include "Message.h"
#include "OrderBook.h"
#include "Exchange.h"
#include "Exceptions.h"

using std::string;

namespace rex {

log4cxx::LoggerPtr OrderBookManager::logger_(log4cxx::Logger::getLogger("rex.OrderBookManager"));

OrderBookManager::OrderBookManager(Exchange *exchange)
  : orderBooks_(), exchange_(exchange){
}

OrderBookManager::~OrderBookManager() {
}

void OrderBookManager::init(const std::vector<std::string> &securities) {
  LOG4CXX_INFO(logger_, "Initializing Order Book Manager");
  for(std::vector<std::string>::const_iterator it = securities.begin(); it != securities.end(); ++it) {
    LOG4CXX_INFO(logger_, "Creating order book for " << *it);
    OrderBookPtr book(new OrderBook(*it, exchange_));
    if(book->open() == 0) {
      if(!orderBooks_.insert(OrderBooks::value_type(book->getSymbol(), book)).second) {
        LOG4CXX_ERROR(logger_, "Error creating an order book: Security already exists: symbol=" << book->getSymbol());
      }
    }
    else {
      LOG4CXX_ERROR(logger_, "Error creating an order book for "<< book->getSymbol() << ": Unable to activate a new thread.");
    }
  }
}

/**
 * Returns 0 on success, -1 on failure. Caller is responsible to deallocate message if -1 is returned.
 */
int OrderBookManager::process(Message *message) {
  string symbol = message->getSymbol();
  OrderBooks::iterator it = orderBooks_.find(symbol);
  if(it == orderBooks_.end()) {
    LOG4CXX_ERROR(logger_, "Security not found.");
    return -1;
  }
  else {
    ACE_Message_Block *mblk = new ACE_Message_Block(reinterpret_cast<char *>(message));
    if(it->second->put(mblk) == -1) {
      mblk->release();
      LOG4CXX_ERROR(logger_, "Failed to enqueue message.");
      return -1;
    }
  }
  return 0;
}

void OrderBookManager::display() const {
  for(OrderBooks::const_iterator it = orderBooks_.begin(); it != orderBooks_.end(); ++it) {
    it->second->display();
  }
}

bool OrderBookManager::symbolExists(const std::string &symbol) const {
  return (orderBooks_.find(symbol) != orderBooks_.end());
}

void OrderBookManager::fini() {
  for(OrderBooks::iterator it = orderBooks_.begin(); it != orderBooks_.end(); ++it) {
    it->second->flush();
    it->second->wait();
    it->second->close();
  }
}

}
