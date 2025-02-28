#ifndef RAYMARCHING_H
#define RAYMARCHING_H

#include <inttypes.h>
#include <functional>

#include "Ray.h"
//
real_t rayMarching(Ray ray, real_t (*hitScene)(const vec3& current_position))
{
    // ray marching config
    constexpr uint32_t number_of_steps      = 512;
    constexpr real_t   max_distance         = 10000.0f;
    constexpr real_t   minimun_hit_distance = 0.001f;
    //

    real_t total_distance_traveled = 0.0f;
    // ---
    for(uint32_t step = 0; step < number_of_steps; ++step)
    {
        vec3 current_position = ray.origin + ray.direction * total_distance_traveled; // ray
        
        real_t distance_to_closest = hitScene(current_position);

        //
        if(distance_to_closest < minimun_hit_distance)
            return total_distance_traveled;

        total_distance_traveled += distance_to_closest;

        if(total_distance_traveled > max_distance)
            break;
    }

    return 0.0f;
}

#endif /* RAYMARCHING_H */