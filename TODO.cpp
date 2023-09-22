//TODO: Request still needs to be fixed in the file and the body when it comes to the POST method

/*
* *After parsing the HTTP request, your web server should typically perform the following steps to handle the request and generate an appropriate response:

! Request Validation and Routing:
Validate the parsed request to ensure it's well-formed and contains all required information.
Implement a routing mechanism to determine how to handle the request based on the request path, HTTP method, and other factors.
Map the request to a specific handler or controller based on the routing rules.

! Authentication and Authorization:
Authenticate the client if your application requires it. This might involve checking credentials, tokens, or session information.
Perform authorization checks to determine if the authenticated client has permission to access the requested resource or perform the requested action.

! Business Logic Processing:
Execute the appropriate business logic based on the request and the route.
If it's a GET request, retrieve data or generate a response based on the requested resource.
For POST and PUT requests, process incoming data and update the server's state or database accordingly.
Handle any other relevant HTTP methods and actions.

! Response Generation:
Create an HTTP response with the appropriate status code and headers.
Set the content type and length headers based on the response data.
Include any required cookies or session information.
Generate the response body, which could be HTML, JSON, XML, or any other content type.

! Error Handling:
Implement error handling to catch exceptions or unexpected situations during request processing.
Return meaningful error responses with appropriate status codes and error messages.

! Sending the Response:
Send the generated response back to the client using the established client socket.
Ensure the response is properly formatted as per the HTTP protocol.

! Logging and Monitoring:
Log important events and actions, both for debugging purposes and for monitoring server health.
Implement metrics and monitoring solutions to track server performance and usage.

! Clean Up:
Release any acquired resources, such as closing database connections or cleaning up temporary files.
Close the client socket if it's not designed for persistent connections (e.g., HTTP/1.1 with Keep-Alive).

!Repeat for Additional Requests:
Your server should continue listening for and handling incoming requests in a loop.

!Testing:
Implement unit tests and integration tests to verify the correctness of your server and its handling of various types of requests and scenarios.

!Optimization and Scaling:
As your application grows, optimize code and database queries for better performance.
Consider load balancing and horizontal scaling strategies to handle increased traffic.

!Documentation:
Maintain clear and up-to-date documentation for your API or web service, describing the available endpoints, request/response formats, and usage instructions.

!Security:
Implement security best practices, including input validation, protection against common web vulnerabilities (e.g., SQL injection, XSS), and regular security audits.
Remember that the specific steps and requirements may vary depending on your project's goals and technology stack. Building a web server is a complex task, and continuous development, testing, and maintenance are essential for a robust and reliable system.

*/


/**
 * Runs main loop that goes through all file descriptors from 0 till the biggest fd in the set.
 * - check file descriptors returend from select():
 *      if server fd --> accept new client
 *      if client fd in read_set --> read message from client
 *      if client fd in write_set:
 *          1- If it's a CGI response and Body still not sent to CGI child process --> Send request body to CGI child process.
 *          2- If it's a CGI response and Body was sent to CGI child process --> Read outupt from CGI child process.
 *          3- If it's a normal response --> Send response to client.
 * - servers and clients sockets will be added to _recv_set_pool initially,
 *   after that, when a request is fully parsed, socket will be moved to _write_set_pool
 */
