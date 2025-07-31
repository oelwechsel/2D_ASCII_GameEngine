#include "ASCIIBlockDictionary.h"

ASCIIBlockDictionary::ASCIIBlockDictionary() {
    blocks = {
        { "wall",       { { '#','#','#','#','#','#' }, BlockColor::White } },
        { "wall2",      { { '8','8','8','8' }, BlockColor::White } },
        { "halfWall",   { { ' ', '#' }, BlockColor::White } },
        { "smallStep",  { { '#' }, BlockColor::White } },
        { "floor",      { { '.' }, BlockColor::White } },
        { "lava",       { { '~' }, BlockColor::White } },
        { "lava2",      { { '^' }, BlockColor::White } },
        { "guy",        { { '@' }, BlockColor::Green } },
        { "tree",       { { '/','%','/','%' }, BlockColor::White } },
        { "tree2",      { { '|','%' }, BlockColor::White } },
        { "grass",      { { ':',':' }, BlockColor::White } },
        { "tallGrass",  { { ':',':',':' }, BlockColor::White } },
        { "colon",      { { ':' }, BlockColor::White } },
        { "leftBrack",  { { '[','[','[','[','[','[' }, BlockColor::White } },
        { "rightBrack", { { ']','[',']',']',']',']' }, BlockColor::White } },
        { "tallLine",   { { '|','|','|','|' }, BlockColor::White } },
        { "wideLine",   { { '-','-','-','-' }, BlockColor::White } },
        { "winLine",    { { '-',' ',' ','-' }, BlockColor::White } },
        { "plus",       { { '+','+','+','+' }, BlockColor::White } },
        { "pillar1",    { { 'O','O','O','O' }, BlockColor::White } },
        { "door",       { { ' ',' ','+',' ' }, BlockColor::White } },
        { "exclamation_NPC",        { { '!' }, BlockColor::Magenta } },
        { "and_NPC",        { { '&' }, BlockColor::Yellow} },
        { "percent_NPC",        { { '%' }, BlockColor::Cyan} },
        { "equals_NPC",        { { '=' }, BlockColor::Orange} },
        { "question_NPC",        { { '?' }, BlockColor::Pink} },
    };
}

BlockData ASCIIBlockDictionary::getBlockData(char c) {
    if (c == '~') {
        lavaVar = (lavaVar + 1) % 3;
        return lavaVar == 0 ? blocks["lava2"] : blocks["lava"];
    }

    switch (c) {
    case '#': return blocks["wall"];
    case '8': return blocks["wall2"];
    case 'O': return blocks["halfWall"];
    case 'o': return blocks["smallStep"];
    case '.': return blocks["floor"];
    case '@': return blocks["guy"];
    case '^': return blocks["lava2"];
    case 'T': return blocks["tree"];
    case '(': return blocks["tree2"];
    case 'w': return blocks["grass"];
    case 'W': return blocks["tallGrass"];
    case ':': return blocks["colon"];
    case '[': return blocks["leftBrack"];
    case ']': return blocks["rightBrack"];
    case '|': return blocks["tallLine"];
    case '-': return blocks["wideLine"];
    case '=': return blocks["winLine"];
    case '+': return blocks["plus"];
    case '0': return blocks["pillar1"];
    case '$': return blocks["door"];
    case '!': return blocks["exclamation_NPC"];
    case '&': return blocks["and_NPC"];
    case '%': return blocks["percent_NPC"];
    case 'a': return blocks["equals_NPC"];
    case '?': return blocks["question_NPC"];
    default:  return { { c }, BlockColor::White };
    }
}

