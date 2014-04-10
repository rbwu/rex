/*
 * CancelReplaceOrderRequestMessage.h
 *
 *  Created on: Jan 5, 2010
 *      Author: ben
 */

#ifndef CANCELREPLACEORDERREQUESTMESSAGE_H_
#define CANCELREPLACEORDERREQUESTMESSAGE_H_

#include "log4cxx/logger.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "Message.h"

namespace rex {

class Exchange;

class CancelReplaceOrderRequestMessage: public Message {
public:
public:
  CancelReplaceOrderRequestMessage(Exchange *exchange, const FIX42::OrderCancelReplaceRequest &fixMsg)
    : exchange_(exchange), fixMsg_(fixMsg) {}
  virtual ~CancelReplaceOrderRequestMessage() {};

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
  FIX42::OrderCancelReplaceRequest fixMsg_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* CANCELREPLACEORDERREQUESTMESSAGE_H_ */
