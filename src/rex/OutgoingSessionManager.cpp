/*
 * OutgoingSessionManager.cpp
 *
 *  Created on: Jan 29, 2010
 *      Author: ben
 */

#include <utility>

#include "OutgoingSessionManager.h"
#include "OutgoingSession.h"
#include "Message.h"

namespace rex {

log4cxx::LoggerPtr OutgoingSessionManager::logger_(log4cxx::Logger::getLogger("rex.OutgoingSessionManager"));

OutgoingSessionManager::OutgoingSessionManager() {

}

OutgoingSessionManager::~OutgoingSessionManager() {

}

int OutgoingSessionManager::send(Message *message, const std::string &targetCompId) {
  ACE_Guard<ACE_Thread_Mutex> guard(mutex_);
  OutgoingSessions::iterator it = outgoingSessions_.find(targetCompId);
  if(it != outgoingSessions_.end()) {
    ACE_Message_Block *mblk = new ACE_Message_Block(reinterpret_cast<char *>(message));
    if(it->second->put(mblk) == -1) {
      mblk->release();
      LOG4CXX_ERROR(logger_, "Failed to enqueue message.");
      return -1;
    }
    else
      return 0;
  }

  LOG4CXX_ERROR(logger_, "Unable to find the outgoing session. Send failed");
  return -1;
}

int OutgoingSessionManager::createSession(const std::string& targetCompId) {
  ACE_Guard<ACE_Thread_Mutex> guard(mutex_);
  OutgoingSessions::iterator it = outgoingSessions_.find(targetCompId);
  if(it == outgoingSessions_.end()) {
    OutgoingSessionPtr session(new OutgoingSession(targetCompId));
    if(session->open() == 0) {
      std::pair<OutgoingSessions::iterator, bool> result = outgoingSessions_.insert(OutgoingSessions::value_type(targetCompId, session));
      if(result.second) {
        return 0;
      }
      else {
        LOG4CXX_ERROR(logger_, "Error creating an outgoing session for " << targetCompId);
        return -1;
      }
    }
    else {
      LOG4CXX_ERROR(logger_, "Error creating an outgoing session for "<< targetCompId << ": Unable to activate a new thread.");
      return -1;
    }
  }
  else
    return 0;
}


int OutgoingSessionManager::removeSession(const std::string &targetCompId) {
  ACE_Guard<ACE_Thread_Mutex> guard(mutex_);
  OutgoingSessions::iterator it = outgoingSessions_.find(targetCompId);
  if(it == outgoingSessions_.end()) {
    LOG4CXX_ERROR(logger_, "Outgoing session for " << targetCompId << " not found.");
  }
  else {
    it->second->flush();
    it->second->wait();
    it->second->close();
    outgoingSessions_.erase(it);
  }

  return 0;
}

void OutgoingSessionManager::fini() {
  ACE_Guard<ACE_Thread_Mutex> guard(mutex_);
  for(OutgoingSessions::iterator it = outgoingSessions_.begin(); it != outgoingSessions_.end(); ++it) {
    it->second->flush();
    it->second->wait();
    it->second->close();
  }
}

}
