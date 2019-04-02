#include "stdafx.h"
#include "BoostAsioWebServerAdapter/SecuredServer.h"

#include "Helpers/HttpsClient.h"

#include "WebServerAdapterInterface/IWebService.h"
#include "WebServerAdapterInterface/Model/Configuration.h"
#include "WebServerAdapterInterface/Model/Request.h"
#include "WebServerAdapterInterface/Model/Reply.h"
#include "WebServerAdapterInterface/Model/SecurityConfiguration.h"

#include "WebServerAdapterTestUtilities/Mocks/MockWebService.h"


using namespace testing;
using namespace systelab::web_server::test_utility;

namespace systelab { namespace web_server { namespace boostasio { namespace unit_test {

	class SecuredServerTest : public Test
	{
	public:

		void SetUp()
		{
			setUpCertificates();
			setUpSecuredServer();
			setUpWebService();
			setUpHttpsClient();
		}

		void setUpCertificates()
		{
			m_serverCertificate =
				"-----BEGIN CERTIFICATE-----\n"
				"MIIDzjCCArYCCQC6r3rmlRLq0DANBgkqhkiG9w0BAQsFADCBqDELMAkGA1UEBhMC\n"
				"RVMxEjAQBgNVBAgMCUJhcmNlbG9uYTESMBAGA1UEBwwJQmFyY2Vsb25hMSMwIQYD\n"
				"VQQKDBpTeXN0ZWxhYiBUZWNobm9sb2dpZXMgUy5BLjENMAsGA1UECwwEQWxiYTEU\n"
				"MBIGA1UEAwwLQWxiYSBTZXJ2ZXIxJzAlBgkqhkiG9w0BCQEWGGx1aXMucnVpekBz\n"
				"eXN0ZWxhYnN3LmNvbTAeFw0xNjEyMDIxODI4MzZaFw0yNjExMzAxODI4MzZaMIGo\n"
				"MQswCQYDVQQGEwJFUzESMBAGA1UECAwJQmFyY2Vsb25hMRIwEAYDVQQHDAlCYXJj\n"
				"ZWxvbmExIzAhBgNVBAoMGlN5c3RlbGFiIFRlY2hub2xvZ2llcyBTLkEuMQ0wCwYD\n"
				"VQQLDARBbGJhMRQwEgYDVQQDDAtBbGJhIFNlcnZlcjEnMCUGCSqGSIb3DQEJARYY\n"
				"bHVpcy5ydWl6QHN5c3RlbGFic3cuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8A\n"
				"MIIBCgKCAQEA0Gz0Qz7rJ9j4jMpY9KpHwboAYzmQP09jNp+s90bFtWOk1fZqEWHv\n"
				"PrFV7Ov2Y9cMGFCnFzUQgLDZzs5S7sje0B2uDL3ZFBAb2wto2WHDLS8xhUFnYq6C\n"
				"2CvfYmYooD3uNvZf5YwiAsO2b3AvWil0bf4ZzDA1a2ivK6+1ncdzaKoGehpJ6zPj\n"
				"nXNfCfdUE3eDRWeb6KAjBUPzSGvfMoRuZIe27Xpmm8Vz4JoAhLqgRjW4Qmh82Gnu\n"
				"Abs4MbGbO5PDNLpWtQCdIur35jLPsTykE2H1cQG394lGf20QyqZEg3B4IoB3CSqZ\n"
				"w+IUY5AagO4YaWcPsCn0OyQva73Yk5tdVwIDAQABMA0GCSqGSIb3DQEBCwUAA4IB\n"
				"AQCH68UG9P0XtbsKpfDnYFYe+Bq3NTzAElOweSZlqSgPXjWAfBpv7X3nyJL7hz0Q\n"
				"o1cEpG2fr3pTqvJuCqojirwIW7wv4HZT/v1a/HyUipE35cDFm38GNJ4tykeTyX9M\n"
				"xoT9D+tzHqDwmLtCCY/zf3R/mHbE9uLWlNiuAG2452fiaRYHObhJpwXdWjLfe+RH\n"
				"Hod81/+s+kF8WFoDQFkuZsmY/xFgfTcGalTOdjQSYoAQoqAoRxNbCWCIkvhk5oOT\n"
				"z/SSJwHykgTqpkdhTSSolMfL2m5WzKnF1DuWxcH47sBg4rMcwYElq4MnjS38I6lo\n"
				"aVotf3vtK/MJH3Hmws9YStoa\n"
				"-----END CERTIFICATE-----\n";

			m_serverPrivateKey =
				"-----BEGIN RSA PRIVATE KEY-----\n"
				"MIIEpAIBAAKCAQEA0Gz0Qz7rJ9j4jMpY9KpHwboAYzmQP09jNp+s90bFtWOk1fZq\n"
				"EWHvPrFV7Ov2Y9cMGFCnFzUQgLDZzs5S7sje0B2uDL3ZFBAb2wto2WHDLS8xhUFn\n"
				"Yq6C2CvfYmYooD3uNvZf5YwiAsO2b3AvWil0bf4ZzDA1a2ivK6+1ncdzaKoGehpJ\n"
				"6zPjnXNfCfdUE3eDRWeb6KAjBUPzSGvfMoRuZIe27Xpmm8Vz4JoAhLqgRjW4Qmh8\n"
				"2GnuAbs4MbGbO5PDNLpWtQCdIur35jLPsTykE2H1cQG394lGf20QyqZEg3B4IoB3\n"
				"CSqZw+IUY5AagO4YaWcPsCn0OyQva73Yk5tdVwIDAQABAoIBAQClVGgGF5jNiQzQ\n"
				"EYqyuYgmIfdAvl6v+bdHky3PELKwChStLMRCeZ8f6eexAhP2apVLjF6eyBcJHkAI\n"
				"Dh43D9u+1mxD6yyQ/545CTnoheYep3ruq0zpOGjONS8DjOnZAoc9zf2PxrML4leK\n"
				"2kCVkqCqx4+LKwqU4vtg+gZk29uuPxE2LFzTZKphKF1MxGYIN9NpOLmcgHC/NNX5\n"
				"ydiazSEc5aUrcAtdvhP1QdWTsWb5vZoZ7lQ9a1cdjtl43CsOotbz/47tCjQHjQOe\n"
				"IVdD3xqkht/nXR16nGRIVeGSla23AJjxaIS/o9qKPJpvlCsQzlGFoMzLJRicEunt\n"
				"MSVK+MEBAoGBAPowG3ij062S06OI6XQKPl+2rGL/K1fEKK5aHrNtamvw0wlyZYoQ\n"
				"LMdQwld5qzTz55wexgMPac38FfUpKNmHR+WL8zQonMWAlqWmZ1yQeeN/BZkgGOaC\n"
				"mHTsfTv5b8X/MJlCKoZSqxDv/p7EPP2NZeXq41daullYZXL14YaReULBAoGBANVE\n"
				"e230boM/RaIgxhVQpb5WEguXb2QC0yF7YGniFoOAMLkY1/FTy7jOpjcYSEhucLgq\n"
				"kOyfm6egNn9CnUgvjf4leJiVWMvqTJ+lNAHzCnBuiK2Y2ZZ5QU78Liy5spZyItEg\n"
				"mhBOnCoAwQjZ7A1RApOutocorvu5J+lcTrtFk94XAoGBAN9CXNFsSLJlCw2tWL7f\n"
				"gUfCaNUy5ge9GZx0+V6erlBCqOCOQERTKNJ4o+SRw5FOcomkAImskHtrkGClaTms\n"
				"+iDCobYS48tvc3/BHg3BM7QqnuFFflL9iXUVsK/7aJ4kmsa4/9+8F8HWmL4vWknd\n"
				"i2gAEv3aDd86/dT3zFhcqvxBAoGAATu7JwCh3vpvsadU3eyweLBQvGZsy3xEsk3f\n"
				"Wd1n2ezU200seoXMiM4RIgJu8JRK7JW6btPhd4+yoPvGoxM7U/vtfZxnfg5l0QqI\n"
				"iXZZWhsZiT94Cqm62fUVY0bTpJEWaHUyzuNVfdyxQRbZiZ4tUKNTBOcUU/pXI79X\n"
				"6TdWypMCgYBPQxR+doNPFGLCYoYFmlsobkYqBh+PXN5jtugQ4J9BOTVQ8SBFkPSO\n"
				"fh8Diif2TMBFazf8Ch4C8010xqSRUZtMU7acdYH1f8x90HrpGasKmztwicAt05kY\n"
				"t97KsYIbZ1rTYIwVGEeb/XmqDtfsa3vhpjx8Q7CHcay0EcgdJ4VwZw==\n"
				"-----END RSA PRIVATE KEY-----\n";

			m_serverDHParams =
				"-----BEGIN DH PARAMETERS-----\n"
				"MIGHAoGBAJaL4ZH6p3xjp9x6g+H8bQX0jA0+Zy6GEJYp03LetlynG1XgjCssiyRj\n"
				"aEfu9/0TYAG7UvJvrLjcmzhU+pDKQQn/DkqnExo5OSQjduxlZU37apaF+LxLCKqN\n"
				"9S4O7LL7IP4h7Eb8kStGB0m0RppNJZhU/06Xt7kDlryl6+Dv4pkLAgEC\n"
				"-----END DH PARAMETERS-----\n";
		}

		void setUpSecuredServer()
		{
			m_hostAddress = "127.0.0.1";
			m_port = 9999;

			Configuration configuration;
			configuration.setHostAddress(m_hostAddress);
			configuration.setPort(m_port);
			configuration.setThreadPoolSize(5);

			SecurityConfiguration& securityConfiguration = configuration.getSecurityConfiguration();
			securityConfiguration.setHTTPSEnabled(true);
			securityConfiguration.setServerCertificate(m_serverCertificate);
			securityConfiguration.setServerPrivateKey(m_serverPrivateKey);
			securityConfiguration.setServerDHParam(m_serverDHParams);

			m_securedServer = std::make_unique<SecuredServer>(configuration);
			m_securedServer->start();
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

			m_securedServer->registerWebService(std::move(m_webService));
		}

		void setUpHttpsClient()
		{
			SecurityConfiguration securityConfiguration;
			securityConfiguration.setServerCertificate(m_serverCertificate);
			m_httpsClient = std::make_unique<HttpsClient>(securityConfiguration, m_hostAddress, std::to_string(m_port));
		}

	protected:
		std::unique_ptr<SecuredServer> m_securedServer;
		std::unique_ptr<MockWebService> m_webService;
		std::unique_ptr<HttpsClient> m_httpsClient;

		std::string m_basePath;
		Reply m_defaultReply;

		std::string m_hostAddress;
		unsigned int m_port;

		std::string m_serverCertificate;
		std::string m_serverPrivateKey;
		std::string m_serverDHParams;
	};


	TEST_F(SecuredServerTest, testExpectedReplyWithValidCredentials)
	{
		std::map<std::string, std::string> headers;
		std::string content;
		m_httpsClient->send("rest/api/users", headers, content);

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

	TEST_F(SecuredServerTest, testSetServeExpectedThrowsWithInvalidCredentials)
	{
		Configuration configuration;
		configuration.setHostAddress(m_hostAddress);
		configuration.setPort(m_port);
		configuration.setThreadPoolSize(5);

		SecurityConfiguration& securityConfiguration = configuration.getSecurityConfiguration();
		securityConfiguration.setHTTPSEnabled(true);
		securityConfiguration.setServerCertificate("Invalid certificate");
		securityConfiguration.setServerPrivateKey(m_serverPrivateKey);
		securityConfiguration.setServerDHParam(m_serverDHParams);

		ASSERT_THROW(std::make_unique<SecuredServer>(configuration), systelab::web_server::IServer::Exception);
	}

}}}}
