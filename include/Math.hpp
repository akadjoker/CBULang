#pragma once
#include <cmath>
#include <raymath.h>

const float RECIPROCAL_PI = 1.0f / PI;
const float HALF_PI = PI / 2.0f;
const float DEGTORAD = PI / 180.0f;
const float RADTODEG = 180.0f / PI;

#define PI2 PI * 2
#define DEG -180 / PI
#define RAD PI / -180
#define r2d -180 / PI
#define d2r PI / -180

#define COLLIDE_MAX(a, b) ((a > b) ? a : b)
#define COLLIDE_MIN(a, b) ((a < b) ? a : b)
#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SGN(a) (((a) < 0) ? -1 : !(a) ? 0 \
                                      : 1)
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define between(a, b, c) ((a) >= MIN(b, c) && (a) <= MAX(b, c))

inline double normalizeAngle(double angle)
{
    // move into range [-180 deg, +180 deg]
    while (angle < -PI)
        angle += PI * 2.0;
    while (angle > PI)
        angle -= PI * 2.0;
    return angle;
}

inline int sign(double value)
{
    return value < 0 ? -1 : (value > 0 ? 1 : 0);
}

inline double Hermite(double value1, double tangent1, double value2, double tangent2, double amount)
{
    double v1 = value1;
    double v2 = value2;
    double t1 = tangent1;
    double t2 = tangent2;
    double s = amount;
    double result;
    double sCubed = s * s * s;
    double sSquared = s * s;

    if (amount == 0)
        result = value1;
    else if (amount == 1)
        result = value2;
    else
        result = (2 * v1 - 2 * v2 + t2 + t1) * sCubed +
                 (3 * v2 - 3 * v1 - 2 * t1 - t2) * sSquared +
                 t1 * s +
                 v1;
    return result;
}
inline double clamp(double value, double min, double max)
{
    return value < min ? min : (value > max ? max : value);
}

inline double SmoothStep(double value1, double value2, double amount)
{

    double result = clamp(amount, 0, 1);
    result = Hermite(value1, 0, value2, 0, result);

    return result;
}

inline double ClampFloat(double value, double min, double max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}

inline int ClampInt(int value, int min, int max)
{
    if (value < min)
        value = min;
    else if (value > max)
        value = max;
    return value;
}
inline double Floor(double f) { return (double)floor(f); }

inline double Repeat(double t, double length)
{
    return ClampFloat(t - Floor(t / length) * length, 0.0f, length);
}
inline double PingPong(double t, double length)
{
    t = Repeat(t, length * 2.0f);
    return length - ABS(t - length);
}

inline double cos_deg(double angle)
{
    return cos(angle * M_PI / 180.0);
}

inline double sin_deg(double angle)
{
    return sin(angle * M_PI / 180.0);
}

inline double lerp(double a, double b, double t)
{
    return a + t * (b - a);
}

inline double lerp_angle(double start_angle, double end_angle, double t)
{
    double delta = end_angle - start_angle;
    if (delta > 180.0)
        delta -= 360.0;
    if (delta < -180.0)
        delta += 360.0;
    return start_angle + delta * t;
}

inline double distance_between_points(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    return sqrt(dx * dx + dy * dy);
}

inline void rotate_point(double *x, double *y, double cx, double cy, double angle)
{
    double radians = angle * M_PI / 180.0;
    double translated_x = *x - cx;
    double translated_y = *y - cy;

    double rotated_x = translated_x * cos(radians) - translated_y * sin(radians);
    double rotated_y = translated_x * sin(radians) + translated_y * cos(radians);

    *x = rotated_x + cx;
    *y = rotated_y + cy;
}

inline void move_along_line(double *x, double *y, double x1, double y1, double x2, double y2, double speed)
{
    double distance = distance_between_points(x1, y1, x2, y2);
    double t = speed / distance;

    *x = lerp(x1, x2, t);
    *y = lerp(y1, y2, t);
}

inline double fget_angle(double x1, double y1, double x2, double y2)
{
    double dx = x2 - x1;
    double dy = y2 - y1;
    double angle = atan2(dy, dx) * 180.0 / M_PI;
    return angle >= 0 ? angle : angle + 360.0;
}

inline double near_angle(double angle, double dest, double incr)
{
    if (angle < dest && dest - angle > 180.0)
        angle += 360.0;
    if (angle > dest && angle - dest > 180.0)
        angle -= 360.0;

    if (angle < dest)
    {
        angle += incr;
        if (angle > dest)
            angle = dest;
    }
    else
    {
        angle -= incr;
        if (angle < dest)
            angle = dest;
    }

    if (angle < 0)
        return angle + 360.0;
    if (angle >= 360.0)
        return angle - 360.0;
    return angle;
}

inline void rotate_lerp_to(double *x, double *y, double cx, double cy, double current_angle, double dest_angle, double t)
{
    double new_angle = lerp_angle(current_angle, dest_angle, t);

    double radians = new_angle * M_PI / 180.0;
    double translated_x = *x - cx;
    double translated_y = *y - cy;

    double rotated_x = translated_x * cos(radians) - translated_y * sin(radians);
    double rotated_y = translated_x * sin(radians) + translated_y * cos(radians);

    *x = rotated_x + cx;
    *y = rotated_y + cy;
}

inline void rotate_left(double *x, double *y, double cx, double cy, double angle)
{
    double radians = angle * M_PI / 180.0;
    double translated_x = *x - cx;
    double translated_y = *y - cy;

    double rotated_x = translated_x * cos(radians) - translated_y * sin(radians);
    double rotated_y = translated_x * sin(radians) + translated_y * cos(radians);

    *x = rotated_x + cx;
    *y = rotated_y + cy;
}

inline double degtorad(double degree)
{
    return degree * M_PI / 180.0;
}

inline double radtodeg(double radian)
{
    return radian * 180.0 / M_PI;
}