#pragma once
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")

#define _CRTDBG_MAP_ALLOC

#define GRAPHICS Graphics::GetInstance()
#define TIME TimeManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()
#define FIXED_DELTA_TIME TimeManager::GetInstance()->GetFixedDeltaTime()

#define DTA_LEFT DWRITE_TEXT_ALIGNMENT_LEADING
#define DTA_CENTER DWRITE_TEXT_ALIGNMENT_CENTER
#define DTA_RIGHT DWRITE_TEXT_ALIGNMENT_TRAILING
#define DTA_TOP DWRITE_PARAGRAPH_ALIGNMENT_NEAR
#define DTA_MIDDLE DWRITE_PARAGRAPH_ALIGNMENT_CENTER
#define DTA_BOTTOM DWRITE_PARAGRAPH_ALIGNMENT_FAR

#include "../framework.h"
#include "Vector2.h"

#include <cstdlib>
#include <crtdbg.h>
#include <d2d1.h>
#include <dwrite.h>

struct FRect
{
    float left;
    float top;
    float right;
    float bottom;
};
