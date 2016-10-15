#pragma once

#define WIN32_LEAN_AND_MEAN 
#define _SCL_SECURE_NO_WARNINGS

#define roundToInt(x) int(round(x))

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

#define AssertWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, false)
#define AssertCriticalWinApiResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, true)
#define AssertWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, false, INVALID_HANDLE_VALUE)
#define AssertCriticalWinApiHandleResult(__result) ExceptionHelper::ThrowOnWinapiError(__result, __WFILE__, __LINE__, true, INVALID_HANDLE_VALUE)

#define GDIPVER     0x0110

#include <SDKDDKVer.h>

#include <windows.h>
#include <mmsystem.h>
#include <shellapi.h>
#include <objidl.h>
#include <gdiplus.h>

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
#include <regex>
#include <sstream>

#include <cpprest\json.h>
#include <cpprest\http_client.h>

#include "sqlite3.h"

#include "duk_config.h"
#include "duktape.h"

#include "gumbo.h"

#include "..\Resources\resource.h"
#include "Helpers\ExceptionHelper.h"

using namespace std;
using namespace Gdiplus;