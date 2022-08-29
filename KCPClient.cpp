#include "KCPClient.h"

KCPClient *KCPClient::Dial(std::string ip, uint16_t port)
{
  KCPClient *client = new KCPClient();

  client->udpClient = UDPClient::Dial(ip, port);
  client->udpClient->SetNonBlocking(true);
  client->kcp = ikcp_create(IUINT32(rand()), client);
  client->kcp->output = kcpSend;

  return client;
}

int KCPClient::kcpSend(const char *buf, int len, ikcpcb *kcp, void *user)
{
  KCPClient *client = static_cast<KCPClient *>(user);
  return client->udpClient->Write(buf, len);
}

IUINT32 KCPClient::getTime()
{
  LARGE_INTEGER fq, t;
  QueryPerformanceFrequency(&fq);
  QueryPerformanceCounter(&t);
  return (IUINT32)(((1000 * t.QuadPart) / fq.QuadPart) & 0xfffffffful);
}

void KCPClient::Update()
{
  int current = getTime();
  while (true)
  {
    int n = this->udpClient->Read(this->buf, 2048);
    if (n <= 0)
    {
      break;
    }

    ikcp_input(this->kcp, this->buf, n);
  }

  ikcp_update(this->kcp, current);
}

void KCPClient::Close()
{
  this->udpClient->Close();
  ikcp_release(this->kcp);
}

int KCPClient::Read(char *buf, int len)
{
  int packetSize = ikcp_peeksize(this->kcp);

  if (packetSize > len)
  {
    return 0;
  }

  return ikcp_recv(this->kcp, buf, len);
}

int KCPClient::Write(const char *buf, int len)
{
  return ikcp_send(this->kcp, buf, len);
}

int KCPClient::GetLastNetworkingError()
{
  return this->udpClient->GetLastNetworkingError();
}
