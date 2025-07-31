#pragma once
#include "fxpch.h"

enum class BlockColor {
    White,
    Green,
    Blue,
    Yellow,
    Magenta,
    Cyan,
    Gray,
    DarkGray,
    Black,
    Red,
    Orange,
    Pink
};

// RGB instead of glcolor4

struct RGBA {
    float r, g, b, a;
};

struct BlockData {
    std::vector<char> chars;
    BlockColor color;
};

inline RGBA GetRGBAFromBlockColor(BlockColor color, float alpha = 1.0f) {
    switch (color) {
    case BlockColor::White:   return { 1.0f, 1.0f, 1.0f, alpha };
    case BlockColor::Green:   return { 0.0f, 1.0f, 0.0f, alpha };
    case BlockColor::Red:     return { 1.0f, 0.0f, 0.0f, alpha };
    case BlockColor::Blue:    return { 0.0f, 0.0f, 1.0f, alpha };
    case BlockColor::Yellow:  return { 1.0f, 1.0f, 0.0f, alpha };
    case BlockColor::Cyan:    return { 0.0f, 1.0f, 1.0f, alpha };
    case BlockColor::Magenta: return { 1.0f, 0.0f, 1.0f, alpha };
    case BlockColor::Gray:    return { 0.5f, 0.5f, 0.5f, alpha };
    case BlockColor::DarkGray:return { 0.3f, 0.3f, 0.3f, alpha };
    case BlockColor::Black:   return { 0.0f, 0.0f, 0.0f, alpha };
    case BlockColor::Orange:  return { 1.0f, 0.5f, 0.0f, alpha };
    case BlockColor::Pink:    return { 1.0f, 0.4f, 0.7f, alpha };
    default:                 return { 1.0f, 1.0f, 1.0f, alpha };
    }
}