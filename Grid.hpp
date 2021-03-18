#ifndef INK_UTILITY_GRID_CLASS_HEADER_FILE_GUARD
#define INK_UTILITY_GRID_CLASS_HEADER_FILE_GUARD

#include <cstdint>
#include <numeric>
#include <span>

namespace ink {
	
	template<typename T, size_t _col, size_t _row = _col>
	requires( (_col - _row) <= std::numeric_limits<size_t>::max() && _col >= 1 && _row >= 1 )
	class Grid {
		
		private: enum
		{ COLS = _col, ROWS = _row, SIZE = COLS * ROWS };
		
		
		
		// Number of elements in the Grid
		public: static constexpr size_t
		size()
		{ return SIZE; }
		
		// Number of elements in a column
		public: static constexpr size_t
		sizeCol()
		{ return COLS; }
		
		// Number of elements in a row
		public: static constexpr size_t
		sizeRow()
		{ return ROWS; }
		
		
		
		// Given a column and row index, returns the equivalent theoretical absolute index as in the underlying 1D array.
		public: static constexpr size_t
		IndexAbs(size_t col, size_t row)
		{ return row * COLS + col; }
		
		// Given an absolute index, returns the equivalent theoretical column index.
		public: static constexpr size_t
		IndexCol(size_t abs)
		{ return abs % COLS; }
		
		// Given an absolute index, returns the equivalent theoretical row index.
		public: static constexpr size_t
		IndexRow(size_t abs)
		{ return (abs - IndexCol(abs)) / COLS; }
		
		
		
		// Default constructor
		public: constexpr
		Grid() requires (std::is_default_constructible_v<T>):
		M_data() {}
		
		// Copy constructor
		public: constexpr
		Grid(Grid const& other)
		{ std::copy(other.M_data, other.M_data + SIZE, M_data); }
		
		// Move constructor
		public: constexpr
		Grid(Grid && other)
		{ std::copy(other.M_data, other.M_data + SIZE, M_data); }
		
		// Construct all elements using given arguments
		public: template<typename... ctr> requires( std::constructible_from<T, ctr...> ) explicit constexpr
		Grid(ctr&&... args)
		{ for (size_t i = 0; i < SIZE; i++) M_data[i] = T(std::forward<ctr>(args)...); }
		
		// List constructor. Every N contiguous elements will be part of a row, where N is the number of columns.
		public: constexpr
		Grid(std::initializer_list<T> list)
		{ std::copy(list.begin(), list.begin() + list.size(), M_data); }
		
		// Span constructor. Every N contiguous element will be part of a row, where N is the number of columns.
		public: constexpr
		Grid(std::span<T, SIZE> const& span)
		{ std::copy(span.begin(), span.end(), M_data); }
		
		// 2D Array Constructor. Works as expected.
		public: constexpr
		Grid(const T(&array2D)[COLS][ROWS]) {
			T const* arr2d = static_cast<int const*>(static_cast<void const*>(array2D));
			std::copy(arr2d, arr2d + SIZE, M_data);
		}
		
		private: template<typename _t, bool isCol> struct IterationImpl {
			struct Slice;
			struct SliceIterator;
			struct SliceView;
			
			struct Slice {
				friend class Grid;
				friend struct SliceIterator;
				friend struct SliceView;
				
				private: static constexpr size_t LEN = isCol ? COLS : ROWS;
				
				public: constexpr auto&
				operator[](size_t i)
				{
					if constexpr(isCol)	return (*M_grid)[{M_section, i}];
					if constexpr(!isCol)	return (*M_grid)[{i, M_section}];
				}
				
				public: constexpr Slice(Slice const&) = default;
				public: constexpr Slice(Slice &&) = default;
				private: constexpr Slice(_t* grid, size_t i):
				M_grid(grid), M_section(i) {}
				
				private: struct Iterator;
				
				public: constexpr auto
				begin()
				{ return Iterator(this, 0); }
				
				public: constexpr auto
				end()
				{ return Iterator(this, LEN); }
				
				private: struct Iterator {
					friend struct Slice;
					using value_type = std::remove_reference_t<decltype(Slice(nullptr,0)[0])>;
					using reference = value_type&;
					using pointer = value_type*;
					using difference_type = std::ptrdiff_t;
					using iterator_category = std::bidirectional_iterator_tag;
					
					public: constexpr bool
					operator==(Iterator const& other) const
					{ return M_curr == other.M_curr; }
					
					public: constexpr bool
					operator!=(Iterator const& other) const
					{ return !(*this == other); }
					
					
					
					public: constexpr reference
					operator*()
					{ return (*M_slice)[M_curr]; }
					
					public: constexpr pointer
					operator->()
					{ return &((*M_slice)[M_curr]); }
					
					
					
					public: constexpr Iterator
					operator+(difference_type i)
					{ return Iterator(M_slice, M_curr + i); }
					
					public: constexpr Iterator
					operator-(difference_type i)
					{ return (*this) + (-i); }
					
					public: constexpr Iterator
					operator++(int)
					{ return Iterator(M_slice, M_curr++); }
					
					public: constexpr Iterator&
					operator++()
					{ M_curr++; return *this; }
					
					public: constexpr Iterator
					operator--(int)
					{ return Iterator(M_slice, M_curr--); }
					
					public: constexpr Iterator&
					operator--()
					{ M_curr--; return *this; }
					
					public: constexpr Iterator&
					operator+=(difference_type i)
					{ M_curr += i; return *this; }
					
					public: constexpr Iterator&
					operator-=(difference_type i)
					{ M_curr -= i; return *this; }
					
					
					
					private: Iterator(Slice* slice, size_t i):
					M_slice(slice), M_curr(i) {}
					
					private: Slice*
					M_slice{nullptr};
					
					private: size_t
					M_curr = 0;
					
				};
				private: _t*
				M_grid{nullptr};
				
				private: size_t
				M_section{0};
				
			};
				
			struct SliceIterator {
				friend class Grid;
				friend struct SliceView;
				
				using value_type = Slice;
				using reference = value_type&;
				using pointer = value_type*;
				using difference_type = std::ptrdiff_t;
				using iterator_category = std::bidirectional_iterator_tag;
				
				public: constexpr bool
				operator==(SliceIterator const& other) const
				{ return M_slice.M_section == other.M_slice.M_section; }
				
				public: constexpr bool
				operator!=(SliceIterator const& other) const
				{ return !(*this == other); }
				
				public: constexpr reference
				operator*()
				{ return M_slice; }
				
				public: constexpr pointer
				operator->()
				{ return &M_slice; }
				
				public: constexpr auto
				operator+(difference_type i)
				{ return SliceIterator(M_slice.M_grid, M_slice.M_section + i); }
				
				public: constexpr auto
				operator-(difference_type i)
				{ return (*this) + (-i); }
				
				public: constexpr auto
				operator++(int)
				{ return SliceIterator(M_slice.M_grid, M_slice.M_section++); }
				
				public: constexpr auto&
				operator++()
				{ M_slice.M_section++; return *this; }
				
				public: constexpr auto
				operator--(int)
				{ return SliceIterator(M_slice.M_grid, M_slice.M_section--); }
				
				public: constexpr auto&
				operator--()
				{ M_slice.M_section--; return *this; }
				
				public: constexpr auto&
				operator+=(difference_type i)
				{ M_slice.M_section += i; return *this; }
				
				public: constexpr auto&
				operator-=(difference_type i)
				{ M_slice.M_section -= i; return *this; }
				
				private: constexpr
				SliceIterator(_t* grid, size_t i):
				M_slice(grid, i) {}
				
				private: Slice
				M_slice;
			};
			
			struct SliceView {
				friend class Grid;
				
				private: static constexpr size_t LEN = isCol ? COLS : ROWS;
				
				public: constexpr auto
				begin()
				{ return SliceIterator(M_grid, 0); }
				
				public: constexpr auto
				end()
				{ return SliceIterator(M_grid, LEN); }
				
				public: constexpr auto
				rbegin()
				{ return end() - 1; }
				
				public: constexpr auto
				rend()
				{ return begin() - 1; }
				
				private: constexpr
				SliceView(_t* grid):
				M_grid(grid) {}
				
				private: _t*
				M_grid;
			};
			
		};
		
		private: template<typename _t, bool isCol> using
		Slice = IterationImpl<_t, isCol>::Slice;
		
		private: template<typename _t, bool isCol> using
		SliceView = IterationImpl<_t, isCol>::SliceView;
		
		public: constexpr auto
		ColumnView()
		{ return SliceView<std::remove_reference_t<decltype(*this)> , true>(this); }
		
		public: constexpr auto
		ColumnView() const
		{ return SliceView<std::remove_reference_t<decltype(*this)> , true>(this); }
		
		public: constexpr auto
		RowView()
		{ return SliceView<std::remove_reference_t<decltype(*this)> , false>(this); }
		
		public: constexpr auto
		RowView() const
		{ return SliceView<std::remove_reference_t<decltype(*this)> , false>(this); }
		
		// Iterate over the underlying array sequentially. Use the begin(Row/Col) and end(Row/Col) to iterate over slices,
		// which can be further iterated over themselves.
		public: constexpr auto
		begin()
		{ return M_data; }
		
		// Iterate over the underlying array sequentially. Use the begin(Row/Col) and end(Row/Col) to iterate over slices,
		// which can be further iterated over themselves.
		public: constexpr auto
		end() const
		{ return M_data + (SIZE); }
		
		// Iterate over the underlying array sequentially. Use the begin(Row/Col) and end(Row/Col) to iterate over slices,
		// which can be further iterated over themselves.
		public: constexpr auto
		rbegin()
		{ return M_data + (SIZE - 1); }
		
		// Iterate over the underlying array sequentially. Use the begin(Row/Col) and end(Row/Col) to iterate over slices,
		// which can be further iterated over themselves.
		public: constexpr auto
		rend() const
		{ return M_data - 1; }
		
		// Get column number 'i'.
		public: constexpr auto
		GetCol(size_t i)
		{ return Slice<std::remove_reference_t<decltype(*this)>, true>(this, i); }
		
		// Get column number 'i'.
		public: constexpr auto
		GetCol(size_t i) const
		{ return Slice<std::remove_reference_t<decltype(*this)>, true>(this, i); }
		
		// Get row number 'i'.
		public: constexpr auto
		GetRow(size_t i)
		{ return Slice<std::remove_reference_t<decltype(*this)>, false>(this, i); }
		
		// Get row number 'i'
		public: constexpr auto
		GetRow(size_t i) const
		{ return Slice<std::remove_reference_t<decltype(*this)>, false>(this, i); }
		
		// Get element at positional index of the form {column, row}.
		public: constexpr auto&
		operator[](std::initializer_list<size_t> col_row)
		{ return M_data[IndexAbs( col_row.begin()[0] , col_row.begin()[1] )]; }
		
		// Get element at positional index of the form {column, row}.
		public: constexpr auto&
		operator[](std::initializer_list<size_t> col_row) const
		{ return M_data[IndexAbs( col_row.begin()[0] , col_row.begin()[1] )]; }
		
		// Get element at the absolute index number 'i'.
		// Use 'GetRow()' or 'GetCol()' to get an indexable slice to a Row or Column, respectively.
		public: constexpr auto&
		operator[](size_t i)
		{ return M_data[i]; }
		
		// Get element at the absolute index number 'i'.
		public: constexpr auto&
		operator[](size_t i) const
		{ return M_data[i]; }
		
		private: T
		M_data[SIZE];
		
	};
	
}


#endif
