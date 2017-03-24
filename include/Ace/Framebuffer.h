#pragma once

#include <Ace/IntTypes.h>
#include <Ace/GraphicsObject.h>

namespace ace
{
	struct Framebuffer : public GraphicsObject<struct FramebufferImpl>
	{
		enum Attachment
		{
			Depth,			/** */
			Stencil,		/** */
			Color0,			/** ColorX attachement, X can be between 0 to 31. */
		};

		Framebuffer(FramebufferImpl* impl = nullptr);

	protected:
		virtual void Init() const;
	};

	typedef Framebuffer::Attachment FramebufferAttachment;
}