#include <Arduino.h>
#include <FastLED.h>

#define DATA_PIN    4
#define NUM_LEDS    64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

// ---------- Display modes ----------
enum DisplayMode { MODE_CALIBRATE, MODE_OVERVIEW, MODE_SWEEP };
DisplayMode currentMode = MODE_CALIBRATE;

// Sweep mode speed (how fast the hue value keeps rotating)
#define SWEEP_HUE_STEP 1
#define SWEEP_DELAY_MS 15
uint8_t sweepHue = 0;

// ---------- Calibration mode ----------
// 16 anchor points distributed around the hue circle (step 16: 0,16,32,...,240)
#define NUM_ANCHORS 16
#define ANCHOR_STEP (256 / NUM_ANCHORS)   // = 16

// Base brightness: this is the starting value that most hues (which already
// look "too bright") should get. Only the few dark outliers get corrected
// upward from here (up to a max of 255).
#define BASE_V 180

// Correction value per anchor point: 128 = base brightness (no change),
// > 128 = brighter than base, < 128 = darker than base
uint8_t correctionTable[NUM_ANCHORS];
uint8_t currentAnchor = 0;

// Set to true whenever something changes that requires a redraw
// (mode switch, anchor switch, correction change, reset, ...)
bool needsUpdate = true;

// --- Funktionsprototypen für Standard C++ (PlatformIO) ---
void resetCorrectionTable();
uint8_t getInterpolatedRawCorrection(uint8_t hue);
uint8_t getFinalV(uint8_t rawCorrection);
void printMenu();
void printCurrentAnchor();
void printTableAsCode();
void handleSerialInput();

void resetCorrectionTable() {
  for (uint8_t i = 0; i < NUM_ANCHORS; i++) correctionTable[i] = 128;
}

// Linear interpolation of the raw correction value (0-255) between the
// two neighboring anchor points
uint8_t getInterpolatedRawCorrection(uint8_t hue) {
  uint8_t index = hue / ANCHOR_STEP;
  uint8_t nextIndex = (index + 1) % NUM_ANCHORS;
  uint8_t fracRaw = hue % ANCHOR_STEP;
  // 0..15
  uint8_t frac = map(fracRaw, 0, ANCHOR_STEP - 1, 0, 255);
  return lerp8by8(correctionTable[index], correctionTable[nextIndex], frac);
}

// Converts the raw correction value (128 = neutral) into the actual
// V value (brightness), based on the base brightness BASE_V
uint8_t getFinalV(uint8_t rawCorrection) {
  int16_t v = (int16_t)BASE_V + ((int16_t)rawCorrection - 128);
  return (uint8_t)constrain(v, 0, 255);
}

void printMenu() {
  Serial.println();
  Serial.println(F("=== Hue Brightness Calibration ==="));
  Serial.println(F("Correction value 128 = neutral (base brightness, currently BASE_V=180)."));
  Serial.println(F("Most points therefore need NO adjustment -"));
  Serial.println(F("only raise the few too-dark anchor points with '+'."));
  Serial.println();
  Serial.println(F("a : Overview mode (all hues shown simultaneously, static)"));
  Serial.println(F("w : Sweep mode (rainbow runs through, with correction)"));
  Serial.println(F("c : back to calibration mode (single anchor point)"));
  Serial.println(F("n : next anchor point"));
  Serial.println(F("p : previous anchor point"));
  Serial.println(F("+ : brighten current anchor point (above base)"));
  Serial.println(F("- : darken current anchor point (below base)"));
  Serial.println(F("r : reset all correction values (to 128/neutral)"));
  Serial.println(F("s : print current table as C++ array"));
  Serial.println(F("? : show this menu again"));
  Serial.println();
  printCurrentAnchor();
}

void printCurrentAnchor() {
  Serial.print(F("Anchor point "));
  Serial.print(currentAnchor);
  Serial.print(F("/"));
  Serial.print(NUM_ANCHORS - 1);
  Serial.print(F("  (Hue = "));
  Serial.print(currentAnchor * ANCHOR_STEP);
  Serial.print(F(")  Correction = "));
  Serial.print(correctionTable[currentAnchor]);
  Serial.print(F(" (128=neutral)  -> actual brightness = "));
  Serial.println(getFinalV(correctionTable[currentAnchor]));
}

void printTableAsCode() {
  Serial.println();
  Serial.println(F("// Copy this into your main program (including BASE_V!):"));
  Serial.println(F("#define BASE_V 180"));
  Serial.print(F("uint8_t correctionTable[16] = {"));
  for (uint8_t i = 0; i < NUM_ANCHORS; i++) {
    Serial.print(correctionTable[i]);
    if (i < NUM_ANCHORS - 1) Serial.print(F(", "));
  }
  Serial.println(F("};"));
  Serial.println(F("// Usage: v = constrain(BASE_V + (interpolatedValue - 128), 0, 255);"));
  Serial.println();
}

void handleSerialInput() {
  if (!Serial.available()) return;
  char c = Serial.read();

  switch (c) {
    case 'a': case 'A':
      currentMode = MODE_OVERVIEW;
      needsUpdate = true;
      Serial.println(F("Mode: OVERVIEW (all hues simultaneously, static)"));
      break;
    case 'w': case 'W':
      currentMode = MODE_SWEEP;
      sweepHue = 0;
      needsUpdate = true;
      Serial.println(F("Mode: SWEEP (rainbow runs through, with correction)"));
      break;

    case 'c': case 'C':
      currentMode = MODE_CALIBRATE;
      needsUpdate = true;
      Serial.println(F("Mode: CALIBRATE (single anchor point)"));
      printCurrentAnchor();
      break;
    case 'n': case 'N':
      currentAnchor = (currentAnchor + 1) % NUM_ANCHORS;
      needsUpdate = true;
      printCurrentAnchor();
      break;

    case 'p': case 'P':
      currentAnchor = (currentAnchor == 0) ?
                      (NUM_ANCHORS - 1) : (currentAnchor - 1);
      needsUpdate = true;
      printCurrentAnchor();
      break;
    case '+':
      correctionTable[currentAnchor] = min(255, correctionTable[currentAnchor] + 5);
      needsUpdate = true;
      printCurrentAnchor();
      break;
    case '-':
      correctionTable[currentAnchor] = max(0, (int)correctionTable[currentAnchor] - 5);
      needsUpdate = true;
      printCurrentAnchor();
      break;
    case 'r': case 'R':
      resetCorrectionTable();
      needsUpdate = true;
      Serial.println(F("Reset done."));
      break;
    case 's': case 'S':
      printTableAsCode();
      break;

    case '?':
      printMenu();
      break;
    // Ignore line breaks etc.
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); // give the USB-CDC connection time to settle
  Serial.println(F(">>> BOOT <<<"));

  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setDither(BINARY_DITHER);
  FastLED.setBrightness(50);
  FastLED.setMaxRefreshRate(60); // cap refresh rate at 60 fps

  resetCorrectionTable();
  printMenu();
}

void loop() {
  handleSerialInput();
  switch (currentMode) {
    case MODE_OVERVIEW: {
      if (needsUpdate) {
        // All 64 LEDs show different hues (0,4,8,...,252),
        // each with the currently calibrated correction applied
        for (uint16_t i = 0; i < NUM_LEDS; i++) {
          uint8_t hue = i * 4; // 64 LEDs * 4 = 256, covers the whole circle
          uint8_t rawCorr = getInterpolatedRawCorrection(hue);
          uint8_t v = getFinalV(rawCorr);
          CRGB rgb;
          hsv2rgb_rainbow(CHSV(hue, 255, v), rgb);
          leds[i] = rgb;
        }
        FastLED.show();
        needsUpdate = false;
      }
      break;
    }

    case MODE_SWEEP: {
      // Rainbow runs continuously over time, with correction applied -
      // a good final test to check whether the calibration also looks
      // good "in motion". This mode always updates, no needsUpdate check.
      uint8_t rawCorr = getInterpolatedRawCorrection(sweepHue);
      uint8_t v = getFinalV(rawCorr);
      CRGB rgb;
      hsv2rgb_rainbow(CHSV(sweepHue, 255, v), rgb);
      fill_solid(leds, NUM_LEDS, rgb);
      FastLED.show();

      sweepHue += SWEEP_HUE_STEP;
      FastLED.delay(SWEEP_DELAY_MS);
      break;
    }

    case MODE_CALIBRATE:
    default: {
      if (needsUpdate) {
        // Calibration mode: all LEDs show the hue of the current anchor point
        uint8_t hue = currentAnchor * ANCHOR_STEP;
        uint8_t v = getFinalV(correctionTable[currentAnchor]);
        CRGB rgb;
        hsv2rgb_rainbow(CHSV(hue, 255, v), rgb);
        fill_solid(leds, NUM_LEDS, rgb);
        FastLED.show();
        needsUpdate = false;
      }
      break;
    }
  }
}