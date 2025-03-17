1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines that the command's output is fully received through the use of an end-of-file/EOF marker. The EOF character (RDSH_EOF_CHAR) signifies that the server has finished sending data. To handle partial reads, the client could read in fixed-size chunks and checking for the EOF marker after each read. In the event of a partial read, the client should continue reading the remaining data until it receives the full message, to make sure of a complete message transmission.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

To handle the lack of message boundaries in TCP, the networked shell protocol should define a clear marker or delimiter (like a special character or message length) to indicate the start and end of a command. Without this, there may be issues where data is received in incomplete chunks, causing wrong or shortened commands being processed.

3. Describe the general differences between stateful and stateless protocols.

Stateful protocols maintain information about the session or interaction across multiple exchanges, while stateless protocols treat each request as independent, with no memory of previous requests. This makes stateful protocols better for complex interactions, while stateless protocols are simpler and scale better.


4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used for applications where low latency and speed are more important than reliability, such as streaming or real-time communications, where occasional packet loss is acceptable. It offers faster performance because it doesn't require the overhead of establishing a connection or guaranteeing delivery.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides the socket interface/abstraction for applications to use network communications, allowing processes to send and receive data over the network using protocols like TCP or UDP.