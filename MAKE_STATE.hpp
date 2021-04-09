/**
 * The purpose of this header file is some light Macro Dark Magic.
 * Define "MAKE_STATE" as a fruit list of "named states" (like what you would put in an enum),
 * e.g.
 * #define MAKE_STATE(v) v(Thing1) v(Thing2) v(CatInHat)
 * 
 * And then include this file. This will do the following:
 * 
 * 	+ Declare an enum class of the name "State".
 * 		- Which can otherwise be named by defining "MAKE_STATE_NAME" as the desired name.
 * 		
 * 		- And can have its underlying type specified by defining "MAKE_STATE_UNDERLYING_TYPE".
 * 		
 * 		- As well, it can be made a non-class enum by defining "MAKE_STATE_NON_CLASS".
 * 		
 * 		* From the above example, the enum would take the form enum class State { Thing1, Thing2, CatInHat, };
 * 
 * 	+ Declare a function for each "State" of the form "void(StateName)()".
 * 		- Each of whose names can be pre-fixed and post-fixed by defining "MAKE_STATE_FPREFIX" and "MAKE_STATE_FPOSTFIX" respectively.
 * 		
 * 		- The return type can be changed by defining "MAKE_STATE_RETURN_TYPE" as the desired return type (this could
 * 			also be used to declare each function as a template, e.g. #define MAKE_STATE_RETURN_TYPE template<typename T> void)
 * 		
 * 		- NOTE: Each function is declared, but not defined. The job of defining each function is left to the user.
 * 		
 * 		* From the above example, the functions defined would be "void Thing1(); void Thing2(); void CatInHat()".
 * 
 * 	+ Define a function called "RunState", takes a "State" as an argument, and returns void. It runs the function associated to the given State.
 * 		- The name of this function can be changed by defining "MAKE_STATE_RUN_STATE_NAME" as the desired name of the function.
 * 		
 * 		- The return type of this function will be the same as that of all other functions.
 * 		
 * 		* From the above example, running "RunState(State::Thing1)" will run "Thing1()", "RunState(State::Thing2)" -> "Thing2()", "RunState(State::CatInHat)" -> "CatInHat()".
 * 
 * 	+ (CONDITIONALLY) Defines a function whose name is defined by "MAKE_STATE_STRINGS", which given any State, will return a 1:1 string of the enum's name.
 * 		* From the above example, if we were to have defined MAKE_STATE_STRINGS in the form "#define MAKE_STATE_STRINGS Stringify",
 * 			then the following would compile successfully:
 * 				static_assert(Stringify(State::Thing1) == "Thing1");
 * 				static_assert(Stringify(State::Thing2) == "Thing2");
 * 				static_assert(Stringify(State::CatInHat) == "CatInHat");
 * 
 * 
*/


#if defined(MAKE_STATE)
	
	// State type name
	#define MAKE_STATE_name State
	#if defined(MAKE_STATE_NAME)
		#undef MAKE_STATE_name
		#define MAKE_STATE_name MAKE_STATE_NAME
	#endif
	
	enum
	#if !defined(MAKE_STATE_NON_CLASS) // If MAKE_STATE_NON_CLASS is defined, then the enum shall be a normal enum
		class
	#endif
	MAKE_STATE_name
	#if defined(MAKE_STATE_UNDERLYING_TYPE) // If MAKE_STATE_UNDERLYING_TYPE is defined, it's definition shall be used as the underlying type of the enum.
		: MAKE_STATE_UNDERLYING_TYPE
		#undef MAKE_STATE_UNDERLYING_TYPE
	#endif
	{
		#define add_comma(l) l,
		MAKE_STATE(add_comma)
		#undef add_comma
	};
	
	
	
	#define MAKE_STATE_fprefix // Function prefix
	#if defined(MAKE_STATE_FPREFIX)
	
		#undef MAKE_STATE_fprefix
		#define MAKE_STATE_fprefix MAKE_STATE_FPREFIX
		
	#endif
	
	
	
	#define MAKE_STATE_fpostfix // Function postfix
	#if defined(MAKE_STATE_FPOSTFIX)
	
		#undef MAKE_STATE_fpostfix
		#define MAKE_STATE_fpostfix MAKE_STATE_FPOSTFIX
		
	#endif
	
	
	
	// Function declarations
	#define THREE_WAY_CONCAT(arg1, arg2, arg3) THREE_WAY_CONCAT_impl(arg1,arg2,arg3)
	#define THREE_WAY_CONCAT_impl(arg1, arg2, arg3) arg1##arg2##arg3
	
	
	#define State_Function(l) MAKE_STATE_RETURN_TYPE THREE_WAY_CONCAT(MAKE_STATE_fprefix, l, MAKE_STATE_fpostfix) ();
		#if !defined(MAKE_STATE_RETURN_TYPE)
			#define MAKE_STATE_RETURN_TYPE void
		#endif
		MAKE_STATE(State_Function)
	#undef State_Function
	
	
	
	#if !defined(MAKE_STATE_RUN_STATE_NAME)
		#define MAKE_STATE_RUN_STATE_NAME RunState // Function that runs the function of any given state
	#endif
	MAKE_STATE_RETURN_TYPE MAKE_STATE_RUN_STATE_NAME(MAKE_STATE_name state) {
		switch (state) {
			#define State_Case(l) case MAKE_STATE_name::l: return THREE_WAY_CONCAT(MAKE_STATE_fprefix, l, MAKE_STATE_fpostfix) (); break;
			MAKE_STATE(State_Case)
			#undef State_Case
		}
	};
	#undef MAKE_STATE_RETURN_TYPE
	#undef MAKE_STATE_RUN_STATE_NAME
	
	
	// Conditionally defined function that returns a string version of the declared states
	#if defined(MAKE_STATE_STRINGS)
		static constexpr char const* MAKE_STATE_STRINGS(MAKE_STATE_name state) {
			constexpr const char* ARRAY[] = {
				
				#define State_String(s) Stringify(s),
					#define Stringify(x) Stringify_impl(x)
					#define Stringify_impl(x) #x
				
						MAKE_STATE(State_String)
				
					#define Stringify_impl(x) #x
					#define Stringify(x) Stringify_impl(x)
				#undef State_String
				
			};
			return ARRAY[static_cast<std::underlying_type_t<decltype(state)>>(state)];
		}
		#undef MAKE_STATE_STRINGS
	#endif
	
	#undef THREE_WAY_CONCAT_impl
	#undef THREE_WAY_CONCAT
	
	#undef MAKE_STATE_fpostfix
	#undef MAKE_STATE_FPOSTFIX
	
	#undef MAKE_STATE_fprefix
	#undef MAKE_STATE_FPREFIX
	
	#undef MAKE_STATE_name
	#undef MAKE_STATE_NAME
	
	#undef MAKE_STATE
	
#endif
