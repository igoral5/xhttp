/*
 * sigset.h
 *
 *  Created on: 27.04.2012
 *      Author: igor
 */

#ifndef SIGSET_H_
#define SIGSET_H_

#include <signal.h>
#include <exception>
#include <string>

namespace Signal
{

class SigSetErr : public std::exception
{
public:
    explicit SigSetErr(const std::string& msg) : msg_(msg) {}
    virtual ~SigSetErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class SigSet
{
public:
    SigSet();
    SigSet(const sigset_t& mask);
    SigSet(const SigSet& mask);
    sigset_t& Get();
    const sigset_t& Get() const;
    SigSet SigProcMask(int what) const;
    void SigEmptySet();
    void SigFillSet();
    void SigAddSet(int signum);
    void SigDelSet(int signum);
    bool SigIsMemeber(int signum) const;
private:
    sigset_t mask_;
};

} // namespace Signal



#endif /* SIGSET_H_ */
