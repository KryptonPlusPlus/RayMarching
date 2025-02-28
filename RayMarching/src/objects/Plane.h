#ifndef PLANE_H
#define PLANE_H

#include "src/math/vec3.h"

inline real_t distanceFromPlane(const vec3& p, const vec3& point, const vec3& normal)
{
    return std::abs((point - p) * normalize(normal * -1.0f));
} 

#endif /* PLANE_H */