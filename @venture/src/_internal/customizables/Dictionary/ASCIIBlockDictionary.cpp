#include "ASCIIBlockDictionary.h"

ASCIIBlockDictionary& ASCIIBlockDictionary::Instance() {
    static ASCIIBlockDictionary instance; 
    return instance;
}

ASCIIBlockDictionary::ASCIIBlockDictionary() {
    m_Blocks = {
        { "wall",       { { '#','#','#','#','#','#' }, BlockColor::e_White } },
        { "wall2",      { { '8','8','8','8' }, BlockColor::e_White } },
        { "halfWall",   { { ' ', '-' }, BlockColor::e_White } },
        { "smallStep",  { { '-' }, BlockColor::e_White } },
        { "floor",      { { '.' }, BlockColor::e_White } },
        { "lava",       { { '~' }, BlockColor::e_White } },
        { "lava2",      { { '^' }, BlockColor::e_White } },
        { "guy",        { { '@' }, BlockColor::e_Green } },
        { "tree",       { { '/','%','/','%' }, BlockColor::e_White } },
        { "tree2",      { { '/','/','X' }, BlockColor::e_White } },
        { "grass",      { { ',',',' }, BlockColor::e_White } },
        { "tallGrass",  { { '.','"' }, BlockColor::e_White } },
        { "leftBrack",  { { '[','[','[','[','[','[' }, BlockColor::e_White } },
        { "rightBrack", { { ']','[',']',']',']',']' }, BlockColor::e_White } },
        { "tallLine",   { { '|','|','|','|' }, BlockColor::e_White } },
        { "wideLine",   { { '-','-','-','-' }, BlockColor::e_White } },
        { "winLine",    { { '-',' ',' ','-' }, BlockColor::e_White } },
        { "plus",       { { 'i','i','i','i' }, BlockColor::e_White } },
        { "pillar1",    { { 'o','o','o' }, BlockColor::e_White } },
        { "door",       { { ' ',' ','+',' ' }, BlockColor::e_White } },
        { "exclamation_NPC",        { { '!' }, BlockColor::e_Magenta } },
        { "and_NPC",        { { '&' }, BlockColor::e_Yellow} },
        { "percent_NPC",        { { '%' }, BlockColor::e_Cyan} },
        { "equals_NPC",        { { '=' }, BlockColor::e_Orange} },
        { "question_NPC",        { { '?' }, BlockColor::e_Pink} },
        { "root_path_R",        { { '.' }, BlockColor::e_White} },
        { "root_path_o",        { { '.' }, BlockColor::e_White} },
        { "root_path_O",        { { '.' }, BlockColor::e_White} },
        { "root_path_t",        { { '.' }, BlockColor::e_White} },
        { "root",        { { '.' }, BlockColor::e_White} },
        { "fence",      { { '-','-' }, BlockColor::e_White} },
        { "bigTree",      { { '.','%','.','%' }, BlockColor::e_White} },
        { "flower",      { { ':','*' }, BlockColor::e_White} },
        { "bigFlower",      { { ':','*','*','*' }, BlockColor::e_White} },
        { "bigPillar",      { { 'I','o','o','O' }, BlockColor::e_White} },
    };
}

BlockData ASCIIBlockDictionary::getBlockData(char _c) {
    if (_c == '~') {
        m_LavaVar = (m_LavaVar + 1) % 3;
        return m_LavaVar == 0 ? m_Blocks["lava2"] : m_Blocks["lava"];
    }

    switch (_c) {
    case '#': return m_Blocks["wall"];
    case '8': return m_Blocks["wall2"];
    case 'S': return m_Blocks["halfWall"];
    case 's': return m_Blocks["smallStep"];
    case '.': return m_Blocks["floor"];
    case '@': return m_Blocks["guy"];
    case '^': return m_Blocks["lava2"];
    case 'T': return m_Blocks["tree"];
    case '(': return m_Blocks["tree2"];
    case 'w': return m_Blocks["grass"];
    case 'W': return m_Blocks["tallGrass"];
    case ':': return m_Blocks["colon"];
    case '[': return m_Blocks["leftBrack"];
    case ']': return m_Blocks["rightBrack"];
    case '|': return m_Blocks["tallLine"];
    case '-': return m_Blocks["wideLine"];
    case '=': return m_Blocks["winLine"];
    case '+': return m_Blocks["plus"];
    case '0': return m_Blocks["pillar1"];
    case '$': return m_Blocks["door"];
    case '!': return m_Blocks["exclamation_NPC"];
    case '&': return m_Blocks["and_NPC"];
    case '%': return m_Blocks["percent_NPC"];
    case 'a': return m_Blocks["equals_NPC"];
    case '?': return m_Blocks["question_NPC"];
    case 'R': return m_Blocks["root_path_R"];
    case 'o': return m_Blocks["root_path_o"];
    case 'O': return m_Blocks["root_path_O"];
    case 't': return m_Blocks["root_path_t"];
    case 'x': return m_Blocks["root"];
    case 'f': return m_Blocks["fence"];
    case 'B': return m_Blocks["bigTree"];
    case 'F': return m_Blocks["flower"];
    case 'P': return m_Blocks["bigPillar"];
    case 'b': return m_Blocks["bigFlower"];
    default:  return { { _c }, BlockColor::e_White };
    }
}

void ASCIIBlockDictionary::setAllColorsRedExceptPlayerAndRootPathway() {
    for (auto& [key, blockData] : m_Blocks) {
        if (key != "guy" && key!= "root_path_R" && key != "root_path_o" && key != "root_path_O" && key != "root_path_t" && key != "root") {
            blockData.color = BlockColor::e_Red;
        }
    }
}

void ASCIIBlockDictionary::SetBlockPattern(const std::string& _name, const std::vector<char>& _newPattern) {
    auto it = m_Blocks.find(_name);
    if (it != m_Blocks.end()) {
        it->second.chars = _newPattern;
    }
    else {
        FX_WARN("Block name '{}' not found in dictionary!", _name);
    }
}
