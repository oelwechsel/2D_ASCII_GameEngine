#pragma once
#include "Flux/Core.h"

namespace Flux {

    struct RenderTile {
        int x; 
        int y;
        int layer; 
        char tileChar;
        float r, g, b, a;
    };

    class  FLUX_API TileRenderer {
    public:
        TileRenderer();
        ~TileRenderer();

        void SetTileset(unsigned int _textureID, int _tileSize, int _tilesetWidth, int _tilesetHeight);

        unsigned int RenderToTexture(
            const std::vector<RenderTile>& _tiles,
            int _mapWidth,
            int _mapHeight,
            int _playerX,
            int _playerY
        );


    private:
        unsigned int  m_fbo = 0;
        unsigned int  m_fboTexture = 0;

        unsigned int  m_tilesetTexture = 0;
        int m_tileSize = 0;
        int m_tilesetWidth = 0;
        int m_tilesetHeight = 0;
        int m_tilesPerRow = 0;

        void InitFBO(int _width, int _height);
    };

} 


