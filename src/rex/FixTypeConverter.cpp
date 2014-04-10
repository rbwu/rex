/*
 * FixTypeConverter.cpp
 *
 *  Created on: Dec 14, 2009
 *      Author: ben
 */

#include <exception>
#include "quickfix/Values.h"
#include "FixTypeConverter.h"

namespace rex {

Order::Side FixTypeConverter::convert(const FIX::Side &side) {
  switch(side) {
  case FIX::Side_BUY:
    return Order::SIDE_BUY;
  case FIX::Side_SELL:
    return Order::SIDE_SELL;
  default:
    throw std::logic_error("Unsupported Side, use buy or sell");
  }
}

FIX::Side FixTypeConverter::convert(Order::Side side) {
  switch(side) {
  case Order::SIDE_BUY:
    return FIX::Side(FIX::Side_BUY);
  case Order::SIDE_SELL:
    return FIX::Side(FIX::Side_SELL);
  default:
    throw std::logic_error("Unsupported Side, use buy or sell");
  }
}


Order::Type FixTypeConverter::convert(const FIX::OrdType &ordType) {
  switch(ordType) {
  case FIX::OrdType_LIMIT:
    return Order::ORDER_TYPE_LIMIT;
  default:
    throw std::logic_error("Unsupported Order Type, use limit");
  }
}

FIX::OrdType FixTypeConverter::convert(Order::Type type) {
  switch (type) {
  case Order::ORDER_TYPE_LIMIT:
    return FIX::OrdType(FIX::OrdType_LIMIT);
  default:
    throw std::logic_error("Unsupported Order Type, use limit");
  }
}


Order::TimeInForce FixTypeConverter::convert(const FIX::TimeInForce &timeInForce) {
  switch(timeInForce) {
  case FIX::TimeInForce_DAY:
    return Order::TIF_DAY;
  default:
    throw std::logic_error("Unsupported time in force, use day");
  }
}

FIX::TimeInForce FixTypeConverter::convert(Order::TimeInForce timeInForce) {
  switch (timeInForce) {
  case Order::TIF_DAY:
    return FIX::TimeInForce(FIX::TimeInForce_DAY);
  default:
    throw std::logic_error("Unsupported time in force, use day");
  }
}

FIX::OrdStatus FixTypeConverter::convert(Order::Status status) {
  switch(status) {
  case Order::ORDER_STATUS_NEW:
    return FIX::OrdStatus(FIX::OrdStatus_NEW);
  case Order::ORDER_STATUS_PARTIALLY_FILLED:
    return FIX::OrdStatus(FIX::OrdStatus_PARTIALLY_FILLED);
  case Order::ORDER_STATUS_FILLED:
    return FIX::OrdStatus(FIX::OrdStatus_FILLED);
  case Order::ORDER_STATUS_CANCELED:
    return FIX::OrdStatus(FIX::OrdStatus_CANCELED);
  case Order::ORDER_STATUS_REPLACED:
    return FIX::OrdStatus(FIX::OrdStatus_REPLACED);
  case Order::ORDER_STATUS_REJECTED:
    return FIX::OrdStatus(FIX::OrdStatus_REJECTED);
  case Order::ORDER_STATUS_EXPIRED:
    return FIX::OrdStatus(FIX::OrdStatus_EXPIRED);
  default:
    throw std::logic_error("Invalid order status");
  }
}


}
