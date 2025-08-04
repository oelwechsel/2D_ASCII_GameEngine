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

    TileRenderer::~TileRenderer() {
        if (m_fboTexture != 0) {
            glDeleteTextures(1, &m_fboTexture);
        }
        if (m_fbo != 0) {
            glDeleteFramebuffers(1, &m_fbo);
        }
    }

    void TileRenderer::SetTileset(unsigned int textureID, int tileSize_, int tilesetWidth_, int tilesetHeight_) {
        m_tilesetTexture = textureID;
        m_tileSize = tileSize_;
        m_tilesetWidth = tilesetWidth_;
        m_tilesetHeight = tilesetHeight_;
        m_tilesPerRow = m_tilesetWidth / m_tileSize;
    }

    void TileRenderer::InitFBO(int width, int height) {
        if (m_fbo == 0) {
            glGenFramebuffers(1, &m_fbo);
        }
        if (m_fboTexture == 0) {
            glGenTextures(1, &m_fboTexture);
        }

        glBindTexture(GL_TEXTURE_2D, m_fboTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboTexture, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            FX_CORE_ASSERT("Framebuffer not complete!");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }



    // TODO: add ENTITY dependencies! -> Entities immer Opacity 1.0f!
    unsigned int TileRenderer::RenderToTexture(const std::vector<RenderTile>& tiles,
        int mapWidth,
        int mapHeight,
        int playerX,
        int playerY)
    {
        int fboWidth = mapWidth * m_tileSize;
        int fboHeight = mapHeight * m_tileSize;

        InitFBO(fboWidth, fboHeight);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        glViewport(0, 0, fboWidth, fboHeight);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, fboWidth, 0, fboHeight, -1, 1); // ImGui-kompatible UVs

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_tilesetTexture);

        // === Parallax & Fading constants ===
        const float layerOffsetFactorX = 0.45f;
        const float layerOffsetFactorY = 0.35f;
        const float minAlpha = 0.35f;
        const float maxAlpha = 0.9f;
        const float darkenOverlayAlpha = 0.45f;

        // === Gruppiere Tiles nach Layer ===
        std::unordered_map<int, std::vector<const RenderTile*>> layerMap;
        int maxLayer = 0;

        for (const auto& tile : tiles) {
            layerMap[tile.layer].push_back(&tile);
            if (tile.layer > maxLayer) maxLayer = tile.layer;
        }

        // === Maximaler Layer pro Position (x,y) ermitteln ===
        std::unordered_map<std::pair<int, int>, int, std::hash<std::pair<int, int>>> maxLayerMap;
        for (const auto& tile : tiles) {
            std::pair<int, int> key = { tile.x, tile.y };
            auto it = maxLayerMap.find(key);
            if (it == maxLayerMap.end() || tile.layer > it->second) {
                maxLayerMap[key] = tile.layer;
            }
        }

        // === Render layerweise (unten -> oben) ===
        for (int layer = 0; layer <= maxLayer; ++layer) {
            auto it = layerMap.find(layer);
            if (it == layerMap.end()) continue;

            for (const RenderTile* tile : it->second) {
                std::pair<int, int> key = { tile->x, tile->y };
                int localMaxLayer = maxLayerMap[key];

                // relative Helligkeit je Tile basierend auf seiner Layer im Vergleich zur max Layer an der Position
                float relative = (localMaxLayer == tile->layer) ? 1.0f : (float)tile->layer / (float)localMaxLayer;

                // Alpha: bei nur einem Layer höher, sonst interpoliert
                float alpha = (localMaxLayer <= 1)
                    ? 0.8f
                    : minAlpha + (maxAlpha - minAlpha) * pow(relative, 0.9f);

                int relX = tile->x - playerX;
                int relY = tile->y - playerY;

                float diffX = relX * layerOffsetFactorX * tile->layer;
                float diffY = relY * layerOffsetFactorY * tile->layer;

                float px = tile->x * m_tileSize + diffX;
                float py = tile->y * m_tileSize + diffY;

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

                // === Dunkelheits-Overlay für Tiles unter der höchsten Layer an der Position ===
                if (tile->layer < localMaxLayer) {
                    // Je tiefer die Layer, desto stärker die Abdunklung (Alpha proportional zur Layer-Distanz)
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

        // === Cleanup ===
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

