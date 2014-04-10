/*
 * Exceptions.h
 *
 *  Created on: Jan 4, 2010
 *      Author: ben
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <stdexcept>
#include <string>

namespace rex {

class RexException : public std::runtime_error {
public:
  explicit RexException(const std::string &s) : std::runtime_error(s) {}
};

class SecurityNotFoundException : public RexException {
public:
  explicit SecurityNotFoundException(const std::string &s) : RexException(s) {}
};

class ConfigException : public RexException {
public:
  explicit ConfigException(const std::string &s) : RexException(s) {}
};

class EnqueueMessageException : public RexException {
public:
  explicit EnqueueMessageException(const std::string &s) : RexException(s) {}
};

class NullPointerException : public RexException {
public:
  explicit NullPointerException(const std::string &s = "Null pointer") : RexException(s) {}
};

}


#endif /* EXCEPTIONS_H_ */


