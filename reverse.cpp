/*
 * reverse.cpp
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */

#include "reverse.h"
#include <cstring>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>

Net::ReverseResolver::ReverseResolver(const struct sockaddr *addr, socklen_t addrlen, int flags, size_t buf_len) : buf_len_(buf_len)
{
    host_ = new char[buf_len_];
    service_ = new char[buf_len_];
    int rc = getnameinfo(addr, addrlen, host_, buf_len_, service_, buf_len_, flags);
    if (rc)
    {
        delete[] host_;
        delete[] service_;
        if (rc == EAI_SYSTEM)
        {
            throw ReverseResolverErr(std::string("getnameinfo: ") + strerror(errno));
        }
        else
        {
            throw ReverseResolverErr(std::string("getnameinfo: ") + gai_strerror(rc));
        }
    }
}

Net::ReverseResolver::~ReverseResolver() throw ()
{
    delete[] host_;
    delete[] service_;
}

const char *Net::ReverseResolver::HostName() const
{
    return host_;
}

const char *Net::ReverseResolver::ServiceName() const
{
    return service_;
}


