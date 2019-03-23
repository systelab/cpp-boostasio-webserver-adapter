#pragma once

#include "BaseServer.h"
#include "WebServerAdapterInterface/ISecuredServer.h"


namespace systelab { namespace web_server {
	class IWebService;
	class SecuredServerCredentials;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class IConnection;
	class SecuredContext;

	class SecuredServer : public ISecuredServer
						, public BaseServer
	{
	public:
		SecuredServer(const Configuration&);
		virtual ~SecuredServer();

		void setServerCredentials(std::unique_ptr<SecuredServerCredentials>);
		void setClientCertificate(const std::string&);

		virtual void setConfiguration(std::unique_ptr<Configuration>);
		virtual void registerWebService(std::unique_ptr<IWebService>);

		virtual bool isRunning() const;
		virtual void start();
		virtual void stop();

	protected:
		boost::shared_ptr<IConnection> buildConnection();

	private:
		std::unique_ptr<SecuredContext> m_context;
	};

	namespace ex
	{
	}

}}}

