#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define PORT 8888
#define MCAST "224.0.0.88"
int main(int argc, char **argb)
{
	int s,ret=0,size,ttl=10,loop=0,times=0;
	char buffer[1024];
	struct sockaddr_in localaddr,fromaddr;
	s = socket(AF_INET,SOCK_DGRAM,0);
	if(s < 0)
	{
		perror("socket error\n");
		return -1;
	}
	localaddr.sin_family = AF_INET;
	localaddr.sin_port = htons(PORT);
	localaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ret = bind(s,(struct sockaddr*)&localaddr,sizeof(localaddr));
	if(ret < 0)
	{
		perror("bind error\n");
		return -1;
	}
	if(setsockopt(s, IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl))<0)
	{
		perror("IP_MULTICAST_TTL");
		return -1;
	}

	if(setsockopt(s, IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop))<0)
	{
		perror("IP_MULTICAST_LOOP");
		return -1;
	}
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if(setsockopt(s,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))<0)
	{
		perror("IP_ADD_MEMBERSHIP");
		return -1;
	}
	//char sendbuf[16] = {0};
	//snprintf(sendbuf, sizeof(sendbuf), "%d",getpid());
	//sendbuf[strlen(sendbuf)] = '\n';
	int pid = getpid();
		printf("start to recieve\n");
	for(times= 0;times<20;times++)
	{
		char sendbuf[16] = {0};
		sprintf(sendbuf,"%d_%d\n",pid, times);
		int len = sizeof(fromaddr);
		memset(buffer,0, sizeof(buffer));
		size = recvfrom(s,buffer,1024,0,(struct sockaddr *)&fromaddr, &len);
		if(size < 0)
		{
			perror("recvfrom");
			return -1;
		}
		printf("receive message :%s\n", buffer);
		char host[32];
		char server[32];
		//printf("Portis %d\n", fromaddr.sin_port);
		getnameinfo((const struct sockaddr *)&fromaddr,sizeof(fromaddr), host,sizeof(host),server,sizeof(server),0);
		printf("host:%s,server:%s\n",host,server);
		size = sendto(s,sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&fromaddr,sizeof(fromaddr));
	}

	ret = setsockopt(s,IPPROTO_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(mreq));
	if(ret < 0)
	{
		perror("IP_DROP_MEMBERSHIP");
		return -1;
	}
	close(s);
	return 0;
}
