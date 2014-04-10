/*
 * FixEngine.h
 *
 *  Created on: Nov 26, 2009
 *      Author: ben
 */

#ifndef FIXENGINE_H_
#define FIXENGINE_H_

#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/ThreadedSocketAcceptor.h"
#include "quickfix/SessionSettings.h"
#include "log4cxx/logger.h"


namespace rex {

class Exchange;
class FixMessageHandler;

class FixEngine {
public:
  FixEngine(Exchange *exchange);
  ~FixEngine();

  void start(const std::string &fixConfigFile);
  void stop();


  Exchange *getExchange() const {
    return exchange_;
  }

  FixMessageHandler *getFixMessageHandler() const {
    return handler_;
  }

private:
  Exchange *exchange_;
  FixMessageHandler *handler_;
  FIX::ThreadedSocketAcceptor *acceptor_;
  FIX::SessionSettings *settings_;
  FIX::FileStoreFactory *storeFactory_;
  FIX::ScreenLogFactory *logFactory_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif /* FIXENGINE_H_ */
