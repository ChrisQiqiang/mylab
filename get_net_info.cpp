#include <stdio.h>
#include <string>
#include <fstream>
#include <time.h>
#include <memory.h>
#include <unistd.h>
#include <sys/time.h>
#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <arpa/inet.h>

using namespace std;

#define MAX_DATA 8

int main( )
{
    unsigned int  sleep_time = 100000;
	struct timeval t_start,t_end; 
	time_t rawtime;
	struct tm* localtm = NULL;
	float useage_net;

	unsigned long data[MAX_DATA];
	memset(data,0,sizeof(unsigned long)*MAX_DATA);

	fstream hFile("netinfo.txt", fstream::out);
	if (!hFile.is_open())
	{
		cout << "open file error!" << endl;	
		return 0;
	}


	//int localSocket;
	//struct sockaddr_in localAddr;
	
	//localSocket = socket(AF_INET, SOCK_DGRAM, 0);
	//bzero(&localAddr, sizeof(localAddr));
	//localAddr.sin_family = AF_INET;
	//localAddr.sin_port = htons(88);
	//localAddr.sin_addr.s_addr = INADDR_ANY;
	//localAddr.sin_addr.s_addr=inet_addr("54.211.231.94");
	
	//cout << "socket returned : " << localSocket << endl;
	//int result = bind(localSocket, (struct sockaddr *)&localAddr, sizeof(localAddr));
	//cout << "bind returned : " << result << endl;

	while(true)
	{
		//printf("running\n");
		ifstream fin("/proc/net/dev");

		gettimeofday(&t_start, NULL); 
		long start = ((long)t_start.tv_sec)*1000+(long)t_start.tv_usec/1000;
		
		while(!fin.eof())
		{
			string inbuf;
			int key_pos;
			getline(fin,inbuf,'\n');
			key_pos = inbuf.find("ens3",0);
			if(key_pos != string::npos && inbuf.find("pens3") == string::npos)
			{
				string & str = inbuf.erase(0,key_pos);
				
				unsigned long v;
				float useage_net;
				sscanf(str.c_str(),
						"ens3:%lu %lu %lu %lu %lu %lu %lu %lu \
							%lu %lu %lu %lu %lu %lu %lu %lu/n",
							&data[0],&data[1],&v,&v,&v,&v,&v,&v,\
							&data[2],&data[3],&v,&v,&v,&v,&v,&v);
				fin.close();
				break;
			}
		}		
		
		usleep(sleep_time);

		ifstream fin2("/proc/net/dev");
		
		time(&rawtime);
		localtm = localtime(&rawtime);
		gettimeofday(&t_end, NULL);
		long end = ((long)t_end.tv_sec)*1000+(long)t_end.tv_usec/1000; 
		//printf("running 2\n");

		while(!fin2.eof())
		{
			
			string inbuf;
			int key_pos;
			getline(fin2,inbuf,'\n');
			key_pos = inbuf.find("ens3",0);
			if(key_pos != string::npos && inbuf.find("pens3") == string::npos)
			{
				string & str = inbuf.erase(0,key_pos);
				unsigned long v;
				float useage_net;
				sscanf(str.c_str(),
					"ens3:%lu %lu %lu %lu %lu %lu %lu %lu \
					%lu %lu %lu %lu %lu %lu %lu %lu/n",
					&data[4],&data[5],&v,&v,&v,&v,&v,&v,\
					&data[6],&data[7],&v,&v,&v,&v,&v,&v);	
				fin2.close();	
				break;
			}
		}

		long int recv = (data[4]-data[0])*10;
        long int send = (data[6]-data[2])*10;
		//printf("recv:%ld, send:%ld\n", recv, send);
		hFile << asctime(localtm) << ": " << send << " " << recv << endl;
		//useage_net = 1 - (float)(data[4]-data[0]+data[6]-data[2])*8/((end-start)*1000000);
		//printf("%ld, %ld , %ld , %ld\n",data[4],data[0],data[6],data[2]);
		//printf("usage %f\t time %ld\n\n",  useage_net , end-start );
		

		//char sendbuffer[7];
		//memset(sendbuffer, 0, sizeof(sendbuffer));
		//sprintf(sendbuffer,"%.4f",  useage_net);

		//char buffer[102];
		//int result = recvfrom(localSocket, &buffer, sizeof(buffer), 0, (struct sockaddr *)&remoteAddr, &remoteAddrLength);
		//cout << "received UDP data." << buffer << endl;
		
		//int result = sendto(localSocket, sendbuffer, sizeof(sendbuffer),0, (struct sockaddr *)&localAddr ,sizeof(localAddr));
		//printf("send return %d\n",result);
	}
	//close(localSocket);
	hFile.close();
	return 0;
}
