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
#include "../world.hpp"
#include "../../gui/mainwindow.hpp"
#include "WorldService.hpp"

using namespace pion;
using namespace pion::net;

namespace pion {		// begin namespace pion
namespace plugins {		// begin namespace plugins

	
// WorldService member functions

/// handles requests for WorldService
void WorldService::operator()(HTTPRequestPtr& request, TCPConnectionPtr& tcp_conn)
{
		MainWindow* _mainwindow = MainWindow::Instance();
		World* _world = World::Instance();

		std::ostringstream out; 
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
		out << "<world>\n";
		out << "<timestamp>"<< _world->timestep <<"</timestamp>\n";
		out << "<matrix>"<< _world->_lastStep <<"</matrix>\n";
		out << "</world>";

		HTTPResponseWriterPtr writer(HTTPResponseWriter::create(tcp_conn, *request,
									boost::bind(&TCPConnection::finish, tcp_conn)));
		writer->write(out.str());
		writer->writeNoCopy(HTTPTypes::STRING_CRLF);
		writer->writeNoCopy(HTTPTypes::STRING_CRLF);
		writer->send();
}


}	// end namespace plugins
}	// end namespace pion


/// creates new HelloService objects
extern "C" PION_SERVICE_API pion::plugins::WorldService *pion_create_WorldService(void)
{
	return new pion::plugins::WorldService();
}

/// destroys HelloService objects
extern "C" PION_SERVICE_API void pion_destroy_WorldService(pion::plugins::WorldService *service_ptr)
{
	delete service_ptr;
}
