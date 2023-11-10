#include <iostream>
#include <network.h>
#include "CustomMessageType.h"

class CustomClient : public net::client_interface<CustomMessageType>
{
public:
	void PingServer()
	{
		net::message<CustomMessageType> msg;
		msg.header.id = CustomMessageType::ServerPing;

		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		msg << timeNow;
		Send(msg);
	}
};
//
//void worker()
//{
//	CustomClient c;
//	c.Connect("127.0.0.1", 60000);
//	std::this_thread::sleep_for(std::chrono::seconds(10));
//
//	while (c.IsConnect())
//	{
//		c.PingServer();
//		if (!c.Incoming().empty())
//		{
//			c.Incoming().pop_front();
//		}
//
//		std::this_thread::sleep_for(std::chrono::milliseconds(10));
//	}
//}

//
//int main()
//{
//	std::vector<std::thread> workers;
//	int maxThread = 1000;
//	workers.resize(maxThread);
//
//	for (int i = 0; i < maxThread; ++i)
//	{
//		workers[i] = std::thread(worker);
//	}
//
//	bool key[3] = { false, false, false };
//	bool old_key[3] = { false, false, false };
//	bool bQuit = false;
//	while (!bQuit)
//	{
//		key[2] = GetAsyncKeyState('3') & 0x8000;
//
//		if (key[2] && !old_key[2]) bQuit = true;
//
//		for (int i = 0; i < 3; ++i)
//			old_key[i] = key[i];
//	}
//	return 0;
//}
//
int main()
{
	CustomClient c;
	c.Connect("127.0.0.1", 60000);

	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };

	bool bQuit = false;
	while (!bQuit)
	{
		/*if (GetForegroundWindow() == GetConsoleWindow())
		{*/
			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
		//}
		
		if (key[0] && !old_key[0]) c.PingServer();
		if (key[2] && !old_key[2]) bQuit = true;

		for (int i = 0; i < 3; ++i)
			old_key[i] = key[i];

		if (c.IsConnect())
		{
			if (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMessageType::ServerPing:
				{
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << std::endl;
				}
				}
			}
		}
		else
		{

		}
	}

	return 0;
}