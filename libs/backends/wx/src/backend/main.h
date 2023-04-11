#pragma once

#include <functional>

#include "api/app.h"
#include "backend/dependency_wx.h"


namespace ride::backends::gl
{

struct StartupArguments
{
};

using CreateAppFunction = std::function<std::unique_ptr<App>(const StartupArguments&)>;

struct ApiApp : public wxApp
{
    bool create_app(CreateAppFunction create_app);
    // bool OnInit() override;
};

}


#define IMPLMENT_APP(APP_NAME, MAKE_APP) struct MyApp : ::ride::backends::gl::ApiApp { bool OnInit() override { return create_app([](const ride::backends::gl::StartupArguments& APP_NAME){ return MAKE_APP; });}};  wxIMPLEMENT_APP(MyApp);

