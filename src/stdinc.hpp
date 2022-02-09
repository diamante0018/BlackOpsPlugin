#pragma once

#define BINARY_PAYLOAD_SIZE 0x0A000000
#define TLS_PAYLOAD_SIZE 0x2000

#define DLL_EXPORT extern "C" __declspec(dllexport)

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include <vector>
#include <cassert>
#include <mutex>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <map>
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#pragma comment(lib, "ntdll.lib")

using namespace std::literals;

#include "game/structs.hpp"
#include "game/game.hpp"
