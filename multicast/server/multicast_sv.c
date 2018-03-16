#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#define MCAST_PORT 8888
#define MCAST_ADDR "224.0.0.88"
int main(int argc, char **argv)
{
	int ret, s,i = 1;
	struct sockaddr_in Multi_addr;
	struct sockaddr_in client_addr;
	s = socket(AF_INET,SOCK_DGRAM,0);
	if(s < 0)
	{
		perror("socket error\n");
		return -1;
	}
	Multi_addr.sin_family = AF_INET;
	Multi_addr.sin_port = htons(MCAST_PORT);
	Multi_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);
	char buffer[1024];
	int flag = 0;
	for(;;)
	{
		//if(!flag)
		{
			printf("start to send\n");
			flag = 1;
		}
		memset(buffer,0,sizeof(buffer));
		sprintf(buffer,"%d",i);
		int size = sendto(s,buffer,strlen(buffer),0,(struct sockaddr*)&Multi_addr,sizeof(Multi_addr));
		if(size < 0)
		{
			perror("sendto error");
		}
		sleep(1);
		i++;
		memset(buffer,0,sizeof(buffer));
		int  len = sizeof(client_addr);
		//if(flag == 1)
		{
			printf("start to recv\n");
			flag = 2;
		}
		size = recvfrom(s, buffer, 1024,0,(struct sockaddr *)&client_addr,&len);
		char host[32] = {0};char server[32]= {0};
		getnameinfo((const struct sockaddr *)&client_addr, len, host,sizeof(host),server,sizeof(server),0);
		printf("%s,%s\n", host,server);
		write(1, buffer, size);
	}
	close(s);
}
