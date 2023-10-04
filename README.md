# *This is the Windows port of my original Linux-based NetScent program  
This will eventually be the main, supported version of NetScent (but currently it is experimental) 

# NetScent

NetScent is a packet analyzer/sniffer designed to monitor local network traffic.  

NetScent allows users to view a continuous stream of network information, including source and destination IP addresses and approximate geographic locations, for devices involved in the exchange of intercepted packets.

This project is currently in an early, unfinished state, but I will continue adding features and fixes over time.

![Screenshot from 2023-09-03 19-54-41](https://github.com/rnofal1/NetScent/assets/57466011/87ff2a84-aeb7-42a8-a6b4-6440f5320bc3)  
*main menu after capturing some packets*

![netscent_demo_new](https://github.com/rnofal1/NetScent/assets/57466011/388fe346-9f46-4b17-9938-64787ac4ae81)  
*small demo of the current UI*


## Purpose
This is a personal project, its main purpose is as a platform for me to use and improve my programming and C/C++ skills. However, in service of that purpose I will try to make this program as usable, useful, accessible, and performant as possible.

## How to Use
1. Ensure that you have an updated version of Windows 10 or 11 installed*
2. Download and unzip the latest (Windows) [release](https://github.com/rnofal1/NetScent/releases/tag/v0.0.1(WIN)) (**NetScent_Release_WIN_v0.0.1.zip**)
3. Open the new `NetScent_Release_WIN_v0.0.1` folder
4. Right-click `NetScent.exe`
5. Click `Run as administrator`
6. Click `Yes`
7. In the "Settings" tab, enter your API key for [IP geolocation](https://ipgeolocation.io/)
8. In the "Capture" tab click "Start Capture"

*Not guaranteed to work on all systems or configurations

## Rough Roadmap
- Improve IP geolocation 
- Add more options for data analysis
- Add proper testing framework
- Build CI/CD pipeline (?)
- Implement graphical improvements

## Links
IP Geolocation API: https://ipgeolocation.io/  

Pcap basics: https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/

Loading icon source: http://www.staredit.net/topic/16487/

Menu background made with Dall-E 2: https://openai.com/dall-e-2  
  
## Misc  
### A small note on style:  
In order to better match STL style conventions, the default naming convention for functions and variables in this project is snake_case. Qt functions and variables typically utilize lowerCamelCase. Class names (Qt and otherwise) are written in UpperCamelCase.  



