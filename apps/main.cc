#include "ride/app.h"

#include "apiact/app.h"
#include "apiact/main.h"

using namespace ride::apigl;

int main(int argc, char** argv)
{
    return run_main(argc, argv, [](const StartupArguments&) -> std::unique_ptr<App> {
        return create_ride_app(create_platform());
    });
}

