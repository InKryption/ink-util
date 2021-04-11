#ifndef INK_UTILITY_2D_VECTOR_CLASS_HEADER_FILE_GUARD
#define INK_UTILITY_2D_VECTOR_CLASS_HEADER_FILE_GUARD

#include <cstdint>
#include <utility>
#include <math.h>
#include <tuple>

namespace ink {
	
	namespace detail {
		
		template<typename T> struct Q_rsqrt_impl;
		
		template<std::convertible_to<float> T> requires(sizeof(T) <= 4)
		struct Q_rsqrt_impl<T> {
			
			// Originally ripped from Stack Overflow, which was a modified version of Quake III's fast reverse square root function.
			// Set extra_iterations to 1 or more for increased accuracy, at the cost of some loss in efficiency.
			static inline float Q_rsqrt(T number, size_t extra_iterations) {
				
				constexpr float THREE_HALVES = 1.5F;
				constexpr auto MAGIC_NUM = 0x5f3759df;
				
				uint32_t i;
				float x2, y;
				
				x2 = number * 0.5F;
				y = number;
				i = *(uint32_t*)&y;
				i = MAGIC_NUM - (i >> 1);
				y = *(float *) &i;
				
				y = y * (THREE_HALVES - (x2 * y * y));
				
				// Extra iterations if requested.
				for (size_t n_iteration = 0; n_iteration < extra_iterations; n_iteration++)
				{ y = y * (THREE_HALVES - (x2 * y * y)); }
				
				return y;
			}
			
			static inline float Q_rsqrt(T number) {
				
				constexpr float THREE_HALVES = 1.5F;
				constexpr auto MAGIC_NUM = 0x5f3759df;
				
				uint32_t i;
				float x2, y;
				
				x2 = number * 0.5F;
				y = number;
				i = *(uint32_t*)&y;
				i = MAGIC_NUM - (i >> 1);
				y = *(float *) &i;
				
				y = y * (THREE_HALVES - (x2 * y * y));
				
				return y;
			}
			
		};
		
		template<std::convertible_to<double> T> requires(sizeof(T) > 4)
		struct Q_rsqrt_impl<T> {
			
			// Set extra_iterations to 1 or more for increased accuracy, at the cost of some loss in efficiency.
			static inline double Q_rsqrt(T number, size_t extra_iterations) {
				
				constexpr double THREE_HALVES = 1.5;
				constexpr auto MAGIC_NUM = 0x5fe6eb50c7b537a9;
				
				uint64_t i;
				double x2, y;
				
				x2 = number * 0.5F;
				y = number;
				i = *(uint64_t*)&y;
				i = MAGIC_NUM - (i >> 1);
				y = *(double *) &i;
				
				y = y * (THREE_HALVES - (x2 * y * y));
				
				// Extra iterations if requested.
				for (size_t n_iteration = 0; n_iteration < extra_iterations; n_iteration++)
				{ y = y * (THREE_HALVES - (x2 * y * y)); }
				
				return y;
			}
			
			static inline double Q_rsqrt(T number) {
				
				constexpr double THREE_HALVES = 1.5;
				constexpr auto MAGIC_NUM = 0x5fe6eb50c7b537a9;
				
				uint64_t i;
				double x2, y;
				
				x2 = number * 0.5F;
				y = number;
				i = *(uint64_t*)&y;
				i = MAGIC_NUM - (i >> 1);
				y = *(double *) &i;
				
				y = y * (THREE_HALVES - (x2 * y * y));
				
				return y;
			}
			
		};
		
	}
	
	/**
	 * An implementation of Quake III's Engine's reverse square root function. Supports floats and doubles.
	 * Set extra_iterations to 1 or more for increased accuracy, at the cost of some loss in efficiency.
	 */
	template<typename T> static inline auto
	Q_rsqrt(T number, size_t extra_iterations)
	{ return detail::Q_rsqrt_impl<T>::Q_rsqrt(number, extra_iterations); }
	
	/**
	 * An implementation of Quake III's Engine's reverse square root function. Supports floats and doubles.
	 */
	template<typename T> static inline auto
	Q_rsqrt(T number)
	{ return detail::Q_rsqrt_impl<T>::Q_rsqrt(number); }
	
	
	
	namespace detail {
		
		template<typename XT, typename YT = XT>
		class Vector2
		{
			
			private: using x_standard_arg = std::remove_reference_t<XT> const&;
			private: using y_standard_arg = std::remove_reference_t<YT> const&;
			
			private: template<typename OX = XT, typename OY = OX>static constexpr bool
			dot_product_exists = requires(XT x1, YT y1, OX x2, OY y2) { (x1 * x2) + (y1 * y2); };
			
			public: XT x; YT y;
			
			// Default constructor. Requires both x and y to be default constructible
			public: constexpr
			Vector2()
			requires( std::is_default_constructible_v<XT> && std::is_default_constructible_v<YT> ):
			x(), y() {}
			
			// Standard constructor
			public: constexpr
			Vector2(x_standard_arg x, y_standard_arg y):
			x(x), y(y) {}
			
			// Returns true if the vector angle pertains to Quadruant 1
			public: constexpr decltype(auto)
			quadrant1() const
			requires requires(XT x, YT y)
			{
				(x >= 0) && (y >= 0);
			}
			{
				return (x >= 0) && (y >= 0);
			}
			
			// Returns true if the vector angle pertains to Quadruant 2
			public: constexpr decltype(auto)
			quadrant2() const
			requires requires(XT x, YT y)
			{
				(x <= 0) && (y >= 0);
			}
			{
				return (x <= 0) && (y >= 0);
			}
			
			// Returns true if the vector angle pertains to Quadruant 3
			public: constexpr decltype(auto)
			quadrant3() const
			requires requires(XT x, YT y)
			{
				(x <= 0) && (y <= 0);
			}
			{
				return (x <= 0) && (y <= 0);
			}
			
			// Returns true if the vector angle pertains to Quadruant 4
			public: constexpr decltype(auto)
			quadrant4() const
			requires requires(XT x, YT y)
			{
				(x >= 0) && (y <= 0);
			}
			{
				return (x >= 0) && (y <= 0);
			}
			
			
			// Return this vector rotated 180 degrees
			public: constexpr decltype(auto)
			rotate180() const
			requires requires(XT x, YT y)
			{
				-x; -y;
			}
			{
				return Vector2(-x, -y);
			}
			
			// Return this vector rotated 90 degrees. If ClockWise == true, rotation will take place in a clock-wise motion. Counter clockwise otherwise.
			public: constexpr decltype(auto)
			rotate90(bool ClockWise = false) const
			requires requires(XT x, YT y)
			{
				y - (2 * y) * !std::declval<bool>(); x - (2 * x) * std::declval<bool>();
			}
			{
				return Vector2( y - (2 * y) * !ClockWise , x - (2 * x) * ClockWise );
			}
			
			// Return this vector rotated R radians
			public: constexpr decltype(auto)
			rotate(long double R) const
			requires requires(XT x, YT y)
			{
				x * std::declval<long double>() - std::declval<long double>() * y;
			}
			{
				Vector2 out;
				
				out.x = std::cos(R) * x - std::sin(R) * y;
				out.y = std::sin(R) * x - std::cos(R) * y;
				
				return out;
			}
			
			// Absolute angle of this vector in radians.
			public: constexpr decltype(auto)
			angle() const
			requires requires(XT x, YT y)
			{
				std::atan2(y, x);
			}
			{
				return std::atan2(y,x);
			}
			
			// Dot product
			public: template<typename OX, typename OY = OX> constexpr decltype(auto)
			dot(const Vector2<OX, OY>& other) const
			requires( dot_product_exists<OX, OY> )
			{
				return (x * other.x) + (y * other.y);
			}
			
			// Cross product
			public: template<typename OX, typename OY = OX> constexpr decltype(auto)
			cross(const Vector2<OX, OY>& other) const
			requires requires(XT x, YT y)
			{
				(x * x) - (y * y);
			}
			{
				return (x * other.y) - (y * other.x);
			}
			
			
			
			// Dot product
			public: template<typename otherX, typename otherY = otherX> constexpr decltype(auto)
			dot(const Vector2<otherX, otherY>& other) const
			{ return x * other.x + y * other.y; }
			
			// Cross product
			public: template<typename otherX, typename otherY = otherX> constexpr decltype(auto)
			cross(const Vector2<otherX, otherY>& other) const
			{ return x * other.y - y * other.x; }
			
			
			
			// Magnitude of the vector squared. Use if the true magnitude of the vector is unimportant, to save doing the square root with magnitude().
			public: constexpr decltype(auto)
			magnitude2() const
			requires( dot_product_exists<> )
			{
				return (x * x) + (y * y);
			}
			
			// Magnitude of the vector
			public: constexpr decltype(auto)
			magnitude() const
			requires( dot_product_exists<> )
			{
				return std::sqrt( magnitude2() );
			}
			
			// Normalized version of this vector
			public: constexpr decltype(auto)
			normalize() const requires( dot_product_exists<> ) &&
			requires(XT x, YT y)
			{
				x = 0;
				y = 0;
				x / ((x * x) + (y * y));
				y / ((x * x) + (y * y));
			}
			{
				auto len = magnitude();
				Vector2 out(0,0); ( len == 0 )
				|| ( out = Vector2{ x/len , y/len } , true );
				return out;
			}
			
			// Normalized version of this vector utilizing an implementation of Q_rsqrt (Quake III's fast reverse square root). Supports increased accuracy at the cost of efficiency.
			public: inline decltype(auto)
			q_normalize(size_t extra_iterations) const requires( dot_product_exists<> )
			{
				auto rsqrt = Q_rsqrt<decltype(magnitude2())>(magnitude2(),extra_iterations);
				Vector2 out(0,0); ( isnan(rsqrt) )
				|| ( out = Vector2( x * rsqrt , y * rsqrt ), true );
				return out;
			}
			
			// Normalized version of this vector utilizing an implementation of Q_rsqrt (Quake III's fast reverse square root).
			public: inline decltype(auto)
			q_normalize() const requires( dot_product_exists<> )
			{
				auto rsqrt = Q_rsqrt<decltype(magnitude2())>(magnitude2());
				Vector2 out(0,0); ( isnan(rsqrt) )
				|| ( out = Vector2( x * rsqrt , y * rsqrt ), true );
				return out;
			}
			
			// Magnitude of the projection of this vector onto the other vector
			public: template<typename OX, typename OY = OX> constexpr decltype(auto)
			proj_mag(Vector2<OX, OY> const& other) const
			requires( dot_product_exists<> && dot_product_exists<OX, OY> )
			{
				return dot(other) / other.magnitude();
			}
			
			// Projection of this vector onto the other vector
			public: template<typename OX, typename OY = OX> constexpr decltype(auto)
			proj_vec(Vector2<OX, OY> const& other) const
			requires( dot_product_exists<> && dot_product_exists<OX, OY> )
			{
				return other.normalize() * projection_mag(other);
			}
			
			// Returns vector aligned with the cardinal direction closest to this vector. That is to say, any of the vectors whose angle in degrees is divisible by 45.
			public: constexpr decltype(auto)
			cardinal() const requires
			requires(XT x, YT y)
			{
				(x >= 0) - (x <= 0);
				(y >= 0) - (y <= 0);
				x *= ( std::abs(x) >= std::abs(y) );
				y *= ( std::abs(x) <= std::abs(y) );
			}
			{
				const Vector2
					abs{	std::abs(x),
							std::abs(y)	};
				
				Vector2
					out{	(x >= 0) - (x <= 0),
							(y >= 0) - (y <= 0)	};
				
				out.x *= (abs.x >= abs.y);
				out.y *= (abs.x <= abs.y);
				
				return out;
			}
			
		};
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator+(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 + x2; y1 + y2;
		}
		{
			return Vector2<decltype(lhs.x + rhs.x), decltype(lhs.y + rhs.y)>(lhs.x + rhs.x, lhs.y + rhs.y);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator+(T const& lhs, Vector2<X, Y> const& rhs)
		{
			return Vector2<T, T>(lhs, lhs) + rhs;
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator+(Vector2<X, Y> const& lhs, T const& rhs)
		{
			return rhs + lhs;
		}
		
		
		
		template<typename X, typename Y = X> static constexpr auto
		operator-(Vector2<X, Y> const& v)
		requires requires(X x, Y y)
		{
			-x; -y;
		}
		{
			return Vector2<decltype(-v.x), decltype(-v.y)>(-v.x, -v.y);
		}
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator-(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		{
			return lhs + (-rhs);
		}
		
		template<typename T, typename X, typename Y> static constexpr auto
		operator-(T const& lhs, Vector2<X, Y> const& rhs)
		{
			return Vector2<T, T>(lhs, lhs) - rhs;
		}
		
		template<typename T, typename X, typename Y> static constexpr auto
		operator-(Vector2<X, Y> const& lhs, T const& rhs)
		{
			return lhs - Vector2<T, T>(rhs, rhs);
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator*(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 * x2; y1 * y2;
		}
		{
			return Vector2<decltype(lhs.x * rhs.x), decltype(lhs.y * rhs.y)> (lhs.x * rhs.x, lhs.y * rhs.y);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator*(T const& lhs, Vector2<X, Y> const& rhs)
		{
			return Vector2<decltype(lhs * rhs.x), decltype(lhs * rhs.y)>(lhs * rhs.x, lhs * rhs.y);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator*(Vector2<X, Y> const& lhs, T const& rhs)
		{
			return rhs * lhs;
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator/(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 / x2; y1 / y2;
		}
		{
			return Vector2<decltype(lhs.x / rhs.x), decltype(lhs.y / rhs.y)> (lhs.x / rhs.x, lhs.y / rhs.y);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator/(Vector2<X, Y> const& lhs, T const& rhs)
		{
			return lhs / Vector2<T, T>(rhs, rhs);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator/(T const& lhs, Vector2<X, Y> const& rhs)
		{
			return Vector2<T, T>(lhs, lhs) / rhs;
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator%(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 % x2; y1 % y2;
		}
		{
			return Vector2<decltype(lhs.x % rhs.x, lhs.y % rhs.y)>(lhs.x % rhs.x, lhs.y % rhs.y);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator%(Vector2<X, Y> const& lhs, T const& rhs)
		{
			return lhs % Vector2<T, T>(rhs, rhs);
		}
		
		template<typename T, typename X, typename Y = X> static constexpr auto
		operator%(T const& lhs, Vector2<X, Y> const& rhs)
		{
			return Vector2<T, T>(lhs, lhs) % rhs;
		}
		
		
		
		template<typename X, typename Y> static constexpr auto
		operator!(Vector2<X, Y> const& v)
		requires requires(X x, Y y)
		{
			!x; !y;
		}
		{
			return Vector2<decltype(!v.x), decltype(!v.y)>(!v.x, !v.y);
		}
		
		
		
		static constexpr auto
		operator||(Vector2<bool> const& lhs, Vector2<bool> const& rhs)
		{
			return Vector2<bool>(lhs.x || rhs.x, lhs.y || rhs.y);
		}
		
		static constexpr auto
		operator||(bool lhs, Vector2<bool> const& rhs)
		{
			return Vector2<bool>(lhs, lhs) || rhs;
		}
		
		static constexpr auto
		operator||(Vector2<bool> const& lhs, bool rhs)
		{
			return lhs || Vector2<bool>(rhs, rhs);
		}
		
		
		
		static constexpr auto
		operator&&(Vector2<bool> const& lhs, Vector2<bool> const& rhs)
		{
			return Vector2<bool>(lhs.x && rhs.x, lhs.y && rhs.y);
		}
		
		static constexpr auto
		operator&&(bool lhs, Vector2<bool> const& rhs)
		{
			return Vector2<bool>(lhs, lhs) && rhs;
		}
		
		static constexpr auto
		operator&&(Vector2<bool> const& lhs, bool rhs)
		{
			return lhs && Vector2<bool>(rhs, rhs);
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator==(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 == x2;
			y1 == y2;
		}
		{
			return Vector2<bool>(lhs.x == rhs.x, lhs.y == rhs.y);
		}
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator!=(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		{
			return !(lhs == rhs);
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator>(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 > x2;
			y1 > y2;
		}
		{
			return Vector2<bool>(lhs.x > rhs.x, lhs.y > rhs.y);
		}
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator<(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		requires requires(X1 x1, Y1 y1, X2 x2, Y2 y2)
		{
			x1 < x2;
			y1 < y2;
		}
		{
			return Vector2<bool>(lhs.x < rhs.x, lhs.y < rhs.y);
		}
		
		
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator>=(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		{
			return (lhs > rhs) || (lhs == rhs);
		}
		
		template<typename X1, typename Y1 = X1, typename X2 = X1, typename Y2 = Y1> static constexpr auto
		operator<=(Vector2<X1, Y1> const& lhs, Vector2<X2, Y2> const& rhs)
		{
			return (lhs < rhs) || (lhs == rhs);
		}
		
		// Forbidden types. Left undefined, as their implementation is of little use.
		// But still open to being defined by anyone who wants to I suppose
		
		template<typename XT, typename YT> class Vector2<const XT, const YT>;
		template<typename XT, typename YT> class Vector2<XT const, YT>;
		template<typename XT, typename YT> class Vector2<XT, YT const>;
		
	}
	
	using detail::Vector2;
	
}

#endif
