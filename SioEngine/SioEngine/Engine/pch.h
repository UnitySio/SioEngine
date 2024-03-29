﻿#pragma once
#define _CRTDBG_MAP_ALLOC

#define CORE Core::GetInstance()
#define GRAPHICS Graphics::GetInstance()
#define TIME_MANAGER TimeManager::GetInstance()
#define SCENE_MANAGER SceneManager::GetInstance()
#define INPUT_MANAGER InputManager::GetInstance()
#define GAMEPAD_MANAGER GamepadManager::GetInstance()
#define AUDIO_MANAGER AudioManager::GetInstance()

#define DELTA_TIME TimeManager::GetInstance()->GetDeltaTime()
#define FIXED_DELTA_TIME TimeManager::GetInstance()->GetFixedDeltaTime()

#define DTA_LEFT DWRITE_TEXT_ALIGNMENT_LEADING
#define DTA_CENTER DWRITE_TEXT_ALIGNMENT_CENTER
#define DTA_RIGHT DWRITE_TEXT_ALIGNMENT_TRAILING
#define DTA_TOP DWRITE_PARAGRAPH_ALIGNMENT_NEAR
#define DTA_MIDDLE DWRITE_PARAGRAPH_ALIGNMENT_CENTER
#define DTA_BOTTOM DWRITE_PARAGRAPH_ALIGNMENT_FAR

#define PI std::atan(1) * 4

#include "../framework.h"
#include "../Resource.h"
#include "Vector2.h"

#include <cstdlib>
#include <crtdbg.h>
#include <memory>
#include <mutex>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <complex>

struct Color
{
    UINT r;
    UINT g;
    UINT b;
    UINT a;

    Color() :
        r(255),
        g(255),
        b(255),
        a(255)
    {
    }

    Color(UINT r, UINT g, UINT b, UINT a = 255) :
        r(r),
        g(g),
        b(b),
        a(a)
    {
    }
};

struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

struct Circle
{
    float radius_x;
    float radius_y;
};

enum class UserIndex : int
{
    kOne = 0,
    kTwo,
    kThree,
    kFour
};
