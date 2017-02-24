#pragma once

#include <Ace/IntTypes.h>

#include <memory>

namespace ace
{
	struct Framebuffer
	{
		struct FramebufferImpl;
		std::shared_ptr<FramebufferImpl> impl;

		bool Init();
	};
}