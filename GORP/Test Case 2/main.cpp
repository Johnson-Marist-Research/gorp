#include <iostream>
#include <string>
// Timer
#include <thread>
// System 
#include <cstdlib>


/*
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

std::string generateIPAddress(std::string ipAddress, int randomNum) {
	// Random number, either 0 or 1. We will use this to decide on the first part of the IP address
	randomNum = rand() % 2;

	// These are private IP addresses that are distributed to devices within a private network
	// Normally it is the network computer, not the requesting device, that assigns the private IP address,
	// but we can just copy this if we want to move the action of assigning addresses to GORP.
	if (randomNum == 0) {
		ipAddress += "172.";
	}
	else if (randomNum == 1) {
		ipAddress += "192.";
	}
	else {
		// Just in case I misunderstood rand()
		ipAddress += "10.";
	}

	// Random number between 0 and 256. We will use this to decide on the first part of the IP address
	// Note: Should not choose 256 as an option
	randomNum = rand() % 256;
	ipAddress += std::to_string(randomNum);
	ipAddress += ".";

	// Repeat for third part of IP address
	randomNum = rand() % 256;
	ipAddress += std::to_string(randomNum);
	ipAddress += ".";

	// Repeat for fourth and final part of IP address
	randomNum = rand() % 256;
	ipAddress += std::to_string(randomNum);

	return ipAddress;
}

int main() {
	// ------------------ CREATING IP ADDRESSES ------------------
	// For random numbers
	srand(time(0));

	std::string ipAddresses[20];
	std::string ipAddress = "";

	int randomNum = 0;
	bool existingAddress = false;

	// sizeof() gets the size of the array in bytes
	// There are four bits in every entry
	// Therefore, if we divide sizeof(ipAddresses) by the size of one of its entries (in this case, the first one);
	// then we get the number of items in the array
	for (int i = 0; i < sizeof(ipAddresses) / sizeof(ipAddresses[0]); i++) {
		// Reset ipAddress for next run of the loop
		ipAddress = "";
		ipAddress = generateIPAddress(ipAddress, randomNum);

		// Checking that there isn't another IP address that is the same as this one
		for (int j = 0; j < sizeof(ipAddresses) / sizeof(ipAddresses[0]); j++) {
			if (ipAddresses[i] == ipAddress) {
				// If the address already exists, then we need to make a new one
				existingAddress = true;
				break;
			}
		}

		// Address doesn't already exist, so we are good to put it in the array
		if (existingAddress == false) {
			ipAddresses[i] = ipAddress;
			std::cerr << "IP Address " << i << ": " << ipAddresses[i] << std::endl;
		}
		else {
			// Address already exists, so we need to redo the element at this index
			existingAddress == false;
			i--;
		}
	}

	// ------------------ LOOP ------------------
	// Useful code for making a pause timer
	using namespace std::this_thread;
	// Allows us to use seconds as a measurement
	using namespace std::chrono;

	std::string command;

	// So the purpose of this loop is to spam the other device
	// As such, I'm not giving it a stop for now
	// We have to stop the program entirely in order to kill it
	while (true) { 
		// Doesn't need to be as sophisticated as Test Case 1
		// Can just spam the other machine with pings
		//sleep_for(seconds(1));
		sleep_for(milliseconds(100));
		std::cerr << "   /" <<std::endl;
		sleep_for(milliseconds(100));
		std::cerr << "  /" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << " /" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << "/" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << "\\" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << " \\" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << "  \\" << std::endl;
		sleep_for(milliseconds(100));
		std::cerr << "   \\" << std::endl;

		for (int i = 0; i < sizeof(ipAddresses) / sizeof(ipAddresses[0]); i++) {
			// Will need the IP address of the other sandbox
			command = "ping "; // Need to add "+ <IP address of other sandbox>"
			// Pings other device (simulating once for every IP address here)
			//system(command.c_str());
		}
	}

	return 0;
}