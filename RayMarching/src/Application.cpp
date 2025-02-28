#include "Application.h"

Application::Application(uint16_t window_width, uint16_t window_height)
    : m_width(window_width), m_height(window_height), m_buffer(new uint8_t[m_width * m_height * 3])
{

}

Application::~Application()
{
    delete[] m_buffer;
}

void Application::run()
{
    onAttach();

    while(m_running)
    {
        onUpdate(0.f);

        // screen update
        for(uint16_t y = 0; y < m_height; y++)
        {
#pragma omp parallel for
            for(uint16_t x = 0; x < m_width; x++)
            {
                real_t v = static_cast<real_t>((m_height - y)) / m_height * 2.0f - 1.0f;
                real_t u = static_cast<real_t>(x)              / m_width  * 2.0f - 1.0f;

                vec3 color = fragment(u, v);

                m_buffer[0 + x * 3 + y * m_width * 3] = static_cast<uint8_t>(color.x * 255.9999f);
                m_buffer[1 + x * 3 + y * m_width * 3] = static_cast<uint8_t>(color.y * 255.9999f);
                m_buffer[2 + x * 3 + y * m_width * 3] = static_cast<uint8_t>(color.z * 255.9999f);
            }
        }
    }

    onExit();
}
