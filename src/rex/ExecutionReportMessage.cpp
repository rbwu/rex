/*
 * ExecutionReportMessage.cpp
 *
 *  Created on: Jan 26, 2010
 *      Author: ben
 */

#include "quickfix/Session.h"
#include "Config.h"
#include "ExecutionReportMessage.h"
#include "Exchange.h"

namespace rex {

log4cxx::LoggerPtr ExecutionReportMessage::logger_(log4cxx::Logger::getLogger("rex.ExecutionReportMessage"));

ExecutionReportMessage::ExecutionReportMessage(Exchange *exchange, const FIX42::ExecutionReport &fixMsg)
  : exchange_(exchange),
    fixMsg_(fixMsg) {
}

ExecutionReportMessage::~ExecutionReportMessage() {
}

void ExecutionReportMessage::send(const std::string &targetCompId) {
  LOG4CXX_INFO(logger_, "Sending a ExecutionReport message to " << targetCompId)
  try {
    FIX::Session::sendToTarget(fixMsg_, exchange_->getConfig()->getSelfCompId(), targetCompId);
  }
  catch(FIX::SessionNotFound&) {
    LOG4CXX_ERROR(logger_, "FIX session not found! Unable to send the ExecutionReport message to " << targetCompId)
  }
}


}
