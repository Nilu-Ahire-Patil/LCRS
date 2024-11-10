#include <iostream>
#include <vector>
#include <iomanip>

#if defined(_WIN32)
#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")
#elif defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <unistd.h>
#endif

#include "lcrs/logger.h"
#include "lcrs/system.h"

const std::unordered_map<std::string, const unsigned char *> System::getMacAddressMap()
{
    static std::unordered_map<std::string, const unsigned char *> macAddressMap;

    if (macAddressMap.empty())
    {
        auto macToString = [](const unsigned char *mac)
        {
            char macStr[18];
            snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X",
                     mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
            return std::string(macStr);
        };

#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
        struct ifaddrs *ifap, *ifa;
        if (getifaddrs(&ifap) != 0)
        {
            lcrsLog(CRITICAL, "Fail getifaddrs");
            return macAddressMap;
        }

        for (ifa = ifap; ifa != nullptr; ifa = ifa->ifa_next)
        {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_PACKET)
            {
                struct sockaddr_ll *s = (struct sockaddr_ll *)ifa->ifa_addr;

                if (s->sll_halen == 6)
                {
                    macAddressMap[ifa->ifa_name] = (unsigned char *)s->sll_addr;
                    lcrsLog(TRACE, "Mac: {} Interface: {}", macToString((unsigned char *)s->sll_addr), ifa->ifa_name);
                }
            }
        }

        freeifaddrs(ifap);
#elif defined(_WIN32)
        IP_ADAPTER_INFO adapterInfo[16];
        DWORD bufferSize = sizeof(adapterInfo);
        DWORD result = GetAdaptersInfo(adapterInfo, &bufferSize);
        if (result == ERROR_SUCCESS)
        {
            PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
            while (pAdapterInfo)
            {
                macAddressMap[pAdapterInfo->AdapterName] = pAdapterInfo->Address;
                lcrsLog(TRACE, "Mac: {} Interface: {}", macToString(pAdapterInfo->Address), pAdapterInfo->AdapterName);
                pAdapterInfo = pAdapterInfo->Next;
            }
        }
        else
        {
            lcrsLog(ERROR, "Failed to get network adapter information");
        }
#else
        lcrsLog(ERROR, "Unsupported platform");
#endif

        if (macAddressMap.empty())
        {
            lcrsLog(CRITICAL, "Unable to found any nework interface");
            return macAddressMap;
        }
    }
    return macAddressMap;
}