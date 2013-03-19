/*
 * sigset.cpp
 *
 *  Created on: 27.04.2012
 *      Author: igor
 */

#include "sigset.h"
#include <cstring>
#include <errno.h>

Signal::SigSet::SigSet()
{
    sigemptyset(&mask_);
}
Signal::SigSet::SigSet(const sigset_t& mask) : mask_(mask)
{
}
Signal::SigSet::SigSet(const SigSet& mask) : mask_(mask.Get())
{
}
sigset_t& Signal::SigSet::Get()
{
    return mask_;
}
const sigset_t& Signal::SigSet::Get() const
{
    return mask_;
}
Signal::SigSet Signal::SigSet::SigProcMask(int what) const
{
    sigset_t old;
    if (sigprocmask(what, &mask_, &old) < 0)
    {
        throw SigSetErr(std::string("sigprocmask: ") + strerror(errno));
    }
    return SigSet(old);
}
void Signal::SigSet::SigEmptySet()
{
    sigemptyset(&mask_);
}
void Signal::SigSet::SigFillSet()
{
    sigfillset(&mask_);
}
void Signal::SigSet::SigAddSet(int signum)
{
    if (sigaddset(&mask_, signum) < 0)
    {
        throw SigSetErr(std::string("sigaddset: ") + strerror(errno));
    }
}
void Signal::SigSet::SigDelSet(int signum)
{
    if (sigdelset(&mask_, signum) < 0)
    {
        throw SigSetErr(std::string("sigdelset: ") + strerror(errno));
    }
}
bool Signal::SigSet::SigIsMemeber(int signum) const
{
    int res;
    if ((res = sigismember(&mask_, signum)) < 0)
    {
        throw SigSetErr(std::string("sigismember: ") + strerror(errno));
    }
    return res;
}



