#ifndef INK_TMP_REBIND_UTILITY_HEADER_FILE_GUARD
#define INK_TMP_REBIND_UTILITY_HEADER_FILE_GUARD

#include <utility>
#include <tuple>
#include <type_traits>

namespace ink::rebind {
	
	namespace detail {
		
		template<typename...>	struct type_list;
		template<auto...>		struct value_list;
		
		
		
		template<typename> struct pack_list_impl;
		template<typename T> using
		unpack = typename pack_list_impl<T>::type;
		
		
		
		template<typename, typename> struct concat_impl;
		template<typename T, typename U> using
		concat = typename concat_impl<T,U>::type;
		
		
		
		template<typename... T> struct
		type_list {
			private:
			
			template<typename> struct repack_impl;
			template<template<typename...> typename C, typename... c>
			struct repack_impl<C<c...>>
			{ using type = C<T...>; };
			
			public:
			
			template<template<typename...> typename B> using
			pack = B<T...>;
			
			template<typename C> using
			repack = typename repack_impl<C>::type;
			
			
			using
			tuple = std::tuple<T...>;
			
			template<size_t n> using
			get = std::tuple_element_t<n, std::tuple<T...>>;
			
			template<typename U> using
			concat = concat<type_list,U>;
			
			template<template<typename> typename Trans> using
			transform = type_list<Trans<T>...>;
			
			// Number of types in the type list
			static constexpr auto
			size = sizeof...(T);
			
		};
		
		template<auto... T> struct
		value_list {
			private:
			
			template<typename> struct repack_impl;
			template<template<auto...> typename C, auto... c>
			struct repack_impl<C<c...>>
			{ using type = C<T...>; };
			
			public:
			
			template<template<auto...> typename B> using
			pack = B<T...>;
			
			template<typename C> using
			repack = typename repack_impl<C>::type;
			
			static constexpr auto
			tuple = std::make_tuple(T...);
			
			template<size_t n> static constexpr auto
			get = std::get<n>( tuple );
			
			template<typename U> using
			concat = concat<value_list,U>;
			
			// Number of values in the value list
			static constexpr auto
			size = sizeof...(T);
			
			using
			type_list = type_list<decltype(T)...>;
			
		};
		
		
		
		template<typename> struct pack_list_impl;
		
		template<template<typename...> typename A, typename... T> struct
		pack_list_impl<A<T...>>
		{ using type = type_list<T...>; };
		
		template<template<auto...> typename A, auto... V> struct
		pack_list_impl<A<V...>>
		{ using type = value_list<V...>; };
		
		template<typename T, T... I> struct
		pack_list_impl<std::integer_sequence<T, I...>>
		{ using type = value_list<I...>; };
		
		template<auto... V> struct
		pack_list_impl<value_list<V...>>
		{ using type = type_list<decltype(V)...>; };
		
		
		
		
		
		template<typename... a, typename... b> struct
		concat_impl<type_list<a...>,type_list<b...>>
		{ using type = type_list<a...,b...>; };
		
		template<auto... a, auto... b> struct
		concat_impl<value_list<a...>,value_list<b...>>
		{ using type = value_list<a...,b...>; };
		
		
		
	}
	
	
	using detail::type_list;
	using detail::value_list;
	
	using detail::concat;
	using detail::unpack;
	
	
	
}

#endif
