#pragma once

#include "BaseServer.h"
#include "WebServerAdapterInterface/IServer.h"


namespace systelab { namespace web_server {
	class IWebService;
	class SecurityConfiguration;
}}

namespace systelab { namespace web_server { namespace boostasio {

	class IConnection;
	class SecuredContext;

	class SecuredServer : public IServer
						, public BaseServer
	{
	public:
		SecuredServer(const Configuration&);
		virtual ~SecuredServer();

		virtual void registerWebService(std::unique_ptr<IWebService>);

		virtual bool isRunning() const;
		virtual void start();
		virtual void stop();

	protected:
		void setHTTPSConfiguration(const SecurityConfiguration&);
		void setMutualSSLConfiguration(const SecurityConfiguration&);
		boost::shared_ptr<IConnection> buildConnection();

	private:
		std::unique_ptr<SecuredContext> m_context;
	};

}}}

