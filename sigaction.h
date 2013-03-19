/*
 * sigaction.h
 *
 *  Created on: 27.04.2012
 *      Author: igor
 */

#ifndef SIGACTION_H_
#define SIGACTION_H_

#include <signal.h>
#include <exception>
#include <string>

namespace Signal
{

class SigActionErr : public std::exception
{
public:
    explicit SigActionErr(const std::string& msg) : msg_(msg) {}
    virtual ~SigActionErr() throw() {}
    virtual const char* what() const throw() { return msg_.c_str(); }
private:
    std::string msg_;
};

class SigAction
{
public:
    SigAction();
    SigAction(const struct sigaction& act);
    SigAction(const SigAction& act);
    void SigFillSet();
    void SigEmptySet();
    void SigAddSet(int signum);
    void SigDelSet(int signum);
    bool SigIsMember(int signum) const;
    struct sigaction& Get();
    const struct sigaction& Get() const;
    SigAction Set(int signum) const;
    int GetFlags() const;
    void SetFlags(int flags);
    __sighandler_t GetHandler() const;
    void SetHandler(__sighandler_t handler);
    void (*GetSigAction()) (int, siginfo_t *, void *)  const;
    void SetSigAction(void (* sigaction)(int, siginfo_t *, void *));
private:
    struct sigaction act_;
};

} // namespace Signal


#endif /* SIGACTION_H_ */
