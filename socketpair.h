/*
 * socketpair.h
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */

#ifndef SOCKETPAIR_H_
#define SOCKETPAIR_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <exception>
#include <string>

namespace Net
{

class SocketPairErr : public std::exception
{
public:
    explicit SocketPairErr(const std::string& msg) : msg_(msg) {}
    virtual ~SocketPairErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class SocketPair
{
public:
    SocketPair(int domain, int type, int protocol);
    explicit SocketPair(const int pipe[2]);
    SocketPair(const SocketPair& pipe);
    int& operator[](size_t index);
    const int& operator[](size_t index) const;
    void Close(size_t index) const;
private:
    int pipe_[2];
};

} // namescpace Net


#endif /* SOCKETPAIR_H_ */
