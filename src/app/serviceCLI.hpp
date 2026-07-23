#pragma once

#include <Arduino.h>
#include <SimpleCLI.h>  // props to spacehuhn for this amazing library

class ServiceCLI {
public:
    ServiceCLI() = default;
    void init();
    void activate();
    void deactivate();
    void tick();

private:
    SimpleCLI m_cli;
    static void errorCallback(cmd_error* e);
    static void pingCallback(cmd* c);
};
