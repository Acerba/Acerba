#include <Ace/UserInterface.h>
#include <Ace/Assert.h>
#include <Ace/Macros.h>
#include <Ace/Platform.h>
#include <Ace/Window.h>
#include <Ace/WindowImpl.h>

#include <imgui.h>
#include <imgui_internal.h>


#include <SDL.h>
#include <SDL_syswm.h>
#include <Ace/GL.h>

namespace ace
{

    const UInt32 UI::s_windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings;

    //static const UInt32 s_windowFlagsNoInput = s_windowFlags | ImGuiWindowFlags_NoInputs;

    static const float s_factorUI = 0.92f;
    static UInt8 s_windowID = 0u;


    class UserInterfaceImpl : public EventBase<Event::SDLEventArg>
    {

    public:

        // NOTE: naming: m_variable...

        double   m_Time;
        bool     m_MousePressed[3];
        float    m_MouseWheel;
        UInt32   m_FontTexture;
        Int32    m_ShaderHandle;
        Int32    m_VertHandle;
        Int32    m_FragHandle;
        Int32    m_AttribLocationTex;
        Int32    m_AttribLocationProjMtx;
        Int32    m_AttribLocationPosition;
        Int32    m_AttribLocationUV;
        Int32    m_AttribLocationColor;
        UInt32   m_VboHandle;
        //UInt32   m_VaoHandle;
        UInt32   m_ElementsHandle;
        Window   m_window;

        UserInterfaceImpl() :
            m_Time(0.0f),
            m_MousePressed(),
            m_MouseWheel(0.0f),
            m_ShaderHandle(0),
            m_VertHandle(0),
            m_FragHandle(0),
            m_AttribLocationTex(0),
            m_AttribLocationProjMtx(0),
            m_AttribLocationPosition(0),
            m_AttribLocationUV(0),
            m_AttribLocationColor(0),
            m_FontTexture(0u),
            m_VboHandle(0u),
            //m_VaoHandle(0u),
            m_ElementsHandle(0u),
            m_window(nullptr)
        {
            m_MousePressed[0] = false;
            m_MousePressed[1] = false;
            m_MousePressed[2] = false;
        };

        ~UserInterfaceImpl()
        {
            Shutdown();
        }

        bool CreateDeviceObjects()
        {

            // Backup GL state
            //Int32 last_texture, last_array_buffer, last_vertex_array;
            //glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            //glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            //glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

            // TODO: Convert Shaders from GLSL 3 to GLSL 1. (#version 100)

            const GLchar *vertex_shader =
                "#version 100                                       \n"
                "uniform mat4 ProjMtx;                              \n"
                "attribute vec2 Position;                           \n"
                "attribute vec2 UV;                                 \n"
                "attribute vec4 Color;                              \n"
                "varying vec2 Frag_UV;                              \n"
                "varying vec4 Frag_Color;                           \n"
                "void main()                                        \n"
                "{                                                  \n"
                "	Frag_UV = UV;                                   \n"
                "	Frag_Color = Color;                             \n"
                "	gl_Position = ProjMtx * vec4(Position.xy,0,1);  \n"
                "}                                                  \n";

            const GLchar* fragment_shader =
                "#version 100                                                       \n"
                "uniform sampler2D Texture;                                         \n"
                "varying lowp vec2 Frag_UV;                                         \n"
                "varying lowp vec4 Frag_Color;                                      \n"
                "void main()                                                        \n"
                "{                                                                  \n"
                "	gl_FragColor = Frag_Color * texture2D( Texture, Frag_UV.st);    \n"
                "}                                                                  \n";

            m_ShaderHandle = glCreateProgram();
            m_VertHandle = glCreateShader(GL_VERTEX_SHADER);
            m_FragHandle = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(m_VertHandle, 1, &vertex_shader, 0);
            glShaderSource(m_FragHandle, 1, &fragment_shader, 0);
            glCompileShader(m_VertHandle);
            glCompileShader(m_FragHandle);
            glAttachShader(m_ShaderHandle, m_VertHandle);
            glAttachShader(m_ShaderHandle, m_FragHandle);
            glLinkProgram(m_ShaderHandle);

            m_AttribLocationTex = glGetUniformLocation(m_ShaderHandle, "Texture");
            m_AttribLocationProjMtx = glGetUniformLocation(m_ShaderHandle, "ProjMtx");
            m_AttribLocationPosition = glGetAttribLocation(m_ShaderHandle, "Position");
            m_AttribLocationUV = glGetAttribLocation(m_ShaderHandle, "UV");
            m_AttribLocationColor = glGetAttribLocation(m_ShaderHandle, "Color");

            glGenBuffers(1, &m_VboHandle);
            glGenBuffers(1, &m_ElementsHandle);

            //glGenVertexArrays(1, &m_VaoHandle);
            //glBindVertexArray(m_VaoHandle);
            glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
            glEnableVertexAttribArray(m_AttribLocationPosition);
            glEnableVertexAttribArray(m_AttribLocationUV);
            glEnableVertexAttribArray(m_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(m_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(m_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(m_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

            CreateFontsTexture();

            // Restore modified GL state
            //glBindTexture(GL_TEXTURE_2D, last_texture);
            //glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            //glBindVertexArray(last_vertex_array); // GLES 2 doesn't support this.

            return true;

        }

        void CreateFontsTexture()
        {
            // Build texture atlas
            ImGuiIO& io = ImGui::GetIO();
            UInt8* pixels;
            Int32 width, height;
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.

            // Upload texture to graphics system
            Int32 last_texture;
            //glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGenTextures(1, &m_FontTexture);
            glBindTexture(GL_TEXTURE_2D, m_FontTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // GLES 2 doesn't support this.
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            // Store our identifier
            io.Fonts->TexID = (void *)(UInt32)m_FontTexture;

            // Restore state
            //glBindTexture(GL_TEXTURE_2D, last_texture);
        }

        static const char* GetClipboardText(void*)
        {
            return SDL_GetClipboardText();
        }

        static UserInterfaceImpl& GetInstance()
        {
            static UserInterfaceImpl instance;
            return instance;
        }

        void Init(const Window& window)
        {
            m_window = window;
            ImGuiIO& io = ImGui::GetIO();
            io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;                     // Keyboard mapping. ImGui will use those indices to peek Int32o the io.KeyDown[] array.
            io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
            io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
            io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
            io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
            io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
            io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
            io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
            io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
            io.KeyMap[ImGuiKey_A] = SDLK_a;
            io.KeyMap[ImGuiKey_C] = SDLK_c;
            io.KeyMap[ImGuiKey_V] = SDLK_v;
            io.KeyMap[ImGuiKey_X] = SDLK_x;
            io.KeyMap[ImGuiKey_Y] = SDLK_y;
            io.KeyMap[ImGuiKey_Z] = SDLK_z;

            io.RenderDrawListsFn = RenderDrawLists;   // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
            io.SetClipboardTextFn = SetClipboardText;
            io.GetClipboardTextFn = GetClipboardText;
            io.ClipboardUserData = NULL;

#ifdef ACE_WIN
            SDL_SysWMinfo wmInfo;
            SDL_VERSION(&wmInfo.version);
            SDL_GetWindowWMInfo((*m_window)->sdlWindow, &wmInfo);
            io.ImeWindowHandle = wmInfo.info.win.window;
#else
            (void)window;
#endif
        }

        // Use if you want to reset your rendering device without losing ImGui state.
        void InvalidateDeviceObjects()
        {
            //if (m_VaoHandle) glDeleteVertexArrays(1, &m_VaoHandle);
            if (m_VboHandle) glDeleteBuffers(1, &m_VboHandle);
            if (m_ElementsHandle) glDeleteBuffers(1, &m_ElementsHandle);
            //m_VaoHandle = m_VboHandle = m_ElementsHandle = 0;
            m_VboHandle = m_ElementsHandle = 0;

            if (m_ShaderHandle && m_VertHandle) glDetachShader(m_ShaderHandle, m_VertHandle);
            if (m_VertHandle) glDeleteShader(m_VertHandle);
            m_VertHandle = 0;

            if (m_ShaderHandle && m_FragHandle) glDetachShader(m_ShaderHandle, m_FragHandle);
            if (m_FragHandle) glDeleteShader(m_FragHandle);
            m_FragHandle = 0;

            if (m_ShaderHandle) glDeleteProgram(m_ShaderHandle);
            m_ShaderHandle = 0;

            if (m_FontTexture)
            {
                glDeleteTextures(1, &m_FontTexture);
                ImGui::GetIO().Fonts->TexID = 0;
                m_FontTexture = 0;
            }
        }

        void NewFrame()
        {
            SDL_Window* window = (*m_window)->sdlWindow;

            if (!m_FontTexture)
                CreateDeviceObjects();

            ImGuiIO& io = ImGui::GetIO();

            // Setup display size (every frame to accommodate for window resizing)
            Int32 w = 0, h = 0;
            Int32 display_w = 0, display_h = 0;
            SDL_GetWindowSize(window, &w, &h);
            SDL_GL_GetDrawableSize(window, &display_w, &display_h);
            io.DisplaySize = ImVec2((float)w, (float)h);
            io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

            // Setup time step
            Uint32 time = SDL_GetTicks();
            double current_time = time / 1000.0;
            io.DeltaTime = m_Time > 0.0 ? (float)(current_time - m_Time) : (float)(1.0f / 60.0f);
            m_Time = current_time;

            // Setup inputs
            // (we already got mouse wheel, keyboard keys & characters from SDL_PollEvent())
            Int32 mx = 0, my = 0;
            Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
            if (SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS)
                io.MousePos = ImVec2((float)mx, (float)my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
            else
                io.MousePos = ImVec2(-1, -1);

            io.MouseDown[0] = m_MousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;		// If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
            io.MouseDown[1] = m_MousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
            io.MouseDown[2] = m_MousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
            m_MousePressed[0] = m_MousePressed[1] = m_MousePressed[2] = false;

            io.MouseWheel = m_MouseWheel;
            m_MouseWheel = 0.0f;

            // Hide OS mouse cursor if ImGui is drawing it
            SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

            // Start the frame
            ImGui::NewFrame();

            glBindBuffer(GL_ARRAY_BUFFER, m_VboHandle);
            glEnableVertexAttribArray(m_AttribLocationPosition);
            glEnableVertexAttribArray(m_AttribLocationUV);
            glEnableVertexAttribArray(m_AttribLocationColor);

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))
            glVertexAttribPointer(m_AttribLocationPosition, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
            glVertexAttribPointer(m_AttribLocationUV, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
            glVertexAttribPointer(m_AttribLocationColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF
        }

        bool ProcessEvent(SDL_Event* _event)
        {
            ImGuiIO& io = ImGui::GetIO();
            switch (_event->type)
            {
            case SDL_MOUSEWHEEL:
            {
                if (_event->wheel.y > 0)
                    m_MouseWheel = 1;
                if (_event->wheel.y < 0)
                    m_MouseWheel = -1;
                return true;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (_event->button.button == SDL_BUTTON_LEFT) m_MousePressed[0] = true;
                if (_event->button.button == SDL_BUTTON_RIGHT) m_MousePressed[1] = true;
                if (_event->button.button == SDL_BUTTON_MIDDLE) m_MousePressed[2] = true;
                return true;
            }
            case SDL_TEXTINPUT:
            {
                io.AddInputCharactersUTF8(_event->text.text);
                return true;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                Int32 key = _event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
                io.KeysDown[key] = (_event->type == SDL_KEYDOWN);
                io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
                io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
                io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
                io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
                return true;
            }
            }
            return false;
        }

        static void Render()
        {
            glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
            ImGui::Render();
        }

        static void RenderDrawLists(ImDrawData* draw_data)
        {
            // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
            ImGuiIO& io = ImGui::GetIO();
            UserInterfaceImpl& impl = GetInstance();

            Int32 fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
            Int32 fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);

            if (fb_width == 0 || fb_height == 0)
                return;


            // Backup GL state
            GLint last_program;
            glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            GLint last_array_buffer;
            glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
            GLint last_element_array_buffer;
            glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);

            GLint last_blend_equation_rgb;
            glGetIntegerv(GL_BLEND_EQUATION_RGB, &last_blend_equation_rgb);
            GLint last_blend_equation_alpha;
            glGetIntegerv(GL_BLEND_EQUATION_ALPHA, &last_blend_equation_alpha);
            GLint last_viewport[4];
            glGetIntegerv(GL_VIEWPORT, last_viewport);

            GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
            GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
            GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
            GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

            // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glActiveTexture(GL_TEXTURE0); 

            draw_data->ScaleClipRects(io.DisplayFramebufferScale);

            // Setup orthographic projection matrix
            glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
            const float ortho_projection[4][4] =
            {
                { 2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f },
                { 0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f },
                { 0.0f, 0.0f, -1.0f, 0.0f },
                { -1.0f, 1.0f, 0.0f, 1.0f },
            };

            glUseProgram(impl.m_ShaderHandle);
            glUniform1i(impl.m_AttribLocationTex, 0);
            glUniformMatrix4fv(impl.m_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);

            for (int n = 0; n < draw_data->CmdListsCount; n++)
            {
                const ImDrawList* cmd_list = draw_data->CmdLists[n];
                const ImDrawIdx* idx_buffer_offset = 0;

                glBindBuffer(GL_ARRAY_BUFFER, impl.m_VboHandle);
                glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.size() * sizeof(ImDrawVert), (GLvoid*)&cmd_list->VtxBuffer.front(), GL_STREAM_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, impl.m_ElementsHandle);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx), (GLvoid*)&cmd_list->IdxBuffer.front(), GL_STREAM_DRAW);

                for (const ImDrawCmd* pcmd = cmd_list->CmdBuffer.begin(); pcmd != cmd_list->CmdBuffer.end(); pcmd++)
                {
                    if (pcmd->UserCallback)
                    {
                        pcmd->UserCallback(cmd_list, pcmd);
                    }
                    else
                    {
                        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                        glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                        glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
                    }
                    idx_buffer_offset += pcmd->ElemCount;
                }
            }
                        
            // Restore modified GL state
            glUseProgram(last_program);
            glBindTexture(GL_TEXTURE_2D, last_texture);
            glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
            glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            if (last_enable_blend)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);

            if (last_enable_cull_face)
                glEnable(GL_CULL_FACE);
            else
                glDisable(GL_CULL_FACE);

            if (last_enable_depth_test)
                glEnable(GL_DEPTH_TEST);
            else
                glDisable(GL_DEPTH_TEST);

            if (last_enable_scissor_test)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);

            glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
        }

        static void SetClipboardText(void*, const char* text)
        {
            SDL_SetClipboardText(text);
        }

        void Shutdown()
        {
            InvalidateDeviceObjects();
            ImGui::Shutdown();
        }

        virtual void OnEvent(Event::SDLEventArg eventArg)
        {
            ProcessEvent(&eventArg.event);
        }

    };


    void UserInterface::BeginUI()
    {
        s_windowID = 0u;
        UserInterfaceImpl::GetInstance().NewFrame();
    }

    void UserInterface::EndUI()
    {
        UserInterfaceImpl::Render();
    }


    void UserInterface::BeginGroup(const Vector2& size, const Vector2& position, const UInt32 flags)
    {
        ACE_ASSERT(s_windowID != 255u, "Too many UI groups created.", "")
        ImGui::SetNextWindowSize(ImVec2(size.x, size.y));
        ImGui::SetNextWindowPos(ImVec2(position.x, position.y));
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.f;
        ImGui::Begin(reinterpret_cast<const char*>(&++s_windowID), nullptr, s_windowFlags);
        ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 1.f;
    }

    void UserInterface::EndGroup()
    {
        ImGui::End();
    }


    void UserInterface::Debug(const Vector2* size, const Vector2* position)
    {
        //The easiest way is to create a dummy window.
        //Call Begin() with NoTitleBar | NoResize | NoMove | NoScrollbar | NoSavedSettings | NoInputs flag
        //zero background alpha,
        //then retrieve the ImDrawList* via GetWindowDrawList() and draw to it in any way you like.

        BeginGroup(size ? *size : Vector2(60.f, 60.f), position ? *position : Vector2(0.f, 0.f), s_windowFlags | ImGuiWindowFlags_NoCollapse);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        EndGroup();
    }


    void UserInterface::Init(const Window& window)
    {
        UserInterfaceImpl::GetInstance().Init(window);
    }


    void UserInterface::Bar(const float progress, const Vector2& size, const char* text)
    {
        ImGui::ProgressBar(progress, ImVec2(size.x * s_factorUI, size.y * s_factorUI), text);
    }


    bool UserInterface::Button(const Vector2& size, const char* text)
    {
        return ImGui::Button(text, ImVec2(size.x * s_factorUI, size.y * s_factorUI));
    }


    void UserInterface::Text(const char* text ...)
    {
        ACE_ASSERT(text, "Invalid UI Text field", "")
        va_list args;
        va_start(args, text);
        ImGui::TextV(text, args);
        va_end(args);
    }


    void UserInterface::Quit()
    {
        UserInterfaceImpl::GetInstance().Shutdown();
    }

}