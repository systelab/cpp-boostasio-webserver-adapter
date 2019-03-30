#pragma once


namespace systelab { namespace web_server {
	class Reply;
	class Request;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class IReplyCORSHeadersBuilderService
	{
	public:
		virtual ~IReplyCORSHeadersBuilderService() {};

		virtual void addCORSHeaders(const Request&, Reply&) const = 0;
	};

}}}

