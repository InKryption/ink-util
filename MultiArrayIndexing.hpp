#ifndef INK_MUTLI_ARRAY_INDEXING_HEADER_FILE_GUARD
#define INK_MUTLI_ARRAY_INDEXING_HEADER_FILE_GUARD

	#include <cstddef>
	
	/**
	 *	@param dimension_sizes Sizes corresponding to the sizes of the arrays to sub-arrays in-order.
	 *	@param indexes Indexes corresponding to the absolute index of interest.
	 *	@return The absolute index corresponding to the supplied indexes based off of the dimension_sizes.
	 */
	template<size_t... dimension_sizes> static constexpr auto
	TransposeToAbsolute(decltype(dimension_sizes)... indexes)
	{
		
		size_t	out = 0,
				factor = 1;
		((	out += (indexes * factor)	,	factor *= dimension_sizes	),...);
		return out;
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
	
#endif
