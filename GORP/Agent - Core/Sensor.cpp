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
    if (this->sensorName == "port_sensor"){
        randomizeTraffic();
    }
    else if (this->sensorName == "ARP_sensor"){
        checkARPTable();
    }
    else if (this->sensorName == "file_sensor"){
        getBinFiles();
        getDirectoryFiles();
    }
    else {
        std::cout << "Unknown sensor" << std::endl;
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
        std::cerr << memory.port_facts[i].traffic << std::endl;

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
    memory.ARP_facts.clear();

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
        std::string ip = getIPAddress(line);
        // 00:00:00:00:00:00 is for broadcast MAC addresses, so they can happen more than once and be fine
        // Other than that, just avoid adding empty/invalid MAC addresses to our list
        if (!memory.ARP_facts.contains(mac)){
            memory.ARP_facts.insert(std::make_pair(mac, ARPData()));
        }
        memory.ARP_facts[mac].ip_addresses.push_back(ip);
    }

    // Done with the file, so we can close it
    // We'll check if there are duplicate MAC addresses when we pivot to Agent.cpp
    arpFile.close();
    return 0;
}

void Sensor::getBinFiles(){
    // Clear the vector first, just in case there are leftover files from last time
    memory.file_facts.bin_files.clear();

    std::string path = "/bin";

    // Using "try" and "catch" to handle any possible exceptions that might occur
    // We are digging through system files, after all.
    try {
        for (const auto& entry : std::filesystem::directory_iterator(path)){
            // Check if the entry is a regular file
            if (std::filesystem::is_regular_file(entry.status())){
                // Store the filename, but not the full path
                memory.file_facts.bin_files.push_back(entry.path().filename().string());
            }
        }

        // Print the file names
        /*std::cerr << "Files in " << path << ":" << std::endl;
        for (const std::string& name : memory.file_facts.bin_files){
            std::cerr << name << std::endl;
        }*/
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error accessing directory " << path << ": " << e.what() << std::endl;
    }
}

void Sensor::getDirectoryFiles(){
    // Clear the vector first, just in case there are leftover files from last time
    memory.file_facts.directory_files.clear();

    // Using "try" and "catch" again (see above)
    try {
        // Iterates through current directory
        for (const auto& entry : std::filesystem::directory_iterator(".")){
            // Check if the file is a regular file
            if (std::filesystem::is_regular_file(entry.status())){
                memory.file_facts.directory_files.push_back(entry.path().filename().string());
            }
        }

        std::cerr << "Files in current directory:" << std::endl;
        for (const std::string& name : memory.file_facts.directory_files){
            std::cerr << name << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e){
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
}
