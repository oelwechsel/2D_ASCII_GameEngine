#pragma once
namespace Flux {

    struct RenderTile {
        int x; 
        int y;
        int layer; 
        char tileChar;
        float r, g, b, a;
    };

    class TileRenderer {
    public:
        TileRenderer();
        ~TileRenderer();

        // Must be called once before rendering
        void SetTileset(unsigned int textureID, int tileSize, int tilesetWidth, int tilesetHeight);

        // Converts a list of RenderTiles into a texture
        unsigned int RenderToTexture(
            const std::vector<RenderTile>& tiles,
            int mapWidth,
            int mapHeight,
            int playerX,
            int playerY
        );


    private:
        unsigned int  m_fbo = 0;
        unsigned int  m_fboTexture = 0;

        unsigned int  m_tilesetTexture = 0;
        int m_tileSize = 0;
        int m_tilesetWidth = 0;
        int m_tilesetHeight = 0;
        int m_tilesPerRow = 0;

        void InitFBO(int width, int height);
    };

} 


