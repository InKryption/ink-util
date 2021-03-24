#ifndef INK_TMP_REBIND_UTILITY_HEADER_FILE_GUARD
#define INK_TMP_REBIND_UTILITY_HEADER_FILE_GUARD

#include <utility>
#include <tuple>
#include <type_traits>

namespace ink::rebind {
	
	namespace detail {
		
		/* Helpers */
		
		template<typename T> using
		Invoke = typename T::type;
		
		/* Forward declarations */
		
		template<typename... Types> struct
		type_list_impl;
		
		template<auto... Values> struct
		value_list_impl;
		
		template<typename Concrete> struct
		pack_impl;
		
		template<typename TypeList, template<typename...> typename Template> struct
		unpack_impl;
		
		template<typename ValueList, template<auto...> typename Template> struct
		unpack_values_impl;
		
		template<typename TypeList, typename Concrete> struct
		emplace_impl;
		
		template<typename TypeList1, typename TypeList2> struct
		concat_impl;
		
		template<size_t TypeIndex, typename TypeList> struct
		get_type_impl;
		
		template<size_t TypeIndex> struct
		what_is_impl;
		
		template<typename TypeList> struct
		reverse_impl;
		
		template<typename TypeList> struct
		pop_first_impl;
		
		template<typename TypeList, template<typename> typename Trans> struct
		transform_impl;
		
		template<size_t N> struct
		repeat_impl;
		
		
		
		/* Aliases */
		
		template<typename... T> using
		type_list = Invoke< type_list_impl<T...> >;
		
		template<auto... V> using
		value_list = Invoke< value_list_impl<V...> >;
		
		template<typename Concrete> using
		pack = Invoke< pack_impl<Concrete> >;
		
		template<typename TypeList, template<typename...> typename Template> using
		unpack = Invoke< unpack_impl<TypeList, Template> >;
		
		template<typename ValueList, template<auto...> typename Template> using
		unpack_values = Invoke< unpack_values_impl<ValueList, Template> >;
		
		template<typename TypeList, typename Concrete> using
		emplace = Invoke< emplace_impl<TypeList, Concrete> >;
		
		template<typename TypeList1, typename TypeList2> using
		concat = Invoke< concat_impl<TypeList1, TypeList2> >;
		
		template<size_t TypeIndex, typename TypeList> using
		get_type = Invoke< get_type_impl<TypeIndex, TypeList> >;
		
		template<size_t TypeIndex> using
		what_is = Invoke< what_is_impl<TypeIndex> >;
		
		template<typename TypeList> using
		reverse = Invoke< reverse_impl<TypeList> >;
		
		template<typename TypeList> using
		pop_first = Invoke< pop_first_impl<TypeList> >;
		
		template<typename TypeList> using
		pop_last = reverse<pop_first<reverse<TypeList>>>;
		
		template<typename TypeList, template<typename> typename Trans> using
		transform = Invoke< transform_impl<TypeList,Trans> >;
		
		template<size_t N> using
		repeat = Invoke< repeat_impl<N> >;
		
		
		
		/* Implementations */
		
		/* type_list */
			// Main usage
			template<typename... T> struct
			type_list_impl {
				
				using
				type = type_list_impl;
				
				template<template<typename...> typename Template> using
				unpack = unpack<type, Template>;
				
				template<typename Concrete> using
				emplace = emplace<type, Concrete>;
				
				template<typename rhs_TypeList> using
				concat = concat<type, rhs_TypeList>;
				
				template<size_t TypeIndex> using
				get = typename what_is<TypeIndex>::in<type>;
				
				using
				reverse = reverse<type>;
				
				using
				pop_first = pop_first<type>;
				
				using
				pop_last = pop_last<type>;
				
			};
			
			// Specialize for flattening; type_list<type_list<...>> will evaluate to type_list<...>
			template<typename... types> struct
			type_list_impl<type_list_impl<types...>>
			{ using type = type_list<types...>; };
		
			// Specialize for empty case
			template<> struct
			type_list_impl<>
			{ using type = type_list_impl; };
			
		/* value_list */
			// Main usage
			template<auto... V> struct
			value_list_impl {
				using type = value_list_impl;
				
				using type_list = type_list<decltype(V)...>;
			};
			
		/* pack */
			// Main usage
			template<template<typename...> typename Template, typename... types> struct
			pack_impl<Template<types...>>
			{ using type = type_list<types...>; };
			
			// Main value usage
			template<template<auto...> typename Template, auto... values> struct
			pack_impl<Template<values...>>
			{ using type = value_list<values...>; };
			
			// Index sequence specialization
			template<typename Integer, size_t... I> struct
			pack_impl<std::integer_sequence<Integer, I...>>
			{ using type = value_list<I...>; };
			
		/* unpack */
			// Main usage
			template<typename... types, template<typename...> typename Template> struct
			unpack_impl<type_list_impl<types...>, Template>
			{ using type = Template<types...>; };
			
		/* unpack_values */
			// Main usage
			template<auto... values, template<auto...> typename Template> struct
			unpack_values_impl<value_list_impl<values...>, Template>
			{ using type = Template<values...>; };
			
		/* emplace */
			// Main usage
			template<template<typename...> typename Concrete, typename... types, typename... discard> struct
			emplace_impl<type_list_impl<types...>, Concrete<discard...>>
			{ using type = Concrete<types...>; };
			
			// Main value usage
			template<template<auto...> typename Concrete, auto... values, auto... discard> struct
			emplace_impl<value_list_impl<values...>, Concrete<discard...>>
			{ using type = Concrete<values...>; };
			
			// Secondary value usage
			template<template<typename...> typename Concrete, auto... vals, typename... discard> struct
			emplace_impl<value_list_impl<vals...>, Concrete<discard...>>
			{ using type = Concrete<decltype(vals)...>; };
			
		/* concat */
			// Main usage
			template<typename... types1, typename... types2> struct
			concat_impl<type_list_impl<types1...>, type_list_impl<types2...>>
			{ using type = type_list<types1..., types2...>; };
			
		/* get_type */
			// This is "main usage", but is more comfortable to use from within what_is, for packing and unpacking syntax
			template<size_t TypeIndex, typename... types> struct
			get_type_impl<TypeIndex, type_list_impl<types...>>
			{ using type = std::tuple_element_t<TypeIndex, std::tuple<types...> >; };
			
		/* what_is */
			// Main usage
			template<size_t TypeIndex> struct
			what_is_impl
			{ using type = what_is_impl; template<typename TypeList> using in = get_type<TypeIndex, TypeList>; };
			
		/* reverse */
			// Main usage
			template<typename T, typename... Ts> struct
			reverse_impl<type_list_impl<T, Ts...>>
			{
				using head = type_list<T>;
				using tail = Invoke< reverse_impl<type_list<Ts...>> >;
				
				using type = concat<tail, head>;
				// using type = decltype(std::tuple_cat(std::declval<tail>(), std::declval<head>()));
			};
			
			// Empty specialization
			template<>
			struct reverse_impl<type_list<>>
			{ using type = type_list<>; };
			
		/* pop_first */
			// Main usage
			template<typename First, typename... types> struct
			pop_first_impl<type_list_impl<First,types...>>
			{ using type = type_list<types...>; };
			
		/* transform */
			// Main usage
			template<typename... types, template<typename> typename Trans> struct
			transform_impl<type_list_impl<types...>,Trans>
			{ using type = type_list<Trans<types>...>; };
			
		/* repeat */
			
			template<typename T> struct helper_make
			{ template<typename U> using from = T; };
			
			template<size_t N, typename T> struct repeat_n_times_impl
			{
				using type = typename pack<std::make_index_sequence<N>>::type_list::transform<typename helper_make<T>::from>;
			};
			
			// Main usage
			template<size_t N> struct
			repeat_impl
			{
				
			};
		
	}
	
	// Exposed API
	
	using detail::type_list;
	using detail::value_list;
	
	using detail::pack;
	using detail::emplace;
	using detail::unpack;
	using detail::unpack_values;
	
	using detail::concat;
	using detail::reverse;
	using detail::transform;
	
	using detail::pop_first;
	using detail::pop_last;
	
	using detail::get_type;
	using detail::what_is;
	
	typename detail::repeat_n_times_impl<5, int>::type f;
		
}

#endif
