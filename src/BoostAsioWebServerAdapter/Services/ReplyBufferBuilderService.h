#pragma once

#include "IReplyBufferBuilderService.h"
#include "WebServerAdapterInterface/Model/Reply.h"


namespace systelab { namespace web_server { namespace boostasio {

	class ReplyBufferBuilderService : public IReplyBufferBuilderService
	{
	public:
		ReplyBufferBuilderService();
		virtual ~ReplyBufferBuilderService();

		std::string buildBuffer(const Reply&) const;

	private:
		std::string translateStatusToString(Reply::StatusType) const;
	};

}}}
