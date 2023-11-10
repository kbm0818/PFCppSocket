#pragma once
#include "common.h"
#include "message.h"

namespace net
{
	template <typename T>
	class client_interface
	{
	public:
		client_interface(){}
		virtual ~client_interface() { Disconnect(); }

	public:
		bool Connect(const std::string& host, const uint16_t port)
		{
			try
			{
				asio::ip::tcp::resolver resolver(m_context);
				asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

				m_connection = std::make_unique<connection<T>>(connection<T>::owner::client, m_context, asio::ip::tcp::socket(m_context), m_qMessagesIn);

				m_connection->ConnectToServer(endpoints);

				// Start Context Thread
				thrContext = std::thread([this]() { m_context.run(); });
			}
			catch (std::exception& e)
			{
				std::cerr << "Client Exception: " << e.what() << "\n";
				return false;
			}
			return true;
		}

		void Disconnect() 
		{
			if (IsConnect())
			{
				m_connection->Disconnect();
			}

			m_context.stop();

			if (thrContext.joinable())
				thrContext.join();

			m_connection.release();
		}

		bool IsConnect() 
		{ 
			if (m_connection)
				return m_connection->IsConnected();
			else
				return false;
		}

		void Send(const message<T>& msg)
		{
			if (IsConnect())
				m_connection->Send(msg);
		}

		message_queue<owned_message<T>>& Incoming()
		{
			return m_qMessagesIn;
		}

	protected:
		asio::io_context m_context;
		std::thread thrContext;
		std::unique_ptr<connection<T>> m_connection;

	private:
		message_queue<owned_message<T>> m_qMessagesIn;

	};
}