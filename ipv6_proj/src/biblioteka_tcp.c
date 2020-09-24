#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/tcp.h>      // struct tcp_hdr
#include "biblioteka_tcp.h"

char inter[15];

void setTCP(struct tcphdr *tcpHdr){
	int srcPort=0;
	int dstPort=0;

	puts("Podaj port źródłowy:");
	scanf("%d",&srcPort);
	tcpHdr->th_sport = htons(srcPort);	/* Port źródłowy */

	puts("Podaj port docelowy:");
	scanf("%d",&dstPort);
	tcpHdr->th_dport = htons(dstPort);		/* Port docelowy */

	tcpHdr->th_seq = htonl(1); 					/* Numer sekwencji */
	tcpHdr->th_ack = 0x0; 				/* Numer potwierdzenia */
	tcpHdr->th_flags = 0; 					/* Flagi TCP */
	tcpHdr->th_win = htons(32767); 		/* Długość danych TCP */
	tcpHdr->th_sum = 0; 					/* Suma kontrolna TCP - liczona w programie głównym z wykorzystaniem pseudonagłówka TCP */
	tcpHdr->th_urp = 0;
}
