#ifndef INK_MUTLI_ARRAY_INDEXING_HEADER_FILE_GUARD
#define INK_MUTLI_ARRAY_INDEXING_HEADER_FILE_GUARD

#include <cstddef>
#include <concepts>
#include <tuple>

namespace ink {	
	namespace detail::MultiArrayIndexing {
		
		template<typename... i> struct Transpose_impl {
			
		};
	
	}
	
	/**
	 * @brief This functions generates and returns a (constexpr) lambda based off of the specified dimensions,
	 * which itself is a function which generates an absolute index from multiple dimensional indexes.
	 * 
	 * @param dimension_sizes
	 * These parameters (this parameter pack) represent the dimensions of, e.g., an array (albeit in reverse order).
	 * So the correspondence would be of the form | int[HEIGHT,WIDTH] | f(WIDTH,HEIGHT) |.
	 * 
	 * @return The returned lambda takes the same number of arguments as dimension_sizes supplied to the generator function.
	 * Each index argument represents an index into the dimension of the same position, and as such is expected to
	 * be less than the size of said dimension (index is out-of-bounds otherwise).
	 * 
	 * i.e. take a 2D array | int arr2D[2][3] |, and map it to a 1D array | int arr1D[6] |.
	 * The mapping using this function would be like of the form | auto lambda = f(3,2); arr2D[1,2] == arr1D[lambda(1,2)] |.
	 * 
	 */
	template<typename... i> requires(std::convertible_to<i,size_t>&&...) static constexpr auto
	TransposeToAbsolute(i... dimension_sizes)
	{
		return [=](i... indexes) constexpr {
			size_t	out = 0,
					factor = 1;
			((	out += (indexes * factor)	,	factor *= dimension_sizes	),...);
			return out;
		};
	}
	
	/**
	 *	@param dimension_sizes Sizes corresponding to the sizes of the arrays to sub-arrays in-order.
	 *	@param i The index of interest.
	 *	@return A tuple of indexes corresponding to the absolute index supplied, in the same order as dimension_sizes.
	 */
	template<size_t... dimension_sizes> static constexpr auto
	TransposeFromAbsolute(size_t i)
	{
		size_t factor = 1;
		
		auto lambda = [&](size_t dimen_size) constexpr
		{ size_t out = (i / factor) - dimen_size * (i / (factor * dimen_size)); factor *= dimen_size; return out; };
		
		return std::make_tuple( lambda(dimension_sizes)... );
	}
}

#endif
