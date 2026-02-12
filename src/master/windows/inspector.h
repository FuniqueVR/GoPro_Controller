#pragma once
#include "base_window.h"

class InspectorWindow : public BaseWindow {
public:
    InspectorWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~InspectorWindow();

    virtual void render() override;

private:

};