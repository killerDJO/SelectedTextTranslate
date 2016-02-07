#pragma once

#define WIN32_LEAN_AND_MEAN 
#define _SCL_SECURE_NO_WARNINGS

#define roundToInt(x) int(round(x))

#define MOUSEEVENTF_FROMTOUCH 0xFF515700

#define WM_TRANLSATE_LOG_RECORD	300542

#include <SDKDDKVer.h>

#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>

#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
#include <string>
#include <map>
#include <ostream>
#include <algorithm>
#include <stdio.h>
#include <cstdio>
#include <commctrl.h>

#include "curl\curl.h"
#include "json\json.h"

#include "..\Resources\resource.h"

using namespace std;