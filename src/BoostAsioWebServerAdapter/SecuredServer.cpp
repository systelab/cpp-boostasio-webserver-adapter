#include "SecuredServer.h"

#include "SecuredContext.h"
#include "Agents/RequestParserAgent.h"
#include "Connections/SecuredConnection.h"
#include "Services/ReplyBufferBuilderService.h"
#include "Services/RequestHandlingService.h"
#include "Services/RequestURIParserService.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/SecuredServerCredentials.h"

#include <boost/asio/ssl.hpp>
#include <openssl/x509v3.h>
#include <openssl/pem.h>


namespace systelab { namespace web_server { namespace boostasio {

	SecuredServer::SecuredServer(const Configuration& configuration)
		:BaseServer(configuration)
	{
		m_context.reset(new SecuredContext(m_io_service));
	}

	SecuredServer::~SecuredServer() = default;

	void SecuredServer::setServerCredentials(std::unique_ptr<SecuredServerCredentials> serverCredentials)
	{
		bool certificateResult = m_context->setServerCertificate(serverCredentials->getCertificate());
		bool privateKeyResult = m_context->setServerPrivateKey(serverCredentials->getPrivateKey());
		bool dhParamResult = m_context->setServerDHParam(serverCredentials->getDHParam());
		bool result = certificateResult && privateKeyResult && dhParamResult;

		if(!result)
		{
			throw Exception("Error setting server credentials");
		}
	}

	void SecuredServer::setClientCertificate(const std::string& clientCertifcate)
	{
		bool result = m_context->setClientCertificate(clientCertifcate);
		if (!result)
		{
			throw Exception("Error setting client credentials");
		}
	}

	void SecuredServer::setConfiguration(std::unique_ptr<Configuration> configuration)
	{
		BaseServer::setConfiguration(std::move(configuration));
	}
	
	void SecuredServer::registerWebService(std::unique_ptr<IWebService> webService)
	{
		BaseServer::registerWebService(std::move(webService));
	}

	bool SecuredServer::isRunning() const
	{
		return BaseServer::isRunning();
	}

	void SecuredServer::start()
	{
		BaseServer::start();
	}

	void SecuredServer::stop()
	{
		BaseServer::stop();
	}

	boost::shared_ptr<IConnection> SecuredServer::buildConnection()
	{
		auto& corsConfiguration = m_configuration->getCORSConfiguration();

		auto requestParserAgent = std::make_unique<RequestParserAgent>();
		auto requestURIParserService = std::make_unique<RequestURIParserService>();
		auto requestHandlingService = std::make_unique<RequestHandlingService>(*m_webServicesMgr, corsConfiguration);
		auto replyBufferBuilderService = std::make_unique<ReplyBufferBuilderService>();

		return boost::shared_ptr<IConnection>(
					new SecuredConnection(m_io_service, *(m_context.get()), std::move(requestParserAgent),
										  std::move(requestURIParserService), std::move(requestHandlingService),
										  std::move(replyBufferBuilderService)) );
	}

}}}
