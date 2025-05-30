#pragma once

namespace conways_game_of_life
{
    inline float normalize(float min, float max, float val) { return (val - min) / (max - min); }
    inline unsigned int convert_indices(int x, int y, int width) { return (y * width) + x; }
} // namespace conways_game_of_life