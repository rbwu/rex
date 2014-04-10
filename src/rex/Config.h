/*
 * Configurator.h
 *
 *  Created on: Nov 22, 2009
 *      Author: ben
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <vector>
#include "Exceptions.h"

namespace rex {

class Config {
public:
  explicit Config(const std::string &homeDir);
  ~Config();

  void load() throw (ConfigException);

  const std::string &getHomeDir() const {
    return homeDir_;
  }

  const std::string getConfigFile() const {
    // the config file is hard coded.
    return homeDir_ + "/config/config.xml";
  }

  const std::string &getFixConfigFile() const {
    return fixConfigFile_;
  }

  const std::string &getLogConfigFile() const {
    return logConfigFile_;
  }

  const std::string &getSelfCompId() const {
    return selfCompId_;
  }

  const std::vector<std::string> &getSecurities() const {
    return securities_;
  }

private:
  std::string homeDir_;
  std::string configFile_;
  std::string fixConfigFile_;
  std::string logConfigFile_;
  std::string selfCompId_;

  std::vector<std::string> securities_;

};

}

#endif /* CONFIG_H_ */
