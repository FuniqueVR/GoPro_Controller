#pragma once
#include "base_window.h"

class CommandWindow : public BaseWindow {
public:
    CommandWindow(
        std::shared_ptr<json> _setting, 
        std::shared_ptr<GlobalState> _state, 
        std::shared_ptr<GoProMaster> _master);
    virtual ~CommandWindow();

    virtual void render() override;

private:
    void render_global();
    void render_local();
};