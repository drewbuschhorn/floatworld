#include <QtGui/QApplication>
#include "mainwindow.hpp"
#include "newworld.hpp"
#include "widgets.hpp"

//Start HelloServer Includes
#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp> 
#include <pion/PionPlugin.hpp>
#include <pion/net/WebServer.hpp>
#include "ShutdownManager.hpp"

#include "../src/HelloService.hpp"
//End HelloServer Includes

#include <sstream>

using namespace std;
//Start HelloServer namespaces
using namespace pion;
using namespace pion::net;
//End HelloServer namespaces




void func(){
//Start HelloServer server startup
	// setup signal handler
	#ifdef PION_WIN32
		SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
	#else
		signal(SIGINT, handle_signal);
	#endif
	
	try {
		// create a new server to handle the Hello TCP protocol
		// create a server for HTTP & add the Hello Service
		WebServer  web_server(8080);

		// add the Pion plug-ins installation directory to our path
		try { PionPlugin::addPluginDirectory("../src/"); }
		catch (PionPlugin::DirectoryNotFoundException&) {
		//	PION_LOG_WARN(main_log, "Default plug-ins directory does not exist: "
		//		<< PION_PLUGINS_DIRECTORY);
		//	cout << "Default plug-ins directory does not exist: "
		//		<< PION_PLUGINS_DIRECTORY;
		}

		pion::net::WebService* service = new pion::plugins::HelloService(); 
		web_server.addService("/hello", service);
		web_server.start();
		main_shutdown_manager.wait();
		delete service;
	} catch (std::exception& e) {
		//PION_LOG_FATAL(main_log, e.what());
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


