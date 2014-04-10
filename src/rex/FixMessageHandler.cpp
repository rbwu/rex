#include <exception>

#include <boost/shared_ptr.hpp>
#include "quickfix/Session.h"
#include "quickfix/fix42/ExecutionReport.h"
#include "quickfix/fix42/OrderCancelReject.h"
#include "CancelOrderRequestMessage.h"
#include "CancelReplaceOrderRequestMessage.h"
#include "Config.h"
#include "IdGenerator.h"
#include "FixMessageHandler.h"
#include "Order.h"
#include "OrderBookManager.h"
#include "Exceptions.h"
#include "Exchange.h"
#include "FixTypeConverter.h"
#include "NewOrderRequestMessage.h"
#include "ExecutionReportMessage.h"
#include "OutgoingSessionManager.h"


using boost::shared_ptr;

namespace rex {

log4cxx::LoggerPtr FixMessageHandler::logger_(log4cxx::Logger::getLogger("rex.FixMessageHandler"));

FixMessageHandler::FixMessageHandler(Exchange *exchange)
  : exchange_(exchange) {
}

FixMessageHandler::~FixMessageHandler() {
}

void FixMessageHandler::onLogon(const FIX::SessionID &sessionId) {
  LOG4CXX_INFO(logger_, "Received a Logon request from session: " << sessionId);
  exchange_->getOutgoingSessionManager()->createSession(sessionId.getTargetCompID());
}

void FixMessageHandler::onLogout(const FIX::SessionID &sessionId) {
  LOG4CXX_INFO(logger_, "Received a Logout request from session: " << sessionId);
  exchange_->getOutgoingSessionManager()->removeSession(sessionId.getTargetCompID());
}

void FixMessageHandler::fromApp(
  const FIX::Message &message,
  const FIX::SessionID &sessionId)
  throw(
    FIX::FieldNotFound,
    FIX::IncorrectDataFormat,
    FIX::IncorrectTagValue,
    FIX::UnsupportedMessageType) {
  try {
    crack(message, sessionId);
  } catch(const RexException &e) {
    LOG4CXX_ERROR(logger_, "RexException thrown: " << e.what());
  } catch(const std::exception &e) {
    LOG4CXX_ERROR(logger_, "STD Exception thrown :" << e.what());
  }
}

void FixMessageHandler::onMessage(
  const FIX42::NewOrderSingle &message,
  const FIX::SessionID &sessionId) {

  LOG4CXX_INFO(logger_, "Received a NewOrderSingle message froms session: " << sessionId);

  OrderBookManager *orderBookManager = exchange_->getOrderBookManager();
  Message *req = new NewOrderRequestMessage(exchange_, message);
  if(orderBookManager->process(req) == -1) {
    req->reject("Rejected");
    delete req;
    req = 0;
  }
}

void FixMessageHandler::onMessage(const FIX42::OrderCancelRequest &message, const FIX::SessionID &sessionId) {

  LOG4CXX_INFO(logger_, "Received an OrderCancelRequest message from session: " << sessionId);

  OrderBookManager *orderBookManager = exchange_->getOrderBookManager();
  Message *req = new CancelOrderRequestMessage(exchange_, message);
  if(orderBookManager->process(req) == -1) {
    req->reject("Rejected");
    delete req;
    req = 0;
  }
}

void FixMessageHandler::onMessage(const FIX42::OrderCancelReplaceRequest &message, const FIX::SessionID &sessionId) {

  LOG4CXX_INFO(logger_, "Received an OrderCancelReplaceRequest message from session: " << sessionId);

  OrderBookManager *orderBookManager = exchange_->getOrderBookManager();
  Message *req = new CancelReplaceOrderRequestMessage(exchange_, message);
  if(orderBookManager->process(req) == -1) {
    req->reject("Rejected");
    delete req;
    req = 0;
  }
}

void FixMessageHandler::sendOrderResponse(boost::shared_ptr<Order> order, Order::Status status) {

  LOG4CXX_INFO(logger_, "Sending an order response: orderId=" << order->getOrderId());

  FIX::TargetCompID targetCompId(order->getSenderCompId());
  FIX::SenderCompID senderCompId(exchange_->getConfig()->getSelfCompId());

  FIX::OrdStatus execReportStatus = FixTypeConverter::convert(status);

  FIX42::ExecutionReport execReport(
    FIX::OrderID(order->getOrderId()),
    FIX::ExecID (exchange_->getIdGenerator()->nextExecutionId()),
    FIX::ExecTransType(FIX::ExecTransType_NEW), // This shall always be New unless it is an order status report
    FIX::ExecType(execReportStatus),
    FIX::OrdStatus(execReportStatus),
    FIX::Symbol(order->getSymbol()),
    FIX::Side(FixTypeConverter::convert(order->getSide())),
    FIX::LeavesQty (order->getOpenQuantity()),
    FIX::CumQty (order->getExecutedQuantity()),
    FIX::AvgPx (order->getAverageExecutedPrice()));

  execReport.set(FIX::ClOrdID(order->getClOrdId()));
  execReport.set(FIX::OrderQty(order->getQuantity()));

  if(execReportStatus == FIX::OrdStatus_FILLED || status == FIX::OrdStatus_PARTIALLY_FILLED ) {
    execReport.set(FIX::LastShares(order->getLastExecutedQuantity()));
    execReport.set(FIX::LastPx(order->getLastExecutedPrice()));
  }

  ExecutionReportMessage *msg = new ExecutionReportMessage(exchange_, execReport);
  if(exchange_->getOutgoingSessionManager()->send(msg, order->getSenderCompId()) ==  -1) {
    delete msg;
    msg = 0;
  }
}

}
