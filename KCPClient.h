#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdint.h>
#include "kcp/ikcp.h"

class KCPClient
{
private:
  KCPClient() = default;
  ~KCPClient() = default;
  int output(const char *buffer, size_t length);

  static KCPClient *dialIPv6(const char *ip, uint16_t port);
  static KCPClient *create(SOCKET sockfd);
  static int out(const char *buf, int len, struct IKCPCB *kcp, void *user);

  SOCKET _sockfd{0};
  ikcpcb *kcp{nullptr};
  char buf[2048];

public:
  static KCPClient *Dial(const char *ip, uint16_t port); // need try catch
  void Close();                                          // need try catch
  int Read(char *buffer, size_t length);
  int Write(const char *buffer, size_t length);
  void Update();
};

IUINT32 time();
