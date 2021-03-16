#ifndef __IRRARRAY_H__
#define __IRRARRAY_H__

#include "irrallocator.h"

namespace irr
{
	namespace core
	{
		template< class T, typename TAlloc = allocator<T> >
		class array
		{
		public:
			array() : data(NULL),used(0),allocated(0),free_it(true),is_sorted(true) {};
			array(u32 start_cnt) : data(NULL),used(0),allocated(0),free_it(true),is_sorted(true)
			{
				reallocate(start_cnt);
			}
			array(const array<T,TAlloc>& other) : data(NULL)
			{
				*this = other;
			}

			~array()
			{
				u32 i;

				if(free_it==true) {
					for(i=0;i<used;i++) allocator.destruct(&data[i]);
					allocator.deallocate(data);
				}
			}

			void reallocate(u32 new_cnt)
			{
				u32 i,j,end;
				T *old_data = data;

				data = allocator.allocate(new_cnt);
				allocated = new_cnt;

				end = used<new_cnt ? used : new_cnt;
				for(i=0;i<end;i++) allocator.construct(&data[i],old_data[i]);
				for(j=0;j<used;j++) allocator.destruct(&old_data[j]);

				if(allocated<used) used = allocated;
				allocator.deallocate(old_data);
			}

			void push_back(const T& elem)
			{
				if((used + 1)>allocated) {
					T e(elem);
					u32 newAlloc;

					newAlloc = used + 1 + (allocated<500 ? (allocated<5 ? 5 : used) : used>>2);

					reallocate(newAlloc);
					allocator.construct(&data[used++],e);
				} else
					allocator.construct(&data[used++],elem);

				is_sorted = false;
			}

			void push_front(const T& elem)
			{
				insert(elem);
			}

			void insert(const T& elem,u32 index = 0)
			{
				u32 i;

				if(index>used) return;
				if((used + 1)>allocated) reallocate(used + 1);

				for(i=used;i>index;i--) {
					if(i<used) allocator.destruct(&data[i]);
					allocator.construct(&data[i],data[i - 1]);
				}
				if(used>index) allocator.destruct(&data[index]);

				allocator.construct(&data[index],elem);
				is_sorted = false;
				used++;
			}

			void erase(u32 index)
			{
				u32 i;

				for(i=index+1;i<used;i++) {
					allocator.destruct(&data[i-1]);
					allocator.construct(&data[i-1],data[i]);
				}
				allocator.destruct(&data[used-1]);
				used--;
			}

			T& operator [](u32 index)
			{
				return data[index];
			}

			const T& operator [](u32 index) const
			{
				return data[index];
			}

			T* pointer()
			{
				return data;
			}

			const T* const_pointer() const
			{
				return data;
			}

			u32 size() const
			{
				return used;
			}

			bool empty() const
			{
				return used==0;
			}

			void set_used(u32 new_cnt)
			{
				if(new_cnt>used) reallocate(new_cnt);
				used = new_cnt;
			}

			void clear()
			{
				u32 i;

				for(i=0;i<used;i++) allocator.destruct(&data[i]);
				allocator.deallocate(data);

				data = NULL;
				used = 0;
				allocated = 0;
				is_sorted = true;
			}

		private:
			T *data;
			TAlloc allocator;
			u32 used;
			u32 allocated;
			bool free_it;
			bool is_sorted;

		};
	}
}

#endif
