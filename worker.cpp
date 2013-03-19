/*
 * worker.cpp
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */
#include "worker.h"
#include "global.h"
#include "sigaction.h"
#include "socket.h"
#include "epoll.h"
#include <deque>
#include <fcntl.h>
#include <cstdlib>

int Workers(int sock)
{
    static unsigned int seed = time(0) + getpid();
    Signal::SigAction act;
    act.SetHandler(SIG_DFL);
    act.Set(SIGRTMIN + 1);
    act.Set(SIGCHLD);
    act.Set(SIGTERM);
    Net::Socket socket(sock);
    socket.SetFlags(O_NONBLOCK);
    File::EPoll epfd(1000);
    //epfd.Control();
    for (;;)
    {
        sleep(rand_r(&seed) % 30 + 1);
        kill(getppid(), SIGRTMIN + 1);
    }
    return EXIT_SUCCESS;
}



