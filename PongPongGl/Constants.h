#pragma once
#include <bitset>
#include <array>

class Component;

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;
constexpr std::size_t maxRenderables = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component*, maxComponents>;

using ComponentID = std::size_t;
using Group = std::size_t;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 640;
const int NET_WIDTH = 4;
const int NET_HEIGHT = 60;
const int NET_SPACE = 12;
const int RADIUS = 10;
const float L_INIT_POS = 50.0f;
const float R_INIT_OFFSET = 50.0f;
// TBD LATER
//using RenderableBitSet = std::bitset<maxRenderables>;

//using RenderableArray = std::array<Renderable*, maxRenderables>;

//using RenderableID = std::size_t;
const float DEF_BALL_SPEED = 2.4f;