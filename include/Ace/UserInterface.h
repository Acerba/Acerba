#pragma once

#include <Ace/Event.h>
#include <Ace/Types.h>


namespace ace
{
    class Window;

    class UserInterface
    {
        static const UInt32 s_windowFlags;

        UserInterface() = delete;
        ~UserInterface() = delete;
        UserInterface(UserInterface&&) = delete;
        UserInterface(const UserInterface&) = delete;
        UserInterface& operator=(const UserInterface&) = delete;

    public:

        /**
            @brief Initializes UI to take in element groups.
        */
        static void BeginUI();

        /**
            @brief Finalizes UI to take no more element groups.
        */
        static void EndUI();

        /**
            @brief Starts an UI element group.
            @param[in] size Size of the group in pixels.
            @param[in] positiom Position of the group in pixels. (0, 0) is the left upper corner.
            @param[in] flags Flags to change group functionality.
        */
        static void BeginGroup(const Vector2& size, const Vector2& position, const UInt32 flags = s_windowFlags);

        /**
            @brief Ends an UI element group.
        */
        static void EndGroup();

        /**
            @brief Creates and shows default Acerba debug information. Doesn't need to be places in a group.
            @param[in] size Size of the group in pixels.
            @param[in] position Position of the group in pixels.
        */
        static void Debug(const Vector2* size = nullptr, const Vector2* position = nullptr);

        /**
            @brief Initializes UI to use target window.
            @param[in] window Target window to render UI elements to.
        */
        static void Init(const Window& window);

        /**
            @brief Makes progression bar to current group
            @param[in] progress Progress of the bar. 0...1
            @param[in] size Size of the bar.
            @param[in] text Text shown in the bar. 0...1 percentage by default.
        */
        static void Bar(const float progress, const Vector2& size, const char* text = nullptr);

        /**
            @brief Makes a clickable button to the current group.
            @param[in] size Size of the button in pixels.
            @param[in] text Text on the button. Empty by default.
        */
        static bool Button(const Vector2& size, const char* text = "");

        /**
            @brief Makes a text label to the current group.
            @param[in] text Text to show in the element.
        */
        static void Text(const char* text ...);

        /**
            @brief Destroys UI.
        */
        static void Quit();

    };


    typedef UserInterface UI;



}