#include "Sensor.h"

Sensor::Sensor(std::string selectedName, WorkingMemory& mem) : memory(mem){
    std::cout << "Running Sensor constructor" << std::endl;

    sensorName = selectedName;

    // For random numbers
    // Need to do it here and not in the function, otherwise the "random" numbers will always be the same
    srand(time(0));

    // ----------------------------- ARP Table -----------------------------
    // Populate the ARP table file with the resources from ARP_Table_Ref.txt
	std::ifstream inFile("/home/kali/Documents/ARP_Table_Ref.txt");

	// Check if the file exists before we make a new one (delete old file if it already exists)
	if (std::filesystem::exists("/home/kali/Documents/ARP_Table.txt")) {
		std::remove("/home/kali/Documents/ARP_Table.txt");
	}
	std::ofstream outFile("/home/kali/Documents/ARP_Table.txt");

	// Need to find which line the IP address is on
	std::string line;

	// Found the line number! Now we know which line to delete
	int currentLineNumber = 0;

	// Write to the temporary file, skipping the line to be deleted
	while (std::getline(inFile, line)) {
		outFile << line << std::endl;
		currentLineNumber++;
	}

	// Can now close both files
	inFile.close();
	outFile.close();
}

void Sensor::sense(){
    if (currentSensor.sensorName == "port_sensor"){
        randomizeTraffic(memory.ports);
    }
    else if (currentSensor.sensorName == "ARP_sensor"){
        checkARPTable(memory.macAddresses);
    }
}

void Sensor::randomizeTraffic(){
    // ----------------------------- PORTS -----------------------------
    std::cerr << "\nNew Ports:" << std::endl;
    // Populate ports{} with some test cases
    for (int i = 0; i < 10; i++) {
        // Random number between 0 and 100
        int randomNum = rand() % 101;
        // i = port number / key, randomNum = traffic
        // ports[i] = randomNum;
        //ports.insert({ i, randomNum });
        memory.port_facts[i] = PortData(randomNum, true);
        std::cerr << memory.port_facts[i].traffic << std::edl;

        // Creating new entry in portVector to correspond to new entry in ports
        /*PortVars newPortValues;
        newPortValues.port_open = true;
        newPortValues.port_blocked = false;
        newPortValues.normal_traffic = true;
        newPortValues.excess_traffic_detected = false;
        // Adding corresponding value to portVarsVector
        portVector.push_back(newPortValues);*/
    }
}

// Easier to do the parsing in a stand-alone function
std::string Sensor::getMACAddress(std::string line) {
    // Allows us to read through a string like its one continuous input/output stream
    std::stringstream ss(line);
    // Makes a bunch of different string variables at once. Takes up less space than creating six lines of "std::string...."
    // Each of these strings correspond to one of the six sections of the MAC address
    std::string ip, hw_type_string, flagsString, macAddress, mask, device;
    // Converts information from file
    ss >> ip >> hw_type_string >> flagsString >> macAddress >> mask >> device;
    return macAddress;
}

std::string Sensor::getIPAddress(std::string line) {
    // Allows us to read through a string like its one continuous input/output stream
    std::stringstream ss(line);
    // Makes a bunch of different string variables at once. Takes up less space than creating six lines of "std::string...."
    // Each of these strings correspond to one of the six sections of the MAC address
    std::string ip, hw_type_string, flagsString, macAddress, mask, device;
    // Converts information from file
    ss >> ip >> hw_type_string >> flagsString >> macAddress >> mask >> device;
    return ip;
}

// ----------------------------- ARP Table -----------------------------
int Sensor::checkARPTable() {
    std::cerr << "\n\nChecking ARP Table\n\n" << std::endl;

    // Clearing MAC addresses in case there is old data from a previous scan
    // macAddresses.clear();
    ARP_facts.clear();

    std::string ip = getIPAddress(line);

    // ARP Table on Linux can be read like a text file by using "/proc/net/arp"
    // We use ifstream to read it
    //std::ifstream arpFile("/proc/net/arp");
    std::ifstream arpFile("/home/kali/Documents/ARP_Table.txt");

    if (!arpFile.is_open()) {
        std::cerr << "Could not locate /proc/net/arp" << std::endl;
        // Indicates that the file couldn't be opened for some reason
        return 1;
    }

    std::string line;
    // Skips the header line (doesn't have any information we need)
    std::getline(arpFile, line);

    while (std::getline(arpFile, line)) {
        std::string mac = getMACAddress(line);
        // 00:00:00:00:00:00 is for broadcast MAC addresses, so they can happen more than once and be fine
        // Other than that, just avoid adding empty/invalid MAC addresses to our list
        if (!memory.ARP_facts.contains(mac)){
            memory.ARP_facts[mac] = ARPData(ip, mac);
        }
        memory.ARP_facts[mac] = ip_addresses.push_back(ip)
    }

    // Done with the file, so we can close it
    // We'll check if there are duplicate MAC addresses when we pivot to Agent.cpp
    arpFile.close();
    return 0;
}
