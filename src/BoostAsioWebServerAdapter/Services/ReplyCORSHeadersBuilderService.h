#pragma once

#include "IReplyCORSHeadersBuilderService.h"

#include <set>

namespace systelab { namespace web_server {
	class CORSConfiguration;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class ReplyCORSHeadersBuilderService : public IReplyCORSHeadersBuilderService
	{
	public:
		ReplyCORSHeadersBuilderService(CORSConfiguration&);
		virtual ~ReplyCORSHeadersBuilderService();

		void addCORSHeaders(const Request&, Reply&) const;

	private:
		std::string getCommaSeparatedList(const std::set<std::string>& items) const;

	private:
		CORSConfiguration& m_corsConfiguration;
	};

}}}
