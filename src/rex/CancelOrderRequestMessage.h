/*
 * CancelOrderRequestMessage.h
 *
 *  Created on: Dec 8, 2009
 *      Author: ben
 */

#ifndef CANCELORDERREQUEST_H_
#define CANCELORDERREQUEST_H_

#include <string>
#include "log4cxx/logger.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "Message.h"

namespace rex {

class Exchange;

class CancelOrderRequestMessage: public Message {
public:
  CancelOrderRequestMessage(Exchange *exchange, const FIX42::OrderCancelRequest &fixMsg)
    : exchange_(exchange), fixMsg_(fixMsg) {}
  virtual ~CancelOrderRequestMessage() {};

  virtual void process(OrderBook &orderBook) const;
  virtual void reject(const std::string &reason) const;
  virtual void send(const std::string &targetCompId) {}
  virtual std::string getSymbol() const;
  virtual std::string getSenderCompId() const;
  virtual std::string getClOrdId() const;
  std::string getOrigClOrdId() const;
  std::string getOrderId() const;

private:
  Exchange *exchange_;
  FIX42::OrderCancelRequest fixMsg_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* CANCELORDERREQUEST_H_ */
