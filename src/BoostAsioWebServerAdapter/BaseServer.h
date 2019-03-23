#pragma once

#include <string>
#include <vector>

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>


namespace systelab { namespace web_server {
	class Configuration;
	class IWebService;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class IConnection;
	class WebServicesMgr;

	class BaseServer
	{
	public:
		BaseServer(const Configuration&);
		virtual ~BaseServer();

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
		virtual boost::shared_ptr<IConnection> buildConnection() = 0;

	protected:
		std::unique_ptr<Configuration> m_configuration;
		std::unique_ptr<WebServicesMgr> m_webServicesMgr;

		boost::asio::io_service m_io_service;
		boost::asio::ip::tcp::acceptor m_acceptor;
		std::vector<boost::shared_ptr<std::thread> > m_threads;
		bool m_running;

		boost::shared_ptr<IConnection> m_newConnection;
	};

}}}

