#include "driver/audio.hpp"
#include "core/config.hpp"
#include "core/system_logger.hpp"
#include "core/notes.hpp"

namespace pixelino::driver {

void Audio::begin() {
    pinMode(core::config::gpio::audio, OUTPUT);
    core::SystemLogger::getInstance().logEvent(core::LogSource::DRIVER, "AUDIO", "AUDIO INITIATED");
}

void Audio::playTone(const core::Note& note) {
    playTone(note.pitch, note.duration);
}

void Audio::playTone(core::Pitch pitch, std::uint16_t duration) {
    playTone(static_cast<std::uint16_t>(pitch), duration);
}

void Audio::playTone(std::uint16_t frequency, std::uint16_t duration) {
    tone(core::config::gpio::audio, frequency, duration);
    m_isPlaying = true;
}

void Audio::stop() {
    noTone(core::config::gpio::audio);
    m_isPlaying = false;
}

} // namespace pixelino::driver