#include <Ace/Mouse.h>
#include <Ace/Window.h>
#include <Ace/Camera.h>
#include <SDL_mouse.h>

#include <Ace/GL.h>

namespace ace
{
    
    math::Vector2 Mouse::GetPosition()
	{
        Int32 x, y;
        SDL_GetMouseState(&x, &y);
        const math::Vector2 size(Window::GetCurrent()->GetSize());
        return math::Vector2(static_cast<float>(x) / size.x, static_cast<float>(y) / size.y);
	}
    
	math::Vector2 Mouse::GetWorldPosition(const Camera& camera)
	{
        const math::Vector2 screenPos(GetPosition());
        
        #if 1
        
        //const math::Vector3 cp(camera.GetPosition());
        //return math::Vector2(mp.x + cp.x, mp.y + cp.y);
        const math::Vector4 screenPosV4(screenPos.x, screenPos.y, 0.f, 1.f);
        const math::Vector4 mp(
            math::ToVektor(
                math::Inverse(
                    camera.GetMatrix()
                ) * screenPosV4
            )
        );
        // const math::Vector4 mp(camera.);
        return math::Vector2(mp.x / mp.w, mp.y / mp.w);
        
        #else
        
        Int32 viewport[4];
        double modelview[16];
        double projection[16];
        float winX, winY, winZ;
        double posX, posY, posZ;
        
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
        glGetDoublev( GL_PROJECTION_MATRIX, projection );
        glGetIntegerv( GL_VIEWPORT, viewport );
        
        winX = (float)screenPos.x;
        winY = (float)viewport[3] - (float)screenPos.y;
        glReadPixels( screenPos.x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
        
        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
        
        return Vector2(posX, posY);
        
        #endif
	}
    
	Mouse::Button Mouse::GetButton()
	{
        
		UInt32 state = SDL_GetMouseState(NULL, NULL);
        
		if (state & SDL_BUTTON(SDL_BUTTON_LEFT))
		{
			return Button::Left;
		}
        
		if (state & SDL_BUTTON(SDL_BUTTON_MIDDLE))
		{
			return Button::Middle;
		}
        
		if (state & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
			return Button::Right;
		}
        
		return Button::Invalid;
	}
}