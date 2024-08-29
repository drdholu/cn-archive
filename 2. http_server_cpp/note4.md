### Explanation

Let's break down each part of the code you provided:

1. **Creating a `std::string` from a `char` buffer:**
    ```cpp
    string request(buffer, bytesReceived);
    ```
2. **Printing the received request:**
    ```cpp
    cout << "Received request:\n" << request << endl;
    ```
3. **Handling the request and generating a response:**
    ```cpp
    string response = handleRequest(request);
    ```
4. **Sending the response back over the socket:**
    ```cpp
    send(new_socket, response.c_str(), response.length(), 0);
    ```

### Detailed Explanation:

#### 1. Creating a `std::string` from a `char` buffer
```cpp
string request(buffer, bytesReceived);
```
- **Constructor Usage:** This line creates a `std::string` object named `request` using a constructor of `std::string` that takes two arguments: a pointer to a character array (buffer) and the number of characters to include (bytesReceived).
- **Purpose:** The constructor `string(const char* s, size_t n)` creates a string that contains the first `n` characters from the character array `s`.
- **Significance:** This is useful when reading data into a buffer from a socket or file, where the buffer may not be null-terminated. By specifying the number of bytes received (`bytesReceived`), you ensure that the `std::string` object contains exactly the data that was read, without relying on null-termination.

#### 2. Printing the received request
```cpp
cout << "Received request:\n" << request << endl;
```
- **Purpose:** This prints the received request to the standard output (typically the console). It's useful for debugging or logging to see what data has been received.

#### 3. Handling the request and generating a response
```cpp
string response = handleRequest(request);
```
- **Passing the `request` to `handleRequest`:**
  - **Type:** The `request` is passed as a `std::string` to the `handleRequest` function.
  - **Value:** The value of `request` is a string containing the data received from the socket, limited to the first `bytesReceived` characters.
  - **Purpose:** The `handleRequest` function processes the request and generates an appropriate response. The exact implementation of `handleRequest` depends on the application's logic (e.g., parsing an HTTP request, performing some action, and generating an HTTP response).

#### 4. Sending the response back over the socket
```cpp
send(new_socket, response.c_str(), response.length(), 0);
```
- **Purpose:** This line sends the response back to the client over the socket.
- **Parameters:**
  - `new_socket`: The socket descriptor through which to send the data.
  - `response.c_str()`: A pointer to the null-terminated character array representing the response. The `c_str()` method returns a `const char*` pointer to the string's internal buffer.
  - `response.length()`: The length of the response string in bytes.
  - `0`: Flags for the `send` function, typically set to `0` for default behavior.

### Summary:

1. **Creating the `request` string:**
   - **Buffer and `bytesReceived`:** The constructor `string(buffer, bytesReceived)` ensures that the `request` string contains exactly the data received from the socket, even if it is not null-terminated.

2. **Printing the `request`:**
   - **Purpose:** Displays the received request for debugging or logging.

3. **Handling the request:**
   - **`handleRequest(request)`:** Processes the request string and returns a response string.

4. **Sending the response:**
   - **`send(new_socket, response.c_str(), response.length(), 0)`:** Sends the response back to the client over the socket using the `send` function.

By creating the `std::string` from the buffer with the exact number of received bytes, you ensure that your string accurately represents the received data, regardless of whether the buffer is null-terminated. This makes it suitable for passing to other functions and for further processing.