#pragma once

#include <iostream>
#include <math.h>

//class Point3;

class Vector3
{
public:
    Vector3();

    Vector3(const Vector3& obj);

    Vector3(const Vector3& a, const Vector3& b);

    Vector3& operator=(const Vector3& obj);

    Vector3(float x, float y, float z);

    Vector3 operator-() const;

    Vector3 operator*(const float& l) const;

    Vector3 operator-(const float& l) const;

    Vector3 operator/(const float& l) const;

    Vector3 operator-(const Vector3& v) const;

    Vector3 operator+(const Vector3& v) const;

    Vector3 operator*(const Vector3& v) const;

    bool operator==(const Vector3& v) const;

    bool operator<(const Vector3& v) const;

    void operator+=(const Vector3& v);

    Vector3 get_normalized() const;

    void normalize();

    static Vector3 zero();

    Vector3 clamp(float min=0, float max=1);

    double magnitudeSquared() const;

    float x;
    float y;
    float z;

    float norm;

  private:
    static Vector3* m_zero;
};

std::ostream& operator<<(std::ostream& out, Vector3 &vect);

//Produit vectoriel
Vector3 crossProd(const Vector3& v1, const Vector3& v2);

//Produit scalaire
float dotProd(const Vector3& v1, const Vector3& v2);

//float operator^(const Vector3& v, const Point3& p);

float norm(const Vector3& a, const Vector3& b);

typedef Vector3 Color;
typedef Vector3 Point3;