/*
 * CancelOrderRejectMessage.cpp
 *
 *  Created on: Jan 25, 2010
 *      Author: ben
 */

#include "quickfix/Session.h"

#include "CancelOrderRejectMessage.h"
#include "Exchange.h"
#include "Config.h"

namespace rex {

log4cxx::LoggerPtr CancelOrderRejectMessage::logger_(log4cxx::Logger::getLogger("rex.CancelOrderRejectMessage"));


CancelOrderRejectMessage::CancelOrderRejectMessage(Exchange *exchange, const FIX42::OrderCancelReject &fixMsg)
  : exchange_(exchange),
    fixMsg_(fixMsg) {
}

CancelOrderRejectMessage::~CancelOrderRejectMessage() {
}

void CancelOrderRejectMessage::send(const std::string &targetCompId) {
  LOG4CXX_INFO(logger_, "Sending a CancelOrderReject message to " << targetCompId);
  try {
    FIX::Session::sendToTarget(fixMsg_, exchange_->getConfig()->getSelfCompId(), targetCompId);
  }
  catch(FIX::SessionNotFound&) {
    LOG4CXX_ERROR(logger_, "FIX session not found! Unable to send the CancelOrderReject message to " << targetCompId)
  }
}

}
