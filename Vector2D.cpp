#include "Vector2D.h"
#include <sstream>
#include <assert.h>

Vector2D::~Vector2D()
{
}

Vector2D Vector2D::Normalize() const
{
	Vector2D tmp = (*this) / this->Length();
	return tmp;
}

inline Vector2D & Vector2D::RotateBy(double rot)
{
	Vector2D newVec(GetRotation() + rot);
	newVec *= Length();
	*this = newVec;
	return *this;
}

inline double Vector2D::Length() const
{
	return sqrt(X * X + Y * Y);
}

inline double Vector2D::LengthSqr() const
{
	return X * X + Y * Y;
}

inline double Vector2D::Dot(const Vector2D& rhs) const
{
	return this->X * rhs.X + this->Y * rhs.Y;
}

inline Vector2D Vector2D::Reflect(const Vector2D & normal) const
{
	double len = this->Length();
	Vector2D result;
	result.X = this->X - 2 * len * normal.X;
	result.X = this->Y - 2 * len * normal.Y;
	return result;
}

inline Vector2D Vector2D::GetReverse() const
{
	return *this * -1;
}

inline void Vector2D::Turncate(double maxLen)
{
	double len = this->Length();
	if (len > maxLen) {
		*this = this->Normalize() * maxLen;
	}
}

inline double Vector2D::Distance(const Vector2D & rhs) const
{
	Vector2D offset = *this - rhs;
	return offset.Length();
}

inline double Vector2D::DistanceSqr(const Vector2D & rhs) const
{
	Vector2D offset = *this - rhs;
	return offset.LengthSqr();
}

std::string Vector2D::ToString() const
{
	std::stringstream ss;
	ss << "X: " << this->X << "Y: " << this->Y;
	return ss.str();
}

Vector2D& Vector2D::operator+=(const Vector2D & rhs) { *this = *this + rhs; return *this; }

Vector2D& Vector2D::operator-=(const Vector2D & rhs) { *this = *this - rhs; return *this; }

Vector2D& Vector2D::operator*=(const double & rhs) { *this = *this * rhs; return *this; }

Vector2D& Vector2D::operator/=(const double & rhs) { *this = *this / rhs; return *this; }

bool Vector2D::operator==(const Vector2D & rhs) const
{
	return this->X == rhs.X && this->Y == rhs.Y;
}

bool Vector2D::operator!=(const Vector2D & rhs) const
{
	return !(*this == rhs);
}

Vector2D operator+(const Vector2D & lhs, const Vector2D & rhs)
{
	Vector2D tmp = lhs;
	tmp.X += rhs.X;
	tmp.Y += rhs.Y;
	return tmp;
}

Vector2D operator-(const Vector2D & lhs, const Vector2D & rhs)
{
	Vector2D tmp = lhs;
	tmp.X -= rhs.X;
	tmp.Y -= rhs.Y;
	return tmp;
}

Vector2D operator*(const Vector2D & lhs, const double & rhs)
{
	Vector2D tmp = lhs;
	tmp.X *= rhs;
	tmp.Y *= rhs;
	return tmp;
}

Vector2D operator/(const Vector2D & lhs, const double & rhs)
{
	assert(abs(rhs) > 0.005);
	Vector2D tmp = lhs;
	tmp.X /= rhs;
	tmp.Y /= rhs;
	return tmp;
}
