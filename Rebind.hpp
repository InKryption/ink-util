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
		
		template<typename List> struct
		unpack_impl;
		
		template<typename List> struct
		emplace_impl;
		
		template<typename TypeList1> struct
		concat_impl;
		
		template<size_t TypeIndex> struct
		what_is_impl;
		
		template<typename TypeList> struct
		reverse_impl;
		
		template<typename TypeList> struct
		pop_first_impl;
		
		template<typename List> struct
		transform_impl;
		
		template<typename Type> struct
		repeat_impl;
		
		template<typename... Types> struct
		size_impl;
		
		
		
		/* Aliases */
		
		template<typename... T> using
		type_list = Invoke< type_list_impl<T...> >;
		
		template<auto... V> using
		value_list = Invoke< value_list_impl<V...> >;
		
		template<typename Concrete> using
		pack = Invoke< pack_impl<Concrete> >;
		
		template<typename List> using
		unpack = Invoke< unpack_impl<List> >;
		
		template<typename List> using
		emplace = Invoke< emplace_impl<List> >;
		
		template<typename List> using
		concat = Invoke< concat_impl<List> >;
		
		template<size_t TypeIndex> using
		what_is = Invoke< what_is_impl<TypeIndex> >;
		
		template<typename TypeList> using
		reverse = Invoke< reverse_impl<TypeList> >;
		
		template<typename TypeList> using
		pop_first = Invoke< pop_first_impl<TypeList> >;
		
		template<typename TypeList> using
		pop_last = reverse<pop_first<reverse<TypeList>>>;
		
		template<typename List> using
		transform = Invoke< transform_impl<List> >;
		
		template<typename Type> using
		repeat = Invoke< repeat_impl<Type> >;
		
		template<typename... Types> using
		size_of = Invoke< size_impl<Types...> >;
		
		
		
		/* Implementations */
		
		/* type_list */
			// Main usage
			template<typename... T> struct
			type_list_impl {
				
				using
				type = type_list_impl;
				
				template<template<typename...> typename Template> using
				unpack_into = typename unpack<type>::into<Template>;
				
				template<typename Concrete> using
				emplace_onto = typename emplace<type>::onto<Concrete>;
				
				template<typename TypeList> using
				concat_with = typename concat<type>::with<TypeList>;
				
				template<size_t TypeIndex> using
				get = typename what_is<TypeIndex>::in<type>;
				
				using
				reverse = reverse<type>;
				
				using
				pop_first = pop_first<type>;
				
				using
				pop_last = pop_last<type>;
				
				template<template<typename> typename Trans> using
				transform_with = typename transform<type>::with<Trans>;
				
				static constexpr auto
				size = size_of<type>::value;
				
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
				
				template<template<auto...> typename Template> using
				unpack_into = typename unpack<type>::into<Template>;
				
				template<typename Concrete> using
				emplace_onto = typename emplace<type>::onto<Concrete>;
				
				template<size_t ValueIndex> using
				get = typename what_is<ValueIndex>::in<type>;
				
				static constexpr auto
				size = size_of<type>::value;
				
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
			// Type unpacking
			template<typename... types> struct
			unpack_impl<type_list_impl<types...>>
			{
				using type = unpack_impl;
				template<template<typename...> typename Template> using
				into = Template<types...>;
			};
			
			// Value unpacking
			template<auto... values> struct
			unpack_impl<value_list_impl<values...>>
			{
				using type = unpack_impl;
				template<template<auto...> typename Template> using
				into = Template<values...>;
			};
			
		/* emplace */
			// Type emplacement
			template<typename... types> struct
			emplace_impl<type_list_impl<types...>>
			{
				private:
				template<typename Concrete> struct onto_impl;
				template<template<typename...> typename Template, typename... discard> struct
				onto_impl<Template<discard...> >
				{ using type = Template<types...>; };
				
				public:
				using type = emplace_impl;
				
				template<typename Concrete> using
				onto = Invoke< onto_impl<Concrete> >;
			};
			
			// Value emplacement
			template<auto... values> struct
			emplace_impl<value_list_impl<values...>>
			{
				private:
				template<typename Concrete> struct onto_impl;
				template<template<auto...> typename Template, auto... discard> struct
				onto_impl<Template<discard...> >
				{ using type = Template<values...>; };
				
				public:
				using type = emplace_impl;
				
				template<typename Concrete> using
				onto = Invoke< onto_impl<Concrete> >;
				
			};
			
		/* concat */
			// Types first concatenation
			template<typename... types1> struct
			concat_impl<type_list_impl<types1...>>
			{
				private:
				template<typename List> struct with_impl;
				template<typename... types2> struct
				with_impl<type_list_impl<types2...>>
				{ using type = type_list<types1..., types2...>; };
				
				template<auto... values2> struct
				with_impl<value_list_impl<values2...>>
				{ using type = value_list<(types1())..., (values2)...>; };
				
				public:
				using type = concat_impl;
				template<typename List> using
				with = Invoke< with_impl<List> >;
				
			};
			
			// Values first concatenation
			template<auto... values1> struct
			concat_impl<value_list_impl<(values1)...>>
			{
				private:
				template<typename List> struct with_impl;
				template<typename... types2> struct
				with_impl<type_list_impl<types2...>>
				{ using type = value_list<(values1)..., (types2())...>; };
				
				template<auto... values2> struct
				with_impl<value_list_impl<(values2)...>>
				{ using type = value_list<(values1)..., (values2)...>; };
				
				public:
				using type = concat_impl;
				template<typename List> using
				with = Invoke< with_impl<List> >;
				
			};
			
		/* get_type */
			template<size_t TypeIndex, typename TypeList> struct
			get_type_impl;
			// This is "main usage", but is more comfortable to use from within what_is, for packing and unpacking syntax
			template<size_t TypeIndex, typename... types> struct
			get_type_impl<TypeIndex, type_list_impl<types...>>
			{ using type = std::tuple_element_t<TypeIndex, std::tuple<types...> >; };
			
		/* what_is */
			// Main usage
			template<size_t TypeIndex> struct
			what_is_impl
			{
				private:
				template<typename List> struct in_impl;
				
				template<typename... types>
				struct in_impl<type_list_impl<types...>>
				{ using type = std::tuple_element_t<TypeIndex, std::tuple<types...>>; };
				
				template<auto val> struct Value
				{ static constexpr auto value = val; };
				
				template<auto... values>
				struct in_impl<value_list_impl<(values)...>>
				{ using type = Value<std::get<TypeIndex>(std::make_tuple(values...))>; };
				
				public:
				using type = what_is_impl;
				template<typename List> using
				in = Invoke< in_impl<List> >;
				
			};
			
		/* reverse */
			// Main usage
			template<typename T, typename... Ts> struct
			reverse_impl<type_list_impl<T, Ts...>>
			{
				using head = type_list<T>;
				using tail = Invoke< reverse_impl<type_list<Ts...>> >;
				
				using type = typename concat<tail>::with<head>;
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
			template<typename... types> struct
			transform_impl<type_list_impl<types...>>
			{
				public:
				template<template<typename> typename Trans> using
				with = type_list<Trans<types>...>;
				
				using type = transform_impl;
			};
			
		/* repeat */
			
			template<size_t N, typename T> struct repeat_n_times_impl
			{
				template<typename> using make = T;
				using type = typename pack<std::make_index_sequence<N>>::type_list::transform<make>;
			};
			
			// Main usage
			template<typename Type> struct
			repeat_impl
			{
				private: template<typename> using
				make = Type;
				
				public:
				using type = repeat_impl;
				
				template<size_t N> using
				times = typename pack<std::make_index_sequence<N>>::type_list::transform<make>;
				
			};
			
		/* size_of */
			// Generic usage
			template<typename... Types> struct
			size_impl
			{ using type = size_impl; static constexpr auto value = std::tuple_size_v<std::tuple<Types...>>; };
			
			// Packed types usage
			template<typename... Types> struct
			size_impl<type_list_impl<Types...>>
			{ using type = size_of<Types...>; };
			
			// Packed values usage
			template<auto... Values> struct
			size_impl<value_list_impl<Values...>>
			{ using type = size_of<decltype(Values)...>; };
			
	}
	
	// Exposed API
	
	using detail::type_list;
	using detail::value_list;
	
	using detail::pack;
	using detail::emplace;
	using detail::unpack;
	using detail::repeat;
	
	using detail::concat;
	using detail::reverse;
	using detail::transform;
	
	using detail::pop_first;
	using detail::pop_last;
	
	using detail::what_is;
	using detail::size_of;
	
}

#endif
