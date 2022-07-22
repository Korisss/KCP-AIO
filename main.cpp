#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include "KCPClient.h"

int main()
{
  KCPClient *client = KCPClient::Dial("127.0.0.1", 10000);

  char buf[2048];

  std::string msg = "msg from client";

  while (true)
  {
    client->Write(msg.data(), int(msg.length()));
    std::cout << "Message sent: " + msg + "\n";

    client->Update();

    int n = 0;
    do
    {
      n = client->Read(buf, 2048);
      if (n > 0)
      {
        std::cout << "Received message: " + std::string(buf, n) + "\n";
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(16));

      client->Update();
    } while (n == 0);
  }

  return 0;
}
