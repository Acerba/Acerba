#pragma once

#include <Ace/IntTypes.h>
#include <Ace/SmartPtr.h>
#include <Ace/Types.h>


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
        Color m_color;

	public:

		static Window* GetCurrent();
		
        Window(WindowImpl* impl);
        
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
		void Clear(const Color32* colour = nullptr);

        /**
            @brief Sets clear colour for the window
        */
        void SetClearColor(const Color& colour);

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