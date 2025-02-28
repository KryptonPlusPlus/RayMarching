#ifndef APPLICATION_H
#define APPLICATION_H

#include <inttypes.h>

#include "math/vec3.h"

class Application
{
public:
        Application(uint16_t window_width, uint16_t window_height);
        virtual ~Application();

        virtual void onAttach() = 0;
        virtual void onUpdate(float timestep) = 0; // TODO:
        virtual vec3 fragment(real_t u, real_t v) = 0; 
        virtual void onExit() = 0;

        //
        bool m_running = true;

private:
        void run();

        friend int main(int argc, char** argv);
protected:
        // screen
        uint16_t m_width;
        uint16_t m_height;

        uint8_t* m_buffer;
};

//
Application* createApplication();

#endif /* APPLICATION_H */
