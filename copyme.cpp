#include <Arduino.h>
#include "ButtonMgr.h"
#include "StateMachine.h"

uint8_t mainStateFunc(StateM*);

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

static const int REPEAT_STEP_TO = 5000;

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

static const uint8_t NO_KEY = INDICATE_CNT;

enum GameState
{
    GAME_STATE_POR,                           //0 - Power on reset

    GAME_STATE_PLAY_SEQUENCE_INIT,            //1
    GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP,   //2
    GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY,  //3

    GAME_STATE_CHECK_SEQUENCE_PAUSE,          //4 - Delay before start
    GAME_STATE_CHECK_SEQUENCE,                //5 - Player plays back sequence
    GAME_STATE_CHECK_SEQUENCE_INDICATE,       //6
    GAME_STATE_GAME_OVER                      //7
  };

static StateM mainSM;


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

  mainSM.init(mainStateFunc);

  randomSeed(10);
}

void generateSequence()
{
  Serial.println("Sequence");
  for(int idx = 0; idx < MAX_LEVEL; idx++)
  {
    sequence[idx] = (uint8_t)(random(0, INDICATE_CNT) & 0xFF);
    Serial.print(idx);
    Serial.print(" ");
    Serial.println(sequence[idx]);
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

static uint8_t getInput()
{
  uint8_t pressed;

  if(!digitalRead(RED_BUTTON_PIN))
  {
      pressed = INDICATE_RED;
  }
  else if(!digitalRead(BLUE_BUTTON_PIN))
  {
      pressed = INDICATE_BLUE;
  }
  else if(!digitalRead(YELLOW_BUTTON_PIN))
  {
      pressed = INDICATE_YELLOW;
  }
  else if(!digitalRead(GREEN_BUTTON_PIN))
  {
      pressed = INDICATE_GREEN;
  }
  else
  {
    pressed = NO_KEY;
  }

  return pressed;
}

static uint8_t mainStateFunc(StateM* sm)
{
  uint8_t retValue = 0;

  switch(sm->getCurrent())
  {
    case GAME_STATE_POR:
    {
      currentLevel = 1;
      generateSequence();
      sm->setNext(GAME_STATE_PLAY_SEQUENCE_INIT);
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INIT:
    {
      if(sm->getDuration() > 2000)
      {
        Serial.print("Level: ");
        Serial.println(currentLevel);
        currentStep = 0;
        sm->setNext(GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP);
      }
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP:
    {
      doIndicateStep(currentStep);
      sm->setNext(GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY);
      break;
    }

    case GAME_STATE_PLAY_SEQUENCE_INDICATE_DELAY:
    {
      if( sm->getDuration() > STEP_DURATION )
      {
        stepEnd();
        currentStep++;
        if(currentStep >= currentLevel)
        {
          //All done, now handle player repeating this level
          sm->setNext(GAME_STATE_CHECK_SEQUENCE_PAUSE);
        }
        else
        {
          //Play next step in this sequence
          sm->setNext(GAME_STATE_PLAY_SEQUENCE_INDICATE_STEP);
        }
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE_PAUSE:
    {
      if( sm->getDuration() > 1000 )
      {
        currentStep = 0;
        sm->setNext(GAME_STATE_CHECK_SEQUENCE);
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE:
    {
      if( sm->getDuration() > REPEAT_STEP_TO )
      {
        Serial.println("Too slow!!!");
        sm->setNext(GAME_STATE_GAME_OVER);
      }
      else
      {
        uint8_t playerKey = getInput();

        if(playerKey != NO_KEY)
        {
          Serial.print("Key: ");
          Serial.println(playerKey);

          if(sequence[currentStep] == playerKey)
          {
            doIndicateStep(currentStep);
            sm->setNext(GAME_STATE_CHECK_SEQUENCE_INDICATE);
          }
          else
          {
            Serial.println("Wrong Key!!!");
            Serial.print("Excpected: ");
            Serial.println(sequence[currentStep]);
            sm->setNext(GAME_STATE_GAME_OVER);
          }
        }
      }
      break;
    }

    case GAME_STATE_CHECK_SEQUENCE_INDICATE:
    {
      if( sm->getDuration() > STEP_DURATION )
      {
        stepEnd();
        currentStep++;
        if(currentStep >= currentLevel)
        {
          //All done, now handle player repeating this level
          currentLevel++;
          if(currentLevel > MAX_LEVEL )
          {
            Serial.println("You won!!");
            sm->setNext(GAME_STATE_GAME_OVER);
          }
          else
          {
            Serial.println("Level complete!!");
            sm->setNext(GAME_STATE_PLAY_SEQUENCE_INIT);
          }
        }
        else
        {
          //Play next step in this sequence
          sm->setNext(GAME_STATE_CHECK_SEQUENCE);
        }
      }
      break;
    }

    case GAME_STATE_GAME_OVER:
    {
      break;
    }

    case StateM::STATE_INVALID:
    default:
    {
      sm->setNext(GAME_STATE_POR);
      break;
    }
  }

  return retValue;
}


// the loop function runs over and over again forever
void loop() {
  ButtonMgr::update();

  mainSM.run();
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
  //

  if(    ButtonMgr::isPressed(RED_BUTTON_IDX)
      || ButtonMgr::isReleased(RED_BUTTON_IDX) )
  {
    tone(SPEAKER_PIN, NOTE_A4, 250);
  }

}
