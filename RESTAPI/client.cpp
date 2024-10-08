#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <bits/stdc++.h> // C++ headers

#include "nlohmann/json.hpp" // JSON library
#include "helpers.h" // Helper functions
#include "requests.h" // HTTP request functions

#define COMMAND_SIZE 100 // Size of command
#define USERNAME_SIZE 100 // Size of username
#define PASSWORD_SIZE 100 // Size of password
#define COOKIE_SIZE 200 // Size of cookie
#define TOKEN_SIZE 300 // Size of authentication token
#define PORT 8080 // Server port
#define CONTENT_TYPE "application/json" // Content type for requests

// API routes
#define REGISTER_ROUTE "/api/v1/tema/auth/register"
#define LOGIN_ROUTE "/api/v1/tema/auth/login"
#define ACCESS_ROUTE "/api/v1/tema/library/access"
#define BOOKS_ROUTE "/api/v1/tema/library/books"
#define ADD_BOOK_ROUTE "/api/v1/tema/library/books"
#define GET_BOOK_ROUTE "/api/v1/tema/library/books/"
#define DELETE_BOOK_ROUTE "/api/v1/tema/library/books/"
#define LOGOUT_ROUTE "/api/v1/tema/auth/logout"

using namespace std;
using json = nlohmann::json; // JSON alias

const char *host = "34.246.184.49"; // Server IP address

// Function to handle "Too many requests" response
bool handle_to_many_request(char *response)
{
    if (strstr(response, "Too many requests") != NULL) {
        cout << "Too many requests, please try again later." << endl;
        return false;
    }
    return true;
}

// Function to extract cookie from server response
void extract_cookie_from_response(char *response, char *cookie)
{
    // Find "Set-Cookie:" header in response
    if (strstr(response, "Set-Cookie: ") != NULL) {
        char *cookie_start = strstr(response, "Set-Cookie: ") + strlen("Set-Cookie: ");
        char *cookie_end = strstr(cookie_start, ";");
        strncpy(cookie, cookie_start, cookie_end - cookie_start); // Copy cookie value
    }
}

// Function to handle user registration
void handle_register()
{
    char username[USERNAME_SIZE];
    string password;

    cout << "username=";
    std::cin >> std::ws; // Clear whitespace from input buffer
    cin.getline(username, COMMAND_SIZE); // Read username

    cout << "password=";
    cin >> password; // Read password

    // Check for invalid username format
    if (string(username).find_first_of(' ') != std::string::npos) {
        cout << "ERROR: Invalid username format." << endl;
        return;
    }

    // Create JSON payload for registration
    json json_payload;
    json_payload["username"] = username;
    json_payload["password"] = password;
    string payload = json_payload.dump();

    // Send registration request to server
    char *message = compute_post_request(host, REGISTER_ROUTE, CONTENT_TYPE, NULL, payload, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        cout << "SUCCESS: User successfully registered!" << endl;
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to handle user login
void handle_login(char *login_cookie)
{
    // Read username and password from user input
    char username[USERNAME_SIZE];
    string password;
    cout << "username=";
    std::cin >> std::ws; // Clear whitespace from input buffer
    cin.getline(username, COMMAND_SIZE); // Read username
    cout << "password=";
    cin >> password; // Read password

    // Check for invalid username format
    if (string(username).find_first_of(' ') != std::string::npos) {
        cout << "ERROR: Invalid username format." << endl;
        return;
    }

    // Create JSON payload for login
    json json_payload;
    json_payload["username"] = username;
    json_payload["password"] = password;
    string payload = json_payload.dump();

    // Send login request to server
    char *message = compute_post_request(host, LOGIN_ROUTE, CONTENT_TYPE, NULL, payload, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        // Extract cookie from server response
        extract_cookie_from_response(server_response, login_cookie);
        cout << "SUCCESS: User has been successfully logged in." << endl;
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to handle access to library
void handle_access(char **cookies, int cookies_count, char *auth_token)
{
    // Send access request to server
    char *message = compute_get_request(host, ACCESS_ROUTE, NULL, NULL, cookies, cookies_count);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            } else if (json_response.contains("token")) {
                memset(auth_token, 0, TOKEN_SIZE);
                strcpy(auth_token, json_response["token"].get<std::string>().c_str());
                cout << "SUCCESS: The user has access to the library." << endl;
            }
        }
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to retrieve list of books from server
void get_books(char *auth_token)
{
    // Send request to get list of books
    char *message = compute_get_request(host, BOOKS_ROUTE, NULL, auth_token, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    char *books = strstr(server_response, "[");
    if (books != NULL){
        json json_response = json::parse(books);
        cout << json_response.dump(2).c_str() << endl;
    } else if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        cout << json::array() << endl;
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to add a book to the library
void add_book(char *auth_token)
{
    // Read book details from user input
    char title[COMMAND_SIZE], author[COMMAND_SIZE], genre[COMMAND_SIZE], page_count[COMMAND_SIZE], publisher[COMMAND_SIZE];
    cout << "title=";
    cin.getline(title, COMMAND_SIZE);
    cin.getline(title, COMMAND_SIZE);
    cout << "author=";
    cin.getline(author, COMMAND_SIZE);
    cout << "genre=";
    cin.getline(genre, COMMAND_SIZE);
    cout << "publisher=";
    cin.getline(publisher, COMMAND_SIZE);
    cout << "page_count=";
    cin.getline(page_count, COMMAND_SIZE);

    // Check for empty fields
    if (string(title).empty() || string(author).empty() || string(genre).empty() ||
        string(publisher).empty() || string(page_count).empty()) {
        cout << "ERROR: One or more fields are empty. All fields are required." << endl;
        return;
    }

    // Check if page_count is a number
    try {
        stoi(page_count);
    } catch (...) {
        cout << "ERROR: page_count must be a number." << endl;
        return;
    }

    // Create JSON payload for adding book
    json json_payload;
    json_payload["title"] = title;
    json_payload["author"] = author;
    json_payload["genre"] = genre;
    json_payload["publisher"] = publisher;
    json_payload["page_count"] = page_count;
    string payload = json_payload.dump();

    // Send request to add book
    char *message = compute_post_request(host, ADD_BOOK_ROUTE, CONTENT_TYPE, auth_token, payload, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        cout << "SUCCESS: The book has been added to the library." << endl;
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to get details of a book by ID
void get_book(char *auth_token)
{
    string id;
    cout << "id=";
    cin >> id;

    // Check if ID is a number
    try {
        stoi(id);
    } catch (...) {
        cout << "ERROR: id must be a number." << endl;
        return;
    }

    string route = GET_BOOK_ROUTE + id;

    // Send request to get book details
    char *message = compute_get_request(host, route.c_str(), NULL, auth_token, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            } else {
                cout << json_response.dump(2).c_str() << endl;
            }
        }
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to delete a book by ID
void delete_book(char *auth_token)
{
    string id;
    cout << "id=";
    cin >> id;

    // Check if ID is a number
    try {
        stoi(id);
    } catch (...) {
        cout << "ERROR: id must be a number." << endl;
        return;
    }

    string route = DELETE_BOOK_ROUTE + id;

    // Send request to delete book
    char *message = compute_delete_request(host, route.c_str(), NULL, auth_token, NULL, 0);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        cout << "SUCCESS: The book with ID "<< id << " was successfully deleted." << endl;
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

// Function to handle user logout
void handle_logout(char **cookies, int cookies_count, char *auth_token){

    // Send logout request to server
    char *message = compute_get_request(host, LOGOUT_ROUTE, NULL, NULL, cookies, cookies_count);
    int sockfd = open_connection(const_cast<char*>(host), PORT, AF_INET, SOCK_STREAM, 0);
    send_to_server(sockfd, message);

    // Receive and process server response
    char *server_response = receive_from_server(sockfd);
    char* response = basic_extract_json_response(server_response);

    if (response != NULL) {
        if (handle_to_many_request(response)) {
            json json_response = json::parse(response);
            if (json_response.contains("error")) {
                cout << "ERROR: " << json_response["error"].get<std::string>().c_str() << endl;
            }
        }
    } else {
        cout << "SUCCESS: The user has successfully logged out!" << endl;
        memset(auth_token, 0, TOKEN_SIZE);
        memset(cookies[0], 0, COOKIE_SIZE);
    }

    close_connection(sockfd);
    free(message);
    free(server_response);
}

int main(int argc, char *argv[])
{
	string command;

    // Initialize cookies and authentication token
    int cookies_count = 1;
    char **cookies = (char **)calloc(cookies_count, sizeof(char *));
    if (cookies == NULL) {
        error("ERROR allocating memory for cookies");
    }

    cookies[0] = (char *)calloc(1, COOKIE_SIZE * sizeof(char));
    if (cookies[0] == NULL) {
        error("ERROR allocating memory for cookie");
    }

    char auth_token[TOKEN_SIZE];
    memset(auth_token, 0, TOKEN_SIZE);

    // Main command loop
    while (true) {
        command.clear();
        cin >> command;

        if (command.compare("register") == 0) {
            handle_register();
        } else if (command.compare("login") == 0) {
            handle_login(cookies[0]);
        } else if (command.compare("enter_library") == 0) {
            handle_access(cookies, cookies_count, auth_token);
        } else if (command.compare("get_books") == 0) {
            get_books(auth_token);
        } else if (command.compare("add_book") == 0) {
            add_book(auth_token);
        } else if (command.compare("get_book") == 0) {
            get_book(auth_token);
        } else if (command.compare("delete_book") == 0) {
            delete_book(auth_token);
        } else if (command.compare("logout") == 0) {
            handle_logout(cookies, cookies_count, auth_token);
        } else if (command.compare("exit") == 0) {
            break;
        } else {
            cout << "Unrecognized command." << endl;
        }

    }

    // Free allocated memory
    free(cookies[0]);
    free(cookies);
    return 0;
}
