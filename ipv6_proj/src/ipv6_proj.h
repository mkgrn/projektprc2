#ifndef IPV6_PROJ_H_
#define IPV6_PROJ_H_

struct ethh{
	unsigned char MAC_des1;
	unsigned char MAC_des2;
	unsigned char MAC_des3;
	unsigned char MAC_des4;
	unsigned char MAC_des5;
	unsigned char MAC_des6;
	unsigned char MAC_src1;
	unsigned char MAC_src2;
	unsigned char MAC_src3;
	unsigned char MAC_src4;
	unsigned char MAC_src5;
	unsigned char MAC_src6;
	unsigned short type;
};

struct lista{
	struct ethh *ETH;
	struct ipv6h *IP;
	struct tcphdr *TCP;
	struct lista *Head;
	struct lista *past;
	struct lista *present;
	struct lista *next;
};

#endif /* IPV6_PROJ_H_ */
