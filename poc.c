#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <asm/types.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <netinet/in.h>
#include <linux/tcp.h>
#include <linux/sock_diag.h>
#include <linux/inet_diag.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>
#include <pwd.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>

#define MY_PORT_NUM 1234

struct sctp_info {
    __u32   sctpi_tag;
    __u32   sctpi_state;
    __u32   sctpi_rwnd;
    __u16   sctpi_unackdata;
    __u16   sctpi_penddata;
    __u16   sctpi_instrms;
    __u16   sctpi_outstrms;
    __u32   sctpi_fragmentation_point;
    __u32   sctpi_inqueue;
    __u32   sctpi_outqueue;
    __u32   sctpi_overall_error;
    __u32   sctpi_max_burst;
    __u32   sctpi_maxseg;
    __u32   sctpi_peer_rwnd;
    __u32   sctpi_peer_tag;
    __u8    sctpi_peer_capable;
    __u8    sctpi_peer_sack;
    __u16   __reserved1;

    /* assoc status info */
    __u64   sctpi_isacks;
    __u64   sctpi_osacks;
    __u64   sctpi_opackets;
    __u64   sctpi_ipackets;
    __u64   sctpi_rtxchunks;
    __u64   sctpi_outofseqtsns;
    __u64   sctpi_idupchunks;
    __u64   sctpi_gapcnt;
    __u64   sctpi_ouodchunks;
    __u64   sctpi_iuodchunks;
    __u64   sctpi_oodchunks;
    __u64   sctpi_iodchunks;
    __u64   sctpi_octrlchunks;
    __u64   sctpi_ictrlchunks;

    /* primary transport info */
    struct sockaddr_storage sctpi_p_address;
    __s32   sctpi_p_state;
    __u32   sctpi_p_cwnd;
    __u32   sctpi_p_srtt;
    __u32   sctpi_p_rto;
    __u32   sctpi_p_hbinterval;
    __u32   sctpi_p_pathmaxrxt;
    __u32   sctpi_p_sackdelay;
    __u32   sctpi_p_sackfreq;
    __u32   sctpi_p_ssthresh;
    __u32   sctpi_p_partial_bytes_acked;
    __u32   sctpi_p_flight_size;
    __u16   sctpi_p_error;
    __u16   __reserved2;

    /* sctp sock info */
    __u32   sctpi_s_autoclose;
    __u32   sctpi_s_adaptation_ind;
    __u32   sctpi_s_pd_point;
    __u8    sctpi_s_nodelay;
    __u8    sctpi_s_disable_fragments;
    __u8    sctpi_s_v4mapped;
    __u8    sctpi_s_frag_interleave;
    __u32   sctpi_s_type;
    __u32   __reserved3;
};

enum {
    SS_UNKNOWN,
    SS_ESTABLISHED,
    SS_SYN_SENT,
    SS_SYN_RECV,
    SS_FIN_WAIT1,
    SS_FIN_WAIT2,
    SS_TIME_WAIT,
    SS_CLOSE,
    SS_CLOSE_WAIT,
    SS_LAST_ACK,
    SS_LISTEN,
    SS_CLOSING,
    SS_MAX
};

enum sctp_state {
    SCTP_STATE_CLOSED       = 0,
    SCTP_STATE_COOKIE_WAIT      = 1,
    SCTP_STATE_COOKIE_ECHOED    = 2,
    SCTP_STATE_ESTABLISHED      = 3,
    SCTP_STATE_SHUTDOWN_PENDING = 4,
    SCTP_STATE_SHUTDOWN_SENT    = 5,
    SCTP_STATE_SHUTDOWN_RECEIVED    = 6,
    SCTP_STATE_SHUTDOWN_ACK_SENT    = 7,
};

enum {
    TCP_ESTABLISHED = 1,
    TCP_SYN_SENT,
    TCP_SYN_RECV,
    TCP_FIN_WAIT1,
    TCP_FIN_WAIT2,
    TCP_TIME_WAIT,
    TCP_CLOSE,
    TCP_CLOSE_WAIT,
    TCP_LAST_ACK,
    TCP_LISTEN,
    TCP_CLOSING,    /* Now a valid state */
    TCP_NEW_SYN_RECV,

    TCP_MAX_STATES  /* Leave at the end! */
};

enum sctp_sock_state {
    SCTP_SS_CLOSED         = TCP_CLOSE,
    SCTP_SS_LISTENING      = TCP_LISTEN,
    SCTP_SS_ESTABLISHING   = TCP_SYN_SENT,
    SCTP_SS_ESTABLISHED    = TCP_ESTABLISHED,
    SCTP_SS_CLOSING        = TCP_CLOSE_WAIT,
};
struct sctp_sndinfo {
	uint16_t snd_sid;
	uint16_t snd_flags;
	uint32_t snd_ppid;
	uint32_t snd_context;
	sctp_assoc_t snd_assoc_id;
};

const char *data = "Hello, Server!";
int conn_fd;





	






int main(){ 
    signal(SIGPIPE, SIG_IGN);
	int  ret;
    struct sockaddr_in servaddr = {
            .sin_family = AF_INET,
            .sin_port = htons(MY_PORT_NUM),
            .sin_addr.s_addr = inet_addr("127.0.0.1"),
    };
	
	 struct cmsghdr   *cmsg;
    struct sctp_sndinfo  *sri;
    char    cbuf[sizeof (*cmsg) + sizeof (*sri)];
    struct msghdr   msg[1];
    struct iovec   iov[1];


    memset(msg, 0, sizeof (*msg));
    iov->iov_base = data;
    iov->iov_len=sizeof(data)+5;
    msg->msg_iov = iov;
    msg->msg_iovlen = 1;
    msg->msg_control = cbuf;
    msg->msg_controllen = sizeof (*cmsg) + sizeof (*sri);

	memset(cbuf, 0, sizeof (*cmsg) + sizeof (*sri));
    cmsg = (struct cmsghdr *)cbuf;
    sri = (struct sctp_sndrcvinfo *)(cmsg + 1);

    cmsg->cmsg_len = sizeof (*cmsg) + sizeof (*sri);
    cmsg->cmsg_level = IPPROTO_SCTP;
    cmsg->cmsg_type  = 2;

	sri->snd_flags=(1 << 6)|(1 <<2);

	



    conn_fd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if (conn_fd < 0)
    {	
    	perror("socket");    
	 
     
    }


   


	sctp_assoc_t id;

    if(sctp_connectx(conn_fd, (struct sockaddr *) &servaddr,1, &id)<0)
	    perror("connectx");
	else 
		printf("id is %llx \n",id);
	servaddr.sin_port = htons(MY_PORT_NUM+1);
	if(sctp_connectx(conn_fd, (struct sockaddr *) &servaddr,1, &id)<0)
            perror("connectx");
        else 
                printf("id is %llx \n",id);    
	
	
	



	if(sendmsg(conn_fd, msg,0)<0)
		perror("sendmsg");

	
  
    close(conn_fd);
	

    return 0;
}

