#pragma once

#include <Arduino.h>
#include <SimpleCLI.h>

namespace pixelino::app {

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
    ServiceCLI() = default; // Private constructor

    SimpleCLI m_cli;
    bool m_isActive = false;
	String m_inputBuffer = "";

	// CLI callback funtions
    static void pingCallback(cmd* c);
    static void exitCallback(cmd* c);
	static void helpCallback(cmd* c);
    static void displayCallback(cmd* c);
    static void logCallback(cmd* c);
    static void setErrorModeCallback(cmd* c);
    static void getErrorModeCallback(cmd* c);
    static void cliErrorCallback(cmd_error* e);
};

}