PROJECT( mpx-blocks CXX )
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.2 FATAL_ERROR )
ADD_EXECUTABLE( xhttp 
	main.cpp 
	common.h 
	common.cpp 
	sigaction.h 
	sigaction.cpp 
	sigset.h 
	sigset.cpp 
	socketpair.h 
	socketpair.cpp 
	socket.h
	socket.cpp
	sockaddrin.h
	reverse.h
	reverse.cpp
	config.h
	global.h
	worker.h
	worker.cpp
	epoll.h
	epoll.cpp
	epollevent.h
)
