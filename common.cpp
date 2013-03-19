/*
 * common.cpp
 *
 *  Created on: 13.04.2012
 *      Author: igor
 */

#include "common.h"
#include <cstring>
#include <sys/wait.h>
#include <cstdlib>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <syslog.h>
#include <unistd.h>


char *Util::prog(char *str)
{
    char *found = strrchr(str, '/');
    if (found)
        return ++found;
    else
        return str;
}

pid_t Util::makedaemon()
{
    pid_t pid;
    int status, stdio;

    if (getppid() == 1) /* если мы уже находимся в режиме демона, ничего делать не надо */
        return 0;
    if ((pid = fork()) < 0)
        return pid;
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        exit(EXIT_SUCCESS);
    }
    if (setsid() < 0)
        return -1;
    if ((pid = fork()) < 0)
        return pid;
    else if (pid > 0)
        exit(EXIT_SUCCESS);
    umask(0);
    if (chdir("/") < 0)
        return -1;
    if (close(STDIN_FILENO) < 0)
        return -1;
    if (close(STDOUT_FILENO) < 0)
        return -1;
    if (close(STDERR_FILENO) < 0)
        return -1;
    if ((stdio = open("/dev/null",O_RDWR)) < 0)
        return -1;
    if (dup(stdio) < 0)
        return -1;
    if (dup(stdio) < 0)
        return -1;
    if (setpgrp() < 0)
        return -1;
    return pid;
}

void Util::out_message(bool daemon, std::basic_ostream<char>& out, int fac_pri, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    char buf[BUFSIZ];
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    if (daemon)
        syslog(fac_pri, "%s", buf);
    else
        out << buf << std::endl;
}



