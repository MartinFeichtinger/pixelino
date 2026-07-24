#pragma once

#include <Arduino.h>
#include <SimpleCLI.h>
#include "config.hpp"

class ServiceCLI {
public:
    // Access the single global instance
    static ServiceCLI& getInstance();

    // Prevent accidental copying
    ServiceCLI(const ServiceCLI&) = delete;
    void operator=(const ServiceCLI&) = delete;

	void begin();
    void activate();
    void deactivate();
    void toggle();
    void tick();

    bool isActive() const { return m_isActive; }

private:
    ServiceCLI(); // Private constructor

    SimpleCLI m_cli;
    bool m_isActive = false;
	String m_inputBuffer = "";

	// CLI callback funtions
    static void errorCallback(cmd_error* e);
    static void pingCallback(cmd* c);
    static void exitCallback(cmd* c);
	static void helpCallback(cmd* c);
};