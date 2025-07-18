#include "Response.h"

#include <iostream>
// Needed for using system calls
#include <cstdlib>

// Initialize the Response variables
Response::Response(std::string name, float cost, WorldState before, WorldState after) {
	this->name = name;
	this->cost = cost;
	this->preconditions = before;
	this->effects = after;
	//this->procedure = proc;
};

void Response::execute(Response next_action) {
	std::cout << "Running Response.execute()" << std::endl;

	// procedure.call()
	
	/* Execution Idea :
		- Use next_action (from Agent) in if/else statements to determine relevent actions
		- Utilize Linux commands to perform said actions
	*/

	/* Response names:
		- block_port
		- unblock_port
		- block_IP_address
		- unblock_IP_address
		- revert_file
		- update_file
		- switch_to_gen_mode
		- switch_to_safe_mode
		- block_dns_response
	*/

	if (next_action.name == "block_port") {
		// Maybe keep a list of ports to block, then run through the list and block all of them
		std::cerr << "GORP is going to block a port" << std::endl;

		/* System calls: 
		// Find processes on the port
		system(findstr :<PORT_NUMBER>);
		// End processes
		system(taskkill /<PORT_NUMBER> /F);
		// Add a firewall to block access to specified port
		system(netsh advfirewall firewall add rule name="Block Port <PORT_NUMBER>" dir=in action=allow protocol=TCP localport=<PORT_NUMBER>);
		*/

		// NOTE: Might have to adjust this if we decide to make a list of ports to be blocked

	}
	else if (next_action.name == "unblock_port") {
		std::cerr << "GORP is going to unblock a port" << std::endl;

		// Process will be the opposite of above; check that it works before copying here
	}
	else if (next_action.name == "block_IP_address") {
		std::cerr << "GORP is going to block an IP address" << std::endl;
	}
	else if (next_action.name == "unblock_IP_address") {
		std::cerr << "GORP is going to unblock an IP address" << std::endl;
	}
	else if (next_action.name == "revert_file") {
		std::cerr << "GORP is going to revert a file's changes" << std::endl;
	}
	else if (next_action.name == "update_file") {
		std::cerr << "GORP is going to update its version of a file to the file's current iteration" << std::endl;
	}
	else if (next_action.name == "switch_to_gen_mode") {
		std::cerr << "GORP is going to switch to general mode" << std::endl;
	}
	else if (next_action.name == "switch_to_safe_mode") {
		std::cerr << "GORP is going to switch to safe mode" << std::endl;
	}
	else if (next_action.name == "block_dns_response") {
		std::cerr << "GORP is going to block a DNS response" << std::endl;
	}
	else {
		std::cerr << "The name of the desired next action is not recognized" << std::endl;
	}
}