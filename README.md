# NetScent

NetScent is a packet analyzer/sniffer designed to monitor local network traffic.  

NetScent allows users to view a continuous stream of network information, including source and destination IP addresses and approximate geographic locations, for devices involved in the exchange of intercepted packets.

This project is currently in an early, unfinished state, but I will continue adding features and fixes over time.

![Screenshot from 2023-09-03 19-54-41](https://github.com/rnofal1/NetScent/assets/57466011/87ff2a84-aeb7-42a8-a6b4-6440f5320bc3)  
*main menu after capturing some packets*

![netscent_demo_2](https://github.com/rnofal1/NetScent/assets/57466011/2a195ef1-9eb1-46ae-8687-d561f71054d7)  
*small demo of the current UI*


## Purpose
This is a personal project, its main purpose is as a platform for me to use and improve my programming and C/C++ skills. However, in service of that purpose I will try to make this program as usable, useful, accessible, and performant as possible.

## How to Use
1. Ensure that you have an updated version of Linux installed*
2. Download dependencies:
    - In a terminal, run the command: ```sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev libboost-all-dev``` 
3. Download and unzip the latest [release](https://github.com/rnofal1/NetScent/releases/latest) (**NetScent_Release_v0.0.3.zip**)
4. In a terminal:
    - ```cd``` into the release folder
    - run the following commands (or similar):
        - ```chmod 777 NetScent.sh bin/NetScent```     
        - ```sudo ./NetScent.sh```
5. In the "Settings" tab, enter your API key for [IP geolocation](https://ipgeolocation.io/)
6. In the "Capture" tab click "Start Capture"

*Not guaranteed to work on all systems or configurations (Tested on a fresh install of **Ubuntu 22.04.2** running within VirtualBox)

## Rough Roadmap
- Improve IP geolocation 
- Add more options for data analysis
- Add proper testing framework
- Build CI/CD pipeline (?)
- Implement graphical improvements
- Port to Windows

## Links
IP Geolocation API: https://ipgeolocation.io/  

Pcap basics: https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/

Qt Linux deployment helper: https://github.com/QuasarApp/CQtDeployer

Loading icon source: http://www.staredit.net/topic/16487/

Menu background made with Dall-E 2: https://openai.com/dall-e-2



