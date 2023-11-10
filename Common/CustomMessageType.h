#pragma once
#include <iostream>

enum class CustomMessageType : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
};

