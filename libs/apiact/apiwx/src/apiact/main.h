#pragma once

#include <functional>

#include "api/app.h"
#include "apiact/dependency_wx.h"


namespace ride::apigl
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


#define IMPLMENT_APP(APP_NAME, MAKE_APP) struct MyApp : ::ride::apigl::ApiApp { bool OnInit() override { return create_app([](const ride::apigl::StartupArguments& APP_NAME){ return MAKE_APP; });}};  wxIMPLEMENT_APP(MyApp);

