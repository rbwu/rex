/*
 * Cheetah.cpp
 *
 *  Created on: Aug 29, 2009
 *      Author: ben
 */

// std includes
#include <string>
#include <exception>
#include "log4cxx/propertyconfigurator.h"
#include "Exceptions.h"
#include "Exchange.h"
#include "Config.h"
#include "FixEngine.h"
#include "IdGenerator.h"
#include "OrderBookManager.h"
#include "OutgoingSessionManager.h"


namespace rex {

log4cxx::LoggerPtr Exchange::logger_(log4cxx::Logger::getLogger("rex.Exchange"));

Exchange::Exchange(const std::string &homeDir) {
  config_ = new Config(homeDir);
  fixEngine_ = new FixEngine(this);
  orderBookManager_ = new OrderBookManager(this);
  idGenerator_ = new IdGenerator();
  outgoingSessionManager_ = new OutgoingSessionManager();
}

Exchange::~Exchange() {
  delete config_;
  config_ = 0;

  delete fixEngine_;
  fixEngine_ = 0;

  delete orderBookManager_;
  orderBookManager_ = 0;

  delete idGenerator_;
  idGenerator_ = 0;

  delete outgoingSessionManager_;
  outgoingSessionManager_ = 0;
}

int Exchange::run() {
  config_->load();
  log4cxx::PropertyConfigurator::configure(config_->getLogConfigFile().c_str());

  try {
    LOG4CXX_INFO(logger_, "Starting Rex");
    LOG4CXX_INFO(logger_, "Using REX_HOME " << config_->getHomeDir());

    orderBookManager_->init(config_->getSecurities());
    fixEngine_->start(config_->getFixConfigFile());

    while (true) {
      std::string value;
      std::cin >> value;
      if( value == "quit" )
        break;
      else if(value == "display") {
        orderBookManager_->display();
      }
      else
        std::cout << "Please enter a valid command" << std::endl;
      std::cout << std::endl;
    }

    fixEngine_->stop();
    orderBookManager_->fini();
    outgoingSessionManager_->fini();
  } catch(const RexException &e) {
    LOG4CXX_ERROR(logger_, "RexException thrown: " << e.what());
    return -1;
  } catch(const std::exception &e) {
    LOG4CXX_ERROR(logger_, "STD Exception thrown :" << e.what());
    return -1;
  }
  return 0;
}


}
