#ifndef VEC3_H
#define VEC3_H

#include <math.h>

#include "real_t.h"

struct vec3
{
    union {
        real_t v[3];
        struct {
            real_t x, y, z;
        };
    };

    inline vec3   operator+(const real_t r) const
    {   
        return vec3{r + this->x, r + this->y, r + this->z};
    }
    inline vec3   operator+(const vec3&  u) const
    {
        return vec3{this->x + u.x, this->y + u.y, this->z + u.z};
    }
    inline vec3   operator-(const vec3&  u) const
    {
        return vec3{this->x - u.x, this->y - u.y , this->z - u.z};
    }
    inline vec3   operator*(const real_t r) const
    {
        return vec3{this->x * r, this->y * r , this->z * r};
    }
    inline real_t operator*(const vec3&  v) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }
};

// --- vec3 operators ---
// Norma de um vetor u (|u|)
inline real_t norm(const vec3& u) 
{
    return std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}
// Normalizar um vetor u (u / |u|) (vetor unitario)
inline vec3 normalize(const vec3& u)
{
    return u * (static_cast<real_t>(1.0) / norm(u));
}
// ---

#endif /* VEC3_H */
