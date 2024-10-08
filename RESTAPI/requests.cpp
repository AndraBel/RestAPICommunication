#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <bits/stdc++.h>  // This header seems unnecessary and non-standard
#include "helpers.h"
#include "requests.h"

// Function to add cookies to the message
void addCookie(char **cookies, 
			   int cookies_count,
			   char *line, char *message)
{
	memset(line, 0, LINELEN);
	strcat(line, "Cookie: ");
	for (int i = 0; i < cookies_count; i++) {
		strcat(line, cookies[i]);
		if (i != cookies_count - 1) {
			strcat(line, "; ");
		}
	}
	compute_message(message, line);
}

// Function to add host to the message
void addHost(char *line, const char *host, char *message)
{
	memset(line, 0, LINELEN);
	sprintf(line, "Host: %s", host);
	compute_message(message, line);
}

// Function to compute a GET request
char *compute_get_request(char const *host, 
						  char const *url, 
						  char const *query_params,
						  char *token,
						  char **cookies, 
						  int cookies_count)
{
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

	// Write method name, URL, and protocol type
	if (query_params != NULL) {
		sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "GET %s HTTP/1.1", url);
	}
	compute_message(message, line);

	// Add host
	addHost(line, host, message);

	// Add token if exists
	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
	}

	// (optional): Add cookies
	if (cookies != NULL) {
		addCookie(cookies, cookies_count, line, message);
	}

	// Add final new line
	compute_message(message, "");
	free(line);
	return message;
}

// Function to compute a POST request
char *compute_post_request(char const *host, 
						   char const *url, 
						   char const *content_type, 
						   char *token,
						   std::string payload, 
						   char **cookies, 
						   int cookies_count)
{
	int size = payload.size();
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

	// Write method name, URL, and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);
	
	// Add host
	addHost(line, host, message);

	// Add token if exists
	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
	}

	// Add content type
	sprintf(line, "Content-Type: %s", content_type);
	compute_message(message, line);

	// Add content length
	sprintf(line, "Content-Length: %d", size);
	compute_message(message, line);

	// (optional): Add cookies
	if (cookies != NULL) {
		addCookie(cookies, cookies_count, line, message);
	}

	// Add newline at end of header
	compute_message(message, "");

	// Add payload
	strcat(message, payload.c_str());

	// Free allocated memory for line
	free(line);
	return message;
}

// Function to compute a DELETE request
char *compute_delete_request(char const *host, 
						  char const *url, 
						  char const *query_params,
						  char *token,
						  char **cookies, 
						  int cookies_count)
{
	char *message = (char*)calloc(BUFLEN, sizeof(char));
	char *line = (char*)calloc(LINELEN, sizeof(char));

	// Write method name, URL, and protocol type
	if (query_params != NULL) {
		sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "DELETE %s HTTP/1.1", url);
	}

	compute_message(message, line);

	// Add host
	addHost(line, host, message);

	// Add token if exists
	if (token != NULL) {
		sprintf(line, "Authorization: Bearer %s", token);
		compute_message(message, line);
	}

	// (optional): Add cookies
	if (cookies != NULL) {
		addCookie(cookies, cookies_count, line, message);
	}

	// Add final new line
	compute_message(message, "");
	free(line);
	return message;
}
