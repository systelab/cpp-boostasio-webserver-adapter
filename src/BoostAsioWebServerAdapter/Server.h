#pragma once

#include "WebServerAdapterInterface/IServer.h"

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>


namespace systelab { namespace web_server {
	class Configuration;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class Connection;
	class WebServicesMgr;

	class Server : public IServer
	{
	public:
		Server(const Configuration&);
		virtual ~Server();

		void setConfiguration(std::unique_ptr<Configuration>);
		void registerWebService(std::unique_ptr<IWebService>);

		bool isRunning() const;
		void start();
		void stop();

	private:
		void runThread();

		void openAcceptor();
		void startAcceptor();
		void handleAccept(const boost::system::error_code&);

	protected:
		std::unique_ptr<Configuration> m_configuration;
		std::unique_ptr<WebServicesMgr> m_webServicesMgr;

		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::vector<boost::shared_ptr<std::thread> > m_threads;
		bool m_running;

		boost::shared_ptr<Connection> m_newConnection;
	};

}}}

