/*
 * Copyright (c) 2016-2017 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, UDPECT, INUDPECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HUTILS_UDP_H
#define HUTILS_UDP_H

#include <hconf/include/hutils/udp.h>
#include <stdlib.h>
#include <hutils/cdecls.h>
#include <hutils/funcattr.h>
#include <hutils/stdint.h>

#if HCONF_BRANCH(SOCKET_H, SOCKET_H)
#	include <socket.h>
#elif HCONF_BRANCH(SOCKET_H, SYS_SOCKET_H)
#	include <sys/socket.h>
#elif HCONF_BRANCH(SOCKET_H, NONE)
#endif

#if HCONF_BRANCH(IPPROTO_UDP, NETINET_IN_H)
#	include <netinet/in.h>
#elif HCONF_BRANCH(IPPROTO_UDP, NOWARN_NETINET_IN_H)
/* HCONF_CPPFLAGS=-D__NO_INCLUDE_WARN__ */
#	include <netinet/in.h>
#endif
#if HCONFING(IPPROTO_UDP)
#	define HCONF_TEST return -1 == IPPROTO_UDP
#endif

#if HCONF_BRANCH(UDP_LOOKUP, GETADDRINFO)
/* HCONF_OPT=noexec */
#	include <netdb.h>
#	if HCONFING(UDP_LOOKUP)
#		define HCONF_TEST return !getaddrinfo(NULL, NULL, NULL, NULL)
#	endif
#elif HCONF_BRANCH(UDP_LOOKUP, GETHOSTBYNAME_SOCKLEN)
/* HCONF_LIBS=-lnetinet */
#	include <netdb.h>
#	define socklen_t int
#	if HCONFING(UDP_LOOKUP)
#		define HCONF_TEST return hconf_test_()
static int hconf_test_(void) {
	socklen_t len;
	return (NULL == gethostbyname(NULL) &&
	    -1 == recvfrom(0, NULL, 0, 0, NULL, &len)) ? 0 : 1;
}
#	endif
#endif

#if HCONF_BRANCH(UDP_EVENT, POLL)
#	include <poll.h>
#	if HCONFING(UDP_EVENT)
#		define HCONF_TEST hconf_test_()
static int hconf_test_() {
	struct pollfd fds[1];
	fds[0].fd = 0;
	fds[0].events = POLLIN;
	return -1 == poll(fds, 1, 0) ? 0 : 1;
}
#	endif
#elif HCONF_BRANCH(UDP_EVENT, SYS_SELECT_H)
#	include <sys/select.h>
#	if HCONFING(UDP_EVENT)
#		define HCONF_TEST hconf_test_()
static void hconf_test_(void) {
	struct timeval tv = {0, 0};
	select(0, NULL, NULL, NULL, &tv);
}
#	endif
#elif HCONF_BRANCH(UDP_EVENT, SELECT_TIME_H)
#	include <time.h>
#	if HCONFING(UDP_EVENT)
#		define HCONF_TEST hconf_test_()
static void hconf_test_(void) {
	struct timeval tv = {0, 0};
	select(0, NULL, NULL, NULL, &tv);
}
#	endif
#endif

enum {
	UDP_IPV4 = 0x0,
	UDP_IPV6 = 0x1
};

struct UDPAddress;
struct UDPClient;
struct UDPServer;
struct UDPDatagram {
	uint8_t	buf[512];
	size_t	size;
};

CDECLS_BEGIN

void			udp_address_free(struct UDPAddress **) FUNC_NONNULL(());
struct UDPClient	*udp_client_create(int, char const *, uint16_t)
	FUNC_NONNULL(()) FUNC_RETURNS;
void			udp_client_free(struct UDPClient **) FUNC_NONNULL(());
int			udp_client_receive(struct UDPClient const *, struct
    UDPDatagram *, double) FUNC_NONNULL(());
int			udp_client_send(struct UDPClient const *, struct
    UDPDatagram const *) FUNC_NONNULL(());
struct UDPServer	*udp_server_create(int, uint16_t) FUNC_RETURNS;
void			udp_server_free(struct UDPServer **) FUNC_NONNULL(());
int			udp_server_receive(struct UDPServer const *, struct
    UDPAddress **, struct UDPDatagram *, double) FUNC_NONNULL(());
int			udp_server_send(struct UDPServer const *, struct
    UDPAddress const *, struct UDPDatagram const *) FUNC_NONNULL(());
int			udp_server_write(struct UDPServer const *, void const
    *, size_t) FUNC_NONNULL(());
int			udp_setup(void) FUNC_RETURNS;
void			udp_shutdown(void);

CDECLS_END

#endif
