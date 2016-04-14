#include <Arduino.h>
#include "ButtonMgr.h"

//Our LEDS
static const uint8_t RED_LED_PIN = 9;
static const uint8_t BLUE_LED_PIN = 10;
static const uint8_t YELLOW_LED_PIN = 11;
static const uint8_t GREEN_LED_PIN = 6;

//Our Buttons
static const uint8_t RED_BUTTON_PIN = 7;
static const uint8_t BLUE_BUTTON_PIN = 8;
static const uint8_t YELLOW_BUTTON_PIN = 12;
static const uint8_t GREEN_BUTTON_PIN = 13;

static const uint8_t SPEAKER_PIN = 3;

static const uint8_t NOTE_G4 = 392; //Blue
static const uint8_t NOTE_C4 = 277; //Yellow
static const uint8_t NOTE_A4 = 440; //Red
static const uint8_t NOTE_G5 = 784; //Green

enum BUTTON_IDX
{
    RED_BUTTON_IDX,
    BLUE_BUTTON_IDX,
    YELLOW_BUTTON_IDX,
    GREEN_BUTTON_IDX,

    BUTTON_CNT
};
ButtonCtx buttons[BUTTON_CNT];

enum GameState
{
    GAME_STATE_INVALID,
    GAME_STATE_POR,   //Power on reset

    GAME_STATE_PLAY_SEQUENCE, //Play this levels sequence to the player

    GAME_STATE_CHECK_SEQUENCE, //Player plays back sequence

};

static GameState previousGameState;
static GameState currentGameState;
static unsigned long stateEnterTime;

static const uint8_t MAX_LEVEL = 64;
static uint8_t sequence[MAX_LEVEL];
static uint8_t currentLevel;

void setup()
{

  Serial.begin(9600);  //turn on serial communication

  //Setup LED pins as OUTPUT
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  //Setup buttons pins as INPUT_PULLUP
  pinMode(RED_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BLUE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(YELLOW_BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_BUTTON_PIN, INPUT_PULLUP);

  buttons[RED_BUTTON_IDX].pin = RED_BUTTON_PIN;
  buttons[BLUE_BUTTON_IDX].pin = BLUE_BUTTON_PIN;
  buttons[YELLOW_BUTTON_IDX].pin = YELLOW_BUTTON_PIN;
  buttons[GREEN_BUTTON_IDX].pin = GREEN_BUTTON_PIN;
  ButtonMgr::init(buttons, BUTTON_CNT);

  previousGameState = GAME_STATE_INVALID;
  currentGameState = GAME_STATE_POR;
  stateEnterTime = millis();
}

void generateSequence()
{

}

static runGame()
{
  GameState nextGameState = GAME_STATE_INVALID;

  switch(currentGameState)
  {
    case GAME_STATE_POR:
    {

      break;
    }

    case GAME_STATE_PLAY_SEQUENCE:
    {

      break;
    }

    case GAME_STATE_CHECK_SEQUENCE:
    {

      break;
    }

    case GAME_STATE_INVALID:
    default:
    {
      break;
    }

  }

  previousGameState = currentGameState;

  if( GAME_STATE_INVALID != nextGameState )
  {
    previousGameState = currentGameState;
    currentGameState = nextGameState;
    stateEnterTime = millis();
  }
}


// the loop function runs over and over again forever
void loop() {
  ButtonMgr::update();

  // Serial.println("And ON");
  // digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(5000);              // wait for a second
  // Serial.println("And OFF");
  // digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  // delay(1000);              // wait for a second

  // digitalWrite(RED_LED_PIN, !digitalRead(RED_BUTTON_PIN));
  // digitalWrite(BLUE_LED_PIN, !digitalRead(BLUE_BUTTON_PIN));
  // digitalWrite(YELLOW_LED_PIN, !digitalRead(YELLOW_BUTTON_PIN));
  // digitalWrite(GREEN_LED_PIN, !digitalRead(GREEN_BUTTON_PIN));
  //
  // if(!digitalRead(RED_BUTTON_PIN))
  // {
  //     tone(SPEAKER_PIN, NOTE_A4, 100);
  // }
  // else if(!digitalRead(BLUE_BUTTON_PIN))
  // {
  //     tone(SPEAKER_PIN, NOTE_G4, 100);
  // }
  // else if(!digitalRead(YELLOW_BUTTON_PIN))
  // {
  //     tone(SPEAKER_PIN, NOTE_C4, 100);
  // }
  // else if(!digitalRead(GREEN_BUTTON_PIN))
  // {
  //     tone(SPEAKER_PIN, NOTE_G5, 100);
  // }


  if(    ButtonMgr::isPressed(RED_BUTTON_IDX)
      || ButtonMgr::isReleased(RED_BUTTON_IDX) )
  {
    tone(SPEAKER_PIN, NOTE_A4, 250);
  }

}
