#include <WS2tcpip.h>
#include <stdexcept>
#include "UDPClient.h"

UDPClient *UDPClient::Dial(std::string ip, uint16_t port)
{
  WSADATA wsa;

  // Init winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    // can get error code WSAGetLastError();
    return nullptr;
  }

  // create socket
  SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock == SOCKET_ERROR)
  {
    // can get error code WSAGetLastError();
    return nullptr;
  }

  // setup address
  sockaddr_in addr;

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  inet_pton(AF_INET, ip.data(), &addr.sin_addr);

  UDPClient *client = new UDPClient();
  client->addr = addr;
  client->sock = sock;
  client->addrLen = sizeof(client->addr);

  return client;
}

int UDPClient::Read(char *buf, size_t n)
{
  int len = recvfrom(this->sock, this->buf, 2048, 0, (struct sockaddr *)&this->addr, &this->addrLen);

  if (len == SOCKET_ERROR)
  {
    return len;
  }

  if (n < len)
  {
    throw std::runtime_error("len of packet is too big");
  }

  memcpy(buf, this->buf, len);

  return len;
}

int UDPClient::Write(const char *buf, size_t n)
{
  int addrLen = sizeof(this->addr);
  int len = sendto(this->sock, buf, int(n), 0, (struct sockaddr *)&this->addr, this->addrLen);

  if (len == SOCKET_ERROR)
  {
    return len;
  }

  if (n < len)
  {
    throw std::runtime_error("sent only part of a packet");
  }

  return len;
}

void UDPClient::Close()
{
  closesocket(this->sock);
  WSACleanup();
}

void UDPClient::SetNonBlocking(bool nonBlocking)
{
  u_long nonBlockingEnabled = nonBlocking ? TRUE : FALSE;
  int error = ioctlsocket(this->sock, FIONBIO, &nonBlockingEnabled);
}

int UDPClient::GetLastNetworkingError()
{
  return WSAGetLastError();
}
