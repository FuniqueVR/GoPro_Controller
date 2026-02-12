#include "websocket_server.h"

WebsocketWindow::WebsocketWindow(
    std::shared_ptr<json> _setting, 
    std::shared_ptr<GlobalState> _state, 
    std::shared_ptr<GoProMaster> _master
) 
    : BaseWindow(_setting, _state, _master) {
    title = "Websocket Dashboard";
}

WebsocketWindow::~WebsocketWindow(){
    
}

void WebsocketWindow::render(){

}