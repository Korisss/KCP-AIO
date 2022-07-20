#include "KCPClient.h"
#include <stdexcept>

KCPClient *KCPClient::Dial(const char *ip, uint16_t port)
{
  struct sockaddr_in saddr;

  memset(&saddr, 0, sizeof(saddr));

  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(port);

  int ret = inet_pton(AF_INET, ip, &(saddr.sin_addr));

  if (ret == 0) // IPv6
  {
    return KCPClient::dialIPv6(ip, port);
  }
  else if (ret == -1)
  {
    int error = WSAGetLastError();
    switch (error)
    {
    case WSAEFAULT:
      throw std::runtime_error("[KCPClient::Dial] Bad address");
      break;

    case WSAEAFNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
      break;

    default:
      throw std::runtime_error("[KCPClient::Dial] unknown error");
      break;
    }

    return nullptr;
  }

  SOCKET sockfd = socket(PF_INET, SOCK_DGRAM, 0);

  if (sockfd == INVALID_SOCKET)
  {
    int error = WSAGetLastError();
    switch (error)
    {
    case WSANOTINITIALISED:
      throw std::runtime_error("[KCPClient::Dial] Successful WSAStartup not yet performed");
      break;

    case WSAENETDOWN:
      throw std::runtime_error("[KCPClient::Dial] Network is down");
      break;

    case WSAEAFNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
      break;

    case WSAEINPROGRESS:
      throw std::runtime_error("[KCPClient::Dial] Operation now in progress");
      break;

    case WSAEMFILE:
      throw std::runtime_error("[KCPClient::Dial] Too many open sockets");
      break;

    case WSAEINVAL:
      throw std::runtime_error("[KCPClient::Dial] Invalid argument");
      break;

    case WSAEINVALIDPROVIDER:
      throw std::runtime_error("[KCPClient::Dial] Service provider is invalid");
      break;

    case WSAEINVALIDPROCTABLE:
      throw std::runtime_error("[KCPClient::Dial] Procedure call table is invalid");
      break;

    case WSAENOBUFS:
      throw std::runtime_error("[KCPClient::Dial] No buffer space available");
      break;

    case WSAEPROTONOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Protocol not supported");
      break;

    case WSAEPROTOTYPE:
      throw std::runtime_error("[KCPClient::Dial] Protocol wrong type for socket");
      break;

    case WSAEPROVIDERFAILEDINIT:
      throw std::runtime_error("[KCPClient::Dial] Service provider failed to initialize");
      break;

    case WSAESOCKTNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Socket type not supported");
      break;

    default:
      throw std::runtime_error("[KCPClient::Dial] Unknown error");
      break;
    }

    return nullptr;
  }

  int error = connect(sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr));

  if (error == 0)
  {
    return KCPClient::create(sockfd);
  }

  closesocket(sockfd);
  switch (error)
  {
  case WSANOTINITIALISED:
    throw std::runtime_error("[KCPClient::Dial] Successful WSAStartup not yet performed");
    break;

  case WSAENETDOWN:
    throw std::runtime_error("[KCPClient::Dial] Network is down");
    break;

  case WSAEADDRINUSE:
    throw std::runtime_error("[KCPClient::Dial] Address already in use");
    break;

  case WSAEINTR:
    throw std::runtime_error("[KCPClient::Dial] Interrupted function call");
    break;

  case WSAEINPROGRESS:
    throw std::runtime_error("[KCPClient::Dial] Operation now in progress");
    break;

  case WSAEALREADY:
    throw std::runtime_error("[KCPClient::Dial] Operation already in progress");
    break;

  case WSAEADDRNOTAVAIL:
    throw std::runtime_error("[KCPClient::Dial] Cannot assign requested address");
    break;

  case WSAEAFNOSUPPORT:
    throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
    break;

  case WSAECONNREFUSED:
    throw std::runtime_error("[KCPClient::Dial] Connection refused");
    break;

  case WSAEFAULT:
    throw std::runtime_error("[KCPClient::Dial] Bad address");
    break;

  case WSAEINVAL:
    throw std::runtime_error("[KCPClient::Dial] Invalid argument");
    break;

  case WSAEISCONN:
    throw std::runtime_error("[KCPClient::Dial] Socket is already connected");
    break;

  case WSAENETUNREACH:
    throw std::runtime_error("[KCPClient::Dial] Network is unreachable");
    break;

  case WSAEHOSTUNREACH:
    throw std::runtime_error("[KCPClient::Dial] No route to host");
    break;

  case WSAENOBUFS:
    throw std::runtime_error("[KCPClient::Dial] No buffer space available");
    break;

  case WSAENOTSOCK:
    throw std::runtime_error("[KCPClient::Dial] Socket operation on nonsocket");
    break;

  case WSAETIMEDOUT:
    throw std::runtime_error("[KCPClient::Dial] Connection timed out");
    break;

  case WSAEWOULDBLOCK:
    throw std::runtime_error("[KCPClient::Dial] Resource temporarily unavailable");
    break;

  case WSAEACCES:
    throw std::runtime_error("[KCPClient::Dial] Permission denied");
    break;

  default:
    throw std::runtime_error("[KCPClient::Dial] Unknown error");
    break;
  }

  return nullptr;
}

KCPClient *KCPClient::dialIPv6(const char *ip, uint16_t port)
{
  struct sockaddr_in6 saddr;

  memset(&saddr, 0, sizeof(saddr));

  saddr.sin6_family = AF_INET6;
  saddr.sin6_port = htons(port);

  int ret = inet_pton(AF_INET6, ip, &(saddr.sin6_addr));
  if (ret != 1)
  {
    int error = WSAGetLastError();
    switch (error)
    {
    case WSAEFAULT:
      throw std::runtime_error("[KCPClient::Dial] Bad address");
      break;

    case WSAEAFNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
      break;

    default:
      throw std::runtime_error("[KCPClient::Dial] unknown error");
      break;
    }

    return nullptr;
  }

  SOCKET sockfd = socket(PF_INET6, SOCK_DGRAM, 0);

  if (sockfd == INVALID_SOCKET)
  {
    int error = WSAGetLastError();
    switch (error)
    {
    case WSANOTINITIALISED:
      throw std::runtime_error("[KCPClient::Dial] Successful WSAStartup not yet performed");
      break;

    case WSAENETDOWN:
      throw std::runtime_error("[KCPClient::Dial] Network is down");
      break;

    case WSAEAFNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
      break;

    case WSAEINPROGRESS:
      throw std::runtime_error("[KCPClient::Dial] Operation now in progress");
      break;

    case WSAEMFILE:
      throw std::runtime_error("[KCPClient::Dial] Too many open sockets");
      break;

    case WSAEINVAL:
      throw std::runtime_error("[KCPClient::Dial] Invalid argument");
      break;

    case WSAEINVALIDPROVIDER:
      throw std::runtime_error("[KCPClient::Dial] Service provider is invalid");
      break;

    case WSAEINVALIDPROCTABLE:
      throw std::runtime_error("[KCPClient::Dial] Procedure call table is invalid");
      break;

    case WSAENOBUFS:
      throw std::runtime_error("[KCPClient::Dial] No buffer space available");
      break;

    case WSAEPROTONOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Protocol not supported");
      break;

    case WSAEPROTOTYPE:
      throw std::runtime_error("[KCPClient::Dial] Protocol wrong type for socket");
      break;

    case WSAEPROVIDERFAILEDINIT:
      throw std::runtime_error("[KCPClient::Dial] Service provider failed to initialize");
      break;

    case WSAESOCKTNOSUPPORT:
      throw std::runtime_error("[KCPClient::Dial] Socket type not supported");
      break;

    default:
      throw std::runtime_error("[KCPClient::Dial] Unknown error");
      break;
    }

    return nullptr;
  }

  int error = connect(sockfd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in6));

  if (error == 0)
  {
    return KCPClient::create(sockfd);
  }

  closesocket(sockfd);
  switch (error)
  {
  case WSANOTINITIALISED:
    throw std::runtime_error("[KCPClient::Dial] Successful WSAStartup not yet performed");
    break;

  case WSAENETDOWN:
    throw std::runtime_error("[KCPClient::Dial] Network is down");
    break;

  case WSAEADDRINUSE:
    throw std::runtime_error("[KCPClient::Dial] Address already in use");
    break;

  case WSAEINTR:
    throw std::runtime_error("[KCPClient::Dial] Interrupted function call");
    break;

  case WSAEINPROGRESS:
    throw std::runtime_error("[KCPClient::Dial] Operation now in progress");
    break;

  case WSAEALREADY:
    throw std::runtime_error("[KCPClient::Dial] Operation already in progress");
    break;

  case WSAEADDRNOTAVAIL:
    throw std::runtime_error("[KCPClient::Dial] Cannot assign requested address");
    break;

  case WSAEAFNOSUPPORT:
    throw std::runtime_error("[KCPClient::Dial] Address family not supported by protocol family");
    break;

  case WSAECONNREFUSED:
    throw std::runtime_error("[KCPClient::Dial] Connection refused");
    break;

  case WSAEFAULT:
    throw std::runtime_error("[KCPClient::Dial] Bad address");
    break;

  case WSAEINVAL:
    throw std::runtime_error("[KCPClient::Dial] Invalid argument");
    break;

  case WSAEISCONN:
    throw std::runtime_error("[KCPClient::Dial] Socket is already connected");
    break;

  case WSAENETUNREACH:
    throw std::runtime_error("[KCPClient::Dial] Network is unreachable");
    break;

  case WSAEHOSTUNREACH:
    throw std::runtime_error("[KCPClient::Dial] No route to host");
    break;

  case WSAENOBUFS:
    throw std::runtime_error("[KCPClient::Dial] No buffer space available");
    break;

  case WSAENOTSOCK:
    throw std::runtime_error("[KCPClient::Dial] Socket operation on nonsocket");
    break;

  case WSAETIMEDOUT:
    throw std::runtime_error("[KCPClient::Dial] Connection timed out");
    break;

  case WSAEWOULDBLOCK:
    throw std::runtime_error("[KCPClient::Dial] Resource temporarily unavailable");
    break;

  case WSAEACCES:
    throw std::runtime_error("[KCPClient::Dial] Permission denied");
    break;

  default:
    throw std::runtime_error("[KCPClient::Dial] Unknown error");
    break;
  }

  return nullptr;
}

KCPClient *KCPClient::create(SOCKET sockfd)
{
  u_long nonblocking_enabled = TRUE;
  int error = ioctlsocket(sockfd, FIONBIO, &nonblocking_enabled);

  if (error != 0)
  {
    switch (error)
    {
    case WSANOTINITIALISED:
      throw std::runtime_error("[KCPClient::create] Successful WSAStartup not yet performed");
      break;

    case WSAENETDOWN:
      throw std::runtime_error("[KCPClient::create] Network is down");
      break;

    case WSAEINPROGRESS:
      throw std::runtime_error("[KCPClient::create] Operation now in progress");
      break;

    case WSAENOTSOCK:
      throw std::runtime_error("[KCPClient::create] Socket operation on nonsocket");
      break;

    case WSAEFAULT:
      throw std::runtime_error("[KCPClient::create] Bad address");
      break;

    default:
      throw std::runtime_error("[KCPClient::create] Unknown error");
      break;
    }

    return nullptr;
  }

  KCPClient *client = new (KCPClient);
  client->_sockfd = sockfd;
  client->kcp = ikcp_create(IUINT32(rand()), client);
  client->kcp->output = KCPClient::out;

  // хз зачем эти строки, но пусть будут, чтобы всё точно работало
  ikcp_setmtu(client->kcp, 1400);
  ikcp_wndsize(client->kcp, 128, 128);
  ikcp_nodelay(client->kcp, 1, 20, 2, 1);

  return client;
}

int KCPClient::out(const char *buf, int len, struct IKCPCB *kcp, void *user)
{
  KCPClient *client = static_cast<KCPClient *>(user);
  return client->output(buf, len);
}

int KCPClient::output(const char *buffer, size_t length)
{
  return send(this->_sockfd, buffer, int(length), 0);
}

int KCPClient::Read(char *buffer, size_t length)
{
  return ikcp_recv(this->kcp, buffer, int(length));
}

void KCPClient::Update()
{
  IUINT32 current = time();

  for (;;)
  {
    int n = recv(this->_sockfd, this->buf, sizeof(this->buf), 0);
    if (n <= 0)
    {
      break;
    }
    ikcp_input(this->kcp, this->buf, n);
  }

  this->kcp->current = current;
  ikcp_flush(this->kcp);
}

int KCPClient::Write(const char *buffer, size_t length)
{
  return ikcp_send(this->kcp, buffer, int(length));
}

void KCPClient::Close()
{
  int error = closesocket(this->_sockfd);
  if (error == 0)
  {
    return;
  }

  switch (error)
  {
  case WSANOTINITIALISED:
    throw std::runtime_error("[KCPClient::Close] Successful WSAStartup not yet performed");
    break;

  case WSAENETDOWN:
    throw std::runtime_error("[KCPClient::Close] Network is down");
    break;

  case WSAENOTSOCK:
    throw std::runtime_error("[KCPClient::Close] Socket operation on nonsocket");
    break;

  case WSAEINPROGRESS:
    throw std::runtime_error("[KCPClient::Close] Operation now in progress");
    break;

  case WSAEINTR:
    throw std::runtime_error("[KCPClient::Close] Interrupted function call");
    break;

  case WSAEWOULDBLOCK:
    throw std::runtime_error("[KCPClient::Close] Resource temporarily unavailable");
    break;

  default:
    throw std::runtime_error("[KCPClient::Close] Unknown error");
    break;
  }
}

IUINT32 time()
{
  LARGE_INTEGER fq, t;
  QueryPerformanceFrequency(&fq);
  QueryPerformanceCounter(&t);
  return (IUINT32)(((1000 * t.QuadPart) / fq.QuadPart) & 0xfffffffful);
}
