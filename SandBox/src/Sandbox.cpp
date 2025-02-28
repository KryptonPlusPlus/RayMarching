// g++ -Wall -fopenmp -O3 --std=c++14 -IRayMarching/ SandBox/src/*.cpp RayMarching/src/*.cpp
// time (ms): 445.876

// g++ -Wall -ggdb -g -pg -O0 --std=c++14 -IRayMarching/ SandBox/src/*.cpp RayMarching/src/*.cpp


//https://github.com/gitEllE-if/ray_marcher
#include <RayMarching.h>

#include <iostream>
#include <cmath>
#include <chrono>

real_t hitScene(const vec3& current_position);
vec3   calculateNormal(const vec3& pos);

// ppm
void   printImage(uint8_t *buffer, uint16_t width, uint16_t height);

class SandBox : public Application
{
public:
    SandBox()
        : Application(1080, 1080)
    {

    }

    ~SandBox() override
    {

    }

    void onAttach() override
    {
        // marca tempo de inicio
        t1 = std::chrono::high_resolution_clock::now();
    }

    void onUpdate(float timestep) override
    {


        // one cycle
        m_running = false;
    }

    void onExit() override
    {
        // marca tempo de fim
        auto t2 = std::chrono::high_resolution_clock::now();

        // calcula a duracao
        std::chrono::duration<double, std::milli> duration = t2 - t1;

        std::clog << "time (ms): " << duration.count() << std::endl;

        //
        printImage(m_buffer, m_width, m_height);
    }

    vec3 fragment(real_t u, real_t v) override
    {   
        constexpr vec3 camera_position = vec3{0.0f, 0.0f, -5.0f};
        const     vec3 ray_direction   = vec3{u, v, 1.0f};

        constexpr uint32_t number_of_lights = 2;
        constexpr vec3 light_postion[number_of_lights]{vec3{2.0f, 5.0f, -3.0f}, vec3{-2.0f,  5.0f, 3.0f}};

        float total_distance_traveled = rayMarching(Ray{camera_position, ray_direction}, hitScene);

        if(total_distance_traveled == 0.0f)
            return vec3{0.0f, 0.0f, 0.0f};

        vec3 current_position = camera_position + ray_direction * total_distance_traveled;
        vec3 normal = calculateNormal(current_position);

        real_t diffuse_intensity = 0.0f;
        for(uint32_t i = 0; i < number_of_lights; ++i)
        {
            vec3 direction_light = normalize(light_postion[i] - current_position);
            diffuse_intensity = std::max(diffuse_intensity, normal * direction_light);
        }

        //
        return vec3{1.0f, 0.0f, 0.0f} * diffuse_intensity;
    }

private:
    std::chrono::system_clock::time_point t1;
};

Application* createApplication()
{
    return new SandBox();
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
            for(uint16_t c = 0; c < 3; c++)
                std::cout << (int32_t)buffer[c + x * 3 + y * width * 3] << " ";
        std::cout << "\n";
    }
    // ---
}

// https://iquilezles.org/articles/smin/
float smooth_min( float a, float b, float k )
{
    float x = b - a;
    return 0.5f * (a + b - std::sqrt(x * x + k * k));
}

real_t hitScene(const vec3& current_position)
{
    constexpr real_t displacement = 0.f;
	   
    real_t d1 = distanceFromSphere(current_position, vec3{1.0f, 0.0f, 0.0f}, 1.0f)  + displacement;
    real_t d2 = distanceFromSphere(current_position, vec3{-1.0f, 0.0f, 0.0f}, 1.0f) + displacement;

    real_t d3 = distanceFromPlane(current_position, vec3{ 0.0f, -5.0f, 5.0f}, vec3{ 0.0f,  1.0f, 0.0f});

    return std::min(smooth_min(d1, d2, .1f), d3);

    // vec3 teste = {current_position.x - std::round(current_position.x), 
    //               current_position.y - std::round(current_position.y), 
    //               current_position.z};

    // return distanceFromSphere(teste, vec3{0.0f, 0.0f, 0.0f}, .25f);
}

vec3 calculateNormal(const vec3& pos)
{
    // constexpr real_t epsilon = 0.001f / 2.0f;

    // real_t dx = hitScene(vec3{epsilon,    0.0f,    0.0f} + pos) - hitScene(vec3{-epsilon,     0.0f,     0.0f} + pos); 
    // real_t dy = hitScene(vec3{   0.0f, epsilon,    0.0f} + pos) - hitScene(vec3{    0.0f, -epsilon,     0.0f} + pos);
    // real_t dz = hitScene(vec3{   0.0f,    0.0f, epsilon} + pos) - hitScene(vec3{    0.0f,     0.0f, -epsilon} + pos);

    // return normalize(vec3{dx, dy, dz});

    // https://iquilezles.org/articles/normalsSDF/
    constexpr real_t epsilon = 0.001f / 2.0f;

    constexpr vec3 xyy { 1.0f, -1.0f, -1.0f};
    constexpr vec3 yyx {-1.0f, -1.0f,  1.0f};
    constexpr vec3 yxy {-1.0f,  1.0f, -1.0f};
    constexpr vec3 xxx { 1.0f,  1.0f,  1.0f};

    vec3 a = xyy * hitScene(pos + xyy * epsilon); 
    vec3 b = yyx * hitScene(pos + yyx * epsilon);
    vec3 c = yxy * hitScene(pos + yxy * epsilon);
    vec3 d = xxx * hitScene(pos + xxx * epsilon);

    return normalize(a + b + c + d);
}
