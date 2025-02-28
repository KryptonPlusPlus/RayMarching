#ifndef SPHERE_H
#define SPHERE_H

#include "src/math/vec3.h"

inline real_t distanceFromSphere(const vec3& p, const vec3& c, real_t r)
{
    return norm(p - c) - r;
}

#endif /* SPHERE_H */