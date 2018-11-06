#include <iostream>
#include "Connection.h"
#include "Model/Configuration.h"
#include "WebServer.h"

int main(int argc, char **argv) {
  std::cout << "Hello World!" << std::endl;
  std::cout << "\t->" << argv[0] << "<-" << std::endl;

  // Web server
  std::string hostAddress = "127.0.0.1";
  unsigned int port = 8080;
  unsigned int threadPoolSize = 5;
  systelab::web_server::Configuration webServerConfiguration(hostAddress, port,
                                                             threadPoolSize);
  std::unique_ptr<systelab::web_server::IWebServer> webServer(
      new systelab::web_server::WebServer(webServerConfiguration));

  return 0;
}
