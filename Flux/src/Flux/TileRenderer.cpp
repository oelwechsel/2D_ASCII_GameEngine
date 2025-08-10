#include "fxpch.h"
#include "TileRenderer.h"
#include <GL/glew.h> 

namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        std::size_t operator()(const std::pair<int, int>& p) const noexcept {
            return std::hash<int>{}(p.first) ^ (std::hash<int>{}(p.second) << 1);
        }
    };
}

namespace Flux {

    TileRenderer::TileRenderer() {}

    TileRenderer::~TileRenderer() 
    {
        if (m_fboTexture != 0) 
        {
            glDeleteTextures(1, &m_fboTexture);
        }
        if (m_fbo != 0) 
        {
            glDeleteFramebuffers(1, &m_fbo);
        }
    }

    void TileRenderer::SetTileset(unsigned int _textureID, int _tileSize_, int _tilesetWidth_, int _tilesetHeight_) 
    {
        m_tilesetTexture = _textureID;
        m_tileSize = _tileSize_;
        m_tilesetWidth = _tilesetWidth_;
        m_tilesetHeight = _tilesetHeight_;
        m_tilesPerRow = m_tilesetWidth / m_tileSize;
    }

    void TileRenderer::InitFBO(int _width, int _height) 
    {
        if (m_fbo == 0) 
        {
            glGenFramebuffers(1, &m_fbo);
        }
        if (m_fboTexture == 0) 
        {
            glGenTextures(1, &m_fboTexture);
        }

        glBindTexture(GL_TEXTURE_2D, m_fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
        {
            FX_CORE_ASSERT("Framebuffer not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    unsigned int TileRenderer::RenderToTexture(
        const std::vector<RenderTile>& _tiles,
        int _viewWidth,
        int _viewHeight,
        int _startX,
        int _startY)
    {
        int fboWidth = _viewWidth * m_tileSize;
        int fboHeight = _viewHeight * m_tileSize;

        InitFBO(fboWidth, fboHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glViewport(0, 0, fboWidth, fboHeight);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, fboWidth, 0, fboHeight, -1, 1); 

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_tilesetTexture);

        const float layerOffsetFactorX = 0.8f;
        const float layerOffsetFactorY = 0.8f;
        const float minAlpha = 0.35f;
        const float maxAlpha = 0.9f;
        const float darkenOverlayAlpha = 0.45f;

        std::unordered_map<int, std::vector<const RenderTile*>> layerMap;
        int maxLayer = 0;

        for (const auto& tile : _tiles) 
        {
            layerMap[tile.layer].push_back(&tile);
            if (tile.layer > maxLayer) maxLayer = tile.layer;
        }

        std::unordered_map<std::pair<int, int>, int, std::hash<std::pair<int, int>>> maxLayerMap;
        for (const auto& tile : _tiles) 
        {
            std::pair<int, int> key = { tile.x, tile.y };
            auto it = maxLayerMap.find(key);
            if (it == maxLayerMap.end() || tile.layer > it->second) 
            {
                maxLayerMap[key] = tile.layer;
            }
        }

        for (int layer = 0; layer <= maxLayer; ++layer) 
        {
            auto it = layerMap.find(layer);
            if (it == layerMap.end()) continue;

            for (const RenderTile* tile : it->second) 
            {
                std::pair<int, int> key = { tile->x, tile->y };
                int localMaxLayer = maxLayerMap[key];

                float relative = (localMaxLayer == tile->layer) ? 1.0f : (float)tile->layer / (float)localMaxLayer;

                float alpha = (localMaxLayer <= 1)
                    ? 0.8f
                    : minAlpha + (maxAlpha - minAlpha) * pow(relative, 0.9f);

                int relX = tile->x - _startX;
                int relY = tile->y - _startY;

                float cameraOffsetX = static_cast<float>(_startX);
                float cameraOffsetY = static_cast<float>(_startY);

                float camMidX = _startX + _viewWidth / 2.0f;
                float camMidY = _startY + _viewHeight / 2.0f;

                float parallaxX = (tile->x - camMidX) * layerOffsetFactorX * tile->layer;
                float parallaxY = (tile->y - camMidY) * layerOffsetFactorY * tile->layer;

                float px = relX * m_tileSize + parallaxX;
                float py = relY * m_tileSize + parallaxY;



                int tileIndex = static_cast<int>(tile->tileChar) - 32;
                int tx = tileIndex % m_tilesPerRow;
                int ty = tileIndex / m_tilesPerRow;

                float u0 = tx * (float)m_tileSize / m_tilesetWidth;
                float v0 = ty * (float)m_tileSize / m_tilesetHeight;
                float u1 = u0 + (float)m_tileSize / m_tilesetWidth;
                float v1 = v0 + (float)m_tileSize / m_tilesetHeight;

                glColor4f(tile->r, tile->g, tile->b, alpha);

                glBegin(GL_QUADS);
                glTexCoord2f(u0, v0); glVertex2f(px, py);
                glTexCoord2f(u1, v0); glVertex2f(px + m_tileSize, py);
                glTexCoord2f(u1, v1); glVertex2f(px + m_tileSize, py + m_tileSize);
                glTexCoord2f(u0, v1); glVertex2f(px, py + m_tileSize);
                glEnd();

                if (tile->layer < localMaxLayer) 
                {
                    float overlayAlpha = darkenOverlayAlpha * (1.0f - relative);

                    glDisable(GL_TEXTURE_2D);
                    glColor4f(0.0f, 0.0f, 0.0f, overlayAlpha);
                    glBegin(GL_QUADS);
                    glVertex2f(px, py);
                    glVertex2f(px + m_tileSize, py);
                    glVertex2f(px + m_tileSize, py + m_tileSize);
                    glVertex2f(px, py + m_tileSize);
                    glEnd();
                    glEnable(GL_TEXTURE_2D);
                }
            }
        }

        glDisable(GL_TEXTURE_2D);
        glDisable(GL_BLEND);

        glMatrixMode(GL_PROJECTION);
        glPopMatrix();

        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
            std::cerr << "OpenGL error after rendering to FBO: " << err << std::endl;

        return m_fboTexture;
    }
} 

