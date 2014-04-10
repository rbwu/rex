/*
 * FixEngine.cpp
 *
 *  Created on: Nov 26, 2009
 *      Author: ben
 */

#include <string>
#include <iostream>
#include <fstream>
#include "quickfix/FileStore.h"
#include "quickfix/SocketAcceptor.h"
#include "quickfix/ThreadedSocketAcceptor.h"
#include "quickfix/SessionSettings.h"
#include "FixEngine.h"
#include "FixMessageHandler.h"
#include "Exchange.h"
#include "OrderBookManager.h"

namespace rex {

log4cxx::LoggerPtr FixEngine::logger_(log4cxx::Logger::getLogger("rex.FixEngine"));

FixEngine::FixEngine(Exchange *exchange)
  : exchange_(exchange), handler_(0), acceptor_(0), settings_(0), storeFactory_(0), logFactory_(0) {
}

FixEngine::~FixEngine() {
  delete handler_;
  handler_ = 0;

  delete acceptor_;
  acceptor_ = 0;

  delete settings_;
  settings_ = 0;

  delete storeFactory_;
  storeFactory_ = 0;

  delete logFactory_;
  logFactory_ = 0;
}

void FixEngine::start(const std::string &fixConfigFile) {
  LOG4CXX_INFO(logger_, "Starting Fix Engine");
  handler_ = new FixMessageHandler(exchange_);
  settings_ = new FIX::SessionSettings(fixConfigFile);
  storeFactory_ = new FIX::FileStoreFactory(*settings_);
  logFactory_ = new FIX::ScreenLogFactory(*settings_);
  acceptor_ = new FIX::ThreadedSocketAcceptor(*handler_, *storeFactory_, *settings_, *logFactory_);
  acceptor_->start();
}

void FixEngine::stop() {
  acceptor_->stop();
}

}
