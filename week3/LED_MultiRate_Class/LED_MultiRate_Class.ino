class Blinker{

  private:
    const int _pin;                 // output pin
    const unsigned long _interval;  // blink interval in ms

    int _state;                     // current state (either HIGH OR LOW)
    unsigned long _lastToggledTimestamp; // last state toggle in ms

  public:

    // Constructor
    Blinker(int pin, unsigned long blinkInterval) :
      _pin(pin), _interval(blinkInterval) // initialize const like this in C++
    {
      _state = LOW;
      _lastToggledTimestamp = 0;
      pinMode(_pin, OUTPUT);
    }

    void update(){
      unsigned long currentTimestampMs = millis();
      
      if (currentTimestampMs - _lastToggledTimestamp >= _interval) {
        _lastToggledTimestamp = currentTimestampMs;
        _state = !_state;
        digitalWrite(_pin, _state);
      }
    }
};

Blinker _led1Blinker(2, 200);  // specify pin and blink interval (200ms)
Blinker _led2Blinker(5, 333);  // specify pin and blink interval (333ms)
Blinker _led3Blinker(9, 1111); // specify pin and blink interval (1111ms)

// The setup function runs once when you press reset or power the board
void setup() {
  // empty 
}

// The loop function runs over and over again forever
void loop() {
  _led1Blinker.update();
  _led2Blinker.update();
  _led3Blinker.update();
}
