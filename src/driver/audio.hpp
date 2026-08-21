#pragma once

#include <cstdint>
#include "core/notes.hpp"

namespace pixelino::driver {

class Audio {
public:
    // single global instance
    static Audio& getInstance() { static Audio instance; return instance; }
    Audio(const Audio&) = delete;
    void operator=(const Audio&) = delete;    

    // basic setup (configure pins)
    void begin();

    // --- Simple Sound Effects ---
    // play a single note immediately
    void playTone(const core::Note& note);
    void playTone(core::Pitch pitch, std::uint16_t duration);
    void playTone(std::uint16_t frequency, std::uint16_t duration);
    void stop();

    // --- RTTTL / Music Playback ---
    // pass the raw RTTTL string here. It will reset the parser and start playing.
    //void playRTTTL(const char* rtttl_string);

    // for more complex audio handling
    //void tick();

    // helper to check if music is currently running
    bool isPlaying() const { return m_isPlaying; };

private:
    // private constructor
    Audio() = default;

    // state variables for non-blocking playback
    bool m_isPlaying{false};
    /* variables fore future implentation of more complex audio options
    std::uint32_t m_noteStartTime{0};
    std::uint16_t m_currentDuration{0};

    const char* m_currentSong{nullptr};
    
    // internal parser state (keeps track of where we are in the string)
    std::uint16_t m_defaultDuration{4};
    std::uint16_t m_defaultOctave{6};
    std::uint16_t m_bpm{63};
    
    // helper function to read the next note from m_currentSong
    core::Note parseNextNote(); 
    */
};

} // namespace pixelino::driver