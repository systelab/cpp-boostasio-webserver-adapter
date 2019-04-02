#pragma once

#include <boost/asio.hpp>


namespace systelab { namespace web_server { namespace test_utility {

	class HttpClient
	{
	public:
		HttpClient(const std::string& server,
				   const std::string& port);
		virtual ~HttpClient();

		bool send(std::string path, std::map<std::string, std::string>& headers, std::string& content);

	private:
		bool receive(std::map<std::string, std::string>& headers, std::string& content);

	private:
		boost::asio::io_service m_io_service;
		std::string m_server;
		std::string m_port;
		std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
	};

}}}

