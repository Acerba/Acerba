#pragma once

#include <Ace/Framebuffer.h>
#include <Ace/GL.h>


namespace ace
{
	struct Framebuffer::FramebufferImpl
	{
		UInt32 framebufferID;

		FramebufferImpl()
		{
			glGenFramebuffers(1, &framebufferID);
		}

		~FramebufferImpl()
		{
			glDeleteFramebuffers(1, &framebufferID);
		}
	};
}
