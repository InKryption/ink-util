#ifndef INK_UTILITY_FPS_LIMITER_HEADER_FILE_GUARD
#define INK_UTILITY_FPS_LIMITER_HEADER_FILE_GUARD

#include <chrono>
#include <thread>

namespace ink {
	
	struct FPS_Limiter {
		using HRC = std::chrono::high_resolution_clock;
		
		FPS_Limiter(ptrdiff_t FPS = 60):
		FPS(FPS) {}
		
		int64_t
		Update()
		{
			HRC::duration HZ = std::chrono::milliseconds{ 1000 / FPS };
			
			tick_diff = HRC::now() - tick_start;
			( ( (HZ - tick_diff).count() > 0 ) || [&]()constexpr{ std::this_thread::sleep_for( HZ - tick_diff ); return true; }());
			
			tick_start = HRC::now();
			
			return tick_diff.count();
		}
		
		void
		Set(ptrdiff_t FPS)
		{ this->FPS = FPS; }
		
		private:
		ptrdiff_t FPS = 60;
		HRC::time_point tick_start{HRC::duration(0)};
		HRC::duration tick_diff{0};
		
	};
	
}

#endif
