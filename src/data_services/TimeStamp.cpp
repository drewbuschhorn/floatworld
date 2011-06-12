// ------------------------------------------------------------------
// pion-net: a C++ framework for building lightweight HTTP interfaces
// ------------------------------------------------------------------
// Copyright (C) 2007-2008 Atomic Labs, Inc.  (http://www.atomiclabs.com)
//
// Distributed under the Boost Software License, Version 1.0.
// See http://www.boost.org/LICENSE_1_0.txt
//

#include <iostream>
#include <pion/net/HTTPResponseWriter.hpp>

#include "json_spirit/json_spirit_reader_template.h"
#include "json_spirit/json_spirit_writer_template.h"

#include "../world.hpp"
#include "../../gui/mainwindow.hpp"
#include "TimeStamp.hpp"

using namespace json_spirit;
using namespace pion;
using namespace pion::net;

namespace pion {		// begin namespace pion
namespace plugins {		// begin namespace plugins

	
// TimestampService member functions

/// handles requests for TimestampService
void TimestampService::operator()(HTTPRequestPtr& request, TCPConnectionPtr& tcp_conn)
{
		MainWindow* _mainwindow = MainWindow::Instance();
		World* _world = World::Instance();

		//std::ostringstream out; 
		json_spirit::Object json_timestamp;
		json_timestamp.push_back(json_spirit::Pair("timestamp",_world->timestep));
		const std::string out = json_spirit::write_string( Value(json_timestamp), pretty_print );		

		HTTPResponseWriterPtr writer(HTTPResponseWriter::create(tcp_conn, *request,
									boost::bind(&TCPConnection::finish, tcp_conn)));
		writer->write(out.c_str());
		writer->writeNoCopy(HTTPTypes::STRING_CRLF);
		writer->writeNoCopy(HTTPTypes::STRING_CRLF);
		writer->send();
}


}	// end namespace plugins
}	// end namespace pion


/// creates new HelloService objects
extern "C" PION_SERVICE_API pion::plugins::TimestampService *pion_create_TimestampService(void)
{
	return new pion::plugins::TimestampService();
}

/// destroys HelloService objects
extern "C" PION_SERVICE_API void pion_destroy_TimestampService(pion::plugins::TimestampService *service_ptr)
{
	delete service_ptr;
}
