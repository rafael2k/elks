
#ifndef __TCP_H__
#define __TCP_H__

#include <sys/types.h>

#include "timer.h"
#include "ip.h"

#define PROTO_TCP	0x06

#define SEQ_LT(a,b)		((long)((a)-(b)) < 0)
#define SEQ_LEQ(a,b)	((long)((a)-(b)) <= 0)
#define SEQ_GT(a,b)		((long)((a)-(b)) > 0)
#define SEQ_GEQ(a,b)	((long)((a)-(b)) >= 0)

#define	TF_FIN	htons(1)
#define TF_SYN	htons(2)
#define	TF_RST	htons(4)
#define	TF_PSH	htons(8)
#define	TF_ACK	htons(16)
#define TF_URG	htons(32)

#define	TCP_DATAOFF(x)		( ((x)->flags & 0xf0) >> 2 )

#define TCP_SETHDRSIZE(c,s)	( *(__u8 *)&(c)->flags |= (s) << 4 )


#define ENTER_TIME_WAIT(cb)		(cb)->time_wait_exp = Now + 120 << 4;\
								(cb)->state = TS_TIME_WAIT;\
								tcp_timeruse++;\
								cbs_in_time_wait++;
								
#define LEAVE_TIME_WAIT(cb)		tcp_timeruse--;\
								cbs_in_time_wait--;

struct tcphdr_s {
	__u16	sport;
	__u16	dport;
	__u32	seqnum;
	__u32	acknum;
	__u16	flags;
	__u16	window;
	__u16	chksum;
	__u16	urgpnt;
	__u8	options;
};

struct iptcp_s {
	struct iphdr_s	*iph;
	struct tcphdr_s	*tcph;
	__u16			tcplen;
};	

#define	TS_CLOSED		0
#define	TS_LISTEN		1
#define	TS_SYN_SENT		2
#define	TS_SYN_RECEIVED	3
#define	TS_ESTABLISHED	4
#define	TS_FIN_WAIT_1	5
#define	TS_FIN_WAIT_2	6
#define	TS_CLOSE_WAIT	7
#define	TS_CLOSING		8
#define	TS_LAST_ACK		9
#define	TS_TIME_WAIT	10

#define CB_IN_BUF_SIZE	2048
#define CB_BUF_USED(x)	(((x)->buf_head - (x)->buf_tail) & (CB_IN_BUF_SIZE-1))
#define CB_BUF_SPACE(x)	((CB_IN_BUF_SIZE - CB_BUF_USED(x) - 1))

struct tcpcb_s {
	__u16	newsock;
	__u16	sock;	/* the socket in kernel space */
#define SS_NULL		0
#define	SS_ACCEPT	1
	__u32	localaddr;
	__u16	localport;
	__u32	remaddr;
	__u16	remport;
	
	__u8	state;
	timeq_t	rtt;
	
	__u32	time_wait_exp;
	__u8	unaccepted;	/* boolean */
	__u16	wait_data;
	__u8	in_buf[CB_IN_BUF_SIZE];
	__u16	buf_head;
	__u16	buf_tail;

	__u32	send_una;
	__u32	send_nxt;
	__u16	send_wnd;
	__u32	iss;
	
	__u32	rcv_nxt;
	__u16	rcv_wnd;
	__u32	irs;
	
	__u32		seg_seq;
	__u32		seg_ack;
	
	__u16		flags;
	__u8		*data;
	__u16		datalen;

};

struct	tcpcb_list_s {
	struct	tcpcb_s			tcpcb;
	struct	tcpcb_list_s	*prev, *next;
};

struct	tcp_retrans_list_s {
	int		retrans_num;
	timeq_t rto;
	timeq_t	next_retrans;
	timeq_t	first_trans;
	
	struct	tcpcb_s	*cb;
	struct	tcphdr_s *tcph;
	struct 	addr_pair apair;
	__u16 len;
	
	struct	tcp_retrans_list_s *prev, *next;	
};

int tcp_timeruse;

#define TCP_RTT_ALPHA		90
#define TCP_RETRANS_MAXMEM	8192*2
int tcp_retrans_memory;

struct tcpcb_list_s *tcpcb_new();
struct tcpcb_list_s *tcpcb_find();
struct tcpcb_list_s *tcpcb_find_by_sock();
void tcpcb_remove();

__u16 tcp_chksum();
void tcp_output();


#endif

