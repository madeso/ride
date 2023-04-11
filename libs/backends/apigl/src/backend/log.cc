#include "backend/log.h"

#include "backend/dependency_sdl.h"

namespace ride::apigl
{

namespace
{
    void
    log_implementation(SDL_LogPriority prio, fmt::string_view format, fmt::format_args args)
    {
        const auto category = SDL_LOG_CATEGORY_APPLICATION;
        const auto text = fmt::vformat(format, args);
        SDL_LogMessage(category, prio, "%s", text.c_str());
    }
}


void
log_info_implementation(fmt::string_view format, fmt::format_args args)
{
    log_implementation(SDL_LOG_PRIORITY_INFO, format, args);
}


void
log_error_implementation(fmt::string_view format, fmt::format_args args)
{
    log_implementation(SDL_LOG_PRIORITY_ERROR, format, args);
}


void
log_warning_implementation(fmt::string_view format, fmt::format_args args)
{
    log_implementation(SDL_LOG_PRIORITY_WARN, format, args);
}

}

