#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Vector2.h>

namespace ace
{
    class Camera;
    
    struct Mouse
    {
        
        enum class Button : UInt8
        {
            Invalid = 0,
            Left,
            Middle,
            Right,
        };
        
		//int SDL_GetMouseState(int* x, int* y);
		//int SDL_CaptureMouse(bool enabled);
		//Mouse();
        //~Mouse();
        
        /**
            @brief Retrieves the current on-screen coordinates of the mouse. Range: [0:0, 1:1]
            @return Vector2
        */
        static math::Vector2 GetPosition();
        
        static math::Vector2 GetWorldPosition(const Camera& camera);
        
        /**
            @brief Returns the pressed mouse button.
            @return MouseButton
        */
        static Button GetButton();
    };
}