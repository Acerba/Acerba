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
			@param[in] title Window title
			@param[in] width
			@param[in] height
		*/
		Window(const char* title, UInt16 width, UInt16 height);
		~Window();

        inline WindowImpl* operator*() const
        {
            return m_windowImpl.get();
        }

		inline WindowImpl* operator*()
		{
			return m_windowImpl.get();
		}

		/**
			@brief Closes the window
		*/
		void Close();

		/**
			@brief Updates the window
		*/
		void Present();

		/**
			@brief Clears the window
		*/
		void Clear(const Color32& color);

		/**
			@return Window size
		*/
		Vector2 GetSize() const;

		/**
			@brief Sets window size
			@param[in] size
		*/
		void SetSize(const Vector2&);

		operator bool() const;
	};
}