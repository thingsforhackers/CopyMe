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

static const int NOTE_G4 = 392; //Blue
static const int NOTE_C4 = 277; //Yellow
static const int NOTE_A4 = 440; //Red
static const int NOTE_G5 = 784; //Green

enum BUTTON_IDX
{
    RED_BUTTON_IDX,
    BLUE_BUTTON_IDX,
    YELLOW_BUTTON_IDX,
    GREEN_BUTTON_IDX,

    BUTTON_CNT
};
ButtonCtx buttons[BUTTON_CNT];

enum INDICATION_ID
{
  INDICATE_RED,
  INDICATE_BLUE,
  INDICATE_YELLOW,
  INDICATE_GREEN,

  INDICATE_CNT

};

enum GameState
{
    GAME_STATE_INVALID,
    GAME_STATE_POR,   //Power on reset

    GAME_STATE_PLAY_SEQUENCE_INIT,
    GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP,
    GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY,

    GAME_STATE_CHECK_SEQUENCE, //Player plays back sequence

    GAME_STATE_GAME_OVER
};

static GameState previousGameState;
static GameState currentGameState;
static unsigned long stateEnterTime;

static const uint8_t MAX_LEVEL = 8;
static uint8_t sequence[MAX_LEVEL];
static uint8_t currentLevel;

static const int STEP_DURATION = 333;

static uint8_t currentStep;

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

  randomSeed(10);
}

void generateSequence()
{
  for(int idx = 0; idx < MAX_LEVEL; idx++)
  {
      sequence[idx] = (uint8_t)(random(0, INDICATE_CNT) & 0xFF);
  }
}

static void doIndicateStep(uint8_t step)
{

  switch(sequence[step])
  {
    case INDICATE_RED:
    {
      tone(SPEAKER_PIN, NOTE_A4);
      digitalWrite(RED_LED_PIN, HIGH);
      break;
    }

    case INDICATE_BLUE:
    {
      tone(SPEAKER_PIN, NOTE_G4);
      digitalWrite(BLUE_LED_PIN, HIGH);
      break;
    }

    case INDICATE_YELLOW:
    {
      tone(SPEAKER_PIN, NOTE_C4);
      digitalWrite(YELLOW_LED_PIN, HIGH);
      break;
    }

    case INDICATE_GREEN:
    {
      tone(SPEAKER_PIN, NOTE_G5);
      digitalWrite(GREEN_LED_PIN, HIGH);
      break;
    }
  }
}

static void stepEnd()
{
  noTone(SPEAKER_PIN);
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(BLUE_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
}

static void doLevel()
{
  uint8_t idx = 0;
  do
  {
    doIndicateStep(idx);
    delay(STEP_DURATION);
    stepEnd();
    idx++;
  } while(idx < currentLevel);
}

static runGame()
{
  GameState nextGameState = GAME_STATE_INVALID;

  switch(currentGameState)
  {
    case GAME_STATE_POR:
    {
      currentLevel = 1;
      generateSequence();
      nextGameState = GAME_STATE_PLAY_SEQUENCE_INIT;
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INIT:
    {
      Serial.print("Level: ");
      Serial.println(currentLevel);
      currentStep = 0;
      //doLevel();
      nextGameState = GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP;

      // if(previousGameState != GAME_STATE_PLAY_SEQUENCE)
      // {
      //
      // }
      // else if( (millis() - stateEnterTime) > STEP_DURATION )
      // {
      //   stepEnd();
      //   nextGameState = GAME_STATE_CHECK_SEQUENCE;
      // }

      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP:
    {
      doIndicateStep(currentStep);
      nextGameState = GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY;
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY:
    {
      if( (millis() - stateEnterTime) > STEP_DURATION )
      {
        stepEnd();
        currentStep++;
        if(currentStep >= currentLevel)
        {
          nextGameState = GAME_STATE_CHECK_SEQUENCE;
        }
        else
        {
          nextGameState = GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP;
        }
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE:
    {
      if( (millis() - stateEnterTime) > 5 * STEP_DURATION )
      {
        currentLevel++;

        if(currentLevel > MAX_LEVEL )
        {
          Serial.println("Game Over");
          nextGameState = GAME_STATE_GAME_OVER;
        }
        else
        {
          nextGameState = GAME_STATE_PLAY_SEQUENCE_INIT;
        }
      }
      break;
    }

    case GAME_STATE_GAME_OVER:
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

  runGame();
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
