#include "stdafx.h"
#include "BoostAsioWebServerAdapter/Agents/RequestParserAgent.h"

#include "WebServerAdapterInterface/Model/Request.h"
#include "TestUtilitiesInterface/EntityComparator.h"

#include <boost/assign/list_of.hpp>
#include <boost/optional.hpp>


namespace systelab { namespace web_server { namespace unit_test {

	struct RequestParserAgentTestData
	{
		std::string buffer;
		boost::optional<bool> expectedResult;
		Request expectedParsedRequest;
	};

	RequestParserAgentTestData testData[] = 
	{
		// Valid requests
		{
			"PATCH /rest/api/panels/1 HTTP/1.1\r\n"
			"Host: localhost:8080\r\n"
			"Content-Type: application/json\r\n"
			"Content-Length: 19\r\n"
			"\r\n"
			"{\"name\": \"MYPANEL\"}",
			true, Request("PATCH", "/rest/api/panels/1", 1, 1,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Host", "localhost:8080"))
							(std::make_pair<std::string, std::string>("Content-Type", "application/json"))
							(std::make_pair<std::string, std::string>("Content-Length", "19")),
						  19, "{\"name\": \"MYPANEL\"}")
		},
		{
			"POST img/design/instrument.png HTTP/123.456\r\n"
			"\r\n",
			true, Request("POST", "img/design/instrument.png", 123, 456,
						  std::vector< std::pair<std::string, std::string> >(),
						  0, "")
		},
		{
			"DELETE /rest/api/workitems/456 HTTP/1.1\r\n"
			"Content-Length: invalid\r\n"
			"\r\n",
			true, Request("DELETE", "/rest/api/workitems/456", 1, 1,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "invalid")),
						  0, "")
		},
		{
			"GET /content/is/trunkated/to/header/length HTTP/1.1\r\n"
			"Content-Length: 4\r\n"
			"\r\n"
			"More content than needed",
			true, Request("GET", "/content/is/trunkated/to/header/length", 1, 1,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "4")),
						  4, "More")
		},

		// Valid requests (with UTF-8 content)
		{
			"GET /test/utf8/2byte-char HTTP/1.0\r\n"
			"Content-Length: 2\r\n"
			"\r\n"
			"\xc3\x91", // Ñ
			true, Request("GET", "/test/utf8/2byte-char", 1, 0,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "2")),
						  2, "\xc3\x91")
		},
		{
			"GET /test/utf8/3byte-char HTTP/1.0\r\n"
			"Content-Length: 3\r\n"
			"\r\n"
			"\xe2\xb7\xa3", // Cyrilic letter DE
			true, Request("GET", "/test/utf8/3byte-char", 1, 0,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "3")),
						  3, "\xe2\xb7\xa3")
		},
		{
			"GET /test/utf8/4byte-char HTTP/1.0\r\n"
			"Content-Length: 4\r\n"
			"\r\n"
			"\xf0\x90\x8c\x80", // Old italic letter A
			true, Request("GET", "/test/utf8/4byte-char", 1, 0,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "4")),
						  4, "\xf0\x90\x8c\x80")
		},
		{
			"GET /test/utf8/combined HTTP/1.0\r\n"
			"Content-Length: 10\r\n"
			"\r\n"
			"\xf0\x90\x8c\x80\xe2\xb7\xa3\xc3\x91z", // 4 utf-8 chars (one of each byte length: 4, 3, 2 and 1)
			true, Request("GET", "/test/utf8/combined", 1, 0,
						  boost::assign::list_of
							(std::make_pair<std::string, std::string>("Content-Length", "10")),
						  10, "\xf0\x90\x8c\x80\xe2\xb7\xa3\xc3\x91z")
		},

		// Invalid requests
		{ "@", false, Request() },
		{ "\n", false, Request() },
		{ "GET\t", false, Request() },
		{ "PATCH>", false, Request() },
		{ "PATCH \r", false, Request() },
		{ "PATCH /rest/api/specimens\r", false, Request() },
		{ "PATCH /rest/api/specimens h", false, Request() },
		{ "PATCH /rest/api/specimens Ht", false, Request() },
		{ "PATCH /rest/api/specimens HTt", false, Request() },
		{ "PATCH /rest/api/specimens HTTp", false, Request() },
		{ "PATCH /rest/api/specimens HTTPx", false, Request() },
		{ "PATCH /rest/api/specimens HTTP\\", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/x", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/7x", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11x", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.x", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2x", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\t", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\n\t", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\n{", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nñ", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nC[", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Type:a", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Type: \t", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Type: app]\r\t", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: a\rx", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r1", false, Request() },
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xa0\xa1", false, Request() }, // Invalid UTF8 character length
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xc3\x28", false, Request() }, // Invalid 2-byte UTF8 character (in 2nd byte)
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xe2\x28\xa1", false, Request() }, // Invalid 3-byte UTF8 character (in 2nd byte)
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xe2\x82\x28", false, Request() }, // Invalid 3-byte UTF8 character (in 3rd byte)
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xf0\x28\x8c\xbc", false, Request() }, // Invalid 4-byte UTF8 character (in 2nd byte)
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xf0\x90\x28\xbc", false, Request() }, // Invalid 4-byte UTF8 character (in 3rd byte)
		{ "PATCH /rest/api/specimens HTTP/11.2\r\nContent-Length: 10\r\n\r\n\xf0\x90\x8c\x28", false, Request() }, // Invalid 4-byte UTF8 character (in 4th byte)

		// Incomplete (but not invalid) requests
		{ "", boost::indeterminate, Request() },
		{ "G", boost::indeterminate, Request() },
		{ "GET", boost::indeterminate, Request() },
		{ "GET ", boost::indeterminate, Request() },
		{ "GET /", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover ", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover H", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HT", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTT", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\n", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nC", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length:", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 1", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n\r", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n\r\n", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n\r\n1", boost::indeterminate, Request() },
		{ "GET /rest/api/instrument/reagentcover HTTP/1.1\r\nContent-Length: 10\r\n\r\n123456789", boost::indeterminate, Request() },
	};


	class RequestParserAgentTest : public ::testing::TestWithParam<RequestParserAgentTestData>
	{
	public:
		void SetUp()
		{
			m_agent.reset(new RequestParserAgent());
		}

	protected:
		std::unique_ptr<RequestParserAgent> m_agent;
	};


	TEST_P(RequestParserAgentTest, testParseBuffer)
	{
		Request parsedRequest;
		boost::tribool result = m_agent->parseBuffer(GetParam().buffer.c_str(), GetParam().buffer.size(), parsedRequest);

		if (!boost::indeterminate(GetParam().expectedResult))
		{
			ASSERT_EQ(GetParam().expectedResult, result) << "While parsing buffer: " << std::endl << GetParam().buffer;
		}
		else
		{
			ASSERT_TRUE(boost::indeterminate(result)) << "While parsing buffer: " << std::endl << GetParam().buffer;
		}

		if (result)
		{
			ASSERT_TRUE(test_utility::EntityComparator()(GetParam().expectedParsedRequest, parsedRequest));
		}
	}

	INSTANTIATE_TEST_CASE_P(WebServer, RequestParserAgentTest, testing::ValuesIn(testData));

}}}