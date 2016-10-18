/*
 * Copyright (c) 2016 Hans Toshihide Törnqvist <hans.tornqvist@gmail.com>
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
#include <hutils/funcattr.h>
#include <hutils/stdint.h>

#if defined(HCONF_mSOCKET_H_bSOCKET_H)
#	include <socket.h>
#elif defined(HCONF_mSOCKET_H_bSYS_SOCKET_H)
#	include <sys/socket.h>
#elif defined(HCONF_mSOCKET_H_bNONE)
#endif

#if defined(HCONF_mIPPROTO_UDP_bNETINET_IN_H)
#	include <netinet/in.h>
#elif defined(HCONF_mIPPROTO_UDP_bZERO)
#	define IPPROTO_UDP 0
#endif
#if defined(HCONFING_mIPPROTO_UDP)
HCONF_TEST(int, (void))
{
	return IPPROTO_UDP;
}
#endif

#if defined(HCONF_mUDP_bGETADDRINFO)
#	if defined(HCONFING_mUDP)
#		include <netdb.h>
HCONF_TEST(int, (void))
{
	return getaddrinfo(0, 0, 0, 0);
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

struct UDPClient	*udp_client_create(int, char const *, uint16_t)
	FUNC_RETURNS;
void			udp_client_free(struct UDPClient **);
int			udp_client_receive(struct UDPClient const *, struct
    UDPDatagram *, double);
int			udp_client_send(struct UDPClient const *, struct
    UDPDatagram const *);
struct UDPServer	*udp_server_create(int, uint16_t) FUNC_RETURNS;
void			udp_server_free(struct UDPServer **);
int			udp_server_receive(struct UDPServer const *, struct
    UDPAddress **, struct UDPDatagram *, double);
int			udp_server_send(struct UDPServer const *, struct
    UDPDatagram const *);
int			udp_setup(void) FUNC_RETURNS;
void			udp_shutdown(void);

#endif
