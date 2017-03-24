#include <Ace/Framebuffer.h>
#include <Ace/GLFramebbuferImpl.h> 

namespace ace
{
	Framebuffer::Framebuffer(FramebufferImpl* impl) : Graphics(impl)
	{

	}
}