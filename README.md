Stranger Things Alphabet Wall
=============================

Inspired and adapted from the Sparkfun Project, I wanted a bit more control over how the lights lit up, so using some excellent ideas from [JeffHass at Halloween Forum](http://www.halloweenforum.com/halloween-props/152337-my-stranger-things-alphabet-wall-3.html#post2003394) I mashed everything together until it worked together.

Using [individually addressable C9 shaped LEDs](https://www.amazon.com/gp/product/B01DZV3PNO/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1) (ubdoubtedly availiable cheaper elsewhere if you look hard enough), you too can recreate Joyce's Christmas light message wall from "Stranger Things". In this repository, you'll find the code necessary to enable a string of LEDs to capture and display tweets with a certain hashtag.

To communicate with the LEDs I used an [Uno R3](http://www.microcenter.com/product/431997/Uno_R3_MainBoard) and a Raspberry Pi 3 to run the python script over WiFi.

-Arduino code was written in the Arduino.cc IDE v1.6.12 with the FastLED library installed

-Python code is running under Python 2.7.3 with the [TwitterSearch](https://pypi.python.org/pypi/TwitterSearch/) library, which can be installed via pip.