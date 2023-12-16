#ifndef EXAMAUTUMN2023_BOARDRENDERER_H
#define EXAMAUTUMN2023_BOARDRENDERER_H

#include "ObjectRenderer.h"
#include "../game/Board.h"
#include "framework/Texture.h"

struct BoardRenderer {
    ObjectRenderer cubeRenderer;
    ObjectRenderer pillarRenderer;

    framework::Texture playerTexture;
    framework::Texture boxTexture;
    framework::Texture wallTexture;

    static BoardRenderer create();

    void draw(const Board &board, const framework::Camera &camera, bool useTextures) const;
};

#endif //EXAMAUTUMN2023_BOARDRENDERER_H
