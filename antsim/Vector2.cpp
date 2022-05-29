//VECTOR2 CPP
#include "Vector2.h"
#include <stdlib.h>

//CONSTRUCTORS
Vector2::Vector2(void) : x(0), y(0) { }
Vector2::Vector2(float32 xValue, float32 yValue) : x(xValue), y(yValue) { }



Vector2::Vector2(const Vector2& v) : x(v.x), y(v.y) { }

Vector2::Vector2(const Vector2* v) : x(v->x), y(v->y) { }

Vector2 Vector2::FromAngle(float32 angle)
{
    float32 rad = angle * PI / 180.0f;
    return Vector2(cos(rad),sin(rad));
}

Vector2 Vector2::RandomUnitVector()
{
    float32 rad = ((float)rand() / (float)RAND_MAX) * 2.0 * PI;

    return Vector2(cosf(rad), sinf(rad));
}

Vector2 Vector2::RandomWithinMap() {
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    return Vector2(x, y);
}


//DECONSTRUCTOR
Vector2::~Vector2(void) { }

//METHODS
void Vector2::Set(float32 xValue, float32 yValue) { x = xValue; y = yValue; }

float32 Vector2::Length() const { return sqrt(x * x + y * y); }
float32 Vector2::LengthSquared() const { return x * x + y * y; }
float32 Vector2::Distance(const Vector2& v) const { return sqrt(((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y))); }
float32 Vector2::DistanceSquared(const Vector2& v) const { return ((x - v.x) * (x - v.x)) + ((y - v.y) * (y - v.y)); }
float32 Vector2::Dot(const Vector2& v) const { return x * v.x + y * v.y; }
float32 Vector2::Cross(const Vector2& v) const { return x * v.y + y * v.x; }

float32 Vector2::GetAngle() const
{
    return (atan2f(y,x)*180)/PI;
}

Vector2& Vector2::Rotate(float32 angle)
{
    angle = angle * PI / 180.0;

    float32 newX = x * cos(angle) - y * sin(angle);
    float32 newY = x * sin(angle) + y * cos(angle);

    x = newX;
    y = newY;
    return *this;
}

Vector2& Vector2::Normal() { Set(-y, x); return *this; }
Vector2& Vector2::Normalize()
{
    if (Length() != 0)
    {
        float32 length = Length();
        x /= length; y /= length;
        return *this;
    }

    x = y = 0;
    return *this;
}

Vector2& Vector2::ClampMagnitude(float32 magnitude)
{
    float32 length = Length();
    if (length > magnitude) {
        x = (x / length) * magnitude;
        y = (y / length) * magnitude;
    }
    return *this;
}
