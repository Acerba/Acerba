#define NK_IMPLEMENTATION 1
#include <Ace/UserInterface.h>

#include <Ace/Window.h>
#include <Ace/WindowImpl.h>
#include <Ace/Log.h>

#include <Ace/GL.h>
#include <SDL.h>

#define MAX_VERTEX_MEMORY 512 * 1024
#define MAX_ELEMENT_MEMORY 128 * 1024

namespace ace
{

    class UIImpl
    {
        friend class UI;

        struct sdl_vertex {
            float position[2];
            float uv[2];
            nk_byte col[4];
        };

        struct sdl_dev
        {
            nk_buffer cmds;
            nk_draw_null_texture nulTex;
            UInt32 fontTex = 0u;
            Uint32 vbo = 0u;
            UInt32 vao = 0u;
            UInt32 ebo = 0u;
            UInt32 prog = 0u;
            UInt32 vert = 0u;
            UInt32 frag = 0u;
            Int32 attrPos = 0u;
            Int32 attrUV = 0u;
            Int32 attrCol = 0u;
            Int32 unifTex = 0u;
            Int32 unifProj = 0u;

            sdl_dev() = default;
            ~sdl_dev() = default;
            ACE_DISABLE_COPY(sdl_dev)
        };


        struct sdl_wrap
        {
            nk_font_atlas atlas;
            nk_context context;
            sdl_dev ogl;
            SDL_Window* window = nullptr;

            sdl_wrap() = default;
            ~sdl_wrap() = default;
            ACE_DISABLE_COPY(sdl_wrap)
        };

        sdl_wrap m_sdl;

        UIImpl() :
            m_sdl()
        {

        }
        ~UIImpl()
        {
            nk_font_atlas_clear(&m_sdl.atlas);
            nk_free(&m_sdl.context);
            glDeleteTextures(1, &m_sdl.ogl.fontTex);
            nk_buffer_free(&m_sdl.ogl.cmds);
        }

        static UIImpl& GetInstance()
        {
            static UIImpl impl;
            return impl;
        }


        static void ClipboardCopy(nk_handle user, const char* text, Int32 length)
        {
            //does this need the other stuff
            SDL_SetClipboardText(text);
        }

        static void ClipboardPaste(nk_handle user, nk_text_edit* edit)
        {
            const char* text = SDL_GetClipboardText();
            if (text)
                nk_textedit_paste(edit, text, nk_strlen(text));
            //why cast user to void here
        }

        void DeviceCreate()
        {
            Int32 status = 0;
            static const GLchar* vertex_shader =
                "#version 300 es\n"
                "uniform mat4 ProjMtx;\n"
                "in vec2 Position;\n"
                "in vec2 TexCoord;\n"
                "in vec4 Color;\n"
                "out vec2 Frag_UV;\n"
                "out vec4 Frag_Color;\n"
                "void main() {\n"
                "   Frag_UV = TexCoord;\n"
                "   Frag_Color = Color;\n"
                "   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
                "}\n";
            static const GLchar* fragment_shader =
                "#version 300 es\n"
                "precision mediump float;\n"
                "uniform sampler2D Texture;\n"
                "in vec2 Frag_UV;\n"
                "in vec4 Frag_Color;\n"
                "out vec4 Out_Color;\n"
                "void main(){\n"
                "   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
                "}\n";

            sdl_dev *dev = &m_sdl.ogl;
            nk_buffer_init_default(&dev->cmds);
            dev->prog = glCreateProgram();
            dev->vert = glCreateShader(GL_VERTEX_SHADER);
            dev->frag = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(dev->vert, 1, &vertex_shader, 0);
            glShaderSource(dev->frag, 1, &fragment_shader, 0);
            glCompileShader(dev->vert);
            glCompileShader(dev->frag);
            glGetShaderiv(dev->vert, GL_COMPILE_STATUS, &status);
            assert(status == GL_TRUE);
            glGetShaderiv(dev->frag, GL_COMPILE_STATUS, &status);
            assert(status == GL_TRUE);
            glAttachShader(dev->prog, dev->vert);
            glAttachShader(dev->prog, dev->frag);
            glLinkProgram(dev->prog);
            glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
            assert(status == GL_TRUE);

            dev->unifTex = glGetUniformLocation(dev->prog, "Texture");
            dev->unifProj = glGetUniformLocation(dev->prog, "ProjMtx");
            dev->attrPos = glGetAttribLocation(dev->prog, "Position");
            dev->attrUV = glGetAttribLocation(dev->prog, "TexCoord");
            dev->attrCol = glGetAttribLocation(dev->prog, "Color");

            {
                /* buffer setup */
                static const Int32 vs = sizeof(sdl_vertex);
                UInt64 vp = offsetof(sdl_vertex, position);
                UInt64 vt = offsetof(sdl_vertex, uv);
                UInt64 vc = offsetof(sdl_vertex, col);

                glGenBuffers(1, &dev->vbo);
                glGenBuffers(1, &dev->ebo);
                glGenVertexArrays(1, &dev->vao);

                glBindVertexArray(dev->vao);
                glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

                glEnableVertexAttribArray((GLuint)dev->attrPos);
                glEnableVertexAttribArray((GLuint)dev->attrUV);
                glEnableVertexAttribArray((GLuint)dev->attrCol);

                glVertexAttribPointer((GLuint)dev->attrCol, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
                glVertexAttribPointer((GLuint)dev->attrUV, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
                glVertexAttribPointer((GLuint)dev->attrCol, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
            }

            glBindTexture(GL_TEXTURE_2D, 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void DeviceDestroy()
        {
            sdl_dev* dev = &m_sdl.ogl;
            glDetachShader(dev->prog, dev->vert);
            glDetachShader(dev->prog, dev->frag);
            glDeleteShader(dev->vert);
            glDeleteShader(dev->frag);
            glDeleteProgram(dev->prog);
            glDeleteTextures(1, &dev->fontTex);
            glDeleteBuffers(1, &dev->vbo);
            glDeleteBuffers(1, &dev->ebo);
            nk_buffer_free(&dev->cmds);
        }

        void FontStashBegin()//nk_font_atlas** atlas)
        {
            nk_font_atlas_init_default(&m_sdl.atlas);
            nk_font_atlas_begin(&m_sdl.atlas);
            //font = nk_font_atlas_add_default(&m_sdl.atlas, 13.0f, nullptr);
            //*atlas = &m_sdl.atlas;
        }

        void FontStashEnd()
        {
            Int32 w = 0, h = 0;
            const void* image = nk_font_atlas_bake(&m_sdl.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
            UploadAtlas(image, w, h);
            nk_font_atlas_end(&m_sdl.atlas, nk_handle_id(static_cast<Int32>(m_sdl.ogl.fontTex)), &m_sdl.ogl.nulTex);

            nk_init_default(&m_sdl.context, &m_sdl.atlas.default_font->handle);
            nk_style_set_font(&m_sdl.context, &m_sdl.atlas.fonts->handle);


            if (m_sdl.atlas.default_font)
                nk_style_set_font(&m_sdl.context, &m_sdl.atlas.default_font->handle);


        }

        Int32 HandleEvent(SDL_Event* evt)
        {
            nk_context* ctx = &m_sdl.context;

            /* optional grabbing behavior */
            if (ctx->input.mouse.grab)
            {
                SDL_SetRelativeMouseMode(SDL_TRUE);
                ctx->input.mouse.grab = 0;
            }
            else if (ctx->input.mouse.ungrab)
            {
                SDL_SetRelativeMouseMode(SDL_FALSE);
                SDL_WarpMouseInWindow(m_sdl.window, static_cast<Int32>(ctx->input.mouse.prev.x), static_cast<Int32>(ctx->input.mouse.prev.y));
                ctx->input.mouse.ungrab = 0;
            }
            if (evt->type == SDL_KEYUP || evt->type == SDL_KEYDOWN)
            {
                /* key events */
                Int32 down = evt->type == SDL_KEYDOWN;
                const UInt8* state = SDL_GetKeyboardState(0);
                switch (evt->key.keysym.sym)
                {
                case SDLK_RSHIFT:       //[[fallthrough]]
                case SDLK_LSHIFT:       nk_input_key(ctx, NK_KEY_SHIFT, down);                                                                                  break;
                case SDLK_DELETE:       nk_input_key(ctx, NK_KEY_DEL, down);                                                                                    break;
                case SDLK_RETURN:       nk_input_key(ctx, NK_KEY_ENTER, down);                                                                                  break;
                case SDLK_TAB:          nk_input_key(ctx, NK_KEY_TAB, down);                                                                                    break;
                case SDLK_BACKSPACE:    nk_input_key(ctx, NK_KEY_BACKSPACE, down);                                                                              break;
                case SDLK_HOME:         nk_input_key(ctx, NK_KEY_TEXT_START, down); nk_input_key(ctx, NK_KEY_SCROLL_START, down);                               break;
                case SDLK_END:          nk_input_key(ctx, NK_KEY_TEXT_END, down); nk_input_key(ctx, NK_KEY_SCROLL_END, down);                                   break;
                case SDLK_PAGEDOWN:     nk_input_key(ctx, NK_KEY_SCROLL_DOWN, down);                                                                            break;
                case SDLK_PAGEUP:       nk_input_key(ctx, NK_KEY_SCROLL_UP, down);                                                                              break;
                case SDLK_z:            nk_input_key(ctx, NK_KEY_TEXT_UNDO, down && state[SDL_SCANCODE_LCTRL]);                                                 break;
                case SDLK_r:            nk_input_key(ctx, NK_KEY_TEXT_REDO, down && state[SDL_SCANCODE_LCTRL]);                                                 break;
                case SDLK_c:            nk_input_key(ctx, NK_KEY_COPY, down && state[SDL_SCANCODE_LCTRL]);                                                      break;
                case SDLK_v:            nk_input_key(ctx, NK_KEY_PASTE, down && state[SDL_SCANCODE_LCTRL]);                                                     break;
                case SDLK_x:            nk_input_key(ctx, NK_KEY_CUT, down && state[SDL_SCANCODE_LCTRL]);                                                       break;
                case SDLK_b:            nk_input_key(ctx, NK_KEY_TEXT_LINE_START, down && state[SDL_SCANCODE_LCTRL]);                                           break;
                case SDLK_e:            nk_input_key(ctx, NK_KEY_TEXT_LINE_END, down && state[SDL_SCANCODE_LCTRL]);                                             break;
                case SDLK_UP:           nk_input_key(ctx, NK_KEY_UP, down);                                                                                     break;
                case SDLK_DOWN:         nk_input_key(ctx, NK_KEY_DOWN, down);                                                                                   break;
                case SDLK_LEFT:         state[SDL_SCANCODE_LCTRL] ? nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, down) : nk_input_key(ctx, NK_KEY_LEFT, down);      break;
                case SDLK_RIGHT:        state[SDL_SCANCODE_LCTRL] ? nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, down) : nk_input_key(ctx, NK_KEY_RIGHT, down);    break;
                default: return 0;
                }
                return 1;
            }
            else if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP)
            {
                /* mouse button */
                if (evt->button.button == SDL_BUTTON_LEFT)
                    nk_input_button(ctx, NK_BUTTON_LEFT, evt->button.x, evt->button.y, static_cast<Int32>(evt->type == SDL_MOUSEBUTTONDOWN));
                if (evt->button.button == SDL_BUTTON_MIDDLE)
                    nk_input_button(ctx, NK_BUTTON_MIDDLE, evt->button.x, evt->button.y, static_cast<Int32>(evt->type == SDL_MOUSEBUTTONDOWN));
                if (evt->button.button == SDL_BUTTON_RIGHT)
                    nk_input_button(ctx, NK_BUTTON_RIGHT, evt->button.x, evt->button.y, static_cast<Int32>(evt->type == SDL_MOUSEBUTTONDOWN));
                else
                    return 0;
                return 1;
            }
            else if (evt->type == SDL_MOUSEMOTION)
            {
                ctx->input.mouse.grabbed ?
                    nk_input_motion(ctx, static_cast<Int32>(ctx->input.mouse.prev.x) + evt->motion.xrel, static_cast<Int32>(ctx->input.mouse.prev.y) + evt->motion.yrel) :
                    nk_input_motion(ctx, evt->motion.x, evt->motion.y);
                return 1;
            }
            else if (evt->type == SDL_TEXTINPUT)
            {
                nk_glyph glyph;
                memcpy(glyph, evt->text.text, NK_UTF_SIZE);
                nk_input_glyph(ctx, glyph);
                return 1;
            }
            else if (evt->type == SDL_MOUSEWHEEL)
            {
                nk_input_scroll(ctx, static_cast<float>(evt->wheel.y));
                return 1;
            }
            return 0;
        }

        void Init(SDL_Window* win)
        {
            m_sdl.window = win;
            nk_init_default(&m_sdl.context, 0);
            m_sdl.context.clip.copy = ClipboardCopy;
            m_sdl.context.clip.paste = ClipboardPaste;
            m_sdl.context.clip.userdata = nk_handle_ptr(nullptr);
            DeviceCreate();
            FontStashBegin();
        }

        void Render(const nk_anti_aliasing AA, const Int32 max_vertex_buffer, const Int32 max_element_buffer)
        {
            sdl_dev* dev = &m_sdl.ogl;
            Int32 width = 0, height = 0;
            Int32 display_width = 0, display_height = 0;
            struct nk_vec2 scale;
            GLfloat ortho[4][4] = {
                { 2.0f, 0.0f, 0.0f, 0.0f },
                { 0.0f, -2.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, -1.0f, 0.0f },
                { -1.0f, 1.0f, 0.0f, 1.0f },
            };
            SDL_GetWindowSize(m_sdl.window, &width, &height);
            SDL_GL_GetDrawableSize(m_sdl.window, &display_width, &display_height);
            ortho[0][0] /= static_cast<float>(width);
            ortho[1][1] /= static_cast<float>(height);

            scale.x = static_cast<float>(display_width) / static_cast<float>(width);
            scale.y = static_cast<float>(display_height) / static_cast<float>(height);

            /* setup global state */
            glViewport(0, 0, display_width, display_height);
            glEnable(GL_BLEND);
            glBlendEquation(GL_FUNC_ADD);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDisable(GL_CULL_FACE);
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_SCISSOR_TEST);
            glActiveTexture(GL_TEXTURE0);

            /* setup program */
            glUseProgram(dev->prog);
            glUniform1i(dev->unifTex, 0);
            glUniformMatrix4fv(dev->unifProj, 1, GL_FALSE, &ortho[0][0]);
            {
                /* convert from command queue into draw list and draw to screen */
                const nk_draw_command* cmd;
                void *vertices = nullptr;
                void* elements = nullptr;
                const nk_draw_index* offset = NULL;

                /* allocate vertex and element buffer */
                //glBindVertexArray(dev->vao);
                glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

                glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

                /* load vertices/elements directly into vertex/element buffer */
                vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
                elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
                {
                    /* fill convert configuration */
                    struct nk_convert_config config;
                    static const struct nk_draw_vertex_layout_element vertex_layout[] = {
                        { NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(sdl_vertex, position) },
                        { NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(sdl_vertex, uv) },
                        { NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(sdl_vertex, col) },
                        { NK_VERTEX_LAYOUT_END }
                    };
                    NK_MEMSET(&config, 0, sizeof(config));
                    config.vertex_layout = vertex_layout;
                    config.vertex_size = sizeof(sdl_vertex);
                    config.vertex_alignment = NK_ALIGNOF(sdl_vertex);
                    config.null = dev->nulTex;
                    config.circle_segment_count = 22;
                    config.curve_segment_count = 22;
                    config.arc_segment_count = 22;
                    config.global_alpha = 1.0f;
                    config.shape_AA = AA;
                    config.line_AA = AA;

                    /* setup buffers to load vertices and elements */
                    {
                        nk_buffer vbuf, ebuf;
                        nk_buffer_init_fixed(&vbuf, vertices, (nk_size)max_vertex_buffer);
                        nk_buffer_init_fixed(&ebuf, elements, (nk_size)max_element_buffer);
                       
                        nk_convert(&m_sdl.context, &dev->cmds, &vbuf, &ebuf, &config);
                    }
                }
                glUnmapBuffer(GL_ARRAY_BUFFER);
                glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

                /* iterate over and execute each draw command */
                nk_draw_foreach(cmd, &m_sdl.context, &dev->cmds)
                {
                    if (!cmd->elem_count) continue;
                    glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
                    glScissor((GLint)(cmd->clip_rect.x * scale.x),
                        (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * scale.y),
                        (GLint)(cmd->clip_rect.w * scale.x),
                        (GLint)(cmd->clip_rect.h * scale.y));
                   glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
                   offset += cmd->elem_count;
                }
                nk_clear(&m_sdl.context);
            }

            glUseProgram(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glDisable(GL_BLEND);
            glDisable(GL_SCISSOR_TEST);
        }

        void UploadAtlas(const void* image, const Int32 width, const Int32 height)
        {
            glGenTextures(1, &m_sdl.ogl.fontTex);
            glBindTexture(GL_TEXTURE_2D, m_sdl.ogl.fontTex);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        }

        ACE_DISABLE_COPY(UIImpl)
    };


    void UI::AddFonts(const std::vector<Font>& fonts)
    {
        UIImpl& impl = UIImpl::GetInstance();

        struct nk_font_config cfg = nk_font_config(0);
        cfg.oversample_h = 3; cfg.oversample_v = 2;

        for (const auto& itr : fonts)
        {
            UInt32 size = 0u;
            const UInt8* data = itr.GetBuffer(size);
            //nk_font* f = nk_font_atlas_add_from_memory(&impl.m_sdl.atlas, (void*)data, size, itr.m_lineHeight, nullptr);
            nk_font* f = nk_font_atlas_add_from_file(&impl.m_sdl.atlas, Path("arial.ttf").GetPath().c_str(), 13.f, &cfg);
            int i = 2 * 3;
            //impl.m_sdl.context.style.font = f;
        }

        impl.FontStashEnd();

    }

    void UI::Draw()
    {
        UIImpl::GetInstance().Render(NK_ANTI_ALIASING_OFF, MAX_VERTEX_MEMORY, MAX_ELEMENT_MEMORY);
    }

    nk_context* UI::GetContext()
    {
        return &UIImpl::GetInstance().m_sdl.context;
    }

    void UI::Init(const Window& win)
    {
        if (win)
        {
            UIImpl::GetInstance().Init((*win)->sdlWindow);
        }
    }

}