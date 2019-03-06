#include "stdafx.h"
#include "Webserver/MimeTypeService.h"

#include <memory>

namespace http { namespace server { namespace unit_test {

	struct MimeTypeServiceTestData
	{
		std::string extension;
		std::string expectedMimeType;
	};

	MimeTypeServiceTestData testData[] = 
	{
		// Known extensions
		{ "htm", "text/html" },
		{ "html", "text/html" },
		{ "js", "application/javascript"},
		{ "css", "text/css"},
		{ "gif", "image/gif" },
		{ "jpg", "image/jpeg" },
		{ "png", "image/png" },
		{ "svg", "image/svg+xml" },
		{ "pdf", "application/pdf"},
		{ "eot", "application/vnd.ms-fontobject"},
		{ "ttf", "application/x-font-ttf"},
		{ "woff", "application/font-woff"},
		{ "woff2", "application/font-woff2"},
		
		// Unknown extensions
		{ "doc", "text/plain"},
		{ "zip", "text/plain"},
		{ "exe", "text/plain"},
		{ "bat", "text/plain"}
	};


	class MimeTypeServiceTest: public ::testing::TestWithParam<MimeTypeServiceTestData>
	{
	public:
		void SetUp()
		{
			m_service.reset(new MimeTypeService());
		}

	protected:
		std::unique_ptr<MimeTypeService> m_service;
	};


	TEST_P(MimeTypeServiceTest, testGetMimeTypeFromExtensionReturnsExpectedType)
	{
		std::string mimeType = m_service->getTypeFromExtension(GetParam().extension);
		ASSERT_EQ(GetParam().expectedMimeType, mimeType);
	}

	INSTANTIATE_TEST_CASE_P(WebServer, MimeTypeServiceTest, testing::ValuesIn(testData));

}}}