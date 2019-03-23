#include "SecuredServer.h"

#include "SecuredContext.h"
#include "Agents/RequestParserAgent.h"
#include "Connections/SecuredConnection.h"
#include "Services/ReplyBufferBuilderService.h"
#include "Services/RequestHandlingService.h"
#include "Services/RequestURIParserService.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"

#include <boost/asio/ssl.hpp>
#include <openssl/x509v3.h>
#include <openssl/pem.h>


namespace systelab { namespace web_server { namespace boostasio {

	SecuredServer::SecuredServer(const Configuration& configuration)
		:BaseServer(configuration)
	{
		m_context = std::make_unique<SecuredContext>(m_io_service);

		const SecurityConfiguration& securityConfiguration = configuration.getSecurityConfiguration();
		setHTTPSConfiguration(securityConfiguration);
		setMutualSSLConfiguration(securityConfiguration);
	}

	SecuredServer::~SecuredServer() = default;

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

	void SecuredServer::setHTTPSConfiguration(const SecurityConfiguration& securityConfiguration)
	{
		bool certificateResult = m_context->setServerCertificate(securityConfiguration.getServerCertificate());
		bool privateKeyResult = m_context->setServerPrivateKey(securityConfiguration.getServerPrivateKey());
		bool dhParamResult = m_context->setServerDHParam(securityConfiguration.getServerDHParam());
		bool result = certificateResult && privateKeyResult && dhParamResult;

		if (!result)
		{
			throw Exception("Error setting server credentials");
		}
	}

	void SecuredServer::setMutualSSLConfiguration(const SecurityConfiguration& securityConfiguration)
	{
		if (securityConfiguration.isMutualSSLEnabled())
		{
			bool result = m_context->setClientCertificate(securityConfiguration.getClientCertificate());
			if (!result)
			{
				throw Exception("Error setting client credentials");
			}
		}
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
