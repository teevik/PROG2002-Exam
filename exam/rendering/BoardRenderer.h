#ifndef EXAMAUTUMN2023_BOARDRENDERER_H
#define EXAMAUTUMN2023_BOARDRENDERER_H

#include "ObjectRenderer.h"
#include "../game/Board.h"
#include "framework/Texture.h"

struct BoardRenderer {
    const ObjectRenderer cubeRenderer;
    const ObjectRenderer pillarRenderer;

    const framework::Texture playerTexture;
    const framework::Texture boxTexture;
    const framework::Texture wallTexture;

    static BoardRenderer create();

    void draw(const Board &board, const Light &light, const framework::Camera &camera, bool useTextures) const;
};

#endif //EXAMAUTUMN2023_BOARDRENDERER_H
