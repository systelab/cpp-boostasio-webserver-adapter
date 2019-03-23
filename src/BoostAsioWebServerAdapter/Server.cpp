#include "Server.h"

#include "Agents/RequestParserAgent.h"
#include "Connections/Connection.h"
#include "Services/ReplyBufferBuilderService.h"
#include "Services/RequestHandlingService.h"
#include "Services/RequestURIParserService.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"

#include <vector>

#include <boost/shared_ptr.hpp>


namespace systelab { namespace web_server { namespace boostasio {

	Server::Server(const Configuration& configuration)
		:BaseServer(configuration)
	{
	}

	Server::~Server() = default;

	void Server::setConfiguration(std::unique_ptr<Configuration> configuration)
	{
		BaseServer::setConfiguration(std::move(configuration));
	}

	void Server::registerWebService(std::unique_ptr<IWebService> webService)
	{
		BaseServer::registerWebService(std::move(webService));
	}

	bool Server::isRunning() const
	{
		return BaseServer::isRunning();
	}

	void Server::start()
	{
		BaseServer::start();
	}

	void Server::stop()
	{
		BaseServer::stop();
	}

	boost::shared_ptr<IConnection> Server::buildConnection()
	{
		auto& corsConfiguration = m_configuration->getCORSConfiguration();

		auto requestParserAgent = std::make_unique<RequestParserAgent>();
		auto requestURIParserService = std::make_unique<RequestURIParserService>();
		auto requestHandlingService = std::make_unique<RequestHandlingService>(*m_webServicesMgr, corsConfiguration);
		auto replyBufferBuilderService = std::make_unique<ReplyBufferBuilderService>();

		return boost::shared_ptr<IConnection>(
			new Connection(m_io_service, std::move(requestParserAgent), std::move(requestURIParserService),
						   std::move(requestHandlingService), std::move(replyBufferBuilderService)) );
	}

}}}
