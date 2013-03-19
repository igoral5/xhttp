/*
 * epoll.h
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */

#ifndef EPOLL_H_
#define EPOLL_H_

#include <exception>
#include <string>
#include <sys/epoll.h>
#include "socket.h"
#include "epollevent.h"

namespace File
{

class EPollErr : public std::exception
{
public:
    explicit EPollErr(const std::string& msg) : msg_(msg) {}
    virtual ~EPollErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class EPoll
{
public:
    EPoll(int size);
    EPoll(const EPoll& epfd);
    ~EPoll();
    void Control(int op, const Net::Socket& sock, const EPollEvent& event);
    int Wait();
private:
    int epfd_;
};

} // namespace File



#endif /* EPOLL_H_ */
