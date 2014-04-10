/*
 * OutgoingSessionManager.h
 *
 *  Created on: Jan 29, 2010
 *      Author: ben
 */

#ifndef OUTGOINGSESSIONMANAGER_H_
#define OUTGOINGSESSIONMANAGER_H_

#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "ace/Thread_Mutex.h"
#include "ace/Guard_T.h"
#include "log4cxx/logger.h"


namespace rex {

class Message;
class OutgoingSession;

class OutgoingSessionManager {
public:
  typedef boost::shared_ptr<OutgoingSession> OutgoingSessionPtr;
  typedef std::map<std::string, OutgoingSessionPtr> OutgoingSessions;

public:
  OutgoingSessionManager();
  virtual ~OutgoingSessionManager();
  int send(Message *message, const std::string &targetCompId);
  int createSession(const std::string &targetCompId);
  int removeSession(const std::string &targetCompId);
  void fini();

private:
  OutgoingSessions outgoingSessions_;
  static log4cxx::LoggerPtr logger_;
  ACE_Thread_Mutex mutex_;
};

}

#endif /* OUTGOINGSESSIONMANAGER_H_ */
