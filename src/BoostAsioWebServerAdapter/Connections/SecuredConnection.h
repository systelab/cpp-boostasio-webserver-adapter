#pragma once

#include "IConnection.h"

#include <boost/array.hpp>
#include <boost/asio/ssl.hpp>


namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace web_server { namespace boostasio {

	typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_socket;

	class IReplyBufferBuilderService;
	class IRequestHandlingService;
	class IRequestParserAgent;
	class IRequestURIParserService;

	class SecuredConnection : public IConnection
							, public boost::enable_shared_from_this<SecuredConnection>
	{
	public:
		SecuredConnection(boost::asio::io_service&,
						  boost::asio::ssl::context&,
						  std::unique_ptr<IRequestParserAgent>,
						  std::unique_ptr<IRequestURIParserService>,
						  std::unique_ptr<IRequestHandlingService>,
						  std::unique_ptr<IReplyBufferBuilderService>);
		virtual ~SecuredConnection();

		void start();
		boost::asio::basic_socket<boost::asio::ip::tcp>& socket();

	private:
		void handleHandshake(const boost::system::error_code& error);
		void handleRead(const boost::system::error_code& e, std::size_t bytes_transferred);
		void handleWrite(const boost::system::error_code& e);

	private:
		boost::asio::io_service::strand m_strand;
		ssl_socket m_socket;
		boost::array<char, 8192> m_requestBuffer;
		std::string m_replyBuffer;

		std::unique_ptr<IRequestParserAgent> m_requestParserAgent;
		std::unique_ptr<IRequestURIParserService> m_requestURIParserService;
		std::unique_ptr<IRequestHandlingService> m_requestHandlingService;
		std::unique_ptr<IReplyBufferBuilderService> m_replyBufferBuilderService;

		std::unique_ptr<Request> m_request;
		std::unique_ptr<Reply> m_reply;
	};

}}}

