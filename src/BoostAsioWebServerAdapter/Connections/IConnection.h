#pragma once

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>


namespace systelab { namespace web_server { namespace boostasio {

	class IConnection : private boost::noncopyable
	{
	public:
		virtual void start() = 0;
		virtual boost::asio::basic_socket<boost::asio::ip::tcp>& socket() = 0;
	};

	typedef boost::shared_ptr<IConnection> ConnectionPtr;

}}}

