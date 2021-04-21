#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>



char* response_generator(char* path);


 char* process_request(char buffer[30000])
{	
	const char *end_of_method = strchr(buffer,' ');
	const char *start_of_the_query = strchr(end_of_method+1,' ');
	char* response = NULL;
	
	char method[end_of_method - buffer];
	char path[start_of_the_query - end_of_method -1];
	
	strncpy(method,buffer,end_of_method - buffer);
	strncpy(path,end_of_method+1,start_of_the_query - end_of_method -1);
	
	method[sizeof(method)] = 0;
	path[sizeof(path)] = 0;
	
	//printf("method: %s\n size: %ld\n",method,sizeof(method));
	//printf("path: %s\n size: %ld\n",path,sizeof(path));
		
	if(strcmp(method,"GET")!=0)
	{
		perror("unable to process request");
		exit(EXIT_FAILURE);
	}
	
	response = response_generator(path);
	return response;
					
}


int main(int argc, char const *argv[])
{	
	unsigned short PORT = 8080;
	struct sockaddr_in address;
	int size = sizeof(address);
	
	int new_socket,len;
	long valread;
	
	memset((char*)&address,0,size);
	
	char* post_message = NULL;
	
	//network is big endian and the host can be either. htons() converts host to network 
	
	address.sin_port = htons(PORT); 
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY; //OS sets the network interface
	
	
	/*set up the socket that constantly listens for connections */
	
	int server_fd = socket(AF_INET,SOCK_STREAM,0);
	
	if(server_fd < 0)
	{
		perror("Cannot get socket");
		exit(EXIT_FAILURE);
	}
	
	/* bind the socket to the PORT */
	
	if(bind(server_fd,(struct sockaddr *)&address,size) < 0)
	{
	
		perror("Cannot be binded");
		exit(EXIT_FAILURE);
	}
	
	if(listen(server_fd,10)<0)
	{
	
		perror("No connections to be made in listen");
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
	
	printf("-----SERVER WAITING FOR CONNECTION-----");
	if((new_socket = accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&size))<0)
	{
	
		perror("In accept state");
		exit(EXIT_FAILURE);
	}
	
	char buffer[30000] = {0};
	valread = read(new_socket,buffer,30000);

	post_message = process_request(buffer);
	
	len = strlen(post_message + 1);
	
	post_message = (char*)realloc(post_message,sizeof(char)*(len));
	
	write(new_socket,post_message,strlen(post_message));
	
	free(post_message);
	
	printf("-----SERVER SENT A MESSAGE-----");
	
	close(new_socket);
	
	}
	
	return 0;
}

