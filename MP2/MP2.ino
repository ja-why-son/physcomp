#include <PushButton.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 _display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LEFT_BUTTON_PIN = 12;
const int RIGHT_BUTTON_PIN = 13;
const int BUZZER_PIN = 11;

PushButton _leftButton(LEFT_BUTTON_PIN);
PushButton _rightButton(RIGHT_BUTTON_PIN);

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  _leftButton.setActiveLogic(LOW);
  _rightButton.setActiveLogic(LOW);
  
  _display.setRotation(1);
  _display.clearDisplay();

  enterMenuSound();
}

// Game setting
const int PLAYER_X_VELOCITY = 7;
const int BLOCK_COUNT = 7;
const int BLOCK_WIDTH = 20;
const int BLOCK_HEIGHT = 2;
const int INITIAL_WORLD_SCROLL_SPEED = 2;
const int GRAVITY = 1;


class Player {
 public:
  int16_t x;
  int16_t y;
  int16_t playerRadius = 3;

  // return y
  int16_t top() { return y - playerRadius; }

  // return y
  int16_t bottom() { return y + playerRadius; }

  // return x
  int16_t right() { return x + playerRadius; }

  // return x
  int16_t left() { return x - playerRadius; }

  int16_t radius() { return playerRadius; }
};

class Block {
 public:
  Block(int16_t x, int16_t y) : x(x), y(y) {
    width = BLOCK_WIDTH;
    height = BLOCK_HEIGHT;
  }

  int getX() { return x; }
  int getY() { return y; }
  int getWidth() {return width;}
  int getHeight() {return height;}

  int16_t x;
  int16_t y;
  int16_t width;
  int16_t height;
};

class World {
 public:
  World() 
    : blocks({Block(0, 0), Block(0, 0), Block(0, 0), Block(0, 0),
      Block(0, 0), Block(0, 0), Block(0, 0)}),
      playerXVelocity(PLAYER_X_VELOCITY), playerYVelocity(1),
      worldScrollSpeed(INITIAL_WORLD_SCROLL_SPEED), player(Player()), levelingUp(false) {  
//    player = Player();
    newGame();
    
  }

  void newGame() {
    blocks[0] = Block(random(0, 64 - BLOCK_WIDTH), 30);
    blocks[1] = Block(random(0, 64 - BLOCK_WIDTH), 46);
    blocks[2] = Block(random(0, 64 - BLOCK_WIDTH), 62);
    blocks[3] = Block(random(0, 64 - BLOCK_WIDTH), 78);
    blocks[4] = Block(random(0, 64 - BLOCK_WIDTH), 94);
    blocks[5] = Block(random(0, 64 - BLOCK_WIDTH), 110);
    blocks[6] = Block(random(0, 64 - BLOCK_WIDTH), 126);
    level = 1;
    levelProgress = 0;
    placePlayerOnBlock();
    worldScrollSpeed = INITIAL_WORLD_SCROLL_SPEED;
    levelingUp = false;
  }

  boolean updateWorld() {
    if (player.top() <= 9 || player.bottom() >= 128) {
      return false;
    }
    
    _leftButton.update();
    _rightButton.update();
    if (_leftButton.isActive()) {
      player.x -= playerXVelocity;
      if (player.left() <= 0) {
        player.x = 3;
      }
    } else if (_rightButton.isActive()) {
      player.x += playerXVelocity;
      if (player.right() >= 64) {
        player.x = 61;
      }
    }
    if (standingOnBlock()) {
      player.y -= worldScrollSpeed;    
    } else {
      // fall until stopping on a block
      boolean isFreeFall = true;
      for (int i = 0; i < BLOCK_COUNT; i++) {
        if (player.x >= blocks[i].getX() && 
            player.x <= blocks[i].getX() + blocks[i].getWidth() &&
            player.bottom() < blocks[i].getY() && 
            player.bottom() + playerYVelocity >= blocks[i].getY() - worldScrollSpeed) {
          // place player on block
          isFreeFall = false;
          player.y = blocks[i].y - player.radius() - worldScrollSpeed;
          playerYVelocity = 0;
        }
      }
      if (isFreeFall) {
        player.y += playerYVelocity;
        playerYVelocity += GRAVITY;  
      }
    }

    for (int i = 0; i < BLOCK_COUNT; i++) {
      blocks[i].y -= worldScrollSpeed;
      if (blocks[i].y <= 10) {
        blocks[i].y = 128;
        blocks[i].x = random(0, 64 - BLOCK_WIDTH);
        levelProgress++;
        if (levelProgress == BLOCK_COUNT) {
          levelProgress = 0;
          level++;
          levelingUp = true;
          tone(BUZZER_PIN, 262);
          levelingUpStartMillis = millis();
          if (level % 5 == 0) {
            worldScrollSpeed += 1;
          }
        }
      }
    }

    levelUpSound();

    return true;
  }

  void drawWorld() {    
    drawTop();
  
    drawPlayer();

    drawBlock();
  }


  
 private:
  void placePlayerOnBlock() {
    Block lowestBlock(0,0);
    for (int i = 0; i < BLOCK_COUNT; i++) {
      if (blocks[i].y > lowestBlock.y) {
        lowestBlock = blocks[i];
      }
    }
    player.x = lowestBlock.x + (BLOCK_WIDTH / 2);
    player.y = lowestBlock.y - player.radius();
  }


  boolean standingOnBlock() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
      if (player.x >= blocks[i].getX() && 
          player.x <= blocks[i].getX() + blocks[i].getWidth() && 
          player.bottom() == blocks[i].getY()) {
        return true;
      }
    }
    return false;
  }

  void drawTop() {
    int16_t x, y;
    int16_t textWidth, textHeight;
 
    char levelStr[] = "Level ";

    // Setup text rendering parameters
    _display.setTextSize(1);
    _display.setTextColor(WHITE, BLACK);
  
    // Measure the text with those parameters
    _display.getTextBounds(levelStr, 0, 0, &x, &y, &textWidth, &textHeight);
  
    // Center the text on the display
    _display.setCursor(_display.width() / 2 - textWidth / 2, 0);
  
    // Print out the string
    _display.print(levelStr);
    _display.print(level);

    _display.drawLine(0, textHeight, _display.width(), textHeight, SSD1306_WHITE);
  }

  void drawPlayer() {
    _display.drawCircle(player.x, player.y, player.radius(), SSD1306_WHITE);
  }

  void drawBlock() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
      _display.drawRect(blocks[i].getX(), blocks[i].getY(), blocks[i].getWidth(), blocks[i].getHeight(), SSD1306_WHITE);
    }   
  }

  void levelUpSound() {
    if (levelingUp) {
      long current = millis();
      if (current - levelingUpStartMillis >= 150 ) {
        noTone(BUZZER_PIN);
        digitalWrite(BUZZER_PIN, LOW);
        levelingUp = false;
      } else if (current - levelingUpStartMillis >= 75) {
        tone(BUZZER_PIN, 392);
      }
    }
  }

  Player player;
  int16_t playerXVelocity;
  int16_t playerYVelocity;
  int16_t worldScrollSpeed;
  Block blocks[BLOCK_COUNT];
  int level;
  int levelProgress;
  boolean levelingUp;
  long levelingUpStartMillis;
};



World _world;
boolean _inGame = false;

void loop() {
  if (_inGame) {
    _display.clearDisplay();
    _inGame = _world.updateWorld();

    _world.drawWorld();
    _display.display();
    if (!_inGame) {
      // draw Game over
      for (int i = 0; i < 5; i++) {
        _display.clearDisplay();
        _world.drawWorld();
        drawGameOver(i % 2);
        _display.display();
        delay(100);
      }
      gameOverSound();
      delay(1000);
      enterMenuSound();
    }
 
    delay(50);
  } else {
    // press any button to start the game
    // display menu
    // if button pressed, _inGame = true, _world.newGame();
    _display.clearDisplay();
    drawGameMenu();
    _leftButton.update();
    _rightButton.update();
    if (_leftButton.isClicked() || _rightButton.isClicked()) {
      _inGame = true; 
      _world.newGame();
    }
    _display.display();
    delay(50);
  }
}

void drawGameMenu() {
  int16_t x, y;
  uint16_t text1Width, text1Height, text2Width, text2Height;
  const char str1[] = "NS-SHAFT";
  const char str2[] = "LITE";

  _display.setTextColor(WHITE, BLACK);
  _display.setTextSize(1);

  // Measure the text with those parameters
  _display.getTextBounds(str1, 0, 0, &x, &y, &text1Width, &text1Height);
  _display.getTextBounds(str2, 0, 0, &x, &y, &text2Width, &text2Height);

  // Center the text on the display
  _display.setCursor(_display.width() / 2 - text1Width / 2, _display.height() / 2 - text1Height / 2);

  // Print out the string
  _display.print(str1);

  _display.setCursor(_display.width() / 2 - text2Width / 2, _display.height() / 2 + text2Height / 2);
  _display.print(str2);

}

void drawGameOver(int style) {
  int16_t x, y;
  uint16_t text1Width, text1Height, text2Width, text2Height;
  const char str1[] = "Game";
  const char str2[] = "Over";

  // Setup text rendering parameters
  _display.setTextSize(2);
  if (style == 0) {
    _display.setTextColor(BLACK, WHITE);
  } else {
    _display.setTextColor(WHITE, BLACK);
  }


  // Measure the text with those parameters
  _display.getTextBounds(str1, 0, 0, &x, &y, &text1Width, &text1Height);
  _display.getTextBounds(str2, 0, 0, &x, &y, &text2Width, &text2Height);

  // Center the text on the display
  _display.setCursor(_display.width() / 2 - text1Width / 2, _display.height() / 2 - text1Height / 2);

  // Print out the string
  _display.print(str1);

  _display.setCursor(_display.width() / 2 - text2Width / 2, _display.height() / 2 + text2Height / 2);
  _display.print(str2);
}

void gameOverSound() {
  tone(BUZZER_PIN, 523);
  delay(100);
  tone(BUZZER_PIN, 440);
  delay(100);
  tone(BUZZER_PIN, 493);
  delay(100);
  tone(BUZZER_PIN, 392);
  delay(100);
  tone(BUZZER_PIN, 440);
  delay(100);
  tone(BUZZER_PIN, 350);
  delay(100);
  tone(BUZZER_PIN, 392);
  delay(100);
  tone(BUZZER_PIN, 330);
  delay(100);
  tone(BUZZER_PIN, 294);
  delay(100);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}

void enterMenuSound() {
  tone(BUZZER_PIN, 262);
  delay(100);
  tone(BUZZER_PIN, 294);
  delay(100);
  tone(BUZZER_PIN, 330);
  delay(100);
  tone(BUZZER_PIN, 350);
  delay(100);
  tone(BUZZER_PIN, 392);
  delay(100);
  noTone(BUZZER_PIN);
  digitalWrite(BUZZER_PIN, LOW);
}
