//#include <Ace\File.h>
//#include <SDL_rwops.h>
//
//namespace ace
//{
//	struct File::FileImpl
//	{
//		SDL_RWops* rwops;
//
//		FileImpl(const char* path, const char* mode) : rwops(SDL_RWFromFile(path, mode))
//		{
//
//		}
//
//		~FileImpl()
//		{
//			SDL_RWclose(rwops);
//		}
//	};
//
//	File::File(const Path& path, const char* mode) : m_fileImpl(new File::FileImpl(path.GetPath(), mode))
//	{
//		
//	}
//
//	File::~File()
//	{
//
//	}
//
//}