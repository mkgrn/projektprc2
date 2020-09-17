#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "lib.h"
#include <netinet/ip6.h>      // struct ip6_hdr
#include <netinet/tcp.h>      // struct tcp_hdr

int main() {
	int status; //zmienna statusu biblioteki

	void (*SET_ETH)(struct ethh*);
	void (*SET_IP)(struct iph*);
	void (*SET_TCP)(struct tcphdr*);
	struct lista* (*THREEinONE)(struct ethh*, struct iph*, struct tcphdr*);
	void (*SEND)(struct lista*);
	void *Biblioteka;

	//otwieranie biblioteki dynamicznej
	Biblioteka = dlopen("/home/miki/projektc/biblioteka/src/biblioteka_ipv6_tcp.so", RTLD_NOW);
	if(!Biblioteka)
	{
		printf ("Error opening: %s\n", dlerror());
		return(1);
	}

//	deklarowanie struktur i alokacja pamięci
	struct ethh *newETH;
	struct ipv6h *newIP;
	struct tcphdr *newTCP;
	struct lista *lista;
	newETH = (struct ethh*)malloc(sizeof(struct ethh));
	newIP = (struct ipv6h*)malloc(sizeof(struct ipv6h));
	newTCP = (struct tcphdr*)malloc(sizeof(struct tcphdr));

//	ustawianie pól pakietu, wpisywanie do listy wiązanej i wysyłanie
	SET_ETH = dlsym(Biblioteka,"setETH");
	SET_ETH(newETH);
	SET_IP = dlsym(Biblioteka, "setIPv6");
	SET_IP(newIP);
	SET_TCP = dlsym(Biblioteka, "setTCP");
	SET_TCP(newTCP);
	THREEinONE = dlsym(Biblioteka, "toTheList");
	lista = THREEinONE(newETH,newIP,newTCP);
	SEND = dlsym(Biblioteka, "send_packet");	//wskaźnik do biblioteki
	SEND(lista);	//wywołanie funkcji

//	zamykanie biblioteki
	status = dlclose(Biblioteka);
	if(status) { printf("ERROR %s\n",dlerror()); return -1; }

	return 0;
}
