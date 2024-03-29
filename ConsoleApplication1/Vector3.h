#ifndef _VECTOR_3_H_
#define _VECTOR_3_H_

#include <iostream>
#include <string>
#include <cmath>
#include <math.h>

constexpr auto PI = 3.141592635;
constexpr auto TWO_PI = 6.28318527;


class Vector3
{
public:
    float x,y,z;

    Vector3();
    Vector3(float X, float Y, float Z);
    void SetVector3(float X, float Y, float Z);

    ~Vector3() {};

    Vector3 operator+(const Vector3& other);
    Vector3 operator-(const Vector3& other);
    Vector3 operator*(float num);
    Vector3 operator/(float num);

    Vector3& operator+=(const Vector3& other);
    Vector3& operator-=(const Vector3& other);
    Vector3& operator*=(float val);
    Vector3& operator/=(float val);
    bool operator!=(const Vector3& other);

    float GetSquaredMagnitude();
    float GetMagnitude();
    float GetDotProduct(const Vector3& other);
    Vector3 GetCrossProduct(const Vector3& other);
    Vector3 GetNormalization();
    void Normalize();
    void SetMagnitude(float val);
    void SetLimit(float limit);
    float GetAngleBetween(Vector3 v);
	static Vector3 Lerp(Vector3& A, Vector3& B, float t) {
		return A * t + B * (1.f - t);
		//return (A + (B - A) * t);
	}
};

class Vector4
{
public:
    float r,g,b,a;

    Vector4()
    {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
        a = 0.0f;
    }

    Vector4(float R, float G, float B, float A)
    {
        r = R;
        g = G;
        b = B;
        a = A;
    }

    ~Vector4() {};
};

#endif // _VECTOR3_H_
