#ifndef INK_UTILITY_2D_VECTOR_CLASS_HEADER_FILE_GUARD
#define INK_UTILITY_2D_VECTOR_CLASS_HEADER_FILE_GUARD

#include <utility>
#include <math.h>
#include <tuple>

namespace ink {
	namespace detail {
		
		template<typename xT, typename yT = xT> class Vector2;
		template<typename xT, typename yT> class Vector2<const xT, const yT>;
		template<typename xT, typename yT> class Vector2<xT const, yT>;
		template<typename xT, typename yT> class Vector2<xT, yT const>;
		
		
		template<typename xT, typename yT>
		class Vector2
		{
			private: static constexpr auto
			x_isref = std::is_reference_v<xT>,
			y_isref = std::is_reference_v<yT>;
			
			private: using x_standard_arg = std::remove_reference_t<xT> const&;
			private: using y_standard_arg = std::remove_reference_t<yT> const&;
			
			public: xT x; yT y;
			
			
			
			public: constexpr
			Vector2() requires( std::is_default_constructible_v<xT> && std::is_default_constructible_v<yT> ):
			x(), y() {}
			
			public: constexpr
			Vector2(x_standard_arg x, y_standard_arg y):
			x(x), y(y) {}
			
			
			
			// Returns true if the vector angle pertains to Quadruant 1
			public: constexpr bool
			Quadrant1() const
			{ return x >= 0 && y >= 0; }
			
			// Returns true if the vector angle pertains to Quadruant 2
			public: constexpr bool
			Quadrant2() const
			{ return x <= 0 && y >= 0; }
			
			// Returns true if the vector angle pertains to Quadruant 3
			public: constexpr bool
			Quadrant3() const
			{ return x <= 0 && y <= 0; }
			
			// Returns true if the vector angle pertains to Quadruant 4
			public: constexpr bool
			Quadrant4() const
			{ return x >= 0 && y <= 0; }
			
			
			// Return this vector rotated 180 degrees
			public: constexpr auto
			rotate180() const
			{ return -(*this); }
			
			// Return this vector rotated 90 degrees. If ClockWise == true, rotation will take place in a clock-wise motion. Counter clockwise otherwise.
			public: constexpr auto
			rotate90(bool ClockWise = false) const
			{ return Vector2( y - (2 * y) * !ClockWise , x - (2 * x) * ClockWise ); }
			
			// Return this vector rotated R radians
			public: constexpr auto
			rotate(long double R) const {
				Vector2 out;
				
				out.x = std::cos(R) * x - std::sin(R) * y;
				out.y = std::sin(R) * x - std::cos(R) * y;
				
				return out;
			}
			
			// Absolute angle of this vector in radians.
			public: constexpr auto
			angle() const
			{ return std::atan2(y,x); }
			
			// Dot product
			public: template<typename otherX, typename otherY = otherX> constexpr auto
			dot(const Vector2<otherX, otherY>& other) const
			{ return x * other.x + y * other.y; }
			
			// Cross product
			public: template<typename otherX, typename otherY = otherX> constexpr auto
			cross(const Vector2<otherX, otherY>& other) const
			{ return x * other.y - y * other.x; }
			
			// Returns vector aligned with the cardinal direction closest to this vector. That is to say, any of the vectors whose angle in degrees is divisible by 45.
			// This means any one of: 0, 45, 90, 135, 180, 225, 270, 315, 360.
			public: constexpr auto
			cardinal() const
			{
				const Vector2	abs{	std::abs(x),
									std::abs(y)			};
				
				Vector2			out{	(x >= 0) - (x <= 0),
									(y >= 0) - (y <= 0)	};
				
				out.x *= (abs.x >= abs.y);
				out.y *= (abs.x <= abs.y);
				
				return out;
			}
			
			// Magnitude of the vector squared. Use if the true magnitude of the vector is unimportant, to save doing the square root with magnitude().
			public: constexpr auto
			magnitude2() const
			{ return (x * x) + (y * y); }
			
			// Magnituede of the vector
			public: constexpr auto
			magnitude() const
			{ return std::sqrt( magnitude2() ); }
			
			public: constexpr auto
			normalize() const
			{
				auto len = magnitude();
				Vector2 out; ( len == 0 )
				|| ( ( out = Vector2{ x/len , y/len } ) , true );
				return out;
			}
			
			public: template<typename otherX, typename otherY = otherX> constexpr auto
			projection_mag(Vector2<otherX,otherY> const& other) const
			{ return dot(other) / other.magnitude(); }
			
			public: template<typename otherX, typename otherY = otherX> constexpr auto
			projection_vec(Vector2<otherX,otherY> const& other) const
			{ return other.normalize() * projection_mag(other); }
			
		};
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator+(Vector2<x1, y1> const& lhs, Vector2<x2, y2> const& rhs) requires ( requires(x1 X1, y1 Y1, x2 X2, y2 Y2) { X1 + X2; Y1 + Y2; } )
		{ return Vector2<decltype(lhs.x + rhs.x),decltype(lhs.y + rhs.y)>(lhs.x + rhs.x, lhs.y + rhs.y); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator+(T const& lhs, Vector2<x, y> const& rhs)
		{ return Vector2<T,T>(lhs,lhs) + rhs; }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator+(Vector2<x, y> const& lhs, T const& rhs)
		{ return rhs + lhs; }
		
		
		
		template<typename x, typename y = x> static constexpr auto
		operator-(Vector2<x,y> const& v) requires ( requires(x X, y Y) { -X; -Y; } )
		{ return Vector2<decltype(-v.x),decltype(-v.y)>(-v.x, -v.y); }
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator-(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return lhs + (-rhs); }
		
		template<typename T, typename x, typename y> static constexpr auto
		operator-(T const& lhs, Vector2<x,y> const& rhs)
		{ return Vector2<T,T>(lhs,lhs) - rhs; }
		
		template<typename T, typename x, typename y> static constexpr auto
		operator-(Vector2<x,y> const& lhs, T const& rhs)
		{ return lhs - Vector2<T,T>(rhs,rhs); }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator*(Vector2<x1, y1> const& lhs, Vector2<x2, y2> const& rhs) requires( requires(x1 X1, y1 Y1, x2 X2, y2 Y2) { X1 * X2; Y1 * Y2; } )
		{ return Vector2<decltype(lhs.x * rhs.x),decltype(lhs.y * rhs.y)>(lhs.x * rhs.x, lhs.y * rhs.y); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator*(T const& lhs, Vector2<x, y> const& rhs)
		{ return Vector2<decltype(lhs * rhs.x), decltype(lhs * rhs.y)>(lhs * rhs.x, lhs * rhs.y); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator*(Vector2<x, y> const& lhs, T const& rhs)
		{ return rhs * lhs; }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator/(Vector2<x1, y1> const& lhs, Vector2<x2, y2> const& rhs) requires( requires(x1 X1, y1 Y1, x2 X2, y2 Y2) { X1 / X2; Y1 / Y2; } )
		{ return Vector2<decltype(lhs.x / rhs.x),decltype(lhs.y / rhs.y)>(lhs.x / rhs.x, lhs.y / rhs.y); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator/(Vector2<x, y> const& lhs, T const& rhs)
		{ return lhs / Vector2<T,T>(rhs,rhs); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator/(T const& lhs, Vector2<x, y> const& rhs)
		{ return Vector2<T,T>(lhs,lhs) / rhs; }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator%(Vector2<x1, y1> const& lhs, Vector2<x2, y2> const& rhs) requires( requires(x1 X1, y1 Y1, x2 X2, y2 Y2) { X1 % X2; Y1 % Y2; } )
		{ return Vector2<decltype(lhs.x % rhs.x, lhs.y % rhs.y)>(lhs.x % rhs.x, lhs.y % rhs.y); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator%(Vector2<x, y> const& lhs, T const& rhs)
		{ return lhs % Vector2<T,T>(rhs,rhs); }
		
		template<typename T, typename x, typename y = x> static constexpr auto
		operator%(T const& lhs, Vector2<x, y> const& rhs)
		{ return Vector2<T,T>(lhs,lhs) % rhs; }
		
		
		
		template<typename x, typename y> static constexpr auto
		operator!(Vector2<x,y> const& v)
		{ return Vector2<decltype(!v.x),decltype(!v.y)>( !v.x , !v.y ); }
		
		
		
		static constexpr auto
		operator||(Vector2<bool> const& lhs, Vector2<bool> const& rhs)
		{ return Vector2<bool>( lhs.x || rhs.x , lhs.y || rhs.y ); }
		
		static constexpr auto
		operator||(bool lhs, Vector2<bool> const& rhs)
		{ return Vector2<bool>(lhs,lhs) || rhs; }
		
		static constexpr auto
		operator||(Vector2<bool> const& lhs, bool rhs)
		{ return lhs || Vector2<bool>(rhs, rhs); }
		
		
		
		static constexpr auto
		operator&&(Vector2<bool> const& lhs, Vector2<bool> const& rhs)
		{ return Vector2<bool>( lhs.x && rhs.x , lhs.y && rhs.y ); }
		
		static constexpr auto
		operator&&(bool lhs, Vector2<bool> const& rhs)
		{ return Vector2<bool>(lhs,lhs) && rhs; }
		
		static constexpr auto
		operator&&(Vector2<bool> const& lhs, bool rhs)
		{ return lhs && Vector2<bool>(rhs,rhs); }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator==(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return Vector2<bool>( lhs.x == rhs.x , lhs.y == rhs.y ); }
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator!=(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return !(lhs == rhs); }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator>(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return Vector2<bool>( lhs.x > rhs.x , lhs.y > rhs.y ); }
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator<(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return Vector2<bool>( lhs.x < rhs.x , lhs.y < rhs.y ); }
		
		
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator>=(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return (lhs > rhs) || (lhs == rhs); }
		
		template<typename x1, typename y1 = x1, typename x2 = x1, typename y2 = y1> static constexpr auto
		operator<=(Vector2<x1,y1> const& lhs, Vector2<x2,y2> const& rhs)
		{ return (lhs < rhs) || (lhs == rhs); }
		
	}
	
	using detail::Vector2;
	
}

#endif
