#pragma once
/*Author: DXTST*/
#include <cmath>
#include <string>

struct Vector2D
{
	double X;
	double Y;

	static constexpr double ACCURACY = 0.005;
	//------------Constructor-----------------------------------------------------------

	/*Default constructor (All zero)*/
	Vector2D() : X(0.0), Y(0.0) {}
	//Double constructor
	Vector2D(double x, double y) :X(x), Y(y) {}
	//Rotation constructor
	explicit Vector2D(double rot) : X(cos(rot)), Y(sin(rot)) {}
	//Copy constructor
	Vector2D(const Vector2D&) = default;
	//Copy constructor
	Vector2D(Vector2D&&) = default;
	//Assignment operator
	Vector2D& operator =(const Vector2D&) = default;
	//Destructor
	~Vector2D();

	//------------Functions-------------------------------------------------------------

	/*Set X and Y to be zero*/
	void Zero() { X = 0.0; Y = 0.0; }
	/*Return true if the vector is zero vector*/
	bool isZero() const { return abs(X) < ACCURACY && abs(Y) < ACCURACY; }
	/*Rotate the vector by rot*/
	Vector2D& RotateBy(double rot);
	/*Return the unit vector of this vector*/
	Vector2D Normalize() const;
	/*Return the dot product of this and @rhs*/
	double Dot(const Vector2D& rhs) const;
	/*Return reflection vector according to the normal line*/
	Vector2D Reflect(const Vector2D& normal) const;
	/*Return the reverse vector of the vector*/
	Vector2D GetReverse() const;
	/*The modular length of the vector*/
	double Length() const;
	/*The length sqare of the vector (Better performance may be)*/
	double LengthSqr() const;
	/*Get the roation value of the vector*/
	double GetRotation() const { return atan2(Y, X); }
	/*Get the roation value of the vector*/
	double GetRotationDegree() const { return atan2(Y, X) / 6.28318530717 * 360; }
	/*Turncate the vector to force its length no more than max*/
	void Turncate(double maxLen);
	/*Return the distance between this and @rhs*/
	double Distance(const Vector2D& rhs) const;
	/*Return the distance's square between this and @rhs*/
	double DistanceSqr(const Vector2D& rhs) const;
	/*Display the string message of the vector (For debug)*/
	std::string ToString() const;

	//-------------Operator--------------------------------------------------------------

	//Negative
	inline Vector2D & operator-() { X = -X; Y = -Y; return *this; }
	//Addition
	Vector2D & operator+=(const Vector2D & rhs);
	//Subtraction
	Vector2D & operator-=(const Vector2D & rhs);
	//Multiply
	Vector2D & operator*=(const double & rhs);
	//Division 
	Vector2D & operator/=(const double & rhs);
	//Eauals
	bool operator ==(const Vector2D& rhs) const;
	//Eauals
	bool operator !=(const Vector2D& rhs) const;

	//-------------Other-----------------------------------------------------------------


};
Vector2D operator+(const Vector2D&, const Vector2D&);

Vector2D operator-(const Vector2D&, const Vector2D&);

Vector2D operator*(const Vector2D&, const double&);

Vector2D operator/(const Vector2D&, const double&);
