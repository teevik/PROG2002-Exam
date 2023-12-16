#ifndef EXAMAUTUMN2023_BOARDRENDERER_H
#define EXAMAUTUMN2023_BOARDRENDERER_H

#include "ObjectRenderer.h"
#include "../game/Board.h"

struct BoardRenderer {
    ObjectRenderer cubeRenderer;
    ObjectRenderer pillarRenderer;

    static BoardRenderer create();

    void draw(const Board &board, const framework::Camera &camera) const;
};

#endif //EXAMAUTUMN2023_BOARDRENDERER_H
