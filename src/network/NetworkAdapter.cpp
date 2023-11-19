/* This file contains definitions for the NetworkAdapter class
 *
 * Ramsey Nofal, 08/2023
 */


//Local
#include "NetworkAdapter.h"

NetworkAdapter::NetworkAdapter(const std::string& name,
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
                               const bool& has_wins)
    : name(name),
    desc(desc),
    phys_addr(phys_addr),
    type(type),
    ip_addr(ip_addr),
    ip_mask(ip_mask),
    gateway(gateway),
    dhcp_server(dhcp_server),
    time_lease_obtained(time_lease_obtained),
    time_lease_expires(time_lease_expires),
    primary_wins_server(primary_wins_server),
    secondary_wins_server(secondary_wins_server),
    combo_index(combo_index),
    index(index),
    dhcp_enabled(dhcp_enabled),
    has_wins(has_wins){}

// Adapted from https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersinfo
//ToDo: change to https://learn.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses (?)
NetworkAdapter::NetworkAdapter(const PIP_ADAPTER_INFO& net_adapter)
    : name(net_adapter->AdapterName),
    desc(net_adapter->Description),
    ip_addr(net_adapter->IpAddressList.IpAddress.String),
    ip_mask(net_adapter->IpAddressList.IpMask.String),
    gateway(net_adapter->GatewayList.IpAddress.String),
    combo_index(net_adapter->ComboIndex),
    index(net_adapter->Index) {

    // Variables used to get DHCP time info
    struct tm newtime;
    char buffer[32];
    errno_t error;

    //phys_addr
    std::stringstream adapter_addr_ss;
    for (size_t i = 0; i < net_adapter->AddressLength; ++i) {
        if (i == (net_adapter->AddressLength - 1)) {
            adapter_addr_ss << std::hex << (int)net_adapter->Address[i];
        } else {
            adapter_addr_ss << std::hex << (int)net_adapter->Address[i] << '-';
        }
    }
    phys_addr = adapter_addr_ss.str();

    //type
    switch (net_adapter->Type) {
    case MIB_IF_TYPE_OTHER:
        type = "Other";
        break;
    case MIB_IF_TYPE_ETHERNET:
        type = "Ethernet";
        break;
    case MIB_IF_TYPE_TOKENRING:
        type = "Token Ring";
        break;
    case MIB_IF_TYPE_FDDI:
        type = "FDDI";
        break;
    case MIB_IF_TYPE_PPP:
        type = "PPP";
        break;
    case MIB_IF_TYPE_LOOPBACK:
        type = "Loopback";
        break;
    case MIB_IF_TYPE_SLIP:
        type = "Slip";
        break;
    default:
        type = "Unknown";
        break;
    }

    //dhcp_enabled
    if(net_adapter->DhcpEnabled) {
        dhcp_enabled = true;

        //dhcp_server
        dhcp_server = net_adapter->DhcpServer.IpAddress.String;

        //time_lease_obtained
        error = _localtime32_s(&newtime, (__time32_t*) &net_adapter->LeaseObtained);
        if(error) {
            std::cout << "Invalid Argument to _localtime32_s\n";
        } else {
            // Convert to an ASCII representation
            error = asctime_s(buffer, 32, &newtime);
            if(error) {
                std::cout << "Invalid Argument to asctime_s\n";
            } else {
                // asctime_s returns the string terminated by \n\0
                time_lease_obtained = buffer;
            }
        }

        //time_lease_expires
        error = _localtime32_s(&newtime, (__time32_t*) &net_adapter->LeaseExpires);
        if(error) {
             std::cout << "Invalid Argument to _localtime32_s\n";
        } else {
             // Convert to an ASCII representation
             error = asctime_s(buffer, 32, &newtime);
             if(error) {
                std::cout << "Invalid Argument to asctime_s\n";
             } else {
                // asctime_s returns the string terminated by \n\0
                time_lease_expires = buffer;
             }
        }

    } else {
        std::cout << "\tDHCP Enabled: No\n";
        dhcp_enabled = false;
    }

    //has_wins
    if (net_adapter->HaveWins) {
        has_wins = true;
        primary_wins_server = net_adapter->PrimaryWinsServer.IpAddress.String;
        printf("\t  Secondary Wins Server:  %s\n",
               net_adapter->SecondaryWinsServer.IpAddress.String);
        secondary_wins_server = net_adapter->SecondaryWinsServer.IpAddress.String;
    } else {
        has_wins = false;
    }
}

NetworkAdapter::NetworkAdapter(){};

std::string NetworkAdapter::get_all_info() const {
    std::string out_s = "\tComboIndex: \t" + std::to_string(combo_index) + '\n'
                    +=  "\tAdapter Name: \t" + name + '\n'
                    +=  "\tAdapter Desc: \t" + desc + '\n'
                    +=  "\tPhysical Adapter Addr: \t" + phys_addr + '\n'
                    +=  "\tIndex: \t" + std::to_string(index) + '\n'
                    +=  "\tType: \t" + type + '\n'
                    +=  "\tIP Address: \t" + ip_addr + '\n'
                    += "\tIP Mask: \t" + ip_mask + '\n'
                    += "\tGateway: \t" + gateway + '\n'
                    += "\t***\n";
    if(dhcp_enabled) {
        out_s += "\tDHCP Enabled: Yes\n";
        out_s += "\t  DHCP Server: \t" + dhcp_server + '\n';
        out_s += "\t  Lease Obtained: " + time_lease_obtained + '\n';
        out_s += "\t  Lease Expires:  " + time_lease_expires + '\n';

    } else {
        out_s += "\tDHCP Enabled: No\n";
    }

    if(has_wins) {
        out_s += "\tHave Wins: Yes\n";
        out_s += "\t  Primary Wins Server:    " + primary_wins_server + '\n';
        out_s += "\t  Secondary Wins Server:    " + secondary_wins_server + '\n';
    } else {
        out_s += "\tHave Wins: No\n";
    }

    return out_s;

}

std::string NetworkAdapter::get_gateway() {
    return gateway;
}

std::string NetworkAdapter::get_name() {
    return name;
}

std::string NetworkAdapter::get_desc() {
    return desc;
}

void NetworkAdapter::print_all_info() {
    std::cout << get_all_info() << std::endl;
}
