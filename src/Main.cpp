//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description :
//============================================================================


#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>

#include "rex/Exchange.h"
#include "rex/Exceptions.h"

using rex::Exchange;
using rex::RexException;

int main(int argc, char **argv) {
  try {
    std::string homeDir;
    char *rexHome = getenv("REX_HOME");
    if(rexHome != 0) {
      homeDir = rexHome;
    }
    else {
      char currentPath[MAXPATHLEN];
      getcwd(currentPath, MAXPATHLEN);
      homeDir = currentPath;
    }

    Exchange exchange(homeDir);
    return exchange.run();
  } catch(const RexException &e) {
    std::cerr << "RexException thrown: " << e.what() << std::endl;
    return -1;
  } catch(const std::exception &e) {
    std::cerr << "STD Exception thrown :" << e.what() << std::endl;
    return -1;
  }
}
