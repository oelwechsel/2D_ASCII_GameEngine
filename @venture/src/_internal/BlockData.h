#pragma once
#include "fxpch.h"

enum class BlockColor {
    e_White,
    e_Green,
    e_Blue,
    e_Yellow,
    e_Magenta,
    e_Cyan,
    e_Gray,
    e_DarkGray,
    e_Black,
    e_Red,
    e_Orange,
    e_Pink
};

struct RGBA {
    float r, g, b, a;
};

struct BlockData {
    std::vector<char> chars;
    BlockColor color;
};

inline RGBA GetRGBAFromBlockColor(BlockColor _color, float _alpha = 1.0f) {
    switch (_color) {
    case BlockColor::e_White:   return { 1.0f, 1.0f, 1.0f, _alpha };
    case BlockColor::e_Green:   return { 0.0f, 1.0f, 0.0f, _alpha };
    case BlockColor::e_Red:     return { 1.0f, 0.0f, 0.0f, _alpha };
    case BlockColor::e_Blue:    return { 0.0f, 0.0f, 1.0f, _alpha };
    case BlockColor::e_Yellow:  return { 1.0f, 1.0f, 0.0f, _alpha };
    case BlockColor::e_Cyan:    return { 0.0f, 1.0f, 1.0f, _alpha };
    case BlockColor::e_Magenta: return { 1.0f, 0.0f, 1.0f, _alpha };
    case BlockColor::e_Gray:    return { 0.5f, 0.5f, 0.5f, _alpha };
    case BlockColor::e_DarkGray:return { 0.3f, 0.3f, 0.3f, _alpha };
    case BlockColor::e_Black:   return { 0.0f, 0.0f, 0.0f, _alpha };
    case BlockColor::e_Orange:  return { 1.0f, 0.5f, 0.0f, _alpha };
    case BlockColor::e_Pink:    return { 1.0f, 0.4f, 0.7f, _alpha };
    default:                 return { 1.0f, 1.0f, 1.0f, _alpha };
    }
}