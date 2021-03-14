#ifndef INK_SIGNAL_DATA_ABSTRACTION_HEADER_FILE
#define INK_SIGNAL_DATA_ABSTRACTION_HEADER_FILE

namespace ink {
	
	/**
	 * Abstraction of an input signal model. It retains a small cache to return simple, but nonetheless useful information.
	 * The basic, raw intended usage is as displayed:
	 * 
	 * 	...
	 * 	SignalData<bool> Up_Button;
	 *	while (Running) {
	 *		bool signalState = ( ´Retrieve State of Up Button on Keyboard´ );
	 * 		
	 * 		Up_Button.Update(signalState);
	 * 		
	 * 		...
	 * 		
	 * 		if ( Up_Button.down() )	puts("UP BUTTON IS DOWN.");
	 * 		if ( Up_Button.fall() )	puts("UP BUTTON PRESSED.");
	 * 		if ( Up_Button.rise() )	puts("UP BUTTON RELEASED.");
	 * 		if ( Up_Button.held() )	puts("UP BUTTON HELD.");
	 *	}
	 *	...
	*/
	class SignalData {
		
		public: SignalData()
		{}
		
		// Provide signal indicating whether input has been activated during this tick.
		public: constexpr void
		Update(bool signal) &
		{
			_prev = _curr;
			_curr = signal;
		}
		
		// True if active during this tick.
		public: constexpr bool
		down() const
		{ return _curr; }
		
		// True if activated during this tick.
		public: constexpr bool
		fall() const
		{ return !_prev && down(); }
		
		// True if deactivated during this tick.
		public: constexpr bool
		rise() const
		{ return _prev && !down(); }
		
		// True if active for more than one tick (previous and current).
		public: constexpr bool
		held() const
		{ return _prev && down(); }
		
		private: bool
		_prev = false,
		_curr = false;
		
	};
	
}

#endif
