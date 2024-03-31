#pragma once

#include <vector>

namespace pixel
{
    class PixelCollection
    {
    private:
        std::vector<int> energy_central;

    public:
        PixelCollection() = default;
        ~PixelCollection() = default;

        void fill_collection();
    };

} // namespace pixel
