#pragma once

#define WIN32_LEAN_AND_MEAN 
#define _SCL_SECURE_NO_WARNINGS

#define roundToInt(x) int(round(x))

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
#include <functional>

#include <cpprest\json.h>
#include <cpprest\http_client.h>

#include "sqlite3.h"

#include "duk_config.h"
#include "duktape.h"

#include "gumbo.h"

#include "..\Resources\resource.h"

using namespace std;
using namespace Gdiplus;