#include "Response.h"

#include <iostream>
// Needed for using system calls
#include <cstdlib>
// Used for checking if a file exists
#include <filesystem>

// Initialize the Response variables
Response::Response(std::string name, float cost, WorldState before, WorldState after) {
	this->name = name;
	this->cost = cost;
	this->preconditions = before;
	this->effects = after;
	//this->procedure = proc;
};

// Locate IP address with the relevant MAC address
std::string Response::findIPFromMAC(std::string targetMAC) {
	// Opens the ARP table as a readable file
	//std::ifstream arpFile("/proc/net/arp");
	std::ifstream arpFile("/Documents/ARP_Table.txt");

	std::cerr << "\nTarget MAC Address: " << targetMAC << std::endl;

	// Couldn't locate the ARP table
	if (!arpFile.is_open()) {
		std::cerr << "Could not locate /proc/net/arp" << std::endl;
		return "";
	}

	// For more information on how these lines of code work, see Sensor.checkARPTable()
	std::string line;
	std::getline(arpFile, line);

	while (std::getline(arpFile, line)) {
		std::stringstream ss(line);
		std::string ipAddress, hwType, flags, macAddress, mask, device;

		// Parse the line according to the /proc/net/arp format
		ss >> ipAddress >> hwType >> flags >> macAddress >> mask >> device;
		std::cerr << "\nLine read from file: " << 
			"\n\t- IP Address: " << ipAddress << 
			"\n\t- HW Type: " << hwType << 
			"\n\t- Flags: " << flags << 
			"\n\t- MAC Address: " << macAddress << 
			"\n\t- Mask: " << mask << 
			"\n\t- Device: " << device << "\n" << std::endl;

		// Return the IP address if we find the MAC address
		if (macAddress == targetMAC) {
			arpFile.close();
			return ipAddress;
		}
	}

	arpFile.close();
	// If the MAC address is not found
	return "";
}

void Response::deleteLineFromFile(std::string target) {
	// Change this to the location of the file in Documents
	std::ifstream inFile("/home/kali/Documents/ARP_Table.txt");
	// Need a temporary file. C++ won't let us simply move all lines up one after we delete the offending line
	// Instead, we'll need to completely rewrite the file
	// Check if the file exists before we make a new one (delete old file if it already exists)
	if (std::filesystem::exists("/home/kali/Documents/temp.txt")) {
		std::remove("/home/kali/Documents/temp.txt");
	}
	std::ofstream outFile("/home/kali/Documents/temp.txt");

	// Need to find which line the IP address is on
	std::string line;
	int lineNumber = 0;

	while (std::getline(inFile, line)) {
		// Increment line number for each line read
		lineNumber++;
		if (line.find(target) != std::string::npos) {
			std::cout << "Found \"" << target << "\" on line: " << lineNumber << std::endl;
			break;
		}
	}

	// Found the line number! Now we know which line to delete
	int currentLineNumber = 0;
	int lineNumberToDelete = lineNumber;

	// Write to the temporary file, skipping the line to be deleted
	while (std::getline(inFile, line)) {
		if (currentLineNumber != lineNumberToDelete) {
			outFile << line << std::endl;
		}
		currentLineNumber++;
	}

	// Can now close both files
	inFile.close();
	outFile.close();

	// Delete the old file and rename the new one
	//std::remove("/home/kali/Documents/ARP_Table.txt");
	//std::rename("/home/kali/Documents/temp.txt", "/home/kali/Documents/ARP_Table.txt");
}

void Response::execute(Response next_action, std::map<std::string, int> macAddresses) {
	// macAddresses will do nothing unless this is in relation to ARP Spoofing
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
		// This will be a bit more complicated than simply detecting if there are multiple IP addresses mapped to one MAC address
		// as we need to actually manipulate the ARP table.
		// First, we remove the offending IP addresses from the table, then we block them.

		std::string ipAddress;
		std::string command;
		int macCount;

		// Scan through sensor.macAddresses for > 1 entries
		for (const auto& pair : macAddresses) {
			// Use methods similar to what is used in Sensor to scan through the ARP table for IP addresses that have that MAC address
			if (pair.second > 1) {
				// pair.second is not a modifiable value, so I'll use an integer variable for this
				macCount = pair.second;
				while (macCount > 0) {
					// Locate IP address with the relevant MAC address and save it in a string
					ipAddress = findIPFromMAC(pair.first);

					// Remove that entry from the ARP table
					//command = "arp -d " + ipAddress;
					//system(command.c_str());

					// In our current format, we'd delete the line from the fake ARP file
					deleteLineFromFile(ipAddress);

					// Using the string, block the IP address
					command = "sudo iptables -A INPUT -s " + ipAddress + " -j DROP";
					system(command.c_str());
					// Subtract 1 from pair.second
					macCount--;
				}
			}
		}
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
