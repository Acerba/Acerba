#pragma once

#include <memory>

namespace ace
{
	template <typename Impl>
	class GraphicsObject
	{
	public:
		typedef void(*DestructorFunc)(Impl*);

	private:

		static DestructorFunc s_destructor;

		inline void HandleInit() const
		{
			if (!m_impl)
			{
				Init();
			}
		}

	protected:

		mutable std::shared_ptr<Impl> m_impl;

		inline void InitImpl(const GraphicsObject& obj) const
		{
			m_impl.swap(obj.m_impl);
		}

		virtual void Init() const = 0;

	public:

		typedef GraphicsObject Graphics;

		GraphicsObject(Impl* impl) : m_impl(impl, s_destructor)
		{

		}

		~GraphicsObject()
		{

		}

		inline Impl* operator->()
		{
			HandleInit();
			return m_impl.get();
		}

		inline const Impl* operator->() const
		{
			HandleInit();
			return m_impl.get();
		}

		inline operator Impl*()
		{
			return m_impl.get();
		}

		inline const Impl* operator*() const
		{
			return m_impl.get();
		}

		inline Impl* operator*()
		{
			return m_impl.get();
		}

		inline operator bool() const
		{
			return m_impl.operator bool();
		}
	};
}