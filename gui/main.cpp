#include <QtGui/QApplication>
#include "mainwindow.hpp"
#include "newworld.hpp"
#include "widgets.hpp"

//Start HelloServer Includes
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp> 
#include <pion/net/TCPServer.hpp>
#include "ShutdownManager.hpp"
//End HelloServer Includes

#include <sstream>

using namespace std;
//Start HelloServer namespaces
using namespace pion;
using namespace pion::net;
//End HelloServer namespaces

//Start HelloServer classes
/// simple TCP server that just sends "Hello there!" to each connection
class HelloServer : public TCPServer {
public:
	HelloServer(const unsigned int tcp_port) : TCPServer(tcp_port) {}
	virtual ~HelloServer() {}
	virtual void handleConnection(TCPConnectionPtr& tcp_conn)
	{
		World* world = World::Instance();
		std::ostringstream out; 
		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?><timestamp>"<< world->getTimestep() <<"</timestamp>";
		tcp_conn->setLifecycle(TCPConnection::LIFECYCLE_CLOSE);	// make sure it will get closed
		tcp_conn->async_write(boost::asio::buffer(out.str()),
							  boost::bind(&TCPConnection::finish, tcp_conn));
	}
};
//End HelloServer classes

void func(){
//Start HelloServer server startup
	// setup signal handler
	#ifdef PION_WIN32
		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	#else
		signal(SIGINT, handle_signal);
	#endif

	// initialize log system (use simple configuration)
	PionLogger main_log(PION_GET_LOGGER("PionHelloServer"));
	PionLogger pion_log(PION_GET_LOGGER("pion"));
	PION_LOG_SETLEVEL_INFO(main_log);
	PION_LOG_SETLEVEL_INFO(pion_log);
	PION_LOG_CONFIG_BASIC;
	
	try {
		// create a new server to handle the Hello TCP protocol
		TCPServerPtr hello_server(new HelloServer(8080));
		hello_server->start();
		main_shutdown_manager.wait();
	} catch (std::exception& e) {
		PION_LOG_FATAL(main_log, e.what());
		cout << e.what() << endl;
	}
//End HelloServer server startup
}

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    NewWorldDialog dialog;
    dialog.CreateDefaultObjects();
    dialog.show();

    //Start HelloServer thread
    boost::thread workerThread(func);

    //Start main thread
    int result = a.exec();

    //Stop HelloServer thread
    main_shutdown_manager.shutdown();

    return result;
}


