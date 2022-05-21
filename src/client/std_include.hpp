#pragma once

#define DLL_EXPORT extern "C" __declspec(dllexport)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <unordered_set>
#include <vector>

#pragma warning(disable : 26812)

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>

#pragma comment(lib, "ntdll.lib")

using namespace std::literals;

// clang-format off
#include "game/structs.hpp"
#include "game/game.hpp"
// clang-format on
