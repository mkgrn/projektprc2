#ifndef BIBLIOTEKA_IPV6_H_
#define BIBLIOTEKA_IPV6_H_

struct ipv6h{
	unsigned char version: 6;
	unsigned char Traffic_Class;
	unsigned int flow_Label: 12;
	unsigned short payload_Length;
	unsigned char next_Header;
	unsigned char hop_Limit;
	unsigned char IPsrc[16];
	unsigned char IPdst[16];
};


void setIPv6(struct ipv6h *IP);

#endif /* BIBLIOTEKA_IPV6_H_ */
