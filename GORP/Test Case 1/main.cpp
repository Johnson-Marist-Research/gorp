#include <iostream>
#include <array>
// Need this for to_string()
#include <string>
// System 
#include <cstdlib>

/*
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

std::string hexHandler(int randomNum) {
	std::string hexNum;

	// Each section of the MAC address has two digits, so we need to generate two numbers
	// Decided to do it this way rather than generate a single number between 16 (0x10 in hex) and 255 (0xFF), then translate it
	for (int i = 0; i < 2; i++) {
		// Generate a random number between 0 and 16 (0 - 15)
		randomNum = rand() % 16;

		if ((randomNum >= 0) && (randomNum <= 9)) {
			hexNum += std::to_string(randomNum);
		}
		else if (randomNum == 10) {
			hexNum += "A";
		}
		else if (randomNum == 11) {
			hexNum += "B";
		}
		else if (randomNum == 12) {
			hexNum += "C";
		}
		else if (randomNum == 13) {
			hexNum += "D";
		}
		else if (randomNum == 14) {
			hexNum += "E";
		}
		else if (randomNum == 15) {
			hexNum += "F";
		}
		else {
			std::cerr << "Something went wrong when generating hex values" << std::endl;
			std::cerr << randomNum << std::endl;
			// Adding an F here as a stopgap (just in case)
			hexNum += "F";
		}
	}

	return hexNum;
}

std::string generateMACAddress(std::string macAddress, int randomNum) {
	// Add a value and a colon to the MAC address five times
	for (int i = 0; i < 5; i++) {
		macAddress += hexHandler(randomNum);
		macAddress += ":";
	}
	// Don't need a colon for the sixth and last value, so will assign it out here
	macAddress += hexHandler(randomNum);

	return macAddress;
}

int main() {
	// For random numbers
	srand(time(0));

	// ------------------ IP ADDRESSES ------------------
	std::string ipAddresses[10];
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


	std::cerr << std::endl;


	// ------------------ MAC ADDRESSES ------------------
	std::string macAddresses[5];
	std::string macAddress = "";
	existingAddress = false;

	for (int i = 0; i < sizeof(macAddresses) / sizeof(macAddresses[0]); i++) {
		// Reset macAddress for next run of the loop
		macAddress = "";
		macAddress = generateMACAddress(macAddress, randomNum);

		// Checking that there isn't another MAC address that is the same as this one
		for (int j = 0; j < sizeof(macAddresses) / sizeof(macAddresses[0]); j++) {
			if (macAddresses[i] == macAddress) {
				// If the address already exists, then we need to make a new one
				existingAddress = true;
				break;
			}
		}

		// Address doesn't already exist, so we are good to put it in the array
		if (existingAddress == false) {
			macAddresses[i] = macAddress;
			std::cerr << "MAC Address " << i << ": " << macAddresses[i] << std::endl;
		}
		else {
			// Address already exists, so we need to redo the element at this index
			existingAddress == false;
			i--;
		}

	}






	// Having trouble figuring out how to make these fake addresses
	// Maybe can share them with the GORP sandbox, have GORP sanbox store them, then manually add them to its ARP table itself?

	// Pick a random MAC address to use with the IP address
	// Send it over as the associated MAC
	// Since there are 10 IP addresses and 5 MAC addresses, there will be overlap

	/* Steps:
		1. Send out a ping to locate the other sandbox
		2. Send a connection request to said sandbox
			- Provide one of the IP addresses, in order from first to last
		3. Have other device send an ARP request (to get MAC address)
			- Provide random MAC address from array
		4. Other sandbox adds information to ARP table
	
	*/

	return 0;
}