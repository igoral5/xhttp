/*
 * reverse.h
 *
 *  Created on: 16.04.2012
 *      Author: igor
 */

#ifndef REVERSE_H_
#define REVERSE_H_

#include <exception>
#include <string>
#include <cstdio>
#include <sys/socket.h>
#include <netdb.h>

namespace Net
{

class ReverseResolverErr : public std::exception
{
public:
    explicit ReverseResolverErr(const std::string& msg) : msg_(msg) {}
    virtual ~ReverseResolverErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class ReverseResolver
{
public:
    ReverseResolver(const struct sockaddr *addr, socklen_t addrlen, int flags, size_t buf_len = BUFSIZ);
    virtual ~ReverseResolver() throw();
    const char *HostName() const;
    const char *ServiceName() const;
private:
    ReverseResolver(const ReverseResolver&); // without implementation
    ReverseResolver& operator=(const ReverseResolver&); // without implementation
    size_t buf_len_;
    char *host_;
    char *service_;
};

} // namespace Net

#endif /* REVERSE_H_ */
