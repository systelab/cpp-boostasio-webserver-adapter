#pragma once

#include "BaseServer.h"
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

	class IConnection;
	class WebServicesMgr;

	class Server : public BaseServer
				 , public IServer
	{
	public:
		Server(const Configuration&);
		virtual ~Server();

		void registerWebService(std::unique_ptr<IWebService>);

		bool isRunning() const;
		void start();
		void stop();

	protected:
		boost::shared_ptr<IConnection> buildConnection();
	};

}}}

