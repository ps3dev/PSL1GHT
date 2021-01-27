#ifndef __IRRALLOCATOR_H__
#define __IRRALLOCATOR_H__

#include <new>

namespace irr
{
	namespace core
	{
		template< typename T >
		class allocator
		{
		public:
			T* allocate(u32 cnt)
			{
				return (T*)operator new(cnt*sizeof(T));
			}

			void deallocate(T *ptr)
			{
				operator delete(ptr);
			}

			void construct(T *ptr,const T& elem)
			{
				new ((void*)ptr) T(elem);
			}

			void destruct(T *ptr)
			{
				ptr->~T();
			}
		};

		template< typename T >
		class allocatorRSX
		{
		public:
			T* allocate(u32 cnt)
			{
				return (T*)operator new(cnt*sizeof(T));
			}

			void deallocate(T *ptr)
			{
				operator delete(ptr);
			}

			void construct(T *ptr,const T& elem)
			{
				new ((void*)ptr) T(elem);
			}

			void destruct(T *ptr)
			{
				ptr->~T();
			}

		protected:
			void* operator new(size_t size)
			{
				return rsxMemalign(64,size);
			}

			void operator delete(void *ptr)
			{
				rsxFree(ptr);
			}
		};
	}
}

#endif
