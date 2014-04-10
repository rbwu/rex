/*
 * CancelOrderRejectMessage.h
 *
 *  Created on: Jan 25, 2010
 *      Author: ben
 */

#ifndef CANCELORDERREJECTMESSAGE_H_
#define CANCELORDERREJECTMESSAGE_H_

#include <string>
#include "log4cxx/logger.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "Message.h"

namespace rex {

class Exchange;

class CancelOrderRejectMessage : public Message {
public:
  CancelOrderRejectMessage(Exchange *exchange, const FIX42::OrderCancelReject &fixMsg);
  virtual ~CancelOrderRejectMessage();
  virtual void process(OrderBook &orderBook) const {};
  virtual void reject(const std::string &reason) const {};
  virtual void send(const std::string &targetCompId);
  virtual std::string getSymbol() const { return ""; };
  virtual std::string getSenderCompId() const { return ""; };
  virtual std::string getClOrdId() const { return ""; };
  virtual std::string getOrigClOrdId() const { return ""; };
  virtual std::string getId() const { return ""; };

private:
  Exchange *exchange_;
  FIX42::OrderCancelReject fixMsg_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* CANCELORDERREJECTMESSAGE_H_ */
