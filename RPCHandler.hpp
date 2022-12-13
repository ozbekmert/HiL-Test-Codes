#ifndef _RPC_HANDLER_HPP
#define _RPC_HANDLER_HPP

// XML-RPC includes
#include <xmlrpc-c/registry.hpp>

// Define the RPC handler classes to be in the cs4513 namespace
namespace cs4513 {

/**
 * Adds two integers together and returns their sum.
 * @return Returns the sum of the two integers.
 */

class setSensorTemp : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class setSensorHum : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class getRelay : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class getPWM : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class get10V : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class MixerIsClosing : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class MixerIsOpening : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class setRcMode : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};
class setRcTemp : public xmlrpc_c::method {
public:
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);
};

/**
 * Performs any tasks necessary to cleanly shut down the server.
 * @return Returns true.
 */
class ShutdownHandler : public xmlrpc_c::method {
public:
	/**
	 * This method is called when the server.shutdown RPC is handled.
	 * Calls the Server::shutdown() method.
	 * @param paramList List of RPC parameters.
	 * @param retvalP Pointer to the return value of the method.  Always returns true.
	 */
	void execute(xmlrpc_c::paramList const&, xmlrpc_c::value* const);




};
class TestHandler : public xmlrpc_c::method {
public:
	/**
	 * This method is called when the server.shutdown RPC is handled.
	 * Calls the Server::shutdown() method.
	 * @param paramList List of RPC parameters.
	 * @param retvalP Pointer to the return value of the method.  Always returns true.	 */

	void execute(xmlrpc_c::paramList const& params, xmlrpc_c::value* const);



};



} // end namespace cs4513

#endif // #define _RPC_HANDLER_HPP
