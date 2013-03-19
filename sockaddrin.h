/*
 * sockaddrin.h
 *
 *  Created on: 17.04.2012
 *      Author: igor
 */

#ifndef SOCKADDRIN_H_
#define SOCKADDRIN_H_

#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>

namespace Net
{

class SockAddrIn
{
public:
    SockAddrIn()
    {
        bzero(&addr_, sizeof(addr_));
    }
    struct sockaddr_in& get()
    {
        return addr_;
    }
    operator struct sockaddr *()
    {
        return reinterpret_cast<struct sockaddr *>(&addr_);
    }
    socklen_t length() const
    {
        return sizeof(addr_);
    }
private:
    struct sockaddr_in addr_;
};

class PSockAddrIn
{
public:
    PSockAddrIn(struct sockaddr *addr) : addr_(reinterpret_cast<struct sockaddr_in *>(addr)) {}
    struct sockaddr_in *operator->()
    {
        return addr_;
    }
    operator struct sockaddr *()
    {
        return reinterpret_cast<struct sockaddr *>(addr_);
    }
private:
    struct sockaddr_in *addr_;
};

} // namespace Net

#endif /* SOCKADDRIN_H_ */
