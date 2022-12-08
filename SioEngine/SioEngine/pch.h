#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#define _CRTDBG_MAP_ALLOC

#include "framework.h"
#include "Resource.h"

#include <cstdlib>
#include <crtdbg.h>
#include <d2d1.h>
#include <dwrite.h>

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()
#define FIXED_DELTA_TIME TimeManager::GetInstance()->GetFixedDeltaTime();
