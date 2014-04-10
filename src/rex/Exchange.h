/*
 * Exchange.h
 *
 *  Created on: Aug 29, 2009
 *      Author: ben
 */

#ifndef EXCHANGE_H_
#define EXCHANGE_H_

#include <string>
#include "log4cxx/logger.h"
#include "Exceptions.h"


namespace rex {

class Config;
class FixEngine;
class OrderBookManager;
class IdGenerator;
class OutgoingSessionManager;

class Exchange {
public:
  explicit Exchange(const std::string &homeDir);
  ~Exchange();
  int run();

  const Config *getConfig() const throw (rex::NullPointerException) {
    if(config_)
      return config_;
    else
      throw rex::NullPointerException();
  }

  const FixEngine *getFixEngine() const throw (rex::NullPointerException) {
    if(fixEngine_)
      return fixEngine_;
    else
      throw rex::NullPointerException();
  }

  OrderBookManager *getOrderBookManager() const throw (rex::NullPointerException) {
    if(orderBookManager_)
      return orderBookManager_;
    else
      throw rex::NullPointerException();
  }

  IdGenerator *getIdGenerator() const throw (rex::NullPointerException) {
    if(idGenerator_)
      return idGenerator_;
    else
      throw rex::NullPointerException();
  }

  OutgoingSessionManager *getOutgoingSessionManager() const throw (rex::NullPointerException) {
    if(outgoingSessionManager_)
      return outgoingSessionManager_;
    else
      throw rex::NullPointerException();
  }

private:
  Config *config_;
  FixEngine *fixEngine_;
  OrderBookManager *orderBookManager_;
  IdGenerator *idGenerator_;
  OutgoingSessionManager *outgoingSessionManager_;

  static log4cxx::LoggerPtr logger_;
};

}

#endif /* EXCHANGE_H_ */
