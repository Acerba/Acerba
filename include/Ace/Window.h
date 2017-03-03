#pragma once

#include <Ace/IntTypes.h>
#include <Ace/Types.h>

#include <memory>

namespace ace
{
	/**
		@brief Window
	*/
	class Window
	{

	public:
		struct WindowImpl;

	private:

		std::shared_ptr<WindowImpl> m_windowImpl;

	public:

		/**
			@brief Constructor
		*/
		Window(const char* title, UInt16 w, UInt16 h);
		~Window();

		inline WindowImpl* operator*()
		{
			return m_windowImpl.get();
		}

		/**
			@brief Updates Window
		*/
		void Present();

		/**
			@brief Clears Windows
		*/
		void Clear(const Color32& color);

		/**
			@return Window Size
		*/
		Vector2 GetSize() const;

		/**
			@brief Sets Window size
			@param[in] size
		*/
		void SetSize(const Vector2&);

		operator bool() const;
	};
}