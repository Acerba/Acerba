#pragma once

namespace ace
{
    struct Drawable
    {
        /**
            @brief Custom Draw method
            Requires manual buffer or sprite logic.
        */
        virtual void Draw() const = 0;
    };
}