#pragma once



namespace ace
{
    class Scene;
    class SpriteManagerImpl;

    class SpriteManager
    {

        SpriteManagerImpl* m_impl;

        SpriteManager();

        ~SpriteManager();

    public:

        static SpriteManager& GetInstance();

        static void Draw(const Scene& scene);

    };



    namespace detail
    {

        template <typename FWIter, typename Type>
        void ACE_iota(FWIter first, FWIter last, Type value)
        {
            while (first != last)
            {
                *first++ = value;
                ++value;
            }
        }

    }

}