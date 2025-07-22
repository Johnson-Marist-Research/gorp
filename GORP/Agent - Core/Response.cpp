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

/*
TEST CASES:



ATTACKS:
IP/MAC Spoofing
	- Goal: Have another sandbox send the GORP sandbox several IP addresses registered under the same MAC addresses
		- Mix it up by having a few legitimate 1:1's in there too
	- Implementation:
		- Can have the sandbox register a specific amount of IP addresses (10?)
		- Randomly generate half that in MAC addresses (so 5 in this example)
		- Generate a random IP address, then roll a number between 1 and 5.
			- Assign the IP address to the corresponding MAC (MAC option 1 - 5)
		- Optionally print out the combinations (so we know what we generated)
	- This should give us some overlap
	- If we wanted to check that 1:1's are actually getting through, we could have a static unique IP and MAC address combo

DDOS Attack
	- Goal: Have one sandbox spam the GORP sandbox with a substantial amount of packets
	- Implementation:
		- Randomly generate a certain amount of IP addresses (How many can we get away with? More than 20?)
		- Have each of them spam GORP with traffic of any kind
			- Connection requests, information, anything that works in our sandboxes
			- If we are doing ports, aim at one or two specific ports
				- This is our "website" port
	- Might have to have these on a timer so we don't blow up the actual computer this is running on,
	  but can scale the threshold for "too much traffic" to accommodate
*/