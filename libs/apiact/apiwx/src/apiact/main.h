#pragma once

#include <functional>

#include "api/app.h"
#include "apiact/dependency_wx.h"


namespace ride::apigl
{
using CreateAppFunction = std::function<std::unique_ptr<App> ()>;

struct ApiApp : public wxApp
{
    bool create_app(CreateAppFunction create_app);
    // bool OnInit() override;
};
}


#define IMPLMENT_APP(APP) struct MyApp : ::ride::apigl::ApiApp { bool OnInit() override { return create_app([]{ std::make_unique<APP>(); });}};  wxIMPLEMENT_APP(MyApp);

