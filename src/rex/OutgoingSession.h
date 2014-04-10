/*
 * OutgoingSession.h
 *
 *  Created on: Jan 16, 2010
 *      Author: ben
 */

#ifndef OUTGOINGSESSION_H_
#define OUTGOINGSESSION_H_

#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Message_Block.h"
#include "log4cxx/logger.h"

namespace rex {

class Message;

class OutgoingSession : public ACE_Task<ACE_MT_SYNCH>  {
public:
  enum { MAX_THREADS = 1 };

public:
  OutgoingSession(const std::string &targetCompId);
  virtual ~OutgoingSession();
  virtual int open(void * = 0) {
    return activate (THR_NEW_LWP, MAX_THREADS);
  }
  virtual int put(ACE_Message_Block *mblk, ACE_Time_Value *timeout = 0) {
    return putq(mblk, timeout);
  }
  virtual int svc();
private:
  std::string targetCompId_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* OUTGOINGSESSION_H_ */
