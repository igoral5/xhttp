/*
 * sigaction.cpp
 *
 *  Created on: 27.04.2012
 *      Author: igor
 */

#include "sigaction.h"
#include <cstring>
#include <errno.h>

Signal::SigAction::SigAction()
{
    sigemptyset(&act_.sa_mask);
    act_.sa_flags = 0;
    act_.sa_handler = 0;
}
Signal::SigAction::SigAction(const struct sigaction& act) : act_(act)
{
}
Signal::SigAction::SigAction(const SigAction& act) : act_(act.Get())
{
}
void Signal::SigAction::SigFillSet()
{
    sigfillset(&act_.sa_mask);
}
void Signal::SigAction::SigEmptySet()
{
    sigemptyset(&act_.sa_mask);
}
void Signal::SigAction::SigAddSet(int signum)
{
    if (sigaddset(&act_.sa_mask, signum) < 0)
    {
        throw SigActionErr(std::string("sigaddset: ") + strerror(errno));
    }
}
void Signal::SigAction::SigDelSet(int signum)
{
    if (sigdelset(&act_.sa_mask, signum) < 0)
    {
        throw SigActionErr(std::string("sigdelset: ") + strerror(errno));
    }
}
bool Signal::SigAction::SigIsMember(int signum) const
{
    int res;
    if ((res = sigismember(&act_.sa_mask, signum)) < 0)
    {
        throw SigActionErr(std::string("sigismember: ") + strerror(errno));
    }
    return res;
}
struct sigaction& Signal::SigAction::Get()
{
    return act_;
}
const struct sigaction& Signal::SigAction::Get() const
{
    return act_;
}
Signal::SigAction Signal::SigAction::Set(int signum) const
{
    struct sigaction old;
    if (sigaction(signum, &act_, &old) < 0)
    {
        throw SigActionErr(std::string("sigaction: ") + strerror(errno));
    }
    return SigAction(old);
}
int Signal::SigAction::GetFlags() const
{
    return act_.sa_flags;
}
void Signal::SigAction::SetFlags(int flags)
{
    act_.sa_flags = flags;
}
__sighandler_t Signal::SigAction::GetHandler() const
{
    return act_.sa_handler;
}
void Signal::SigAction::SetHandler(__sighandler_t handler)
{
    act_.sa_handler = handler;
}
void (*Signal::SigAction::GetSigAction()) (int, siginfo_t *, void *)  const
{
    return act_.sa_sigaction;
}
void Signal::SigAction::SetSigAction(void (* sigaction)(int, siginfo_t *, void *))
{
    act_.sa_sigaction = sigaction;
}



