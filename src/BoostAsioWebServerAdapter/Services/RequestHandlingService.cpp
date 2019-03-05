#include "RequestHandlingService.h"

#include "Model/WebServicesMgr.h"

#include "IWebService.h"
#include "Model/CORSConfiguration.h"
#include "Model/Reply.h"
#include "Model/Request.h"

namespace systelab {
namespace web_server {

RequestHandlingService::RequestHandlingService(
    WebServicesMgr& webServicesMgr, CORSConfiguration& corsConfiguration)
    : m_webServicesMgr(webServicesMgr),
      m_corsConfiguration(corsConfiguration) {}

RequestHandlingService::~RequestHandlingService() {}

std::unique_ptr<Reply> RequestHandlingService::processRequest(
    const Request& request) const {
  unsigned nWebServices = m_webServicesMgr.getWebServiceCount();
  for (unsigned int i = 0; i < nWebServices; i++) {
    const IWebService* webService = m_webServicesMgr.getWebService(i);
    std::unique_ptr<Reply> reply = webService->process(request);
    if (reply) {
      addCORSHeaders(request, *reply);
      return std::move(reply);
    }
  }

  std::unique_ptr<Reply> notFoundReply(new Reply());
  notFoundReply->setStatus(Reply::NOT_FOUND);
  return std::move(notFoundReply);
}

void RequestHandlingService::addCORSHeaders(const Request& request,
                                            Reply& reply) const {
  if (m_corsConfiguration.isEnabled() &&
      request.getHeaders().hasHeader("Origin")) {
    std::string origin = request.getHeaders().getHeader("Origin");
    if (m_corsConfiguration.isAllowedOrigin(origin)) {
      reply.addHeader("Access-Control-Allow-Origin", origin);
    }
  }
}

}  // namespace web_server
}  // namespace systelab
