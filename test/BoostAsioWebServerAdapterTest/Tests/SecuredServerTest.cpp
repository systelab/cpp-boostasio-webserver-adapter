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
			setUpSecuredServer(false);
			setUpWebService();
			setUpHttpsClient(false);
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

			m_clientCertificate =
				"-----BEGIN CERTIFICATE-----\n"
				"MIIDzjCCArYCCQC9BH7ZmAptXjANBgkqhkiG9w0BAQsFADCBqDELMAkGA1UEBhMC\n"
				"RVMxEjAQBgNVBAgMCUJhcmNlbG9uYTESMBAGA1UEBwwJQmFyY2Vsb25hMSMwIQYD\n"
				"VQQKDBpTeXN0ZWxhYiBUZWNobm9sb2dpZXMgUy5BLjENMAsGA1UECwwEQWxiYTEU\n"
				"MBIGA1UEAwwLQWxiYSBDbGllbnQxJzAlBgkqhkiG9w0BCQEWGGx1aXMucnVpekBz\n"
				"eXN0ZWxhYnN3LmNvbTAeFw0xNjEyMDIxODMwMTNaFw0yNjExMzAxODMwMTNaMIGo\n"
				"MQswCQYDVQQGEwJFUzESMBAGA1UECAwJQmFyY2Vsb25hMRIwEAYDVQQHDAlCYXJj\n"
				"ZWxvbmExIzAhBgNVBAoMGlN5c3RlbGFiIFRlY2hub2xvZ2llcyBTLkEuMQ0wCwYD\n"
				"VQQLDARBbGJhMRQwEgYDVQQDDAtBbGJhIENsaWVudDEnMCUGCSqGSIb3DQEJARYY\n"
				"bHVpcy5ydWl6QHN5c3RlbGFic3cuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8A\n"
				"MIIBCgKCAQEArO9mpUXqNtFbUSfM2AmOopaCXrRnT+TByszYKlwUya82yS74ZfHH\n"
				"ShetV0K0bqfYJsd7I3qtqesK96ML/IxOixmLl6XTfSX/iltnTvOSGM8BzrJWaxVT\n"
				"NMyxDdTOSqfSi0RWweRaq9v045HbWupM8vAcxJWUgWG1xjRTeOaiU9eXmxJqb4Vn\n"
				"gOGLEjdq9ethp2llucaSGrmMNVtErDEg5tIh8OhW3VmlYG+XacIQKxhHHU9wEYJX\n"
				"VyrHx0h1W0R0CiPfc9c0QL7QdBbEUvM9pb0gae65TSgCJE6RebHsccMTavkFI8NB\n"
				"2GWGrC/buOTCu8RA7buBtbJHx524KNwGuwIDAQABMA0GCSqGSIb3DQEBCwUAA4IB\n"
				"AQCohhuXRh2JEBcOT8Np/tb14fkDWe6Qb1x/5mveBKoXDgLoDWD2XmJkU0q/5t7c\n"
				"sbur5LptcwkzfKh/sYbuyGZfPXEk5ZApsaSlPHjl8NQTE9U/mIvmcZjjUEmevGaR\n"
				"ElNf8DWfZkQaUo0j13vskps8ZVGDyuVOHAscGwnIlol0qC1ebZUC9YJy28gQfcb+\n"
				"/owR3v9V+B0EMqp6QMF3Cvg2XvjE7Ey+tu1+ezGx1+WQmfPJ7fyKzEh2AXZFkp+w\n"
				"j4QyZFY/hU+JJFbWfeGMVX8wu+Ip9KXvGRsnlVipO1Xo2GBVLqPo6i4/SNcvk0Zp\n"
				"F78/5ToWhDdN72/VOS+NXfnT\n"
				"-----END CERTIFICATE-----\n";

			m_clientPrivateKey =
				"-----BEGIN RSA PRIVATE KEY-----\n"
				"MIIEogIBAAKCAQEArO9mpUXqNtFbUSfM2AmOopaCXrRnT+TByszYKlwUya82yS74\n"
				"ZfHHShetV0K0bqfYJsd7I3qtqesK96ML/IxOixmLl6XTfSX/iltnTvOSGM8BzrJW\n"
				"axVTNMyxDdTOSqfSi0RWweRaq9v045HbWupM8vAcxJWUgWG1xjRTeOaiU9eXmxJq\n"
				"b4VngOGLEjdq9ethp2llucaSGrmMNVtErDEg5tIh8OhW3VmlYG+XacIQKxhHHU9w\n"
				"EYJXVyrHx0h1W0R0CiPfc9c0QL7QdBbEUvM9pb0gae65TSgCJE6RebHsccMTavkF\n"
				"I8NB2GWGrC/buOTCu8RA7buBtbJHx524KNwGuwIDAQABAoIBAErspgIxGlqJthCh\n"
				"N8TZzyxuHSm+64vXxoaA3Z7sqiz/drMPYJt6dyeIvpcx1U6j9ahHgPhMLHWT2fqU\n"
				"b5sJhvYJKUuYktqD3gUpuMaiDMgm19efuIVIhfY4QPWHH6GEuFmSUCxNLWOMNrtp\n"
				"9CmdnZ0bnrslO2Qa21B2VzvoYKgF56mRfRwo2znwNBTdrmweAgEYyRtT7OzJosYK\n"
				"/kljzpb54BSBeW1pw/kzVZNdX3a2jtq7XGTnzsWOponxsa0yaZv1uFjVhQppuOkY\n"
				"MpF/4Un3rDiFxHt85qCpDDVTUjKF0nJsnXGqA1l4cGvq9mF5h1g/6qcQ5adG4hLr\n"
				"DXZzWnECgYEA3IQIbhOLcz9E+SMMETqIiX9m0OoTaQNzLjRqk5YyqGPWtquNczLV\n"
				"0NowI+DnZR4oM7eUV2uPj5wWNND55iQpf3+D+8YlCq5e1f7oxBoPa+6oSz0Q4yV2\n"
				"wodJxj/EEl3BslwNEji+m3UKThsCAhN+z+RCEh2iw9QQLOuAicejFy8CgYEAyMNS\n"
				"/bueSPs86QThyVVt7tSfgwNDEkrE5swSkiO7yeakAi1XKEUOaVVnzMS/c1AbUgLE\n"
				"97gtR2KH0MnWgYVc/8WmN0Bw/vZ7SOzwJ3sG9NfDwl27uqB+gWXhPre+EkkE7DV2\n"
				"544tKJ6luuYmvi3v7AJ8Qp8EhXnM3VYk7nX85jUCgYAKvzyR8M3pDfpUkgqyhEHH\n"
				"QbhHEkFj6tEgPVm8W6OYZVU/jdjU9nNJPHkAJqPgX7nh216VbB6RBzBl3pFvT+/d\n"
				"0N8j1CJWBO2GTGgXtpVt0zHMiOtbxqWlEWbdDJqHsp4QDUBt1OolNW0NJW/RnhWe\n"
				"GCfILj2SgbFQHyIilOIz7QKBgBpfViqQl9Vcc37krkhFWaCS+7U5G3luqvjqe9II\n"
				"saVvD8JddGKM17zMhEso2UFrl3spc/hakFAdSFwL94qiGzpaG2CMrYtAk6dbwJxf\n"
				"E+Xb5hJFP+DfLl8PJAyaFHTQdL4Ws9ztF0+Lr5einhpDbI3jMbGKtAxYmKnGpClT\n"
				"BHq1AoGAYTnf0SUrruRdA02fV/pdPaC2fOKS7HgxGwXqSyUYoZah4in9e/y6ZEU3\n"
				"rCru1RPkhr4kovoynd2+hnremKqHeglhTZ7QEiJGK5hSmZNiQv9U1M3Vak/4cTV3\n"
				"z+aXsea1uDV7p3Vg7rrbgAawZz7Vc+lfUWmqFinUAFG/c+yKLQU=\n"
				"-----END RSA PRIVATE KEY-----\n";
		}

		void setUpSecuredServer(bool mutualSSLEnabled)
		{
			m_hostAddress = "127.0.0.1";
			m_port = 9999;

			Configuration configuration;
			configuration.setSingleHostAddress(true);
			configuration.setHostAddress(m_hostAddress);
			configuration.setPort(m_port);
			configuration.setThreadPoolSize(5);

			SecurityConfiguration& securityConfiguration = configuration.getSecurityConfiguration();
			securityConfiguration.setHTTPSEnabled(true);
			securityConfiguration.setServerCertificate(m_serverCertificate);
			securityConfiguration.setServerPrivateKey(m_serverPrivateKey);
			securityConfiguration.setServerDHParam(m_serverDHParams);

			if (mutualSSLEnabled)
			{
				securityConfiguration.setMutualSSLEnabled(true);
				securityConfiguration.setClientCertificate(m_clientCertificate);
			}

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

		void setUpHttpsClient(bool mutualSSLEnabled)
		{
			SecurityConfiguration securityConfiguration;
			securityConfiguration.setServerCertificate(m_serverCertificate);

			if (mutualSSLEnabled)
			{
				securityConfiguration.setMutualSSLEnabled(true);
				securityConfiguration.setClientCertificate(m_clientCertificate);
			}

			m_httpsClient = std::make_unique<HttpsClient>(securityConfiguration, m_hostAddress,
														  std::to_string(m_port), m_clientPrivateKey);
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

		std::string m_clientCertificate;
		std::string m_clientPrivateKey;
	};


	// HTTPS server
	TEST_F(SecuredServerTest, testHttpsServerWithValidCredentials)
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

	TEST_F(SecuredServerTest, testHttpsServerThrowsExceptionWhenCertifcateIsInvalid)
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


	// Mutual SSL
	TEST_F(SecuredServerTest, testHttpsServerWithMutualSSL)
	{
		setUpSecuredServer(true);
		setUpWebService();
		setUpHttpsClient(true);

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

	TEST_F(SecuredServerTest, testHttpsServerWithMutualSSLThrowsExceptionWhenClientHasNotACertificate)
	{
		setUpSecuredServer(true);
		setUpWebService();
		setUpHttpsClient(false);

		std::map<std::string, std::string> headers;
		std::string content;
		ASSERT_THROW(m_httpsClient->send("rest/api/users", headers, content), std::exception);
	}

}}}}
