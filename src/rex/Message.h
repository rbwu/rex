/*
 * Message.h
 *
 *  Created on: Dec 7, 2009
 *      Author: ben
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>

namespace rex {

class OrderBook;

class Message {
public:
  virtual ~Message() {};
  /**
   * Processes an incoming message
   */
  virtual void process(OrderBook &orderBook) const = 0;

  /**
   * Rejects an incoming message
   */
  virtual void reject(const std::string &reason) const = 0;

  /**
   * Sends an outgoing message
   */
  virtual void send(const std::string &targetCompId) = 0;
  virtual std::string getSymbol() const = 0;
  virtual std::string getSenderCompId() const = 0;
  virtual std::string getClOrdId() const = 0;
  virtual std::string getOrigClOrdId() const = 0;
  virtual std::string getId() const;
};

}

#endif /* MESSAGE_H_ */
