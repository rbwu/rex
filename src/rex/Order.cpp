/*
 * OrderBase.cpp
 *
 *  Created on: Aug 31, 2009
 *      Author: ben
 */

#include "Order.h"

namespace rex {

log4cxx::LoggerPtr Order::logger_(log4cxx::Logger::getLogger("rex.Order"));

Order:: Order(
    const std::string &orderId,
    const std::string &clOrdId,
    const std::string &senderCompId,
    const std::string &symbol,
    Side side,
    long quantity,
    double price,
    Type type,
    TimeInForce timeInForce)
  : orderId_(orderId),
    clOrdId_(clOrdId),
    senderCompId_(senderCompId),
    symbol_(symbol),
    side_(side),
    quantity_(quantity),
    price_(price),
    type_(type),
    timeInForce_(timeInForce),
    stopPrice_(0),
    openQuantity_(quantity),
    executedQuantity_(0),
    averageExecutedPrice_(0),
    lastExecutedPrice_(0),
    lastExecutedQuantity_(0) {
}

void Order::setQuantity(long quantity) {
  if(quantity <= executedQuantity_) {
    quantity_ = executedQuantity_;
    openQuantity_ = 0;
  }
  else {
    quantity_ = quantity;
    openQuantity_ = quantity_ - executedQuantity_;
  }
}

void Order::fill(double price, long quantity) {
  averageExecutedPrice_ = ((quantity * price) + (averageExecutedPrice_ * executedQuantity_))
                          / (quantity + executedQuantity_);

  openQuantity_ -= quantity;
  executedQuantity_ += quantity;
  lastExecutedPrice_ = price;
  lastExecutedQuantity_ = quantity;
}

void Order::log() const {
  LOG4CXX_INFO(logger_, "Order data: orderId=" << orderId_ << ", clOrdId=" << clOrdId_ << ", senderCompId=" << senderCompId_
        << ", symbol=" << symbol_ << ", side=" << side_ << ", quantity=" << quantity_ << ", price=" << price_
        << ", timeInForce=" << timeInForce_ << ", openQuantity=" << openQuantity_ << ", executedQuantity=" << executedQuantity_
        << ", lastExecutedPrice=" << lastExecutedPrice_ << ", lastExecutedQuantity=" << lastExecutedQuantity_);
}



}


