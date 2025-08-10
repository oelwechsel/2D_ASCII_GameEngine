#include "ASCIIBlockDictionary.h"

ASCIIBlockDictionary& ASCIIBlockDictionary::Instance() {
    static ASCIIBlockDictionary instance; // Einzige Instanz, thread-safe in C++11+
    return instance;
}

ASCIIBlockDictionary::ASCIIBlockDictionary() {
    blocks = {
        { "wall",       { { '#','#','#','#','#','#' }, BlockColor::White } },
        { "wall2",      { { '8','8','8','8' }, BlockColor::White } },
        { "halfWall",   { { ' ', '-' }, BlockColor::White } },
        { "smallStep",  { { '-' }, BlockColor::White } },
        { "floor",      { { '.' }, BlockColor::White } },
        { "lava",       { { '~' }, BlockColor::White } },
        { "lava2",      { { '^' }, BlockColor::White } },
        { "guy",        { { '@' }, BlockColor::Green } },
        { "tree",       { { '/','%','/','%' }, BlockColor::White } },
        { "tree2",      { { '/','/','X' }, BlockColor::White } },
        { "grass",      { { ',',',' }, BlockColor::White } },
        { "tallGrass",  { { '.','"' }, BlockColor::White } },
        { "leftBrack",  { { '[','[','[','[','[','[' }, BlockColor::White } },
        { "rightBrack", { { ']','[',']',']',']',']' }, BlockColor::White } },
        { "tallLine",   { { '|','|','|','|' }, BlockColor::White } },
        { "wideLine",   { { '-','-','-','-' }, BlockColor::White } },
        { "winLine",    { { '-',' ',' ','-' }, BlockColor::White } },
        { "plus",       { { 'i','i','i','i' }, BlockColor::White } },
        { "pillar1",    { { 'o','o','o' }, BlockColor::White } },
        { "door",       { { ' ',' ','+',' ' }, BlockColor::White } },
        { "exclamation_NPC",        { { '!' }, BlockColor::Magenta } },
        { "and_NPC",        { { '&' }, BlockColor::Yellow} },
        { "percent_NPC",        { { '%' }, BlockColor::Cyan} },
        { "equals_NPC",        { { '=' }, BlockColor::Orange} },
        { "question_NPC",        { { '?' }, BlockColor::Pink} },
        { "root_path_R",        { { '.' }, BlockColor::White} },
        { "root_path_o",        { { '.' }, BlockColor::White} },
        { "root_path_O",        { { '.' }, BlockColor::White} },
        { "root_path_t",        { { '.' }, BlockColor::White} },
        { "root",        { { '.' }, BlockColor::White} },
        { "fence",      { { '-','-' }, BlockColor::White} },
        { "bigTree",      { { '.','%','.','%' }, BlockColor::White} },
        { "flower",      { { ':','*' }, BlockColor::White} },
        { "bigFlower",      { { ':','*','*','*' }, BlockColor::White} },
        { "bigPillar",      { { 'I','o','o','O' }, BlockColor::White} },
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
    case 'S': return blocks["halfWall"];
    case 's': return blocks["smallStep"];
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
    case 'R': return blocks["root_path_R"];
    case 'o': return blocks["root_path_o"];
    case 'O': return blocks["root_path_O"];
    case 't': return blocks["root_path_t"];
    case 'x': return blocks["root"];
    case 'f': return blocks["fence"];
    case 'B': return blocks["bigTree"];
    case 'F': return blocks["flower"];
    case 'P': return blocks["bigPillar"];
    case 'b': return blocks["bigFlower"];
    default:  return { { c }, BlockColor::White };
    }
}

void ASCIIBlockDictionary::setAllColorsRedExceptPlayerAndRootPathway() {
    for (auto& [key, blockData] : blocks) {
        if (key != "guy" && key!= "root_path_R" && key != "root_path_o" && key != "root_path_O" && key != "root_path_t" && key != "root") {
            blockData.color = BlockColor::Red;
        }
    }
}

void ASCIIBlockDictionary::SetBlockPattern(const std::string& name, const std::vector<char>& newPattern) {
    auto it = blocks.find(name);
    if (it != blocks.end()) {
        it->second.chars = newPattern;
    }
    else {
        FX_WARN("Block name '{}' not found in dictionary!", name);
    }
}
