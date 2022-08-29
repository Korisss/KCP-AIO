#include "UDPClient.h"
#include "ikcp.h"

class KCPClient
{
public:
  KCPClient() = default;
  ~KCPClient() = default;

  static KCPClient *Dial(std::string ip, uint16_t port);
  static int kcpSend(const char *buf, int len, ikcpcb *kcp, void *user);
  static IUINT32 getTime();

  void Update();
  void Close();
  int Read(char *buf, int len);
  int Write(const char *buf, int len);

  int GetLastNetworkingError();

private:
  ikcpcb *kcp;
  UDPClient *udpClient;
  char buf[2048];
};
