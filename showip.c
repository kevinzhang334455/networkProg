/*
** showip.c -- show IP addresses for a host given on the command line
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char const *argv[]) 
{
  struct addrinfo hints, *res = NULL, *p = NULL;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  if (argc != 2) 
  {
    fprintf(stderr, "usage: showip hostname\n");
    return 0;
  }

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC; // AF_INET for ipv4 and 
  // AF_INET6 for ipv6

  hints.ai_socktype = SOCK_STREAM; // TCP

  if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) 
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
  }

  printf("IP addresses for %s:\n", argv[1]);

  for (p = res; p != NULL; p = p->ai_next)
  {
    void *addr = NULL;
    char *ipver = NULL;

    // get the pointer to the address itself,
    // different fields in IPv4 and IPv6:

    if (p->ai_family == AF_INET) 
    {
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "ipv4";
    } 
    else 
    {
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "ipv6";
    }

    // convert the IP to a string and print it:
    inet_ntop(p->ai_family, addr, ipstr, sizeof(ipstr));
    printf("  %s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(res); // free the linked list

  return 0;
}
