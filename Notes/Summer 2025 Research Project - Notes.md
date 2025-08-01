# Summer 2025 Research Project - Notes

## Notes
- Use an adapted goal-oriented action planning program to address problems 
- Have multiple threat scenarios to simulate (Scenario A, Scenario B, etc) 
- Focus on things in the sent email
- Be very careful about preventing memory leaks 
- Some malicious programs hoard memory. For example, a DDoS attack is just a form of resource exhaustion. 
- Pointers: An area of memory that points to another area of memory 
  - Do not let users/programs... 
    - Change where the pointer is pointing to
    - Receive a null value back
    - Force the program to print out debug info
    - Bypass some security feature 
- Use locks to prevent other threads from accessing a variable or value while this thread is using it 
- Research how to put together requirements document
  - Essentials
  - Should have
  - Would be nice to have, but not very important for this project (Ex: Authentication)
 
- For now, do not have GORP wait for user response. That is out of scope for the summer.
 
## GORP Goals and Associated World States
**Goals (as stated in the Solution Strategy Document)**
- Successfully protect a device from cybersecurity threats
  - Associated World States:
    - threat_detected
    - normal_processes
- Inform users when a threat is present
  - Associated World States:
    - is_user_notified
    - all_threats_have_been_notified
    - waiting_for_response
- Perform defensive cybersecurity actions to mitigate the risk of a cyber attack on the device
  - Associated World States:
    - threat_detected
    - ports_temporarily_shut
    - addresses_temporarily_blocked

## Possible Focuses
**Authentication? --> Not included in original email**
- Something you _know_ 
  - Password or phrase
  - PIN 
- Something you have 
  - Code from app or SMS
  - Push notification
  - USB token 
- Something you _are_
  - Finger or thumb print
  - Face scan
  - Iris scan 
- Just one of the three types is usually insecure, but two of the three is considered secure (two-factor authentication)
- *Implementation Ideas*
  - Fairly simple. To begin with, we can use a password to access the agent. If we want to be more secure, we could add two-factor authentication.
  - Agent could monitor failed login attempts. If it detects multiple failed logins from the same IP address, it can temporarily block the IP address. It can notify an admin, and if the admin doesn't unblock/perminantly block the IP address, the IP address will be re-enabled after a few days.

**Scan and close any ports**
- Common practice to close unused ports 
- *Implementation Ideas*
  - Send packages to different ports and analyze their responses to determine if the port is open or closed
  - Close ports that are not in use
    - Filter open ports? Most of the time, data from non-recognized addresses should not be allowed in? 

**Check for sudo in scripts/commands**
- sudo: Allows a normal user to run a program with the security privileges of another user 
- *Implementation Ideas*
  - Check if the user is a sudo user or an administrator
  - Require all sudo users to enter a password. If the user changes sudo configurations, reset the configurations to default after the sudo user exits the application. 

**Reverse shells**
- A shell that is running on one computer but accepts requests and relays responses to another computer
- A type of cyber-attack in which the targeted system is tricked into establishing a connection to the attacker’s computer
- Allows attackers to remotely control the system
- *Implementation Ideas*
  - Check for strange outbound traffic
  - Check connection attempts. Report to administrator if they come from a suspicious location (Ex: A Marist computer is trying to connect to a computer located outside of Marist University)
  - Monitor general traffic. Report to administrator if there is an unusual uptick in traffic. Maybe have the agent close the affected ports for a little bit? 

**Check file integrity**
- Checks a file’s credentials, privileges and security settings, content, core attributes and size, hash values, and configuration variables 
- Implementation varies (I think), but we just have to check the core components of a file. Maybe delete it if it has an unauthorized change
- Files usually have read, write, or execute permissions. Check if a file has execute permission when it should not, isolate/delete the file.

**Check ARP table for odd entries (multiple IPs mapping to one MAC)**
- *Implementation Ideas*
  - Keep a list of all IP addresses and periodically run through it. If a single device is listed under multiple IP addresses, temporarily block those IP addresses and report to network administrator. 

**Domain Name System (DNS) high-jacking**
- DNS translates domain names into IP addresses
  - Ex: “nytimes.com” is translated into an IP address when typed into a browser so users do not have to remember the IP address 
- DNS Tunneling: Uses DNS protocol to communicate non-DNS traffic over the default port for DNS traffic, port 53.
  - Normally sends HTTP and other protocol traffic
  - Can be used to disguise outbound traffic as DNS, concealing data that is typically shared through an internet connection
  - Extract data from a system into the attacker’s infrastructure
  - Can also be used for command and control callbacks from the attacker’s infrastructure into the system 
- DNS Poisoning: Fake information is entered into the cache of a domain name server, resulting in DNS queries producing an incorrect reply and sending users to the wrong website 
- *Implementation Ideas*
  - Hide primary DNS server from public access via network isolation and firewalls
  - Check the information contained in DNS messages. If it doesn’t look like it should be there, investigate.
    - Don’t process input if it isn’t checked as being:
      - SQL/JavaScript/Any injection attack
      - Within the correct length
      - Within the correct data type
    - Close used port, isolate/block associated IP address, report to network administration.
    - If we want to get really in-depth, run a virus scan or something similar. 
  - Filter DNS requests and block malicious domains
  - Only allow network administrators to access the DNS?
  - If something fishy is detected, close the outgoing port while the agent isolates the issue?
  - If we are not a DNS server, can have an option for the agent to be in "safe mode" where it parrots the website it got from the DNS query to the user. If the website does not match, the user has the option to not go to it.

**Check for outdated packages**
- A package is outdated if it no longer receives updates or support from developers 
  - Can have known security vulnerabilities that new, updated packages have fixed 
- *Implementation Ideas*
  - Routinely check packages. If a package is not on a list of supported packages, delete it.
  - If a package stops being supported, remove it from the list of supported packages and run through the packages to delete them. 

**Check for hidden files and directories**
- *Implementation Ideas* 
  - Regularly run a virus scan on the hidden files
  - Check to see if their size has increased suspiciously since they were last checked? 

**Check for gaps in logs**
- *Implementation Ideas* 
  - Have the agent log everything it does
  - Have a clear amount of time logs must be retained
  - Occasionally review logs
  - Analyze the logs for gaps. If a gap is found, find the file associated with the gap and run a virus scan to check for suspicious activity.
  - Have agent analyze their own logs. It makes a note of when it starts and stops. If there is a gap before it stops doing something, then there is a problem.

 ## Scenario Brainstorming for World States
 
Scan, and potentially close, various ports
- Initial World State: port_open
- Target World State: port_closed
  - Change in World State triggers when unusual traffic is detected from a port. GORP will change the port from open to closed after ending the high-traffic processes traveling through the port.
  - Not sure if there would be a "port_open"/"port_closed" World State for each individual port

Check ARP table for odd entries
- Initial World State: no_ARP_anomalies
- Target World State: ARP_anomaly_quarantined
  - GORP will scan the ARP table to check for odd entries, such as multiple IP addresses being mapped to a single MAC address. If such an event is detected, actions must be taken to isolated the affected IP addresses.

Monitor for unusual traffic
- Unsure whether we will keep this one as a separate process from "Scanning ports", so will add World States later if we decide to keep it.

Check permissions of files and notify the user if a file has an unusual or altered permission
- Initial World State: files_unchanged
- Target World State: change_detected
  - If a file is detected to have changed permissions from the last time it was scanned, GORP can make note of the change and decide whether or not to revert the file to its original permissions. Once the file scan is complete, "change_detected" can be reverted back to "files_unchanged".

Check for gaps in its own logs
- Initial World State: no_gaps
- Target World State: gap_detected
  - If a gap in the logs is detected, then GORP must deduce how the gap occurred. While it is fixing this, "gap_detected" is true. Once the issue has either been fixed or GORP has restarted, "gap_detected" will revert back to "no_gaps".

Enter a "safe mode" where it parrots website names back to the user to determine if the DNS address was correctly translated
- Initial World State: general_mode
- Target World State: safe_mode
  - Safe mode could potentially be annoying, so maybe we can allow a feature to turn it off and on.
- Intitial World State: dns_match
- Target World State: dns_mismatch
  - If "dns_mismatch" comes back as true, then the returned DNS address does not match the expected one. While "dns_mismatch", send another DNS request. Scrap the entire process if more than a certain number of requests come back as a mismatch.

 
## Misc. Notes From Day 2
- *Network Security*: Any action intended to safeguard the integrity and usefulness of data and the network 
  - Levels of Network Security:
    - Physical Network Security: Focuses the data and network
      - Protects from unauthorized personnel acquiring control over the confidentiality of the network
    - Technical Network Security: Focuses on protecting the data stored in the network/data involved in transitions through the network
      - Protects from unauthorized users and malicious activities
    - Administrative Network Security: Focuses on user behavior such as how permissions are granted and how authorization processes take place
      - Protects the network through attacks
  - Types of Network Security:
    - Email Security
    - Network Segmentation: Network traffic is divided into several categories that each enforce security regulations
    - Access Control: Network is not accessible to every user. Endpoint devices that are noncompliant with security policies might have their access restricted or blocked.
    - Sandboxing: Files are opened or code is performed on a host computer that simulates end-user environments in a secure, isolated environment.
    - Cloud Network Security: Data protected while on the cloud
    - Web Security: Restricts access to harmful websites, stops web-based risks, and manages staff internet usage. Web gateway should be safeguarded both locally and in the cloud.
    - Intrusion Prevention System (IPS): Monitors network or system activities for malicious activity
    - Antivirus and Anti-Malware Software: Ensures that malicious software does not enter the network and threaten the security of the data
    - Firewall: A network security device which monitors all incoming and outgoing traffic and, based on a defined set of security rules, rejects or drops specific traffic
    - Application Security: Security measures used at the application level
    - Wireless Security: Wireless networks are less secure than wired ones. Need dedicated products to protect it from exploits and unauthorized access 

- *Packet Sniffing*: Act of capturing data packets across the computer network 
  - Done by using packet sniffers. Can be filtered or unfiltered. 
    - Filtered: Specific data packets have to be captured
    - Unfiltered: All packets must be captured
    - Ex: WireShark, SmartSniff 
  - How to Prevent: 
    - Encrypt data you send or receive
    - Use trusted Wi-Fi networks
    - Scan network for dangers or issues
  
- *Address Resolution Protocol (ARP)*: A communication protocol used for discovering the link layer address, such as a MAC address, given an IP address 
- *ARP Cache Poisoning Attack*: Attackers fool the victim into accepting forged IP-to-MAC mappings 
  - Can cause the victim’s packets to be redirected to the computer with the forged MAC address

- *ICMP Redirect*: An error message sent by a router to the sender of an IP packet 
  - Used when a router believes a packet is being routed incorrectly
  - Informs the sender that it should use a different router for the subsequent packets sent to the same destination
  - Can be used by attackers to change a victim’s routing

- DNS translates a hostname to IP address and vice versa through DNS resolution 
- *DNS Pharming Attack*: Manipulate the resolution with an intent to misdirect users to alternate destinations 
- Remote DNS Cache Poisoning Attack (aka Kaminsky DNS Attack): More challenging than a local attack since packet sniffing is not possible 
