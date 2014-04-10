/*
 * Config.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: ben
 */

#include <string>
#include "pugixml/pugixml.hpp"
#include "Config.h"

using std::string;
using pugi::xml_document;
using pugi::xml_parse_result;
using pugi::xml_node;

namespace rex {

Config::Config(const std::string &homeDir)
  : homeDir_(homeDir) {
}

Config::~Config() {
}

void Config::load() throw (ConfigException) {
  xml_document doc;
  if(doc.load_file(getConfigFile().c_str())) {
    logConfigFile_ = homeDir_ + "/" + doc.first_element_by_path("exchange/log").attribute("configFile").value();
    fixConfigFile_ = homeDir_ + "/" + doc.first_element_by_path("exchange/fix").attribute("configFile").value();
    selfCompId_ = doc.first_element_by_path("exchange/self/targetCompId").attribute("name").value();
    xml_node securitiesNode = doc.first_element_by_path("exchange/securities");
    for(xml_node securityNode = securitiesNode.child("security"); securityNode; securityNode = securityNode.next_sibling("security")) {
      string symbol = securityNode.attribute("symbol").value();
      securities_.push_back(symbol);
    }
  }
  else
    throw ConfigException("Config file not found.");
}

}
