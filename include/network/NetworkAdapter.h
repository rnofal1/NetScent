/* This file contains declarations for the NetworkAdapter class
 *
 * Ramsey Nofal, 08/2023
 */


#ifndef NETWORKADAPTER_H
#define NETWORKADAPTER_H


/* Standard C/C++ */
#include <sstream>
#include <iostream>

/* 3rd-Party */
#include <winsock2.h>
#include <iphlpapi.h> //https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/


/* The NetworkAdapter class stores info about a physical network device and provides
 * methods to access that info. Its purpose is simply to provide a more intuitive interface
 * than the built-in Windows (Win32) adapter info functions.
 *
 * Its construction is derived from:
 * https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersinfo
 *
 */
class NetworkAdapter {
public:
    /* Functions */
    NetworkAdapter( const std::string& name,
                    const std::string& desc,
                    const std::string& phys_addr,
                    const std::string& type,
                    const std::string& ip_addr,
                    const std::string& ip_mask,
                    const std::string& gateway,
                    const std::string& dhcp_server,
                    const std::string& time_lease_obtained,
                    const std::string& time_lease_expires,
                    const std::string& primary_wins_server,
                    const std::string& secondary_wins_server,
                    const u_int& combo_index,
                    const u_int& index,
                    const bool& dhcp_enabled,
                    const bool& has_wins);
    NetworkAdapter(const PIP_ADAPTER_INFO& net_adapter);
    NetworkAdapter();

    std::string get_gateway();
    std::string get_name();
    std::string get_desc();
    std::string get_all_info() const;

    void print_all_info();

private:
    /* Variables */
    std::string name;
    std::string desc;
    std::string phys_addr;
    std::string type;
    std::string ip_addr;
    std::string ip_mask;
    std::string gateway;
    std::string dhcp_server;
    std::string time_lease_obtained;
    std::string time_lease_expires;
    std::string primary_wins_server;
    std::string secondary_wins_server;

    u_int combo_index;
    u_int index;

    bool dhcp_enabled;
    bool has_wins;

};

#endif // NETWORKADAPTER_H
