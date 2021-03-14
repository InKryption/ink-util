#ifndef INK_UTILITY_MATHS_V2D_HEADER_FILE_GUARD
#define INK_UTILITY_MATHS_V2D_HEADER_FILE_GUARD



#if !(__cplusplus > 201703L && __cpp_concepts >= 201907L)
	#pragma error "Header file at '"__FILE__"' requires a c++20 enabled compiler, with concepts, to compile."
#endif
#include <concepts>

#include <math.h>
#include <numbers>
#include <string>

namespace ink {
	
	template<typename T> requires (std::is_arithmetic_v<std::remove_reference_t<T>>)
	class V2D {
		
		public: T
		x, y;
		
		public: using
		unit = std::remove_reference_t<T>;
		
		private: constexpr static bool
		is_ref = std::is_arithmetic_v< std::remove_reference_t<T> > && std::is_reference_v<T>;
		
		private: constexpr static bool
		is_const = std::is_const_v<unit>;
		
		// Ensures that all 't's are operable types (arithmetic T or reference to arithmetic T).
		private: template<typename... t> constexpr static bool
		operable = (std::is_arithmetic_v< std::remove_reference_t<t> >&&...);
		
		
		
		// Standard Constructors
		
		public: constexpr
		V2D() requires( !is_ref ):
		x(0), y(0) {}
		
		public: constexpr
		V2D(T x, T y):
		x(x), y(y) {}
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		V2D(t1 x, t2 y) requires( !is_ref ):
		x(static_cast<T>(x)), y(static_cast<T>(y)) {}
		
		
		
		// Copy & Move Constructors
		
		public: constexpr
		V2D(V2D && other):
		x(other.x), y(other.y) {}
		
		public: constexpr
		V2D(V2D const& other) requires( !is_ref || (is_ref && is_const) ):
		x(other.x), y(other.y) {}
		
		public: template<typename t> requires( operable<t> ) constexpr
		V2D(V2D<t> && other) requires( !is_ref ):
		x( static_cast<T>(other.x) ), y( static_cast<T>(other.y) ) {}
		
		public: template<typename t> requires( operable<t> ) constexpr
		V2D(V2D<t> const& other) requires( !is_ref ):
		x( static_cast<T>(other.x) ), y( static_cast<T>(other.y) ) {}
		
		
		
		// Pair Constructors
		
		public: constexpr
		V2D(std::pair<T, T> && pair):
		x(pair.first), y(pair.second) {}
		
		public: constexpr
		V2D(std::pair<T, T> const& pair) requires( !is_ref || (is_ref && is_const) ):
		x(pair.first), y(pair.second) {}
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		V2D(std::pair<t1, t2> && pair) requires( !is_ref ):
		x(static_cast<unit>(pair.first)), y(static_cast<unit>(pair.second)) {}
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		V2D(std::pair<t1, t2> const& pair) requires( !is_ref ):
		x(static_cast<unit>(pair.first)), y(static_cast<unit>(pair.second)) {}
		
		
		
		// Tuple Constructors
		
		public: constexpr
		V2D(std::tuple<T, T> && tuple):
		x(std::get<0>(tuple)), y(std::get<1>(tuple)) {}
		
		public: constexpr
		V2D(std::tuple<T, T> const& tuple) requires( !is_ref || (is_ref && is_const) ):
		x(std::get<0>(tuple)), y(std::get<1>(tuple)) {}
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		V2D(std::tuple<t1, t2> && tuple) requires( !is_ref ):
		x( static_cast<unit>(std::get<0>(tuple)) ), y( static_cast<unit>(std::get<1>(tuple)) ) {}
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		V2D(std::tuple<t1, t2> const& tuple) requires( !is_ref ):
		x( static_cast<unit>(std::get<0>(tuple)) ), y( static_cast<unit>(std::get<1>(tuple)) ) {}
		
		
		
		// Copy & Move Assignment operators
		
		public: constexpr V2D&
		operator=(V2D && other) requires ( !is_const )
		{ x = other.x; y = other.y; return *this; }
		
		public: constexpr V2D&
		operator=(V2D const& other) requires( !is_const )
		{ x = other.x; y = other.y; return *this; }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator=(V2D<t> && other) requires( !is_const )
		{ x = static_cast<unit>(other.x); y = static_cast<unit>(other.y); return *this; }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator=(V2D<t> const& other) requires( !is_const )
		{ x = static_cast<unit>(other.x); y = static_cast<unit>(other.y); return *this; }
		
		
		
		// Pair Assignment operators
		
		public: constexpr V2D&
		operator=(std::pair<T, T> && pair) requires ( !is_const )
		{ x = pair.first; y = pair.second; return *this; }
		
		public: constexpr V2D&
		operator=(std::pair<T, T> const& pair) requires( !is_const )
		{ x = pair.first; y = pair.second; return *this; }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr V2D&
		operator=(std::pair<t1, t2> && pair) requires( !is_const )
		{ x = static_cast<unit>(pair.first); y = static_cast<unit>(pair.second); return *this; }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr V2D&
		operator=(std::pair<t1, t2> const& pair) requires( !is_const )
		{ x = static_cast<unit>(pair.first); y = static_cast<unit>(pair.second); return *this; }
		
		
		
		// Tuple Assignment operators
		
		public: constexpr V2D&
		operator=(std::tuple<T, T> && tuple) requires( !is_const )
		{ x = std::get<0>(tuple); y = std::get<1>(tuple); return *this; }
		
		public: constexpr V2D&
		operator=(std::tuple<T, T> const& tuple) requires( !is_const )
		{ x = std::get<0>(tuple); y = std::get<1>(tuple); return *this; }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr V2D&
		operator=(std::tuple<t1, t2> && tuple) requires( !is_const )
		{ x = static_cast<unit>(std::get<0>(tuple)); y = static_cast<unit>(std::get<1>(tuple)); return *this; }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr V2D&
		operator=(std::tuple<t1, t2> const& tuple) requires( !is_const )
		{ x = static_cast<unit>(std::get<0>(tuple)); y = static_cast<unit>(std::get<1>(tuple)); return *this; }
		
		
		
		// Sign Inversal operator
		
		public: friend constexpr auto
		operator-(V2D const& v)
		{ return V2D<decltype(-v.x)>(-v.x, -v.y); }
		
		
		
		// Addition operators
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator+(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype( lhs.x + rhs.x )>(lhs.x + rhs.x, lhs.y + rhs.y); }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator+=(V2D<t> const& rhs)
		{ x += rhs.x; y += rhs.y; return *this; }
		
		
		
		// Subtraction operators
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator-(V2D const& lhs, V2D const& rhs)
		{ return V2D<decltype(lhs.x - rhs.x)>(lhs.x - rhs.x, lhs.y - rhs.y); }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator-=(V2D<t> const& rhs) requires( !is_const )
		{ x -= rhs.x; y -= rhs.y; return *this; }
		
		
		
		// Multiplication operators
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator*(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x * rhs.x)>(lhs.x * rhs.x, lhs.y * rhs.y); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator*(V2D const& lhs, t const& rhs)
		{ return V2D<decltype(lhs.x * rhs)>(lhs.x * rhs, lhs.y * rhs); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator*(t const& lhs, V2D const& rhs)
		{ return V2D<decltype(lhs * rhs.x)>(lhs * rhs.x, lhs * rhs.y); }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator*=(V2D<t> const& rhs) requires( !is_const )
		{ x *= rhs.x; y *= rhs.y; return *this; }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator*=(t const& rhs) requires( !is_const )
		{ x *= rhs; y *= rhs; return *this; }
		
		
		
		// Division operators
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator/(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x / rhs.x)>( lhs.x / rhs.x, lhs.y / rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator/(V2D const& lhs, t const& rhs)
		{ return V2D<decltype(lhs.x / rhs)>( lhs.x / rhs, lhs.y / rhs ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator/(t const& lhs, V2D const& rhs)
		{ return V2D<decltype(lhs / rhs.x)>( lhs / rhs.x, lhs / rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator/=(V2D<t> const& rhs) requires( !is_const )
		{ x /= rhs.x; y /= rhs.y; return *this; }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator/=(t const& rhs) requires( !is_const )
		{ x /= rhs; y /= rhs; return *this; }
		
		
		
		// Modulus operators
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator%(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x % rhs.x)>( lhs.x % rhs.x, lhs.y % rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator%(V2D const& lhs, t const& rhs)
		{ return V2D<decltype(lhs.x % rhs)>( lhs.x % rhs, lhs.y % rhs ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator%(t const& lhs, V2D const& rhs)
		{ return V2D<decltype(lhs % rhs.x)>( lhs % rhs.x, lhs % rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator%=(V2D<t> const& rhs) requires( !is_const )
		{ x %= rhs.x; y %= rhs.y; return *this; }
		
		public: template<typename t> requires( operable<t> ) constexpr V2D&
		operator%=(t const& rhs) requires( !is_const )
		{ x %= rhs; y %= rhs; return *this; }
		
		
		
		// Logical operators
		
		public: friend constexpr auto
		operator !(V2D const& v)
		{ return V2D<decltype(!v.x)>(!v.x, !v.y); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator==(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x == rhs.x)>(lhs.x == rhs.x, lhs.y == rhs.y); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator!=(V2D const& lhs, V2D<t> const& rhs)
		{ return !(lhs == rhs); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator&&(V2D const& lhs, const V2D<t>& rhs)
		{ return V2D<decltype(lhs.x && rhs.x)>(lhs.x && rhs.x, lhs.y && rhs.y); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator||(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x || rhs.x)>(lhs.x || rhs.x, lhs.y || rhs.y); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator >(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x > rhs.x)>(lhs.x > rhs.x, lhs.y > rhs.y); } 
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator>=(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype( lhs.x >= rhs.x )>( lhs.x >= rhs.x, lhs.y >= rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator <(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x < rhs.x)>( lhs.x < rhs.x, lhs.y < rhs.y ); }
		
		public: template<typename t> requires( operable<t> ) friend constexpr auto
		operator<=(V2D const& lhs, V2D<t> const& rhs)
		{ return V2D<decltype(lhs.x <= rhs.x)>(lhs.x <= rhs.x, lhs.y <= rhs.y); }
		
		
		
		// Descriptors
		
		// Returns true if the vector angle pertains to Quadruant 1
		public: constexpr bool
		Q1() const
		{ return x >= 0 && y >= 0; }
		
		// Returns true if the vector angle pertains to Quadruant 2
		public: constexpr bool
		Q2() const
		{ return x <= 0 && y >= 0; }
		
		// Returns true if the vector angle pertains to Quadruant 3
		public: constexpr bool
		Q3() const
		{ return x <= 0 && y <= 0; }
		
		// Returns true if the vector angle pertains to Quadruant 4
		public: constexpr bool
		Q4() const
		{ return x >= 0 && y <= 0; }
		
		
		
		// Return this vector rotated 180 degrees
		public: constexpr V2D
		rotate180() const
		{ return -(*this); }
		
		// Return this vector rotated 90 degrees. If ClockWise == true, rotation will take place in a clock-wise motion. Counter clockwise otherwise.
		public: constexpr V2D
		rotate90(bool ClockWise = false) const
		{ return V2D( y - (2 * y) * !ClockWise , x - (2 * x) * ClockWise ); }
		
		// Return this vector rotated R radians
		public: constexpr V2D
		rotate(long double R) const {
			V2D out;
			
			out.x = std::cos(R) * x - std::sin(R) * y;
			out.y = std::sin(R) * x - std::cos(R) * y;
			
			return out;
		}
		
		// Absolute angle of this vector in radians.
		public: constexpr long double
		angle() const
		{ return std::atan2(static_cast<long double>(y),static_cast<long double>(x)); }
		
		// Dot product
		public: template<arithmetic t> constexpr decltype( std::declval<T>() * std::declval<t>() )
		dot(const V2D<t>& other) const
		{ return x * other.x + y * other.y; }
		
		// Cross product
		public: template<arithmetic t> constexpr decltype( std::declval<T>() * std::declval<t>() )
		cross(const V2D<t>& other) const
		{ return x * other.y - y * other.x; }
		
		// Returns vector aligned with the cardinal direction closest to this vector. That is to say, any of the vectors whose angle in degrees is divisible by 45.
		// This means any one of: 0, 45, 90, 135, 180, 225, 270, 315, 360.
		public: constexpr V2D
		cardinal() const
		{
			const V2D	abs{	std::abs(x),
								std::abs(y)			};
			
			V2D			out{	(x >= 0) - (x <= 0),
								(y >= 0) - (y <= 0)	};
			
			out.x *= (abs.x >= abs.y);
			out.y *= (abs.x <= abs.y);
			
			return out;
		}
		
		// Magnitude of the vector squared. Use if the true magnitude of the vector is unimportant, to save doing the square root with magnitude().
		public: constexpr unit
		magnitude2() const
		{ return (x * x) + (y * y); }
		
		// Magnituede of the vector
		public: constexpr unit
		magnitude() const
		{ return std::sqrt( magnitude2() ); }
		
		public: constexpr V2D
		normalize() const
		{
			auto len = magnitude();
			V2D out; ( len == 0 )
			|| ( ( out = V2D{ x/len , y/len } ) , true );
			return out;
		}
		
		public: constexpr T
		projection_mag(const V2D& other) const
		{ return dot(other) / other.magnitude(); }
		
		public: constexpr V2D
		projection_vec(const V2D& other) const
		{ return other.normalize() * projection_mag(other); }
		
		
		
		// Implicit conversions to pair and tuple
		
		public: constexpr
		operator std::pair<T, T>()
		{ return std::tie(x, y); }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		operator std::pair<t1, t2>() const
		{ using T1 = std::remove_reference_t<t1>; using T2 = std::remove_reference_t<t2>; return std::make_pair<T1, T2>( static_cast<T1>(x) , static_cast<T2>(y) ); }
		
		public: constexpr
		operator std::tuple<T, T>()
		{ return std::tie(x, y); }
		
		public: template<typename t1, typename t2> requires( operable<t1, t2> ) constexpr
		operator std::tuple<t1, t2>() const
		{ using T1 = std::remove_reference_t<t1>; using T2 = std::remove_reference_t<t2>; return std::make_tuple<T1, T2>( static_cast<T1>(x) , static_cast<T2>(y) ); }
		
		
		
		// String representation
		
		public: constexpr
		std::string fmt() const
		{ return std::to_string(x) + ',' + std::to_string(y); }
		
	};
	
}

#endif
