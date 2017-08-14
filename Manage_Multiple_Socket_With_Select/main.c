#include    "general.h"
#include    <arpa/inet.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <sys/time.h>

#define PORT 8888
#define MAX_CLIENTS 30

int 
main (int argc, char * argv[])
{
    int i;
    int sd;
    int opt = TRUE;
    int max_sd;
    int valread;
    int addr_len;
    int activity;
    int new_socket;
    int master_socket;
    int client_socket[MAX_CLIENTS];
    char buffer[1025];  // Data buffer of 1K
    char * message = "ECHO DAEMON v1.0 \r\n";

    fd_set readfds;
    struct sockaddr_in address;

    // Initialise all client_socket[] to 0 so not checked
    memset (client_socket, 0, sizeof(client_socket));

    /*
     * int socket (int domain, int type, int protocol)
     * domain:
     *      AF_INET -- IPv4 Internet Protocols
     * type:
     *      SOCK_STREAM -- Provides sequenced
     */
    if (0 == (master_socket = socket (AF_INET, SOCK_STREAM, 0)))
        errExit ("Socket System Call failed.");

    /*
     * Set master socket to allow multiple connections.
     * Get and Set options on sockets
     * int setsockopt (int sockfd, int level, int optname,
     *                  const void * optval, socklen_t optlen)
     * level:
     *      SOL_SOCKET -- To manipulate options at the sockets API level.
     *  optname:
     *      SO_REUSEADDR -- Reuse of local addresses is supported.
     */
    if (0 > setsockopt (master_socket, SOL_SOCKET,
                SO_REUSEADDR, (char *) &opt, sizeof (opt)))
        errExit ("Setsockopt System Call failed.");

    address.sin_family      = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port        = htons (PORT);

    // Bind the socket to localhost port 8888
    if (0 > bind (master_socket, (struct sockaddr *) &address, sizeof (address)))
        errExit ("Bind System Call failed.");

    printf ("Listener on port %d\n", PORT);

    /*
     * Try to specify maximum of 3 pending
     * connections for the master socket.
     *
     * Listen for connections on a socket
     * int listen (int sockfd, int backlog)
     * backlog:
     *      This argument defines the maximum length to which
     *      the queue of pending connections for sockfd may grow.
     */
    if ( 0 > listen (master_socket, 3))
        errExit ("Listen System Call failed.");

    // Accept the incoming connection.
    addr_len = sizeof (address);
    printf("Waiting for connections ...\n");

    while (TRUE)
    {
        // Clear the socket set
        FD_ZERO (&readfds);

        // Add master socket to set
        FD_SET (master_socket, &readfds);
        max_sd = master_socket;

        for (i = 0; i < MAX_CLIENTS; i++)
        {
            // Socket descriptor
            sd = client_socket[i];

            // If valid socket descriptor then add to read list
            if (0 < sd)
                FD_SET (sd, &readfds);

            // Highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }

        // Wait for an activity on one of the sockets,
        // timeout is NULL, so wait indefinitely
        activity = select (max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((0 > activity) && (EINTR != errno))
            errExit ("Select System Call failed.");

        // If something happened on the master socket, then its an incoming connection
        if (FD_ISSET (master_socket, &readfds))
        {
            if ( 0 > (new_socket = accept (master_socket, 
                            (struct sockaddr *) &address, (socklen_t *) &addr_len)))
                errExit ("Accept System Call failed");
            
            printf("New connection, socket fd is %d, IP is : %s, port : %d\n",
                    new_socket, inet_ntoa(address.sin_addr), ntohs (address.sin_port));

            // Send new connection greeting message
            if (strlen (message) != (size_t) send (new_socket, message, strlen (message), 0))
                perror("Send system call failed");

            printf ("Welcome message sent successfully\n");

            // Add new socket to array of sockets
            for (i = 0; i < MAX_CLIENTS; i++)
                if (0 == client_socket[i])
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
        }

        // Else its some IO operation on some other socket
        for (i = 0; i < MAX_CLIENTS; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET (sd, &readfds))
            {
                // Check if it was for closing, and also read the incoming message
                if (0 == (valread = read (sd, buffer, 1024)))
                {
                    // Someone disconnected, get his details and print it
                    getpeername (sd, (struct sockaddr *) &address, (socklen_t *) &addr_len);
                    printf ("Host disconnected, ip %s, port %d\n", 
                            inet_ntoa (address.sin_addr), ntohs(address.sin_port));

                    // Close the socket and mark as 0 in list for reuse
                    close (sd);
                    client_socket[i] = 0;
                }
                // Echo back the message that came in
                else 
                {
                    // Set the string terminating NULL byte on the end of the data read
                    buffer [valread] = '\0';
                    send (sd, buffer, strlen (buffer), 0);
                }
            }
        }
    }

    exit(EXIT_SUCCESS);
}
