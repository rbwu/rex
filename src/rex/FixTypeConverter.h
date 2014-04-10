/*
 * FixTypeConverter.h
 *
 *  Created on: Dec 14, 2009
 *      Author: ben
 */

#ifndef FIXTYPECONVERTER_H_
#define FIXTYPECONVERTER_H_

#include "quickfix/Fields.h"
#include "Order.h"

namespace rex {

struct FixTypeConverter {
public:
  // Type conversions
  static Order::Side convert(const FIX::Side &side);
  static FIX::Side convert(Order::Side side);

  static Order::Type convert(const FIX::OrdType &ordType);
  static FIX::OrdType convert(Order::Type type);

  static Order::Status convert(const FIX::OrdStatus &ordStatus);
  static FIX::OrdStatus convert(Order::Status status);

  static Order::TimeInForce convert(const FIX::TimeInForce &timeInForce);
  static FIX::TimeInForce convert(Order::TimeInForce timeInForce);

  static FIX::ExecType convert(Order::ExecutionType &execType);
};

}

#endif /* FIXTYPECONVERTER_H_ */
