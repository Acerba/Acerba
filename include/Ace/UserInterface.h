#pragma once

#include <Ace/Font.h>
#include <Ace/IntTypes.h>
#include <Ace/Macros.h>

#define NK_INCLUDE_DEFAULT_ALLOCATOR 1
#define NK_INCLUDE_DEFAULT_FONT 1
#define NK_INCLUDE_FONT_BAKING 1
#define NK_INCLUDE_STANDARD_IO 1
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT 1
#include <nuklear/nuklear.h>

#include <memory>

struct nk_context;

namespace ace
{
    class Font;
    class Window;

    class UI
    {
        //class UIImpl;
        //friend class Module;

        //std::unique_ptr<UIImpl> m_impl;
        UI() = delete;
        ~UI() = delete;
        ACE_DISABLE_COPY(UI)



    public:

        static void AddFonts(const std::vector<Font>& fonts);

        static void Draw();

        static nk_context* GetContext();

        static void Init(const Window& win);


    };

}