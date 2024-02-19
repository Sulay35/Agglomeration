/*
// Created by aysul on 18/02/24.
*/
#include "../include/vector2.h"
Vector2::Vector2() {
    x = 0.0;
    y = 0.0;
}

Vector2::Vector2(double nx, double ny) {
    x = nx;
    y = ny;
}

Vector2 Vector2::operator+(const Vector2 &rhs) {
    return Vector2(x+rhs.x, y+rhs.y);
}

Vector2& Vector2::operator+=(const Vector2 &rhs) {
    x+=rhs.x;
    y+=rhs.y;
    return *this;
}

Vector2 Vector2::operator*(double rhs) const
{
    return Vector2(x*rhs, y*rhs);
}

Vector2 Vector2::operator/(double rhs) const
{
    return Vector2(x/rhs, y/rhs);
}

Vector2 Vector2::operator*=(double rhs) const
{
    return Vector2(x*rhs, y*rhs);
}

Vector2 Vector2::operator/=(double rhs) const
{
    return Vector2(x/rhs, y/rhs);
}

Vector2 Vector2::normalized(){
    Vector2 normalized = (*this)/norm();
    return normalized;
}


void Vector2::setMag(double magnitude){
    if(magnitude!=0.0f)
        *this = this->normalized() * magnitude;
    else
        *this = Vector2(0.0f, 0.0f);
    //printf("\t%f\tf = %f \n",magnitude, this->norm());
}

void Vector2::limit(double limit){
    if(this->norm() > limit){
        setMag(limit);
    }
}
double Vector2::norm() const
{
    return sqrt(x*x + y*y);
}

void Vector2::add(Vector2 *v2)
{
    x += v2->x;
    y += v2->y;
}











