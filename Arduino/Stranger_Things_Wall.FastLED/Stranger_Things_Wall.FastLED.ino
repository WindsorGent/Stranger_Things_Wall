// Stranger Things Alphabet Wall
// From a Halloween Forum discussion:
// http://www.halloweenforum.com/halloween-props/152337-my-stranger-things-alphabet-wall.html
// Original code by Vater
// Modified by Windsor Gent

// The Mokungit RGB addressable LEDs come with 50 pixels in the string, so this code
// has been set up to start the alphabet at the end of the string (pixel #50)
// with the LEDs winding back and forth on the wall.  

// When you have the LEDs and letters on the wall, uncomment the "abcedefgh" 
// and other two messages below to check alignment with the letters.  Then comment
// them back out when you're done.

// Updated 10/4/2016 to move "all on" and "flicker" effects into their own functions.
// This makes timing the entire effect easier, and restarts the whole effect each time
// all the messages have been shown.  Also added test strings of letters.

#include <FastLED.h>
#define DATA_PIN 6    // LED pin.  Change this for your setup if needed.

template<typename T, size_t N> constexpr size_t countof(const T(&)[N]) { return N; }

// Set this to match the number LEDs in the string.  Originally 50.
CRGB leds[50];  

// Only a few colors
CRGB colors[] = {
//white
//0xFFFFFF,
//yellow 
0xFFFF00,
//blue 
0x0000FF,
//purple
0x9400D3,
//red 
0xFF0000,
//green
0x00FF00
};

//Buffer more than one character at a time
//Modified from: https://hackingmajenkoblog.wordpress.com/2016/02/01/reading-serial-on-the-arduino/

int readmessage(int readch, char *buffer, int len)
{
  static int pos = 0;
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
        break;
      case '\r': // Return on CR
        rpos = pos;
        pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (pos < len-1) {
          buffer[pos++] = readch;
          buffer[pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}


uint8_t index[countof(leds)];

int fadeLevel = 0;

void setup() {
  for (auto &color : colors) napplyGamma_video(color, 2.2);
  for (auto &i : index) i = random8(countof(colors));
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, countof(leds));
  FastLED.setBrightness(140);

  Serial.begin(115200); //Listen to serial Port from Pi
}

// The loop controls the main effect.  Change pauses to get the timing you want.

void loop() {

static char buffer[80];

  if (readmessage(Serial.read(), buffer, 80) > 0) {

  FadeUp();     // Fade up the strand of LEDs
  Flicker();    // Flicker them
  delay(2000);  // Wait two seconds before the message is shown
  
//  write("abcdefgh");  // Uncomment these to test your setup
//  write("ijklmnop");
//  write("qrstuvwxyz");

Serial.println(buffer);
write ( buffer );
Serial.println(fadeLevel);
FadeUp();
 // delay(5000);   // Wait five seconds after the last message before it starts again
} else {
FadeUp();
}
}



int getIndex(char c) {    
  c = toUpperCase(c);
  switch (c) {
    case 'A'...'H':
      return 50 - (c - 'A');  

    case 'I'...'P':
      return 30 + (c - 'I');  

    case 'Q':     
      return 39;

    case 'R'...'Y':  
      return 26 - (c - 'R');

    case 'Z':   
      return 17;

    default:
      return 0;
  }
}

void write(char c) {
  if (isAlpha(c)) {
    // Get index and convert to 0-based indexing
    int i = getIndex(c)-1;  // (c)-2 gets to first pixel in strand if needed
    
    leds[i] = colors[index[i]];
    FastLED.show();
    delay(750);
    
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(250);
  } else if (isSpace(c)) {
    delay(1000);
  }
}

void write(const char *str) {
  while (*str) write(*str++);
} 

void FadeUp() //Fade lights in from black
{
  FastLED.setBrightness(100);
  if (fadeLevel < 256) { //Check if lights are black
for(fadeLevel = 0; fadeLevel<256; fadeLevel++)
    { 
        for (size_t i = 0; i < countof(leds); i++) 
        { 
          leds[i] = blend(CRGB::Black, colors[index[i]], fadeLevel);
          FastLED.show();
        }
    }
    delay(4500);  
    }
}

void LiteUp(){        // Turn all on
  for (size_t i = 0; i < countof(leds); i++) leds[i] = colors[index[i]];
  FastLED.show();
}

void Flicker (){      // Flicker the strand
FastLED.setBrightness(164);
FastLED.showColor(CRGB::White);
  for (int i = 0; i < 128; i++) {
    if (random8() > ease8InOutQuad(i*2))
      FastLED.show();
    else
      // To prevent having to repopulate leds[], we
      // simply show black instead of clearing the array.
      FastLED.showColor(CRGB::Black);

    delay(33);
  }
  FastLED.clear();
  fadeLevel=0;
}
