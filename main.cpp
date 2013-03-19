/*
 * main.cpp
 *
 *  Created on: 28.04.2012
 *      Author: igor
 */
#include <iostream>
#include <cstdlib>
#include <locale>
#include <exception>
#include <signal.h>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <map>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <syslog.h>
#include <fcntl.h>
#include "config.h"
#include "common.h"
#include "sigaction.h"
#include "sigset.h"
#include "socketpair.h"
#include "socket.h"
#include "sockaddrin.h"
#include "reverse.h"
#include "worker.h"

struct DescWorker
{
    DescWorker(): sock(0), load(0) {}
    int sock;
    int load;
};

static std::map<pid_t, DescWorker> desc_workers;

char *prog;

bool daemon_opt = false;

char *root;

void handler_rt(int signum, siginfo_t *siginfo, void *)
{
    --desc_workers[siginfo->si_pid].load;
    Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Получен %s от %d", strsignal(signum), siginfo->si_pid);

}

void handler_ch(int signum)
{
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        desc_workers.erase(pid);
        Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Уничтожен Worker pid=%d", pid);
    }
}

void handler_term(int signum)
{
    Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Вызван handler_term");
    std::map<pid_t, DescWorker>::const_iterator i = desc_workers.begin();
    std::map<pid_t, DescWorker>::const_iterator i_end = desc_workers.end();
    for (;i != i_end; ++i)
    {
        Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Worker pid=%d sock=%d, load=%d", i->first, i->second.sock, i->second.load);
    }
    kill(0, SIGTERM);
    exit(EXIT_SUCCESS);
}


pid_t FindMinLoad(const std::map<pid_t, DescWorker>& desc_workers)
{
    std::multimap<int, pid_t> temp;
    std::map<pid_t, DescWorker>::const_iterator i = desc_workers.begin();
    std::map<pid_t, DescWorker>::const_iterator i_end = desc_workers.end();
    for(; i != i_end; ++i)
    {
        temp.insert(std::make_pair(i->second.load, i->first));
    }
    return temp.begin()->second;
}


int
main(int argc, char *argv[]) try 
{
    std::locale::global(std::locale(""));
    prog = Util::prog(argv[0]);
    int arg;
    int port = PORT;
    bool daemon = false;
    size_t nom_workers = DEFAULT_WORKERS;
    while ((arg = getopt(argc, argv, "dp:w:r:")) != EOF)
    {
        switch (arg) {
        case 'd':
            daemon = true;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'w':
            nom_workers = atoi(optarg);
            break;
        case 'r':
            root = optarg;
            break;
        default:
            Util::out_message(false, std::cerr, 0, "Usage: %s -r file_root [-w nom_workers] [-d] [-p nom_port]", prog);
            exit(EXIT_FAILURE);
            break;
        }
    }
    if (!root)
    {
        Util::out_message(false, std::cerr, 0, "Usage: %s -r file_root [-w nom_workers] [-d] [-p nom_port]", prog);
        exit(EXIT_FAILURE);
    }
    if (daemon)
    {
        if (Util::makedaemon() < 0)
        {
            Util::out_message(false, std::cerr, 0, "%s: makedaemon: %s", prog, strerror(errno));
            exit(EXIT_FAILURE);
        }
        openlog(prog, LOG_PID|LOG_CONS|LOG_NDELAY|LOG_NOWAIT, LOG_LOCAL0);
        setlogmask(LOG_UPTO(LOG_DEBUG));
        syslog(LOG_LOCAL0|LOG_INFO, "Start %s", prog);
        daemon_opt = true;
    }
    Signal::SigAction act;
    act.SigAddSet(SIGRTMIN + 1);
    act.SigAddSet(SIGCHLD);
    act.SetSigAction(handler_rt);
    act.SetFlags(SA_SIGINFO);
    act.Set(SIGRTMIN + 1);
    act.SetHandler(handler_ch);
    act.SetFlags(SA_NOCLDSTOP);
    act.Set(SIGCHLD);
    act.SigEmptySet();
    act.SetHandler(handler_term);
    act.SetFlags(0);
    act.Set(SIGTERM);
    Signal::SigSet mask;
    mask.SigAddSet(SIGCHLD);
    mask.SigAddSet(SIGRTMIN + 1);
    Net::Socket listenfd(AF_INET, SOCK_STREAM, 0);
    int delay = 1;
    listenfd.SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &delay, sizeof(delay));
    Net::SockAddrIn servaddr;
    servaddr.get().sin_family = AF_INET;
    servaddr.get().sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.get().sin_port = htons(port);
    listenfd.Bind(servaddr, servaddr.length());
    listenfd.Listen(LISTENQ);
    for (;;)
    {
        mask.SigProcMask(SIG_BLOCK);
        while (desc_workers.size() < nom_workers)
        {
            mask.SigProcMask(SIG_UNBLOCK);
            Net::SocketPair socks(PF_UNIX, SOCK_STREAM, 0);
            pid_t pid = fork();
            if (pid < 0)
            {
                Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: fork: %s", prog, strerror(errno));
                kill(0, SIGTERM);
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                socks.Close(1);
                return Workers(socks[0]);
            }
            Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Добавлен Workers pid=%d", pid);
            socks.Close(0);
            mask.SigProcMask(SIG_BLOCK);
            desc_workers[pid].sock = socks[1];
        }
        mask.SigProcMask(SIG_UNBLOCK);
        try
        {
            Net::SockAddrIn cliaddr;
            socklen_t cliaddr_len = cliaddr.length();
            Net::Socket connfd = listenfd.Accept(cliaddr, &cliaddr_len, false);
            try
            {
                Net::ReverseResolver reverse(cliaddr, cliaddr_len, 0);
                Util::out_message(daemon_opt, std::cout, LOG_LOCAL0|LOG_INFO, "Connect from %s:%s", reverse.HostName(), reverse.ServiceName());
            }
            catch(Net::ReverseResolverErr& err)
            {
                Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog, err.what());
            }
            mask.SigProcMask(SIG_BLOCK);
            pid_t worker = FindMinLoad(desc_workers);
            ++desc_workers[worker].load;
            Net::Socket fd(desc_workers[worker].sock);
            mask.SigProcMask(SIG_UNBLOCK);
            fd.SendFd(connfd);
        }
        catch(Net::SocketErr& err)
        {
            if (errno == EINTR)
                continue;
            Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog, err.what());
        }
    }
    return EXIT_SUCCESS;
}
catch(std::exception& err) 
{
    Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog, err.what());
    kill(0, SIGTERM);
    return EXIT_FAILURE;
}
catch(...) 
{
    Util::out_message(daemon_opt, std::cerr, LOG_LOCAL0|LOG_ERR, "%s: %s", prog, "unknown exception");
    kill(0, SIGTERM);
    return EXIT_FAILURE;
}

