/// main.cpp
/// ---

///
/// Entry Point file of application
///

#include "Application.h"

extern Application* createApplication();

int main(int argc, char* argv[])
{
    Application* app = createApplication();
    app->run();
    delete app;

    return 0;
}

