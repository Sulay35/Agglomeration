/*
// Created by aysul on 18/02/24.
*/
#ifndef GRID_VECTOR_2_H
#define GRID_VECTOR_2_H

#include <cmath>

class Vector2{
public:
    Vector2();
    Vector2(double nx, double ny);
    Vector2 operator+(Vector2 const& rhs);
    Vector2& operator +=(Vector2 const& rhs);
    Vector2 operator*(double rhs) const;
    Vector2 operator/(double rhs) const;
    Vector2 operator*=(double rhs) const;
    Vector2 operator/=(double rhs) const;
    Vector2 normalized();
    void setMag(double magnitude);
    void limit(double limit);
    [[nodiscard]] double norm() const;
    void add(Vector2 *v2);
    double x,y;
};
#endif //GRID_VECTOR_2_H
