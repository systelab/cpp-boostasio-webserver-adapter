#include "SecuredConnection.h"

#include "Agents/IRequestParserAgent.h"
#include "Services/IReplyBufferBuilderService.h"
#include "Services/IRequestHandlingService.h"
#include "Services/IRequestURIParserService.h"

#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/Request.h"

#include <boost/bind.hpp>
#include <vector>


namespace systelab { namespace web_server { namespace boostasio {

	SecuredConnection::SecuredConnection(boost::asio::io_service& io_service,
										 boost::asio::ssl::context& context,
										 std::unique_ptr<IRequestParserAgent> requestParserAgent,
										 std::unique_ptr<IRequestURIParserService> requestURIParserService,
										 std::unique_ptr<IRequestHandlingService> requestHandlingService,
										 std::unique_ptr<IReplyBufferBuilderService> replyBuffersBuilderService)
		:m_strand(io_service)
		,m_socket(io_service, context)
		,m_requestBuffer()
		,m_replyBuffer()
		,m_requestParserAgent(std::move(requestParserAgent))
		,m_requestURIParserService(std::move(requestURIParserService))
		,m_requestHandlingService(std::move(requestHandlingService))
		,m_replyBufferBuilderService(std::move(replyBuffersBuilderService))
		,m_request()
		,m_reply()
	{
	}

	boost::asio::basic_socket<boost::asio::ip::tcp>& SecuredConnection::socket()
	{
		return m_socket.lowest_layer();
	}

	void SecuredConnection::start()
	{
		SSL_set_cipher_list(m_socket.native_handle(), "ECDHE-RSA-AES128-GCM-SHA256");
		SSL_set_ecdh_auto(m_socket.native_handle(), 1);

		m_socket.async_handshake(boost::asio::ssl::stream_base::server,
								m_strand.wrap(boost::bind(&SecuredConnection::handleHandshake, shared_from_this(),
								  boost::asio::placeholders::error)));
	}

	void SecuredConnection::handleHandshake(const boost::system::error_code& e)
	{
		if (!e)
		{
			m_socket.async_read_some(boost::asio::buffer(m_requestBuffer),
									m_strand.wrap(boost::bind(&SecuredConnection::handleRead, shared_from_this(),
												boost::asio::placeholders::error,
												boost::asio::placeholders::bytes_transferred)));
		}
		else
		{
			std::string message = e.message();
			message = message;
		}
	}

	void SecuredConnection::handleRead(const boost::system::error_code& e, std::size_t bytes_transferred)
	{
		if (!e)
		{
			boost::optional<bool> result = m_requestParserAgent->parseBuffer(m_requestBuffer.data(), bytes_transferred, *m_request);

			if (!result.is_initialized())
			{
				m_socket.async_read_some(
					boost::asio::buffer(m_requestBuffer),
					m_strand.wrap(
						boost::bind(&SecuredConnection::handleRead, shared_from_this(),
							boost::asio::placeholders::error,
							boost::asio::placeholders::bytes_transferred)));
			}
			else
			{
				if (result)
				{
					result = m_requestURIParserService->parse(*m_request);
				}

				if (result)
				{
					m_reply = m_requestHandlingService->processRequest(*m_request);
				}
				else
				{
					m_reply.reset(new Reply());
					m_reply->setStatus(Reply::BAD_REQUEST);
				}

				m_replyBuffer = m_replyBufferBuilderService->buildBuffer(*m_reply);

				boost::asio::async_write(m_socket, boost::asio::buffer(m_replyBuffer),
					m_strand.wrap(boost::bind(
						&SecuredConnection::handleWrite, shared_from_this(),
						boost::asio::placeholders::error)));
			}
		}
		else
		{
			std::string message = e.message();
			message = message;
		}
	}

	void SecuredConnection::handleWrite(const boost::system::error_code& e)
	{
		if (!e)
		{
			boost::system::error_code ignored_ec;
			m_socket.shutdown(ignored_ec);
		}
	}

}}}
