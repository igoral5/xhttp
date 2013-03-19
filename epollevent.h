/*
 * epollevent.h
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */

#ifndef EPOLLEVENT_H_
#define EPOLLEVENT_H_

#include <sys/epoll.h>
#include <cstring>

namespace File
{

class EPollEvent
{
public:
    EPollEvent()
    {
        bzero(&event_, sizeof(event_));
    }
    EPollEvent(const EPollEvent& event) : event_(event.Get())
    {
    }
    EPollEvent(const struct epoll_event& event) : event_(event)
    {
    }
    struct epoll_event& Get()
    {
        return event_;
    }
    const struct epoll_event& Get() const
    {
        return event_;
    }
    uint32_t GetEvents() const
    {
        return event_.events;
    }
    void SetEvents(uint32_t events)
    {
        event_.events = events;
    }
    void *GetPtr() const
    {
        return event_.data.ptr;
    }
    void SetPtr(void *ptr)
    {
        event_.data.ptr = ptr;
    }
    int GetFd() const
    {
        return event_.data.fd;
    }
    void SetFd(int fd)
    {
        event_.data.fd = fd;
    }
    uint32_t GetU32() const
    {
        return event_.data.u32;
    }
    void SetU32(uint32_t u32)
    {
        event_.data.u32 = u32;
    }
    uint64_t GetU64() const
    {
        return event_.data.u64;
    }
    void SetU64(uint64_t u64)
    {
        event_.data.u64 = u64;
    }
private:
    struct epoll_event event_;
};

} // namespace File


#endif /* EPOLLEVENT_H_ */
