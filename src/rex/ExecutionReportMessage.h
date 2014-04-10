/*
 * ExecutionReportMessage.h
 *
 *  Created on: Jan 26, 2010
 *      Author: ben
 */

#ifndef EXECUTIONREPORTMESSAGE_H_
#define EXECUTIONREPORTMESSAGE_H_

#include <string>
#include "log4cxx/logger.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "Message.h"

namespace rex {

class Exchange;

class ExecutionReportMessage : public Message {
public:
  ExecutionReportMessage(Exchange *exchange, const FIX42::ExecutionReport &fixMsg);
  virtual ~ExecutionReportMessage();

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
  FIX42::ExecutionReport fixMsg_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* EXECUTIONREPORTMESSAGE_H_ */
