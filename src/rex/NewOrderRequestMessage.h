/*
 * NewOrderRequestMessage.h
 *
 *  Created on: Dec 7, 2009
 *      Author: ben
 */

#ifndef NEWORDERREQUEST_H_
#define NEWORDERREQUEST_H_

#include <string>
#include "log4cxx/logger.h"
#include "quickfix/fix42/NewOrderSingle.h"

#include "Message.h"


namespace rex {

class Exchange;

class NewOrderRequestMessage: public Message {
public:
  NewOrderRequestMessage(Exchange *exchange, const FIX42::NewOrderSingle &fixMsg)
    : exchange_(exchange), fixMsg_(fixMsg) {};
  virtual ~NewOrderRequestMessage() {};

  virtual void process(OrderBook &orderBook) const;
  virtual void reject(const std::string &reason) const;
  virtual void send(const std::string &targetCompId) {}
  virtual std::string getSymbol() const;
  virtual std::string getSenderCompId() const;
  virtual std::string getClOrdId() const;
  virtual std::string getOrigClOrdId() const;
  FIX::Side getSide() const;

private:
  Exchange *exchange_;
  FIX42::NewOrderSingle fixMsg_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* NEWORDERREQUEST_H_ */
