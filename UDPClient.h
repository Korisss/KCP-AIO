#include <stdint.h>
#include <WinSock2.h>
#include <string>

class UDPClient
{
public:
  UDPClient() = default;
  ~UDPClient() = default;

  static UDPClient *Dial(std::string ip, uint16_t port);

  int Read(char *buf, size_t n);
  int Write(const char *buf, size_t n);
  void Close();
  void SetNonBlocking(bool nonBlocking);

  int GetLastNetworkingError();

private:
  sockaddr_in addr;
  SOCKET sock;
  char buf[2048];
  int addrLen;
};
