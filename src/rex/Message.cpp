/*
 * Message.cpp
 *
 *  Created on: Dec 7, 2009
 *      Author: ben
 */

#include <sstream>
#include "Message.h"

namespace rex {

std::string Message::getId() const {
  std::stringstream stream;
  stream << this->getSenderCompId() << "-" << this->getClOrdId();
  return stream.str();
}

}
