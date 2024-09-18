/*

*/

#ifndef LCCRS_NETWORK_H
#define LCCRS_NETWORK_H

LCRS_NAMESPACE_START

class lcrsNetwork {
    enum SocketFlags {
        UNINITILISED = 0x00,
        UDP         = 0x01,
        BROADCAST   = UDP | 0x02,
        MULTICAST   = UDP | 0x04,
        LOOPBACK    = UDP | MULTICAST | 0x08,
        TCP         = 0x10,
        REUSEADDR   = TCP | 0x20,
        SNDTIMEO    = TCP | 0x30
    };

    lcrsNetwork(const uint8_t socketFlags = SocketFlags::TCP); // default tcp
    ~lcrsNetwork();

private:
    bool InitSocket();

private:
    const int mDomain;
    const int mType;
    const int mProtocol;

    const uint8_t mSocketFlags;
    int mSocket;
};

LCRS_NAMESPACE_END

#endif