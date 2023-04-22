#include "ride/app.h"

#include "backend/app.h"
#include "backend/main.h"

using namespace ride::backends::gl;

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<ride::api::App>
    {
        return ride::libride::create_ride_app(create_platform());
    });
}

