
#if defined(MAKE_STATE)
	
	enum class State {
		#define add_comma(l) l,
		MAKE_STATE(add_comma)
		#undef add_comma
	};
	
	void RunState(State state) {
		switch (state) {
			#define State_Case(l) case State::l: l(); break;
			MAKE_STATE(State_Case)
			#undef State_Case
		}
	};
	
	#define State_Function(l) void l();
	MAKE_STATE(State_Function)
	#undef State_Function
	
	#undef MAKE_STATE
	
#endif
