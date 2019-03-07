#pragma once


namespace systelab { namespace web_server {
	class Request;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class IRequestURIParserService
	{
	public:
		virtual ~IRequestURIParserService() {};

		virtual bool parse(Request&) const = 0;
	};

}}}
