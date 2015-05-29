#ifndef VEC2_H
#define VEC2_H

#include <cmath>
using namespace std;

template<typename T>
class Vec2
{
public:
    T x,y;
public:
    Vec2():x(0),y(0){

    }

    Vec2(T x0, T y0):x(x0),y(y0){

    }

    ~Vec2(){

    }

    Vec2 operator +(Vec2 v) {
        Vec2 result;
        result.x = v.x + x;
        result.y = v.y + y;
        return result;
    }
    Vec2 operator -(Vec2 v) {
        Vec2 result;
        result.x = x - v.x;
        result.y = y - v.y;
        return result;
    }
    /*relates to angle between two vector
     * | a . b | = |a|.|b| . cos (theta)
     */
    T dotProduct(Vec2 v) {
        return x * v.x + y * v.y;
    }
    /*twice the area of the triangle between vector a and vector b,
     * positive and negative result means clockwise, counter clockwise;
     *  Another property of crossProduct related to the sine of the angle between the two vectors:
            | a x b | = |a| . |b| . sine(theta)
     */
    T crossProduct(Vec2 v2) {
        return x * v2.y - v2.x * y;
    }
    /*simulate the product of complex numbers*/
    Vec2 operator *(Vec2 v) {
        return  Vec2(x * v.x - y * v.y, x * v.y + y * v.x);
    }
    Vec2 conjugate() {
        return  Vec2(x, -y);
    }
    /*simulate the divide of complex numbers*/
    Vec2 operator /(Vec2 v) {
        Vec2 result = this * v.conjugate();
        return result.scale(1 / (v.x * v.x + v.y * v.y));
    }
    /*scalar product. Multiply a scala value not a vector value. */
    Vec2 scale(T n) {
        return  Vec2(x * n, y * n);
    }

    T abs() {
        return sqrt(x * x + y * y);
    }

    Vec2 normalize() {
        T a = abs();
        return  Vec2(x / a, y / a);
    }
    T distance(Vec2 other) {
        return (other - this).abs();
    }

    bool operator ==(Vec2 other) {
        if (other.x == x && other.y == y){
            return true;
        }
        return false;
    }
};

#endif // VEC2_H
