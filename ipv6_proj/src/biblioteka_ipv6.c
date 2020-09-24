#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/in.h>
#include "biblioteka_ipv6.h"

void setIPv6(struct ipv6h *IP){

  	char input[16];

  	IP->version = 6; // ipv6
  	IP->Traffic_Class = 0;
  	IP->flow_Label = 0; // best effort

  	IP->payload_Length = 17;
  	IP->next_Header = 6;
  	IP->hop_Limit = 10;

  	puts("Podaj adres źródłowy IPv6 :");
  	scanf("%s",input);
    if (inet_pton (AF_INET6, input, IP->IPsrc) != 1) {
      printf("inet_pton() failed.\nError message: %s\n", strerror(errno));
      exit (1);
    }
  	//strcpy(IP->IPsrc,input);

  	puts("Podaj adres docelowy IPv6 :");
  	scanf("%s",input);
    if (inet_pton (AF_INET6, input, IP->IPdst) != 1) {
      printf("inet_pton() failed.\nError message: %s\n", strerror(errno));
      exit (1);
    }
  	//strcpy(IP->IPdst,input);
}
