#include "Subsystem.h"

void Subsystem::_ready() {
    std::cout << "Running Subsystem.ready()" << std::endl;
    // We fetch an instance of subject

    // We fetch an instance of blackboard

    // Connect the "on_posted" function to the "posted" function in Blackboard
}

void Subsystem::_on_posted(int key) {
    std::cout << "Running Subsystem._on_posted()" << std::endl;
}

// Locate IP address with the relevant MAC address
std::string Subsystem::findIPFromMAC(std::string targetMAC) {
	// Opens the ARP table as a readable file
	//std::ifstream arpFile("/proc/net/arp");
	std::ifstream arpFile("/home/kali/Documents/ARP_Table.txt");

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

void Subsystem::deleteLineFromFile(std::string target) {
	// Change this to the location of the file in Documents
	std::ifstream inFile("/home/kali/Documents/ARP_Table.txt");
	// Need a temporary file. C++ won't let us simply move all lines up one after we delete the offending line
	// Instead, we'll need to completely rewrite the file
	std::ofstream outFile("/home/kali/Documents/temp.txt");

	// Need to find which line the IP address is on
	std::string line = "";
	int lineNumber = 0;

	while (std::getline(inFile, line)) {
		if (line.find(target) != std::string::npos) {
			std::cerr << "Found \"" << target << "\" on line: " << lineNumber << std::endl;
			break;
		}
		else {
            // Increment line number for each line read
            lineNumber++;
		}
	}

	// Found the line number! Now we know which line to delete
	int currentLineNumber = 0;
	int lineNumberToDelete = lineNumber;

	// Need to reset line back to begining of file
	// Clear any error flags that might have occurred (possibly from reaching the end of the file)
	inFile.clear();
	// Move file pointer to the beginning
    inFile.seekg(0, std::ios::beg);

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
	std::remove("/home/kali/Documents/ARP_Table.txt");
	std::rename("/home/kali/Documents/temp.txt", "/home/kali/Documents/ARP_Table.txt");
}

// If there is a current plan, take the first element in current_plan (next_action) and executes it
// Continues to execute the steps until there are no more remaining.
void Subsystem::execute_plan(std::vector<Response>& current_plan, WorkingMemory& workingMemory) {
	if (current_plan.empty()) {
        std::cerr << "current_plan is empty; not executing" << std::endl;
		return;
	}

	/*
	// Used if we need to delete IP addresses from the ARP table
	std::string findIPFromMAC(std::string targetMAC);
	// For our current fake ARP file testing method
	void deleteLineFromFile(std::string target);
	// This calls procedure, but again, C++ doesn't have Callables, so we'll have to figure that out
	void execute(Response next_action, WorkingMemory& workingMemory);
	*/

	std::cerr << "\n\n------------------------ Executing plan ------------------------\n\n" << std::endl;

	Response next_action = current_plan.front();
	//next_action.execute(next_action, workingMemory);

	if (next_action.name == "block_port") {
		// Maybe keep a list of ports to block, then run through the list and block all of them
		std::cerr << "GORP is going to block a port" << std::endl;

		for (int i = 0; i < workingMemory.port_facts.size(); i++) {
            if(workingMemory.port_facts[i].traffic >= ((workingMemory.averageTraffic * 0.5) + workingMemory.averageTraffic)){
                workingMemory.port_facts[i].traffic = 0;
            }
        }

        // Debugging
        std::cerr << "Final Port Traffic: " << std::endl;
        for (int i = 0; i < workingMemory.port_facts.size(); i++) {
            std::cerr << i + 1 << ") " << workingMemory.port_facts[i].traffic << std::endl;
        }

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
	}
	else if (next_action.name == "block_IP_address") {
		std::cerr << "GORP is going to block an IP address" << std::endl;
		// This will be a bit more complicated than simply detecting if there are multiple IP addresses mapped to one MAC address
		// as we need to actually manipulate the ARP table.
		// First, we remove the offending IP addresses from the table, then we block them.

		std::string ipAddress;
		std::string command;
		int macCount;

		// Scan through workingMemory.macAddresses for > 1 entries
		for (auto pair : workingMemory.ARP_facts) {
			// Use methods similar to what is used in Sensor to scan through the ARP table for IP addresses that have that MAC address
			// pair.second is not a modifiable value, so I'll use an integer variable for this
			macCount = pair.second.count();
			if (macCount > 1) {
				// pair.second is not a modifiable value, so I'll use an integer variable for this
				// macCount = pair.second;
				while (macCount > 0) {
					// Locate IP address with the relevant MAC address and save it in a string
					ipAddress = findIPFromMAC(pair.first);

					// Remove that entry from the ARP table
					//command = "arp -d " + ipAddress;
					//system(command.c_str());

					// In our current format, we'd delete the line from the fake ARP file
					deleteLineFromFile(ipAddress);

					// Using the string, block the IP address
					//command = "sudo iptables -A INPUT -s " + ipAddress + " -j DROP";
					//system(command.c_str());
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

	// Clear sensor.macAddresses so that GORP doesn't get confused with outdated information next time
	//workingMemory.macAddresses.clear();

	std::cerr << "\n****************** COMPLETED PLAN EXECUTION ******************\n\n" << std::endl;
}
