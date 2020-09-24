#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>      // struct tcp_hdr
#include <netinet/ether.h>
#include <linux/if_packet.h>
#include <errno.h>
#include "biblioteka_ipv6.h"
#include "biblioteka_tcp.h"
#include "ipv6_proj.h"

char inter[15];

void setETH(struct ethh *ETH){
	puts("\nPodaj nazwe interfejsu z ktorego wyslac wiadomosc:\n");
	scanf("%s",inter);

	printf("interface %s \n", inter);

	// pobieranie informacji o adresie źródła.
	//int sock_raw=socket(AF_PACKET,SOCK_RAW,0);
	int sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
	if(sock_raw < 0) {
		printf("error in socket\n");
		printf("socket failed: %s\n", strerror(errno));
		exit(1);
	}

	struct ifreq ifreq_c;
	memset(&ifreq_c,0,sizeof(ifreq_c));
	strncpy(ifreq_c.ifr_name,inter,IFNAMSIZ-1);//giving name of Interface


	if(ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c)<0) //getting MAC Address
		printf("error in SIOCGIFHWADDR ioctl reading\n");

	puts("Przypisywanie adresów MAC: \n");
	ETH->MAC_src1 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
	ETH->MAC_src2 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
	ETH->MAC_src3 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
	ETH->MAC_src4 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
	ETH->MAC_src5 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
	ETH->MAC_src6 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);


	//robocza pętla
	ETH->MAC_des1 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
	ETH->MAC_des2 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
	ETH->MAC_des3 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
	ETH->MAC_des4 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
	ETH->MAC_des5 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
	ETH->MAC_des6 = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);

	//przypisanie typu
	ETH->type = 0x86DD;

	//Wypisanie adresów MAC
	printf("\nAdres źródłowy MAC: \n %x:%x:%x:%x:%x:%x",ETH->MAC_src1,ETH->MAC_src2,ETH->MAC_src3,ETH->MAC_src4,ETH->MAC_src5,ETH->MAC_src6);
	printf("\nAdres docelowy MAC: \n %x:%x:%x:%x:%x:%x",ETH->MAC_des1,ETH->MAC_des2,ETH->MAC_des3,ETH->MAC_des4,ETH->MAC_des5,ETH->MAC_des6);

	printf("\nTyp: %d\n",ETH->type); //wypisanie typu
}

struct lista * toTheList(struct ethh *eth, struct ipv6h *ip, struct tcphdr *tcpHdr){
	puts("\nIle razy wyslac pakiet?");
	unsigned int ilerazy;
	scanf("%u",&ilerazy);
	if(ilerazy == 0){
		puts("0 to za mało, wyślę raz");
		ilerazy = 1;
	}
//	przypadek tworzenia jednoelementowej listy
	if(ilerazy == 1){
		struct lista *HEAD;
		HEAD = (struct lista*)malloc(sizeof(struct lista));
		HEAD->Head = HEAD;
		HEAD->next = NULL;
		HEAD->past = NULL;
		HEAD->ETH = eth;
		HEAD->IP = ip;
		HEAD->TCP = tcpHdr;
		return HEAD;
	}
//	przypadek listy wiazanej
	else{
//		tworzymy pierwszy element
		struct lista *HEAD;
		struct lista *NEW;
		struct lista *PAST;
		HEAD = (struct lista*)malloc(sizeof(struct lista));
		PAST = (struct lista*)malloc(sizeof(struct lista));
		HEAD->Head = HEAD;
		HEAD->next = NULL;
		HEAD->past = NULL;
		HEAD->ETH = eth;
		HEAD->IP = ip;
		HEAD->TCP = tcpHdr;
		PAST = HEAD;
//		a resztę pętlą
		for(int i = 2; i <=ilerazy; i ++){
			NEW = (struct lista*)malloc(sizeof(struct lista));
			NEW->ETH = eth;
			NEW->IP = ip;
			NEW->TCP = tcpHdr;
			NEW->Head = HEAD;
			NEW->next = NULL;
			if(i==2){
				HEAD->next = NEW;
			}else{
				PAST->next = NEW;
			}
			PAST = NEW;
		}
//		zwracamy wskaźnik na początek listy

		return HEAD;
	}
}

void send_packet(struct lista *list){
	int size=71;

	int sock_raw=socket(AF_PACKET,SOCK_RAW,IPPROTO_RAW);
	if(sock_raw == -1)
		printf("error in socket");

	struct ifreq ifreq_i;
	memset(&ifreq_i,0,sizeof(ifreq_i));
	strncpy(ifreq_i.ifr_name,inter,IFNAMSIZ-1);
	//pobieranie indexu interfejsu po nazwie (bo interfejsy są normalnie poprostu numerkami)
	if((ioctl(sock_raw,SIOCGIFINDEX,&ifreq_i))<0)
	printf("error in index ioctl reading");//getting Index Name

	struct ifreq ifreq_c;
	memset(&ifreq_c,0,sizeof(ifreq_c));
	strncpy(ifreq_c.ifr_name,inter,IFNAMSIZ-1);//giving name of Interface

	if((ioctl(sock_raw,SIOCGIFHWADDR,&ifreq_c))<0) //getting MAC Address
	printf("error in SIOCGIFHWADDR ioctl reading");

	struct ifreq ifreq_ip;
	memset(&ifreq_ip,0,sizeof(ifreq_ip));
	strncpy(ifreq_ip.ifr_name,inter,IFNAMSIZ-1);//giving name of Interface
	if(ioctl(sock_raw,SIOCGIFADDR,&ifreq_ip)<0){ //getting IP Address
		printf("error in SIOCGIFADDR \n");
	}

	unsigned char *sendbuff=(unsigned char*)malloc(size); //alokacja pamieci bufora o zmiennym rozmiarze
	memset(sendbuff,0,size);
	int i;

//	uzupełnianie bufora

//	ETH
	sendbuff[0]= list->ETH->MAC_des1;
	sendbuff[1]= list->ETH->MAC_des2;
	sendbuff[2]= list->ETH->MAC_des3;
	sendbuff[3]= list->ETH->MAC_des4;
	sendbuff[4]= list->ETH->MAC_des5;
	sendbuff[5]= list->ETH->MAC_des6;

	sendbuff[6]= list->ETH->MAC_src1;
	sendbuff[7]= list->ETH->MAC_src2;
	sendbuff[8]= list->ETH->MAC_src3;
	sendbuff[9]= list->ETH->MAC_src4;
	sendbuff[10]= list->ETH->MAC_src5;
	sendbuff[11]= list->ETH->MAC_src6;

	i = list->ETH->type;
	i = i >> 8;
	sendbuff[12] = i;
	sendbuff[13] = list->ETH->type;

//	IP
	sendbuff[14]=list->IP->version;
	sendbuff[14]=sendbuff[14]<<4;
	i=list->IP->Traffic_Class;
	i=i>>4;
	sendbuff[14] = sendbuff[14] + i;

	sendbuff[15] = list->IP->Traffic_Class;
	sendbuff[15] = sendbuff[15] << 4;
	i=list->IP->flow_Label;
	i=i>>16;

	sendbuff[15]= sendbuff[15] + i;
	i=list->IP->flow_Label;
	i=i>>8;

	sendbuff[16]= sendbuff[16] + i;

	sendbuff[17]= list->IP->flow_Label;

	i=list->IP->payload_Length;
	i=i>>8;
	sendbuff[18]= i;
	sendbuff[19]= list->IP->payload_Length;
	sendbuff[20]= list->IP->next_Header;
	sendbuff[21]= list->IP->hop_Limit;

	for(i=0; i<16;i++){
	   sendbuff[22+i]= list->IP->IPsrc[i];
	}
	for(i=0; i<16;i++){
	   sendbuff[38+i]= list->IP->IPdst[i];
	}

//	TCP
	memcpy(&sendbuff[54],list->TCP,17);

//	pobieranie mac interfejsu
	struct sockaddr_ll sadr_ll;
	sadr_ll.sll_ifindex = ifreq_i.ifr_ifindex; // index of interface
	sadr_ll.sll_halen = ETH_ALEN; // length of destination mac address
  sadr_ll.sll_family = AF_PACKET;
	sadr_ll.sll_addr[0] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[0]);
	sadr_ll.sll_addr[1] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[1]);
	sadr_ll.sll_addr[2] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[2]);
	sadr_ll.sll_addr[3] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[3]);
	sadr_ll.sll_addr[4] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[4]);
	sadr_ll.sll_addr[5] = (unsigned char)(ifreq_c.ifr_hwaddr.sa_data[5]);
	int send_len;
	int counter = 0;

//	wysyłamy pakiety dopóki sie nie skonczy lista a ponieważ wszystkie elementy są takie same to
//	bufor można uzupełnić tylko raz, nie trzeba przepisywać tego samego cały czas
	while(list){
	send_len = sendto(sock_raw,sendbuff,size,0,(const struct sockaddr*)&sadr_ll,sizeof(struct sockaddr_ll));
		if(send_len<0){
			printf("error in sending....sendlen=%d....\n",send_len);
			break;
		}
	list = list->next;
	counter++;
	}
	if(list==NULL)
	printf("%d pakietow zostalo wyslane\n",counter);
}



int main() {
	int status; //zmienna statusu biblioteki

	void (*SET_IP)(struct ipv6h*);
	void (*SET_TCP)(struct tcphdr*);
	void *BibliotekaIPv6;
	void *BibliotekaTCP;

	//otwieranie biblioteki dynamicznej ipv6
	BibliotekaIPv6 = dlopen("./biblioteka_ipv6.so", RTLD_NOW);
	if(!BibliotekaIPv6)
	{
		printf ("Error opening: %s\n", dlerror());
		return(1);
	}

	//otwieranie biblioteki dynamicznej tcp
	BibliotekaTCP = dlopen("./biblioteka_tcp.so", RTLD_NOW);
	if(!BibliotekaTCP)
	{
		printf ("Error opening: %s\n", dlerror());
		return(1);
	}

//	deklarowanie struktur i alokacja pamięci
	struct ethh *newETH;
	struct ip6_hdr *newIP;
	struct tcphdr *newTCP;
	struct lista *lista;
	newETH = (struct ethh*)malloc(sizeof(struct ethh));
	newIP = (struct ipv6h*)malloc(sizeof(struct ipv6h));
	newTCP = (struct tcphdr*)malloc(sizeof(struct tcphdr));

	setETH(newETH);

	//	ustawianie pól pakietu, wpisywanie do listy wiązanej i wysyłanie
	SET_IP = dlsym(BibliotekaIPv6, "setIPv6");
	SET_IP(newIP);
	SET_TCP = dlsym(BibliotekaTCP, "setTCP");
	SET_TCP(newTCP);


	lista = toTheList(newETH,newIP,newTCP);
	send_packet(lista);	//wywołanie funkcji




	//	zamykanie biblioteki ipv6
	status = dlclose(BibliotekaIPv6);
	if(status) {
		printf("ERROR %s\n",dlerror()); return -1;
	}

	//	zamykanie biblioteki ipv6
	status = dlclose(BibliotekaTCP);
	if(status) {
		printf("ERROR %s\n",dlerror()); return -1;
	}

	return 0;
}
