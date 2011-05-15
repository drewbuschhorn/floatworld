// ------------------------------------------------------------------
// pion-net: a C++ framework for building lightweight HTTP interfaces
// ------------------------------------------------------------------
// Copyright (C) 2007-2008 Atomic Labs, Inc.  (http://www.atomiclabs.com)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#include "HelloService.hpp"
#include <pion/net/HTTPResponseWriter.hpp>

using namespace pion;
using namespace pion::net;

namespace pion {		// begin namespace pion
namespace plugins {		// begin namespace plugins

	
// HelloService member functions

/// handles requests for HelloService
void HelloService::operator()(HTTPRequestPtr& request, TCPConnectionPtr& tcp_conn)
{
	std::ostringstream hello_html;
	hello_html << "<html><body><h1>Response Code:"
		<< HTTPTypes::RESPONSE_CODE_NOT_FOUND << ":" 
		<< HTTPTypes::RESPONSE_MESSAGE_NOT_FOUND
		<< "</h1><hr/><div>Floatworld Server is running, but this route is not configured.</div></body></html>";
	HTTPResponseWriterPtr writer(HTTPResponseWriter::create(tcp_conn, *request,
															boost::bind(&TCPConnection::finish, tcp_conn)));
	writer->write(hello_html.str());
	writer->getResponse().setStatusCode(HTTPTypes::RESPONSE_CODE_NOT_FOUND);
	writer->getResponse().setStatusMessage(HTTPTypes::RESPONSE_MESSAGE_NOT_FOUND);
	writer->writeNoCopy(HTTPTypes::STRING_CRLF);
	writer->writeNoCopy(HTTPTypes::STRING_CRLF);
	writer->send();
}


}	// end namespace plugins
}	// end namespace pion


/// creates new HelloService objects
extern "C" PION_SERVICE_API pion::plugins::HelloService *pion_create_HelloService(void)
{
	return new pion::plugins::HelloService();
}

/// destroys HelloService objects
extern "C" PION_SERVICE_API void pion_destroy_HelloService(pion::plugins::HelloService *service_ptr)
{
	delete service_ptr;
}
