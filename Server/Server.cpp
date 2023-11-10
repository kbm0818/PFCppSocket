#include "network.h"
#include "CustomMessageType.h"

class CustomServer : public net::server_interface<CustomMessageType>
{
public:
	CustomServer(uint16_t nPort)
		: net::server_interface<CustomMessageType>(nPort)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<net::connection<CustomMessageType>> client)
	{
		net::message<CustomMessageType> msg;
		msg.header.id = CustomMessageType::ServerAccept;
		client->Send(msg);
		return true;
	}

	virtual void OnClientDisconnect(std::shared_ptr<net::connection<CustomMessageType>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	virtual void OnMessage(std::shared_ptr<net::connection<CustomMessageType>> client, net::message<CustomMessageType>& msg)
	{
		switch (msg.header.id)
		{
		case CustomMessageType::ServerPing:
		{
			//std::cout << "[" << client->GetID() << "]: Server Ping" << std::endl;
			client->Send(msg);
		}
		break;
		case CustomMessageType::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All" << std::endl;

			net::message<CustomMessageType> msg;
			msg.header.id = CustomMessageType::ServerMessage;
			msg << client->GetID();
			MessageAllClients(msg, client);
		}
		break;

		}
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}

	return 0;
}