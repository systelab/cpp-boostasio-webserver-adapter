#include "stdafx.h"
#include "BoostAsioWebServerAdapter/Server.h"

#include "Helpers/HttpClient.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"

#include "WebServerAdapterTestUtilities/Mocks/MockWebService.h"


using namespace testing;
using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace boostasio { namespace unit_test {

	class ServerTest : public Test
	{
	public:

		void SetUp()
		{
			setUpServer();
			setUpWebService();
			setUpHttpClient();
		}

		void setUpServer()
		{
			m_hostAddress = "localhost";
			m_port = 9999;

			Configuration configuration;
			configuration.setSingleHostAddress(false);
			configuration.setPort(m_port);
			configuration.setThreadPoolSize(5);

			m_server = std::make_unique<Server>(configuration);
			m_server->start();
		}

		void setUpWebService()
		{
			m_defaultReply.setStatus(Reply::OK);
			m_defaultReply.setContent("Expected reply goes here");
			m_defaultReply.addHeader("Content-Type", "text/plain");
			m_defaultReply.addHeader("Authorization", "Bearer 12345679012345689");

			m_webService = std::make_unique<MockWebService>();
			ON_CALL(*m_webService, processProxy(_)).WillByDefault(Invoke(
				[this](const Request&) ->  Reply*
				{
					return new Reply(m_defaultReply);
				}
			));

			m_server->registerWebService(std::move(m_webService));
		}

		void setUpHttpClient()
		{
			m_httpClient = std::make_unique<HttpClient>(m_hostAddress, std::to_string(m_port));
		}

	protected:
		std::unique_ptr<Server> m_server;
		std::unique_ptr<MockWebService> m_webService;
		std::unique_ptr<HttpClient> m_httpClient;

		std::string m_basePath;
		Reply m_defaultReply;

		std::string m_hostAddress;
		unsigned int m_port;
	};


	TEST_F(ServerTest, testSendRequestReturnsExpectedReply)
	{
		std::map<std::string, std::string> headers;
		std::string content;
		m_httpClient->send("rest/api/users", headers, content);

		std::string expectedContent = m_defaultReply.getContent();
		EXPECT_EQ(expectedContent, content);

		std::map<std::string, std::string> expectedHeaders = m_defaultReply.getHeaders();
		EXPECT_EQ(expectedHeaders.size(), headers.size());
		for (auto expectedHeader : expectedHeaders)
		{
			ASSERT_TRUE(headers.find(expectedHeader.first) != headers.end()) << "for header '" << expectedHeader.first << "'";
			EXPECT_EQ(expectedHeader.second, headers[expectedHeader.first]) << "for header '" << expectedHeader.first << "'";
		}
	}

}}}}
