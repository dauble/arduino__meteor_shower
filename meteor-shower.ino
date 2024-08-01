#include <FastLED.h>
#define NUM_LEDS 42
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  // Serial.begin(9600);
  FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // limit my draw to 5W
  FastLED.setMaxPowerInMilliWatts(500);
  FastLED.clear();
  FastLED.show();

  // // for shooting star
  // FastLED.addLeds<WS2811, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);  // Set power limit of LED strip to 5V, 1500mA
  // FastLED.clear();                                  // Initialize all LEDs to "OFF"
}

void loop() {
  // colorWipe(40, 255, 0, NUM_LEDS);
  // colorWipe(0, 0, 0, NUM_LEDS);

  // rainbowCycle(14);

  // randomStar();
  randomMeteor();
}

void randomStar() {
  int randTime = random(15, 30);
  // r, g, b, tail, delay, interval, direction
  // shootingStarAnimation(180, 255, 20, NUM_LEDS, random(5, 20), (randTime * 1000), 1);
  shootingStarAnimation(255, 255, 255, NUM_LEDS, random(5, 20), (randTime * 1000), 1);
}

void randomMeteor() {
  int randomColor = random(1, 7);
  int colors[7][3] = {
    { 0, 255, 0 },     // red
    { 40, 255, 0 },    // orange
    { 100, 255, 0 },   // yellow
    { 255, 0, 0 },     // green
    { 0, 0, 255 },     // blue
    { 0, 255, 255 },   // purple
    { 255, 255, 255 }  // white
  };

  // green, red, blue, meteorSize, tailDecay, randomDecay, speedDelay
  meteorRain(colors[randomColor][0], colors[randomColor][1], colors[randomColor][2], 1, 100, true, random(1, 30));

  int randTime = random(5, 30);
  delay(randTime * 1000);
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    setPixel(i, red, green, blue);
    showStrip();
    delay(SpeedDelay);
  }
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {  // 5 cycles of all colors on wheel
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c + 1), *(c + 2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte *Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void meteorRain(byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
  setAll(0, 0, 0);

  for (int i = 0; i < NUM_LEDS + NUM_LEDS; i++) {

    // fade brightness all LEDs one step
    for (int j = 0; j < NUM_LEDS; j++) {
      if ((!meteorRandomDecay) || (random(10) > 5)) {
        fadeToBlack(j, meteorTrailDecay);
      }
    }

    // draw meteor
    for (int j = 0; j < meteorSize; j++) {
      if ((i - j < NUM_LEDS) && (i - j >= 0)) {
        setPixel(i - j, red, green, blue);
      }
    }

    showStrip();
    delay(SpeedDelay);
  }
}

void fadeToBlack(int ledNo, byte fadeValue) {
  // FastLED
  leds[ledNo].fadeToBlackBy(fadeValue);
}

void showStrip() {
  // FastLED
  FastLED.show();
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
}

unsigned long previousMillis = 0;  // Stores last time LEDs were updated
int count = 0;                     // Stores count for incrementing up to the NUM_LEDs

void shootingStarAnimation(int red, int green, int blue, int tail_length, int delay_duration, int interval, int direction) {
  /*
   * red - 0 to 255 red color value
   * green - 0 to 255 green color value
   * blue - 0 to 255 blue color value
   * tail_length - value which represents number of pixels used in the tail following the shooting star
   * delay_duration - value to set animation speed. Higher value results in slower animation speed.
   * interval - time between each shooting star (in miliseconds)
   * direction - value which changes the way that the pixels travel (uses -1 for reverse, any other number for forward)
  */
  unsigned long currentMillis = millis();  // Get the time
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Save the last time the LEDs were updated
    count = 0;                       // Reset the count to 0 after each interval
  }
  if (direction == -1) {  // Reverse direction option for LEDs
    if (count < NUM_LEDS) {
      leds[NUM_LEDS - (count % (NUM_LEDS + 1))].setRGB(red, green, blue);  // Set LEDs with the color value
      count++;
    }
  } else {
    if (count < NUM_LEDS) {                                   // Forward direction option for LEDs
      leds[count % (NUM_LEDS + 1)].setRGB(red, green, blue);  // Set LEDs with the color value
      count++;
    }
  }
  fadeToBlackBy(leds, NUM_LEDS, tail_length);  // Fade the tail LEDs to black
  FastLED.show();
  delay(delay_duration);  // Delay to set the speed of the animation
}
