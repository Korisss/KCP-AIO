#include <iostream>
#include <vector>
#include "KCPClient.h"

void main()
{
  WSADATA wsaData;
  WSAStartup(MAKEWORD(2, 2), &wsaData);

  try
  {
    KCPClient *client = KCPClient::Dial("127.0.0.1", 10000);
    std::vector<char> buf;
    buf.reserve(2048);
    buf.resize(1);

    std::cout << "CONNECT!\n";

    for (int i = 0; i < 10; i++)
    {
      client->Write(&buf[0], buf.size());
      client->Update();

      buf.clear();

      int n = 0;

      std::cout << "SENT!\n";

      do
      {
        n = client->Read(&buf[0], 128);
        client->Update();
      } while (n <= 0);

      std::cout << "GET PACKET!!!!\n";
      std::cout << std::string(buf.begin(), buf.end());
      std::cout << "\n";
      std::cout << n;
      std::cout << "\n";
    }
  }
  catch (std::exception e)
  {
    std::cout << "EXCEPTION!\n";
    std::cout << e.what();
  }
}