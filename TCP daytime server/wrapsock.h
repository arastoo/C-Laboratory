#ifndef		WRAPLIB_H
#define		WRAPLIB_H

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<netinet/in.h>		/* struct sockaddr_in */
#include	<arpa/inet.h>		/* htons */
#include	"error.h"


#define		LISTENQ		1024
#define		SA			struct sockaddr

/* 
 * socket wrapper macro
 * d = domain, this is int data type
 * t = type, this is int data type
 * p = protocol, this is int data type
 * f = this is address of variable whose has int data type,
 *		this is a file discriptor that is returned from this macro.
 */
#define Socket( d, t, p, f)								\
	do {												\
														\
		if( 0 > (*f = socket( d, t, p)))				\
			errExit("socket error");					\
														\
	} while(0)

/*
 * bind wrappper macro
 * s = socket,file discriptor, this is int data type
 * a = address, this is pointer to struct sockaddr data type
 * l = address len, this is socklen_t data type
 */
#define Bind( s, a, l)									\
	do {												\
														\
		if( -1 == bind( s, a, l))						\
			errExit("bind error");						\
														\
	} while(0)											\

/*
 * listen wrapper macro
 * s = socket, file discriptor, this is int data type
 * b = backlog, defines  the  maximum length to
 *		which the queue of pending connections 
 *		for sockfd may grow. this is int data type
 */
#define Listen( s, b)									\
	do {												\
														\
		if( -1 == listen( s, b))						\
			errExit("listen error");					\
														\
	} while(0)

/*
 * accept wrapper macro
 * s = socket file discriptor, this is int data type
 * a = address, this is pointer to struct sockaddr data type
 * l = address len, this is pointer to socklen_t data type
 * f = this is address of variable whose has int data type,
 *		this is a file discriptor that is returned from this macro.
 */
#define Accept( s, a, l, f)								\
	do {												\
														\
		if( -1 == (*f = accept( s, a, l)))				\
			errExit("accept error");					\
														\
	} while(0)

/*
 * connect wrapper macro
 * s = socket file discriptor, this is int data type
 * a = address, this is pointer to struct sockaddr data type
 * l = address len, this is socklen_t data type
 */
#define Connect( s, a, l)								\
	do {												\
														\
		if(-1 == connect( s, a, l))						\
			errExit("connect error");					\
														\
	} while(0)

/*
 * close wrapper macro
 * f = file discriptor, this is int data type
 */
#define Close(f)										\
	do {												\
														\
		if(-1 == close(f))								\
			errExit("close error");						\
														\
	} while(0)

#endif
