#include "stdafx.h"
#include "BoostAsioWebServerAdapter/Services/ReplyBufferBuilderService.h"

#include "WebServerAdapterInterface/Model/Reply.h"

#include <string>
#include <memory>


namespace systelab { namespace web_server { namespace boostasio { namespace unit_test {

	struct ReplyBufferBuilderServiceTestData
	{
		Reply reply;
		std::string expectedBuffer;
	};

	ReplyBufferBuilderServiceTestData testData[] =
	{
		{
			Reply(Reply::OK, { {"Content-Type", "application/json"} }, "{}"),
			"HTTP/1.0 200 OK\r\n"
			"Content-Type: application/json\r\n"
			"\r\n"
			"{}"
		},
		{
			Reply(Reply::CREATED, { {"Content-Type", "application/json"}, {"Authorization", "Bearer 123456789"} }, "{}"),
			"HTTP/1.0 201 Created\r\n"
			"Authorization: Bearer 123456789\r\n"
			"Content-Type: application/json\r\n"
			"\r\n"
			"{}"
		},
		{
			Reply(Reply::ACCEPTED, { }, "Here goes my content"),
			"HTTP/1.0 202 Accepted\r\n"
			"\r\n"
			"Here goes my content"
		},
		{
			Reply(Reply::NO_CONTENT, { }, ""),
			"HTTP/1.0 204 No Content\r\n"
			"\r\n"
		},
		{
			Reply(Reply::MULTI_STATUS, { }, ""),
			"HTTP/1.0 207 Multi-Status\r\n"
			"\r\n"
		},
		{
			Reply(Reply::MULTIPLE_CHOICES, { }, ""),
			"HTTP/1.0 300 Multiple Choices\r\n"
			"\r\n"
		},
		{
			Reply(Reply::MOVED_PERMANENTLY, { }, ""),
			"HTTP/1.0 301 Moved Permanently\r\n"
			"\r\n"
		},
		{
			Reply(Reply::MOVED_TEMPORARILY, { }, ""),
			"HTTP/1.0 302 Moved Temporarily\r\n"
			"\r\n"
		},
		{
			Reply(Reply::NOT_MODIFIED, { }, ""),
			"HTTP/1.0 304 Not Modified\r\n"
			"\r\n"
		},
		{
			Reply(Reply::BAD_REQUEST, { }, ""),
			"HTTP/1.0 400 Bad Request\r\n"
			"\r\n"
		},
		{
			Reply(Reply::UNAUTHORIZED, { }, ""),
			"HTTP/1.0 401 Unauthorized\r\n"
			"\r\n"
		},
		{
			Reply(Reply::FORBIDDEN, { }, ""),
			"HTTP/1.0 403 Forbidden\r\n"
			"\r\n"
		},
		{
			Reply(Reply::NOT_FOUND, { }, ""),
			"HTTP/1.0 404 Not Found\r\n"
			"\r\n"
		},
		{
			Reply(Reply::REQUEST_TIMEOUT, { }, ""),
			"HTTP/1.0 408 Request Timeout\r\n"
			"\r\n"
		},
		{
			Reply(Reply::INTERNAL_SERVER_ERROR, { }, ""),
			"HTTP/1.0 500 Internal Server Error\r\n"
			"\r\n"
		},
		{
			Reply(Reply::NOT_IMPLEMENTED, { }, ""),
			"HTTP/1.0 501 Not Implemented\r\n"
			"\r\n"
		},
		{
			Reply(Reply::BAD_GATEWAY, { }, ""),
			"HTTP/1.0 502 Bad Gateway\r\n"
			"\r\n"
		},
		{
			Reply(Reply::SERVICE_UNAVAILABLE, { }, ""),
			"HTTP/1.0 503 Service Unavailable\r\n"
			"\r\n"
		}
	};


	class ReplyBufferBuilderServiceTest : public ::testing::TestWithParam<ReplyBufferBuilderServiceTestData>
	{
	public:
		void SetUp()
		{
			m_service = std::make_unique<ReplyBufferBuilderService>();
		}

	protected:
		std::unique_ptr<ReplyBufferBuilderService> m_service;
	};


	TEST_P(ReplyBufferBuilderServiceTest, testBuildBuffer)
	{
		ASSERT_EQ(GetParam().expectedBuffer, m_service->buildBuffer(GetParam().reply));
	}

	INSTANTIATE_TEST_CASE_P(WebServer, ReplyBufferBuilderServiceTest, testing::ValuesIn(testData));

}}}}
