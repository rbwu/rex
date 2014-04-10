/*
 * IdGenerator.h
 *
 *  Created on: Nov 20, 2009
 *      Author: ben
 */

#ifndef IDGENERATOR_H_
#define IDGENERATOR_H_

#include <string>
#include <sstream>

namespace rex {

class IdGenerator {
public:
  IdGenerator() : orderId(0), executionId(0) {}
  ~IdGenerator() {}

  std::string nextOrderId() {
    std::stringstream stream;
    stream << ++orderId;
    return stream.str();
  }

  std::string nextExecutionId() {
    std::stringstream stream;
    stream << ++executionId;
    return stream.str();
  }

private:
  long orderId;
  long executionId;
};

}
#endif /* IDGENERATOR_H_ */
