/*
 * OrderBase.h
 *
 *  Created on: Aug 31, 2009
 *      Author: ben
 */

#ifndef ORDER_H_
#define ORDER_H_

#include <string>

#include "log4cxx/logger.h"


namespace rex {

class Order {
public:

  // Note enum names must be unique within the same class scope,
  // so we are prefixing them to avoid name collision
  enum Side {
    SIDE_BUY = 1,
    SIDE_SELL
  };
  enum Type {
    ORDER_TYPE_LIMIT = 1
  };
  enum TimeInForce {
    TIF_DAY = 1
  };
  // Identifies status of an order
  enum Status {
    ORDER_STATUS_NEW = 1,
    ORDER_STATUS_PARTIALLY_FILLED,
    ORDER_STATUS_FILLED,
    ORDER_STATUS_CANCELED,
    ORDER_STATUS_REPLACED,
    ORDER_STATUS_REJECTED,
    ORDER_STATUS_EXPIRED
   };
  // Identifies the nature of an execution report
  enum ExecutionType {
    EXEC_TYPE_NEW = 1,
    EXEC_TYPE_PARTIAL_FILL,
    EXEC_TYPE_FILL,
    EXEC_TYPE_CANCELED,
    EXEC_TYPE_REPLACE,
    EXEC_TYPE_REJECTED,
    EXEC_TYPE_EXPIRED
  };

public:
  Order(
    const std::string &orderId,
    const std::string &clOrdId,
    const std::string &senderCompId,
    const std::string &symbol,
    Side side,
    long quantity,
    double price,
    Type type,
    TimeInForce timeInForce);

  ~Order() {};

  const std::string &getOrderId() const { return orderId_; }
  const std::string &getClOrdId() const { return clOrdId_; }
  const std::string &getSenderCompId() const { return senderCompId_; }
  const std::string &getSymbol() const { return symbol_; }
  Side getSide() const { return side_; }
  long getQuantity() const { return quantity_; }
  double getPrice() const { return price_; }
  Type getType() const { return type_; }
  TimeInForce getTimeInForce() const { return timeInForce_; }

  long getOpenQuantity() const { return openQuantity_; }
  long getExecutedQuantity() const { return executedQuantity_; }
  double getAverageExecutedPrice() const { return averageExecutedPrice_; }
  double getLastExecutedPrice() const { return lastExecutedPrice_; }
  long getLastExecutedQuantity() const { return lastExecutedQuantity_; }
  bool isOpen() const { return openQuantity_ > 0; }
  void setStopPrice(double stopPrice) { stopPrice_ = stopPrice; }
  void setClOrdId(const std::string &clOrdId) { clOrdId_ = clOrdId; }
  void setPrice(double price) { price_ = price; }
  void setQuantity(long quantity);
  void setOpenQuantity(long openQuantity) { openQuantity_ = openQuantity; }

  void fill(double price, long quantity);

  void log() const;

  Order *clone() {
    return new Order(*this);
  }

private:
  std::string orderId_;
  std::string clOrdId_;
  std::string senderCompId_;
  std::string symbol_;
	Side side_;
	long quantity_;
	double price_;
	Type type_;
	TimeInForce timeInForce_;

	double stopPrice_;

	long openQuantity_;
	long executedQuantity_;
	double averageExecutedPrice_;
	double lastExecutedPrice_;
	long lastExecutedQuantity_;

  static log4cxx::LoggerPtr logger_;
};

}

#endif /* ORDER_H_ */
