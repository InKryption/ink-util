#ifndef INK_UTILITY_GRID_CLASS_HEADER_FILE_GUARD
#define INK_UTILITY_GRID_CLASS_HEADER_FILE_GUARD

#include <numeric>
#include <span>

namespace ink {
	
	template<typename T, size_t _col, size_t _row = _col>
	requires( (_col - _row) <= std::numeric_limits<size_t>::max() && _col >= 1 && _row >= 1 )
	class Grid {
		
		private: enum
		{ COLS = _col, ROWS = _row, SIZE = COLS * ROWS };
		
		
		
		// Number of elements in the Grid
		public: static constexpr size_t size()
		{ return SIZE; }
		
		// Number of elements in a column
		public: static constexpr size_t size_col()
		{ return COLS; }
		
		// Number of elements in a row
		public: static constexpr size_t size_row()
		{ return ROWS; }
		
		
		
		// Given a column and row index, returns the equivalent theoretical absolute index as in the underlying 1D array.
		public: static constexpr size_t Index_Absolute(size_t col, size_t row)
		{ return row * COLS + col; }
		
		// Given an absolute index, returns the equivalent theoretical column index.
		public: static constexpr size_t Index_Column(size_t abs)
		{ return abs % COLS; }
		
		// Given an absolute index, returns the equivalent theoretical row index.
		public: static constexpr size_t Index_Row(size_t abs)
		{ return (abs - Index_Column(abs)) / COLS; }
		
		
		
		// Default constructor
		public: constexpr Grid() requires (std::is_default_constructible_v<T>):
		M_data({}) {}
		
		public: constexpr Grid(Grid const& other)
		{ std::copy(other.M_data, other.M_data + SIZE, M_data); }
		
		public: constexpr Grid(Grid && other)
		{ std::copy(other.M_data, other.M_data + SIZE, M_data); }
		
		// Construct all elements using given arguments
		public: template<typename... ctr> requires( std::constructible_from<T, ctr...> ) explicit constexpr Grid(ctr&&... args)
		{ for (size_t i = 0; i < SIZE; i++) M_data[i] = T(std::forward<ctr>(args)...); }
		
		// List constructor. Every N contiguous elements will be part of a row, where N is the number of columns.
		public: constexpr Grid(std::initializer_list<T> list)
		{ std::copy(list.begin(), list.begin() + list.size(), M_data); }
		
		// 2D List constructor. Every inner list is a row.
		public: constexpr Grid(std::initializer_list<std::initializer_list<T>> list)
		{  }
		
		// Span constructor. Every N contiguous element will be part of a row, where N is the number of columns.
		public: constexpr Grid(std::span<T, SIZE> const& span)
		{ std::copy(span.begin(), span.end(), M_data); }
		
		// 2D Array Constructor. Works as expected.
		public: constexpr Grid(const T(&array2D)[COLS][ROWS]) {
			T const* arr2d = static_cast<int const*>(static_cast<void const*>(array2D));
			std::copy(arr2d, arr2d + SIZE, M_data);
		}
		
		
		
		public: constexpr auto& operator[](std::initializer_list<size_t> col_row)
		{ return M_data[Index_Absolute(col_row.begin()[0],col_row.begin()[1])]; }
		
		public: constexpr auto& operator[](std::initializer_list<size_t> col_row) const
		{ return M_data[Index_Absolute(col_row.begin()[0],col_row.begin()[1])]; }
		
		private: T
		M_data[SIZE];
		
	};
	
}


#endif
