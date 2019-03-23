#pragma once

#include "BaseServer.h"

#include <boost/asio/ssl/context.hpp>


namespace systelab { namespace web_server { namespace boostasio {

	class SecuredContext : public boost::asio::ssl::context
	{
	public:

		explicit SecuredContext(boost::asio::io_service&);

		bool setServerCertificate(const std::string& certificate);
		bool setServerPrivateKey(const std::string& privateKey);
		bool setServerDHParam(const std::string& dhParam);

		bool setClientCertificate(const std::string& certificate);
	};

}}}

