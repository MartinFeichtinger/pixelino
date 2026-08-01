#pragma once

#include "core/error_types.hpp"
#include <SimpleCLI.h>

namespace pixelino::driver { class ButtonManager; } // forward declaration

namespace pixelino::app {

class ServiceCLI {
public:
    // singleton defintion
    static ServiceCLI& getInstance() { static ServiceCLI instance; return instance; }
    ServiceCLI(const ServiceCLI&) = delete;
    void operator=(const ServiceCLI&) = delete;

	void begin(driver::ButtonManager& buttonManager);
    void activate();
    void deactivate();
    void toggle();
    void tick();
    void print(const String& msg);

    // public helper functions
    void printLiveError(core::ErrorCode code, core::ErrorMode mode);
    void printSystemCrashMsg();
    void printLogHistory();

private:
    ServiceCLI() = default;

    SimpleCLI m_cli;
    bool m_isActive = false;
    bool m_parsingInput = false;
	String m_inputBuffer = "";

    // private helpers
    void printFormattedTimestamp(uint32_t ms);

	// CLI callback funtions
    static void pingCallback(cmd* c);
    static void exitCallback(cmd* c);
	static void helpCallback(cmd* c);
    static void displayCallback(cmd* c);
    static void logCallback(cmd* c);
    static void errorHandlerCallback(cmd* c);
    static void cliErrorCallback(cmd_error* e);
};

}