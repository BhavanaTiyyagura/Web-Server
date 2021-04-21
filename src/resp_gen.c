
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>

char* response_generator(char* path)
{
	FILE* fp;
	long numbytes;
	
	char* final_path = (char*)malloc(sizeof(char)*(strlen(path)+2));
	final_path[0] = '.';
	strcat(final_path,path);
	
	char size[5];
	char* buffer_data=NULL; 
	long int len;
	long int size_of_message;
	
	char* message = (char*)malloc(sizeof(char)*100);
	
	fp =  fopen(final_path,"r");
	
	if (path == NULL) 
	{
	size_of_message = strlen("HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nContent-Type: text/html\r\n") + 1;
	message = (char*)realloc(message,sizeof(char)*(size_of_message));
        strcpy (message, "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\nContent-Type: text/html\r\n");
        return message;
    	}
	
	if (!fp) 
	{
	size_of_message = strlen("HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/html\r\n") + 1;
	message = (char*)realloc(message,sizeof(char)*(size_of_message));
    	perror("Failed to open text file\n");
    	strcpy (message, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/html\r\n");
    	return message;
    	}
    	
    	size_of_message = strlen("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: ") + 1;
    	message = (char*)realloc(message,sizeof(char)*(size_of_message));
    	strcpy(message,"HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ");
	 
	fseek(fp,0L,SEEK_END);
	numbytes = ftell(fp);
	
	fseek(fp,0L,SEEK_SET);
	
	buffer_data = (char*)malloc(sizeof(char)*(numbytes+1));
	fread(buffer_data,sizeof(char),numbytes,fp);
	
	buffer_data[numbytes+1]='\0';
	
	fclose(fp);
	
	sprintf(size,"%ld",numbytes);
	
	len = size_of_message + strlen(size) + strlen(buffer_data) + 3;
	
	message = (char*)realloc(message,sizeof(char)*(len));
	
	//form the final message
	strcat(message,size);
	strcat(message,"\n\n");
	strcat(message,buffer_data);
	
	//add the null terminator
	message[len-1] = 0;
	 
	free(buffer_data);
	
	free(final_path);
	
	return message;
	
	
}
