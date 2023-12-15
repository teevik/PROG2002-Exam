#ifndef EXAMAUTUMN2023_SPARSEGRID_H
#define EXAMAUTUMN2023_SPARSEGRID_H

#include <cstdint>
#include <vector>
#include <optional>
#include "glm/fwd.hpp"
#include "glm/detail/type_vec2.hpp"

/// Row major sparse grid
template<typename T>
class SparseGrid {
private:
    glm::uvec2 size;

    std::vector<std::optional<T>> data;
    const std::optional<T> empty = std::nullopt;

    /// Has no bounds check
    uint32_t getIndex(glm::ivec2 position) {
        return (position.y * size.x) + position.x;
    }

public:
    SparseGrid(glm::uvec2 size) :
        size(size),
        data(std::vector<std::optional<T>>(size.x * size.y)) {}

    const std::vector<std::optional<T>> &getData() {
        return data;
    }

    bool isInBounds(glm::ivec2 position) {
        return
            position.x >= 0 &&
            position.x < size.x &&
            position.y >= 0 &&
            position.y < size.y;
    }

    bool contains(glm::ivec2 position) {
        if (!isInBounds(position)) return false;
        auto index = getIndex(position);

        return data[index].has_value();
    }

    const std::optional<T> &get(glm::ivec2 position) {
        if (!isInBounds(position)) return empty;

        auto index = getIndex(position);
        std::optional<T> &value = data[index];

        return value;
    }

    void set(glm::ivec2 position, T value) {
        if (!isInBounds(position)) return;

        auto index = getIndex(position);
        data[index] = value;
    }
};

#endif //EXAMAUTUMN2023_SPARSEGRID_H
