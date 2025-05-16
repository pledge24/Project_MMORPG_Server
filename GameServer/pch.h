#pragma once

#define WIN32_LEAN_AND_MEAN             

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobuf.lib")

#endif

#include "CorePch.h"

using GameSessionRef = shared_ptr<class GameSession>;
using RoomRef = shared_ptr<class Room>;
using PlayerRef = shared_ptr<class Player>;