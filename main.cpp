#include <iostream>
#include <cmath>
#include <chrono>

using real_t = float;

struct vec3;

       void   printImage(uint8_t *buffer, uint16_t width, uint16_t height);
inline real_t distanceFromSphere(const vec3& p, const vec3& c, real_t r);
inline real_t distanceFromPlane(const vec3& p, const vec3& point, const vec3& normal);
       real_t hitScene(const vec3& current_position);
       vec3   calculateNormal(const vec3& pos);
inline real_t length(const vec3& u);
inline vec3   normalize(const vec3& u);

       vec3 fragment(real_t u, real_t v);

struct vec3
{
    real_t x, y, z;

    inline vec3 operator+(const real_t r) const
    {
        return vec3{r + this->x, r + this->y, r + this->z};
    }

    inline vec3 operator+(const vec3& u) const
    {
        return vec3{u.x + this->x, u.y + this->y, u.z + this->z};
    }

    inline vec3 operator-(const vec3& u) const 
    {
        return vec3{this->x - u.x, this->y - u.y , this->z - u.z};
    }

    inline vec3 operator*(const real_t r) const 
    {
        return vec3{this->x * r, this->y * r , this->z * r};
    }

    inline real_t operator*(const vec3& v) const
    {
        return this->x * v.x + this->y * v.y + this->z * v.z;
    }
};

vec3 rayMarching(vec3 ray_direction)
{
    //
    real_t total_distance_traveled = 0.0f;

    constexpr uint32_t number_of_steps      = 64;
    constexpr real_t   max_distance         = 10000.0f;
    constexpr real_t   minimun_hit_distance = 0.001f;
    constexpr vec3     camera_position      = vec3{0.0f, 0.0f, -5.0f};
              vec3     light_postion_1      = vec3{ 2.0f, 5.0f, -3.0f};
              vec3     light_postion_2      = vec3{-2.0f,  5.0f, 3.0f};

    // ---
    for(uint32_t step = 0; step < number_of_steps; ++step)
    {
        vec3 current_position = camera_position + ray_direction * total_distance_traveled;
        
        real_t distance_to_closest = hitScene(current_position);

        //
        if(distance_to_closest < minimun_hit_distance)
        {
            vec3 direction_light_1 = normalize(light_postion_1 - current_position);
            vec3 direction_light_2 = normalize(light_postion_2 - current_position);

            vec3 normal = calculateNormal(current_position);

            real_t diffuse_intensity = std::max(std::max(0.0f, normal * direction_light_1), std::max(0.0f, normal * direction_light_2));

            return vec3{1.0f, 0.0f, 0.0f} * diffuse_intensity;
        }

        if(total_distance_traveled > max_distance)
            break;

        total_distance_traveled += distance_to_closest;
    }

    return vec3{0.0f, 0.0f, 0.0f};
}

int main()
{
    // create buffer
    constexpr uint16_t width = 1080, height = 1080;

    uint8_t buffer[width * height * 3];

    // marca tempo de inicio
    auto t1 = std::chrono::high_resolution_clock::now();

    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            real_t v = static_cast<real_t>((height - y)) / height * 2.0f - 1.0f;
            real_t u = static_cast<real_t>(x)            / width  * 2.0f - 1.0f;

            vec3 color = fragment(u, v);

            buffer[0 + x * 3 + y * width * 3] = static_cast<uint8_t>(color.x * 255.9999f);
            buffer[1 + x * 3 + y * width * 3] = static_cast<uint8_t>(color.y * 255.9999f);
            buffer[2 + x * 3 + y * width * 3] = static_cast<uint8_t>(color.z * 255.9999f);
        }
    }
    // ---

    // marca tempo de fim
    auto t2 = std::chrono::high_resolution_clock::now();

    // calcula a duracao
    std::chrono::duration<double, std::milli> duration = t2 - t1;

    std::clog << "time (ms): " << duration.count() << std::endl;

    printImage(buffer, width, height);

    return 0;
}

vec3 fragment(real_t u, real_t v)
{   
    return rayMarching(vec3{u, v, 1.0f});
}

void printImage(uint8_t *buffer, uint16_t width, uint16_t height)
{
    // --- file ppm ---
    // header ppm
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    // print buffer ppm
    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            for(uint16_t c = 0; c < 3; c++)
            {
                std::cout << (int32_t)buffer[c + x * 3 + y * width * 3] << " ";
            }
        }
        std::cout << "\n";
    }
    // ---
}

inline real_t distanceFromSphere(const vec3& p, const vec3& c, real_t r)
{
    return length(p - c) - r;
}

inline real_t distanceFromPlane(const vec3& p, const vec3& point, const vec3& normal)
{
    return std::abs((point - p) * normalize(normal * -1.0f));
} 

real_t hitScene(const vec3& current_position)
{
    real_t displacement = 0.0f;
	   
    real_t d1 = distanceFromSphere(current_position, vec3{2.0f, 0.0f, 0.0f}, 1.0f)  + displacement;
    real_t d2 = distanceFromSphere(current_position, vec3{-2.0f, 0.0f, 0.0f}, 1.0f) + displacement;

    real_t d3 = distanceFromPlane(current_position, vec3{ 0.0f, -5.0f, 5.0f}, vec3{ 0.0f,  1.0f, 0.0f});

    return std::min(std::min(d1, d2), d3);
}

// --- vec3 operators ---
inline real_t length(const vec3& u)
{
    return std::sqrt(u.x * u.x + u.y * u.y + u.z * u.z);
}

inline vec3 normalize(const vec3& u)
{
    return u * (1.0f / length(u));
}

vec3 calculateNormal(const vec3& pos)
{
    constexpr real_t small_step = 0.001f / 2.0f;

    real_t dx = hitScene(vec3{small_step,       0.0f,       0.0f} + pos) - hitScene(vec3{-small_step,        0.0f,        0.0f} + pos); 
    real_t dy = hitScene(vec3{      0.0f, small_step,       0.0f} + pos) - hitScene(vec3{       0.0f, -small_step,        0.0f} + pos);
    real_t dz = hitScene(vec3{      0.0f,       0.0f, small_step} + pos) - hitScene(vec3{       0.0f,        0.0f, -small_step} + pos);

    return normalize(vec3{dx, dy, dz});
}

// ---

