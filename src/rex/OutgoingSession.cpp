/*
 * OutgoingSession.cpp
 *
 *  Created on: Jan 16, 2010
 *      Author: ben
 */

#include <exception>

#include "OutgoingSession.h"
#include "Exceptions.h"
#include "Message.h"

namespace rex {

log4cxx::LoggerPtr OutgoingSession::logger_(log4cxx::Logger::getLogger("rex.OutgoingSession"));


OutgoingSession::OutgoingSession(const std::string &targetCompId)
  : targetCompId_(targetCompId) {

}

OutgoingSession::~OutgoingSession() {
}

int OutgoingSession::svc() {
  LOG4CXX_INFO(logger_, "OutgoingSession thread running for " << targetCompId_);
  for (ACE_Message_Block *mblk; getq (mblk) != -1; ) {
    Message *msg = reinterpret_cast<Message *> (mblk->rd_ptr ());
    mblk->release();
    if(msg) {
      msg->send(targetCompId_);
      delete msg;
      msg = 0;
    }
    else
      LOG4CXX_ERROR(logger_, "msg is a null pointer!");
  }
  LOG4CXX_INFO(logger_, "OutgoingSession thread exited for " << targetCompId_);
  return 0;
}

}
