// System includes
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <pthread.h>
#include <signal.h>
#include <vector>

// XML-RPC includes
#include <xmlrpc-c/base.hpp>
#include <xmlrpc-c/registry.hpp>
#include <xmlrpc-c/server_abyss.hpp>

// Local includes
#include "RPCHandler.hpp"
#include "Server.hpp"

// Declare which objects from std we're using.
using std::string;
using std::cout;
using std::cerr;
using std::endl;

// Use the RPC handlers from RPCHandler.hpp
using cs4513::ShutdownHandler;
using cs4513::setSensorTemp;
using cs4513::setSensorHum;
using cs4513::getRelay;
using cs4513::getPWM;
using cs4513::get10V;
using cs4513::MixerIsClosing;
using cs4513::MixerIsOpening;
using cs4513::setRcMode;
using cs4513::setRcTemp;

// Define Server to be in the cs4513 namespace
namespace cs4513 {

// Initialize constants...
// Initialize the singleton instance to NULL.
Server* cs4513::Server::serverInstance = NULL;
// Initialize the path to the XML-RPC server log.
const string cs4513::Server::LOG_PATH = "/root/Mert/AutomatisiertesTestenAG/log/server.log";
// Define the RPC_NAME constants.

const string cs4513::Server::SHUTDOWN_RPC_NAME = "server.shutdown";
const string cs4513::Server::TESTREGLER = "server.testRegler";
const string cs4513::Server::SET_SENSOR_TEMP_NAME = "server.setSensorTemp";
const string cs4513::Server::SET_SENSOR_HUM_NAME = "server.setSensorHum";
const string cs4513::Server::GET_RELAY_NAME = "server.getRelay";
const string cs4513::Server::GET_PWM_NAME = "server.getPWM";
const string cs4513::Server::get10V_NAME = "server.get10V";
const string cs4513::Server::MixerIsClosing_NAME = "server.MixerIsClosing";
const string cs4513::Server::MixerIsOpening_NAME = "server.MixerIsOpening";
const string cs4513::Server::setRcMode_NAME = "server.setRcMode";
const string cs4513::Server::setRcTemp_NAME = "server.setRcTemp";

/**
 * The destructor.  Frees memory allocated to XML-RPC objects.
 */
Server::~Server() {
	delete abyssServer;
	delete xmlrpcRegistry;
}

/**
 * @return Returns the singleton instance of the Server class.
 */
Server* Server::instance() {
	if (Server::serverInstance == NULL)
		Server::serverInstance = new Server();
	return serverInstance;
}

/**
 * Sets the port on which the server will listen.
 * @param serverPort The server port.
 */
void Server::setServerPort(int serverPort) {
	this->serverPort = serverPort;
}

/**
 * @return Returns true if the server is running, false otherwise.
 */
bool Server::isRunning() const {
	return running;
}

/**
 * Initializes the XML-RPC server and 
 * registers RPC handlers.
 */
void Server::initializeXMLRPCServer() {
	/*
	 * The XML-RPC registry maps RPCs
	 * to their handler functions.
	 */
	xmlrpcRegistry = new xmlrpc_c::registry();

	/*
	 * Register the RPC handlers.
	 * Note that the first parameter to the call to addMethod
	 * is the name of the RPC.  This is the name that the client
	 * will specify when it executes the RPC.  The addMethod 
	 * call associates an RPC handler with an RPC name.
	 */
	xmlrpc_c::methodPtr const shutdownHandler(new ShutdownHandler);
	xmlrpc_c::methodPtr const testHandler(new TestHandler);
	xmlrpc_c::methodPtr const setSensorBrigde(new setSensorTemp);
	xmlrpc_c::methodPtr const setSensorHumBrigde(new setSensorHum);
	xmlrpc_c::methodPtr const getRelayBridge(new getRelay);
	xmlrpc_c::methodPtr const getPWMBridge(new getPWM);
	xmlrpc_c::methodPtr const get10VBridge(new get10V);
	xmlrpc_c::methodPtr const MixerIsClosingBridge(new MixerIsClosing);
	xmlrpc_c::methodPtr const MixerIsOpeningBrigde(new MixerIsOpening);
	xmlrpc_c::methodPtr const setRcModeBridge(new setRcMode);
	xmlrpc_c::methodPtr const setRcTempBridge(new setRcTemp);

	xmlrpcRegistry->addMethod(cs4513::Server::SHUTDOWN_RPC_NAME, shutdownHandler);
	xmlrpcRegistry->addMethod(cs4513::Server::TESTREGLER, testHandler);
	xmlrpcRegistry->addMethod(cs4513::Server::SET_SENSOR_TEMP_NAME,setSensorBrigde );
	xmlrpcRegistry->addMethod(cs4513::Server::SET_SENSOR_HUM_NAME,setSensorHumBrigde );
	xmlrpcRegistry->addMethod(cs4513::Server::GET_RELAY_NAME, getRelayBridge);
	xmlrpcRegistry->addMethod(cs4513::Server::GET_PWM_NAME,getPWMBridge );
	xmlrpcRegistry->addMethod(cs4513::Server::get10V_NAME,get10VBridge );
	xmlrpcRegistry->addMethod(cs4513::Server::MixerIsClosing_NAME, MixerIsClosingBridge);
	xmlrpcRegistry->addMethod(cs4513::Server::MixerIsOpening_NAME,MixerIsOpeningBrigde );
	xmlrpcRegistry->addMethod(cs4513::Server::setRcMode_NAME, setRcModeBridge);
	xmlrpcRegistry->addMethod(cs4513::Server::setRcTemp_NAME,setRcTempBridge );


	// Initialize the XML-RPC server.
	abyssServer = new xmlrpc_c::serverAbyss(
		*xmlrpcRegistry, serverPort, Server::LOG_PATH);
}

/**
 * Starts the XML-RPC server in its own thread.
 */
void Server::run() {
	// Register signal handler for SIGINT to gracefully shut down.
	signal(SIGINT, Server::shutdown);

	// Start the XML-RPC server thread.
	cout << "Server: Starting XML-RPC Server." << endl;
	int status = 
		pthread_create(&serverThread, NULL, Server::runServerThread, abyssServer);
	if (status != 0) {
		cerr << "Server: Error creating XML-RPC server thread.  Exiting." << endl;
		exit(EXIT_FAILURE);
	}

	// Set the server state to running.
	running = true;
}

/**
 * Kills the server thread.
 */
void Server::stop() {
	// Cancel the server thread
	pthread_cancel(serverThread);

	// Wait for the XML-RPC server thread to terminate.
	pthread_join(serverThread, NULL);
}

/**
 * This is the server thread.  It simply starts the XML-RPC server,
 * which will run in this thread until the application is terminated.
 */
void* Server::runServerThread(void* serverPtr) {
	// Cast the thread parameter to an abyss server object.
	xmlrpc_c::serverAbyss* server = static_cast<xmlrpc_c::serverAbyss*>(serverPtr);

	// Start the XML-RPC server in this thread.
	// This thread will run until the server
	// is shut down.
	server->run();

	pthread_exit(EXIT_SUCCESS);
}

/**
 * Shuts down the server by setting the running flag to false.
 */
void Server::shutdown() {
	cout << "Server: Shutting down..." << endl;

	// Set the running flag to false, which will cause the main to exit.
	this->running = false;
}

/**
 * Catches a signal and shuts down the singleton server.
 * @param signum The signal number that was caught.
 */
void Server::shutdown(int signum) {
	cout << "Server: Caught signal " << signum << "." << endl;
	Server::instance()->shutdown();
}

} // end namespace cs4513
