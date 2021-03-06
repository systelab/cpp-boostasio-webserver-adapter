#pragma once

#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>


namespace systelab { namespace web_server { namespace test_utility {

	class HttpsClient
	{
	public:
		HttpsClient(SecurityConfiguration&,
					const std::string& server,
					const std::string& port,
					const std::string& clientPrivateKey);
		virtual ~HttpsClient();

		bool send(std::string path, std::map<std::string, std::string>& headers, std::string& content);

	private:
		bool receive(std::map<std::string, std::string>& headers, std::string& content);
		bool setServerCertificate(const std::string& certificate);
		bool setClientCertificate(const std::string& certificate);
		bool setClientPrivateKey(const std::string& privateKey);

	private:
		boost::asio::io_service m_io_service;
		std::string m_server;
		std::string m_port;
		std::string m_root;
		std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_socket;
		boost::asio::ssl::context m_context;
	};

}}}

