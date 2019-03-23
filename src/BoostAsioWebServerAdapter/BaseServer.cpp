#include "BaseServer.h"

#include "Agents/RequestParserAgent.h"
#include "Connections/IConnection.h"
#include "Model/WebServicesMgr.h"
#include "Services/ReplyBufferBuilderService.h"
#include "Services/RequestHandlingService.h"
#include "Services/RequestURIParserService.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/CORSConfiguration.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include <vector>

#include <boost/shared_ptr.hpp>


namespace systelab { namespace web_server { namespace boostasio {

	BaseServer::BaseServer(const Configuration& configuration)
		:m_configuration(new Configuration(configuration))
		,m_webServicesMgr(new WebServicesMgr())
		,m_acceptor(m_io_service)
		,m_running(false)
		,m_newConnection()
	{
	}

	BaseServer::~BaseServer()
	{
		stop();
	}

	void BaseServer::registerWebService(std::unique_ptr<IWebService> webService)
	{
		m_webServicesMgr->addWebService(std::move(webService));
	}

	bool BaseServer::isRunning() const
	{
		return m_running;
	}

	void BaseServer::start()
	{
		openAcceptor();

		unsigned int threadPoolSize = m_configuration->getThreadPoolSize();
		for (unsigned int i = 0; i < threadPoolSize; i++)
		{
			boost::shared_ptr<std::thread> thread(new std::thread(&BaseServer::runThread, this));
			m_threads.push_back(thread);
		}
	}

	void BaseServer::stop()
	{
		m_io_service.stop();

		for (unsigned int i = 0; i < m_threads.size(); ++i)
		{
			m_threads[i]->join();
		}

		m_running = false;
	}

	void BaseServer::runThread()
	{
		m_io_service.run();
	}

	void BaseServer::openAcceptor()
	{
		std::string hostAddress = m_configuration->getHostAddress();

		std::stringstream portStream;
		portStream << m_configuration->getPort();
		std::string port = portStream.str();

		boost::asio::ip::tcp::resolver resolver(m_io_service);
		boost::asio::ip::tcp::resolver::query query(hostAddress, port);
		boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);

		m_acceptor.open(endpoint.protocol());
		m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
		m_acceptor.bind(endpoint);
		m_acceptor.listen();

		m_running = true;

		startAcceptor();
	}

	void BaseServer::startAcceptor()
	{
		m_newConnection = buildConnection();

		m_acceptor.async_accept(m_newConnection->socket(),
			boost::bind(&BaseServer::handleAccept, this, boost::asio::placeholders::error));
	}

	void BaseServer::handleAccept(const boost::system::error_code& e)
	{
		if (!e)
		{
			m_newConnection->start();
		}

		if (m_running)
		{
			startAcceptor();
		}
	}

}}}
