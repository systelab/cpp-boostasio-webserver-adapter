#include "SecuredContext.h"


namespace systelab { namespace web_server { namespace boostasio {

	SecuredContext::SecuredContext(boost::asio::io_service& io_service)
		: boost::asio::ssl::context(boost::asio::ssl::context::sslv23_server)
	{
		set_options(boost::asio::ssl::context::default_workarounds |
					boost::asio::ssl::context::no_sslv2 |
					boost::asio::ssl::context::no_sslv3 |
					SSL_OP_NO_TICKET |
					boost::asio::ssl::context::no_tlsv1 |
					boost::asio::ssl::context::single_dh_use);

		SSL_CTX_set_session_cache_mode(native_handle(), SSL_SESS_CACHE_OFF);
	}

	bool SecuredContext::setServerCertificate(const std::string& certificate) //context_->use_certificate_file("certs/server/cert.crt", boost::asio::ssl::context::pem);
	{
		bool result = false;

		BIO *bio_mem = BIO_new(BIO_s_mem());
		BIO_puts(bio_mem, certificate.c_str());
		X509 * x509 = PEM_read_bio_X509(bio_mem, NULL, NULL, NULL);
		
		result = SSL_CTX_use_certificate(native_handle(), x509) == 1;
		
		BIO_free(bio_mem);
		X509_free(x509);

		return result;
	}

	bool SecuredContext::setServerPrivateKey(const std::string& privateKey) //context_->use_private_key_file("certs/server/key.pem", boost::asio::ssl::context::pem);
	{
		bool result = false;

		BIO *key_mem = BIO_new(BIO_s_mem());
		BIO_puts(key_mem, privateKey.c_str());

#if (OPENSSL_VERSION_NUMBER >= 0x10101000)
		EVP_PKEY* pkey = PEM_read_bio_PrivateKey(key_mem, NULL,
												 SSL_CTX_get_default_passwd_cb(native_handle()),
												 SSL_CTX_get_default_passwd_cb_userdata(native_handle()));
#else
		EVP_PKEY* pkey = PEM_read_bio_PrivateKey(key_mem, NULL,
												 native_handle()->default_passwd_callback,
												 native_handle()->default_passwd_callback_userdata);
#endif

		if (pkey != NULL)
		{
			result = SSL_CTX_use_PrivateKey(native_handle(), pkey) == 1;
			EVP_PKEY_free(pkey);
		}

		BIO_free(key_mem);

		return result;
	}

	bool SecuredContext::setServerDHParam(const std::string& dhParam) //context_->use_tmp_dh_file("certs/server/dhparam.pem");
	{
		bool result = false;

		BIO *dh_mem = BIO_new(BIO_s_mem());
		BIO_puts(dh_mem, dhParam.c_str());

		DH* dh = ::PEM_read_bio_DHparams(dh_mem, 0, 0, 0);
		BIO_free(dh_mem);

		if (dh)
		{
			result = SSL_CTX_set_tmp_dh(native_handle(), dh) == 1;
			DH_free(dh);
		}

		return result;
	}

	bool SecuredContext::setClientCertificate(const std::string& certificate) //context_->load_verify_file("certs/client/cert.crt");
	{
		set_verify_mode(boost::asio::ssl::context::verify_peer | boost::asio::ssl::context::verify_fail_if_no_peer_cert); // | boost::asio::ssl::context::verify_client_once
		
		BIO *cert_mem = BIO_new(BIO_s_mem());
		BIO_puts(cert_mem, certificate.c_str());

		STACK_OF(X509_INFO) *inf = PEM_X509_INFO_read_bio(cert_mem, NULL, NULL, NULL);
		BIO_free(cert_mem);

		unsigned int count = 0;

		if (inf)
		{
			X509_STORE_add_lookup(SSL_CTX_get_cert_store(native_handle()), X509_LOOKUP_file());

			for (int i = 0; i < sk_X509_INFO_num(inf); i++)
			{
				X509_INFO *itmp = sk_X509_INFO_value(inf, i);

				if (itmp->x509)
				{
					X509_STORE_add_cert(SSL_CTX_get_cert_store(native_handle()), itmp->x509);
					count++;
				}

				if (itmp->crl)
				{
					X509_STORE_add_crl(SSL_CTX_get_cert_store(native_handle()), itmp->crl);
					count++;
				}
			}

			sk_X509_INFO_pop_free(inf, X509_INFO_free);
		}

		return count  > 0;
	}

}}}

