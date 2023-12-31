# NetScent

NetScent is a packet sniffer designed to monitor and analyze local network traffic.  

NetScent allows users to view a continuous stream of network information from packets intercepted on their network adapter. This information includes IP addresses, packet types, and packet-specific data. NetScent can also determine approximate geographical and organizational data for intercepted (public) IP addresses, which it can plot on a Google Maps-style display.


This project is currently in an early, unfinished state, but I will continue adding features and fixes over time.  

![NetScentMainMenu](https://github.com/rnofal1/NetScent/assets/57466011/8dfe5a26-9c06-4507-bb5d-1db9ce60036d)  
*main menu after capturing some packets*

![map_screenshot](https://github.com/rnofal1/NetScent/assets/57466011/a180cb33-ebe2-4be6-bf0d-f363d6e097c8)
*interactive ip geolocation map with a couple of pinged locations*

![map_info_screenshot](https://github.com/rnofal1/NetScent/assets/57466011/27fe3ae8-1e6e-4118-a1e5-7b85c047e917)  
*hovering over a map marker reveals some info*

<video src="https://github.com/rnofal1/NetScent/assets/57466011/a9fb4715-47d8-4319-8073-20e3072ddcfa" placeholder="https://github.com/rnofal1/NetScent/assets/57466011/a9fb4715-47d8-4319-8073-20e3072ddcfa" autoplay loop controls muted title="NetScentDemo">
Sorry, your browser doesn't support HTML 5 video.
</video> 

*small demo of the current UI*

## Purpose
This is a personal project, its main purpose is as a platform for me to use and improve my programming and C/C++ skills. However, in service of that purpose I will try to make this program as usable, useful, accessible, and performant as possible.

## How to Use
1. Ensure that you have an updated version of Windows 10 or 11 installed*
2. Download and unzip the latest (Windows) [release](https://github.com/rnofal1/NetScent/releases/tag/v0.0.4(WIN)) (**NetScent_Release_WIN_v0.0.4.zip**)
3. Open the new `NetScent_Release_WIN_v0.0.4` folder
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
- Implement CI/CD pipeline on Github

## Links
IP Geolocation API: https://ipgeolocation.io/  

Pcap basics: https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/

Loading icon source: http://www.staredit.net/topic/16487/

Menu background made with Dall-E 2: https://openai.com/dall-e-2  
  
## Misc  
### A small note on style:  
In order to better match STL style conventions, the default naming convention for functions and variables in this project is snake_case. Qt functions and variables typically utilize lowerCamelCase. Class names (Qt and otherwise) are written in UpperCamelCase.  



