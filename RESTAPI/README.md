# Web Client. Communication with REST API.

# Code Skeleton

For the implementation of this project, I started from the code skeleton provided in the lab exercises. I utilized and adapted the `helpers.c` and `buffer.c` files without modifying them. In particular, I extensively used functions from `helpers.c`, such as `open_connection`, `close_connection`, `send_to_server`, `receive_from_server`, and `basic_extract_json_response`.

Additionally, I used the `requests.c` file from the lab exercises but modified and implemented it according to the project requirements. In this file, I implemented the following functions:

- `compute_get_request`: Composes a GET request that needs to be sent to the server. It includes parameters such as host, URL, query parameters, authentication token, cookies, and their count.

- `compute_post_request`: Composes a POST request that needs to be sent to the server. It includes parameters such as host, URL, content type, authentication token, payload (request body), cookies, and their count.

- `compute_delete_request`: Composes a DELETE request that needs to be sent to the server. It includes parameters such as host, URL, query parameters, authentication token, cookies, and their count.

# JSON Library

I chose the nlohmann JSON library for manipulating JSON objects due to its good documentation and ease of use. To print a JSON object, I used the `dump(1)` function with an indentation level of 1 for better readability. Adding a field to a JSON object is straightforward, similar to working with an array, and parsing is done using the `parse` function. For searching within JSON objects, I used the `contains` function.

## Client Implementation

For the client implementation, I ensured proper memory allocation and deallocation whenever necessary. If memory allocation failed, an error message is displayed.

For each command, I implemented a separate function:

### Client Registration

- The `handle_register` function allows the registration of a new account.
- It reads the username and password from the keyboard.
- A connection to the server is opened, and a JSON object is created with the entered data.
- The POST request is sent to the server.
- The server response is checked, and corresponding messages are displayed.
- The connection is closed at the end.

### Client Authentication

- The `handle_login` function performs authentication for an existing account.
- The username and password are read from the keyboard.
- A JSON object is created with the entered data, and a POST request for authentication is sent to the server.
- The server response is checked, and the session cookie is saved for later use.
- The connection is closed at the end.

### Access Request

- The `handle_access` function requests access.
- A GET request is sent to the server with the session cookie.
- The server response is checked, and the token is saved for later use.
- The connection is closed at the end.

### Book Information

- The `get_books` function displays information about all the books.
- A GET request is sent to the server with the access token.
- The list of books or an error message is displayed based on the server response.
- The connection is closed at the end.

### Information about a Specific Book

- The `get_book` function displays information about a specific book.
- The book ID is read from the keyboard and sent in the GET request.
- The server response is checked, and information about the book or an error message is displayed accordingly.
- The connection is closed at the end.

### Add Book

- The `add_book` function adds a new book.
- Book details are read from the keyboard and transformed into a JSON object.
- A POST request is sent to the server with the access token and book data.
- The server response is checked, and a corresponding message is displayed.
- The connection is closed at the end.

### Delete Book

- The `delete_book` function deletes a book.
- The ID of the book to be deleted is read from the keyboard and sent in the DELETE request.
- The request is sent to the server with the access token.
- The server response is checked, and a corresponding message is displayed.
- The connection is closed at the end.

### Client Logout

- The `handle_logout` function performs the client logout.
- A GET request is sent to the server to close the session.
- The server response is checked, and a corresponding message is displayed.
- The connection is closed at the end.
- The session cookie and access token are cleared from memory.

The main program runs in an infinite loop where it reads commands from the user and calls the corresponding function accordingly.
