
#ifndef FIXMESSAGEHANDLER_H
#define FIXMESSAGEHANDLER_H

#include <queue>
#include <iostream>

#include <boost/shared_ptr.hpp>

#include "quickfix/Application.h"
#include "quickfix/MessageCracker.h"
#include "quickfix/Values.h"
#include "quickfix/Utility.h"
#include "quickfix/Mutex.h"
#include "quickfix/fix42/NewOrderSingle.h"
#include "quickfix/fix42/OrderCancelRequest.h"
#include "quickfix/fix42/OrderCancelReplaceRequest.h"
#include "quickfix/fix42/MarketDataRequest.h"
#include "quickfix/fix43/MarketDataRequest.h"

#include "log4cxx/logger.h"

#include "Order.h"

namespace rex {

class Exchange;
class NewOrderRequestMessage;
class CancelOrderRequestMessage;

class FixMessageHandler : public FIX::Application, public FIX::MessageCracker {
public:
  FixMessageHandler(Exchange *exchange);
  virtual ~FixMessageHandler();

  void sendOrderResponse(boost::shared_ptr<Order> order, Order::Status status);
  //void sendNewOrderRejectResponse(const NewOrderRequestMessage &req, const std::string &reason);
  //void sendCancelOrderRejectResponse(const CancelOrderRequestMessage &req, const std::string &reason);

private:
  // FIX::Application overloads
  virtual void onCreate(const FIX::SessionID &) {};
  virtual void onLogon(const FIX::SessionID &sessionID);
  virtual void onLogout(const FIX::SessionID &sessionID);
  void toAdmin(FIX::Message &, const FIX::SessionID &) {}
  void toApp(FIX::Message &, const FIX::SessionID &) throw(FIX::DoNotSend) {}
  void fromAdmin(const FIX::Message &, const FIX::SessionID &)
    throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::RejectLogon) {}
  void fromApp(const FIX::Message &message, const FIX::SessionID &sessionID)
    throw(FIX::FieldNotFound, FIX::IncorrectDataFormat, FIX::IncorrectTagValue, FIX::UnsupportedMessageType);

  // MessageCracker overloads
  void onMessage(const FIX42::NewOrderSingle &, const FIX::SessionID &);
  void onMessage(const FIX42::OrderCancelRequest &, const FIX::SessionID &);
  void onMessage(const FIX42::OrderCancelReplaceRequest &, const FIX::SessionID &);
  void onMessage(const FIX42::MarketDataRequest &, const FIX::SessionID &) {}
  void onMessage(const FIX43::MarketDataRequest &, const FIX::SessionID &) {}

private:
  Exchange *exchange_;
  static log4cxx::LoggerPtr logger_;
};

}

#endif
