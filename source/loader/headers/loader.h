#pragma once
#include <sstream>
#include <functional>
#include <thread>
#include <vector>
#include <mutex>
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <filesystem>
#include <fstream>

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#define LOADER_VERSION "1.0.6"

extern bool Killed;

#include "colors.h"
#include "logger.h"
#include "inject.h"
#include "strings.h"
#include "util.h"
#include "http.h"
#include "api.h"