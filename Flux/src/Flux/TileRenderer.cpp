#include "fxpch.h"
#include "TileRenderer.h"

#include <GL/glew.h> 


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
        glOrtho(0, fboWidth, 0, fboHeight, -1, 1); // invert cause ImGui interprets UV inverted to OpenGl

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glLoadIdentity();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_tilesetTexture);

        // Constants for parallax
        const float layerOffsetFactorX = 0.0f;
        const float layerOffsetFactorY = 0.0f;

        // Draw all tiles
        for (const auto& tile : tiles) {
            int relX = tile.x - playerX;
            int relY = tile.y - playerY;

            float diffX = relX * layerOffsetFactorX * tile.layer;
            float diffY = relY * layerOffsetFactorY * tile.layer;

            float px = tile.x * m_tileSize + diffX;
            float py = tile.y * m_tileSize + diffY;

            int tileIndex = static_cast<int>(tile.tileChar) - 32;
            int tx = tileIndex % m_tilesPerRow;
            int ty = tileIndex / m_tilesPerRow;

            float u0 = tx * (float)m_tileSize / m_tilesetWidth;
            float v0 = ty * (float)m_tileSize / m_tilesetHeight;
            float u1 = u0 + (float)m_tileSize / m_tilesetWidth;
            float v1 = v0 + (float)m_tileSize / m_tilesetHeight;

            glColor4f(tile.r, tile.g, tile.b, tile.a);

            glBegin(GL_QUADS);
            glTexCoord2f(u0, v0); glVertex2f(px, py);
            glTexCoord2f(u1, v0); glVertex2f(px + m_tileSize, py);
            glTexCoord2f(u1, v1); glVertex2f(px + m_tileSize, py + m_tileSize);
            glTexCoord2f(u0, v1); glVertex2f(px, py + m_tileSize);
            glEnd();
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

