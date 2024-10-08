#ifndef _REQUESTS_
#define _REQUESTS_

// Function to compute a GET request
// Arguments:
// - host: the host to which the request is sent
// - url: the URL path of the resource
// - query_params: optional query parameters
// - token: optional token for authentication
// - cookies: array of cookies to include in the request
// - cookies_count: number of cookies in the cookies array
// Returns:
// - a string representing the computed GET request
char *compute_get_request(char const *host, 
                          char const *url, 
                          char const *query_params,
                          char *token,
                          char **cookies, 
                          int cookies_count);


// Function to compute a POST request
// Arguments:
// - host: the host to which the request is sent
// - url: the URL path of the resource
// - content_type: the content type of the payload
// - token: optional token for authentication
// - payload: the payload data to include in the request
// - cookies: array of cookies to include in the request
// - cookies_count: number of cookies in the cookies array
// Returns:
// - a string representing the computed POST request
char *compute_post_request(char const *host, 
                           char const *url, 
                           char const *content_type, 
                           char *token,
                           std::string payload, 
                           char **cookies, 
                           int cookies_count);

// Function to compute a DELETE request
// Arguments:
// - host: the host to which the request is sent
// - url: the URL path of the resource
// - query_params: optional query parameters
// - token: optional token for authentication
// - cookies: array of cookies to include in the request
// - cookies_count: number of cookies in the cookies array
// Returns:
// - a string representing the computed DELETE request
char *compute_delete_request(char const *host, 
                             char const *url, 
                             char const *query_params,
                             char *token,
                             char **cookies, 
                             int cookies_count);

#endif
