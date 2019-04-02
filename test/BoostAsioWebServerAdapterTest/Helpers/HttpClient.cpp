#include "stdafx.h"
#include "HttpClient.h"

#include <sstream>
#include <boost/bind.hpp>


using boost::asio::ip::tcp;

namespace systelab { namespace web_server { namespace test_utility {

	HttpClient::HttpClient(const std::string& server,
						   const std::string& port)
		:m_server(server)
		,m_port(port)
		,m_io_service()
	{
		m_socket = std::make_unique<boost::asio::ip::tcp::socket>(m_io_service);
	}

	HttpClient::~HttpClient() = default;

	bool HttpClient::send(std::string path, std::map<std::string, std::string>& headers, std::string& content)
	{
		tcp::resolver resolver(m_io_service);
		tcp::resolver::query query(m_server, m_port);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

		boost::system::error_code error;
		boost::asio::connect(m_socket->lowest_layer(), endpoint_iterator, error);

		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << path << " HTTP/1.0\r\n";
		request_stream << "Host: " << m_server << "\r\n";
		request_stream << "Accept: */*\r\n";
		request_stream << "Connection: close\r\n\r\n";

		boost::asio::write(*(m_socket.get()), request);

		bool result = receive(headers, content);

		m_socket->shutdown(boost::asio::socket_base::shutdown_both, error);
		m_socket->lowest_layer().close(error);
		m_io_service.stop();

		return result;
	}

	bool HttpClient::receive(std::map<std::string, std::string>& headers, std::string& content)
	{
		boost::asio::streambuf response;
		boost::asio::read_until(*(m_socket.get()), response, "\r\n");

		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;

		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);

		if (!response_stream || "HTTP/" != http_version.substr(0, 5) || 200 != status_code)
		{
			return false;
		}
		else
		{
			// Read the response headers, which are terminated by a blank line.
			boost::asio::read_until(*(m_socket.get()), response, "\r\n\r\n");

			// Process the response headers.
			std::string header;
			while (std::getline(response_stream, header) && header != "\r")
			{
				std::pair<std::string, std::string> header_struct;
				header_struct.first = header.substr(0, header.find(":"));
				header_struct.second = header.substr(header.find(":") + 2, header.size());

				if (*header_struct.second.rbegin() == '\n')
				{
					header_struct.second.pop_back();
				}

				if (*header_struct.second.rbegin() == '\r')
				{
					header_struct.second.pop_back();
				}

				headers.insert(header_struct);
			}

			std::stringstream content_os;

			// Write whatever content we already have to output.
			if (response.size() > 0)
			{
				content_os << &response;
			}

			// Read until EOF, writing data to output as we go.
			boost::system::error_code error;
			while (boost::asio::read(*(m_socket.get()), response, boost::asio::transfer_at_least(1), error))
			{
				content_os << &response;
			}

			content = content_os.str();

			return (boost::asio::error::eof == error);
		}
	}

}}}
