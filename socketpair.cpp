/*
 * socketpair.cpp
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */
#include "socketpair.h"
#include <cstring>
#include <errno.h>

Net::SocketPair::SocketPair(int domain, int type, int protocol)
{
    if (socketpair(domain, type, protocol, pipe_) < 0)
    {
        throw SocketPairErr(std::string("socketpair: ") + strerror(errno));
    }
}
Net::SocketPair::SocketPair(const int pipe[2])
{
    pipe_[0] = pipe[0];
    pipe_[1] = pipe[1];
}
Net::SocketPair::SocketPair(const SocketPair& pipe)
{
    pipe_[0] = pipe[0];
    pipe_[1] = pipe[1];
}

int& Net::SocketPair::operator[](size_t index)
{
    if (index > 1)
    {
        throw SocketPairErr("index out of range");
    }
    return pipe_[index];
}
const int& Net::SocketPair::operator[](size_t index) const
{
    if (index > 1)
    {
        throw SocketPairErr("index out of range");
    }
    return pipe_[index];
}
void Net::SocketPair::Close(size_t index) const
{
    if (index > 1)
    {
        throw SocketPairErr("index out of range");
    }
    if (close(pipe_[index]) < 0)
    {
        throw SocketPairErr(std::string("close: ") + strerror(errno));
    }
}



