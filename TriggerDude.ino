/*

  Changé le fichier /Users/benjamincochois/Library/Arduino15/packages/STMicroelectronics/hardware/stm32/2.2.0/system/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_adc.h
  ligne 1881 -> commenté la variable Register non utilisée par library STM32 (bug console)

*/


// ancien rgb ROUGE : 219, 0, 37

const char FW_VERSION[] = "1.0.1";

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_NeoPixel.h>
#include <U8g2lib.h>
#include "DAC8554.h"

#include <Prandom.h>
Prandom RandomNum;

#define FLASH_BASE_ADDRESS  0x08060000
#define FLASH_PAGE_SIZE     ((uint32_t)(40*1024))
#include <FlashStorage_STM32.h>


#define TIMER_INTERRUPT_DEBUG         0
#define _TIMERINTERRUPT_LOGLEVEL_     0
#include "STM32TimerInterrupt.h"

STM32Timer CLOCK_INT_timer(TIM2);
STM32Timer LFO1_timer(TIM3);
STM32Timer LFO2_timer(TIM4);
STM32Timer GLIDE_timer(TIM5);


// DAC8564
#define dacSync_nss       PB12
#define dacClock_sck      PB13
#define dacIn_mosi        PB15
DAC8554 Dac(dacIn_mosi, dacClock_sck, dacSync_nss);
//DAC8554 Dac(dacSync_nss);

#define clockInPin         PB7
#define resetInPin         PB4
//#define clockOutPin        PA11

//BUTTONS
#define patternBank_Button PB3
#define AbLength_Button    PB6
#define muteSolo_Button    PA10
#define fillShift_Button   PA15
#define deleteClear_Button PB8

#define altBtnLed          PA0

//ENCODERS
#define encoder1_A_Pin     PC13
#define encoder1_B_Pin     PC14
#define encoder1_Btn       PC15
#define encoder2_A_Pin     PA2 // changer pour PA2 sur pcb v4
#define encoder2_B_Pin     PA12
#define encoder2_Btn       PB5

//74HC165
#define SR_Input_p_load   PB2 //PB11 sur blue pill
#define SR_Input_clock    PB14
#define SR_Input_data     PB10
#define DATA_WIDTH        16

//#define trig_DURATION    2000

//74HC595
#define SR_Output_latch   PA9
#define SR_Output_clock   PA8
#define SR_Output_data    PA1

//CVINs
#define CV_IN_1           PB0
#define CV_IN_2           PA6

//OLED : HW SPI pins
#define OLED_MOSI         PA7 // SDA (I2C) / DI sur oled
#define OLED_CLK          PA5 // SCL (I2C) / DO sur oled
//OLED : can be any pins
#define OLED_CS           PA4 // SS / NSS sur stm / CS sur oled -- Chip Select
#define OLED_DC           PB1 // Data/Command / DC sur oled         
#define OLED_RESET        PA3 // RES sur oled
U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI u8g2(U8G2_R0, OLED_CS, OLED_DC, OLED_RESET);

//NEOPIXEL
#define LED_DATA          PB9
#define LED_COUNT         15
//#define BRIGHTNESS        50
Adafruit_NeoPixel strip(LED_COUNT, LED_DATA, NEO_GRB + NEO_KHZ800);

uint8_t led_brightness = 50;

//BPM
#define MIN_BPM 20
#define MAX_BPM 320

#define BANKS    1
#define PATTERNS 8 //12
#define TRACKS 12
#define TRACKS_AB (TRACKS*2)

#define MAX_STEPS_4PPQN      128
#define MAX_STEPS_16PPQN     512 //128 * 4
#define ARP_MAX_STEPS 8

#define LFO_WAVE_RESOLUTION  256

#define DEBOUNCE_BTNS_MILLIS   15
#define DEBOUNCE_ENC_MILLIS    5
#define DEBOUNCE_PADS_MICROS   15000L


const uint8_t triggerDude_big_font[1530] U8G2_FONT_SECTION("triggerDude_big_font") =
  "\340\0\3\3\3\4\3\4\4\5\11\0\376\7\376\7\0\1\27\2J\5\335 \5\0\242\3!\6\71\243"
  "\243\22\42\7\223\266#\42\11#\12-\246'\245\222RI\1$\14=\242+d\11\225\42\246\20\0%"
  "\7-\242#\226\17&\15\65\242\203\22\12\211\42\222P%\0'\6\21\267C\0(\10\272\242G\222S"
  "\0)\11:\243#\24\311\42\1*\12-\242+%e\222)\4+\12-\242+\30*\5C\0,\6"
  "\222\242G\2-\6\15\252\243\0.\6\11\243#\0/\13=\242\63\26\214\5c\211\0\60\13=\242\303"
  "\246\22\221Dd\6\61\7\273\242c\224\17\62\11=\242\243\30<\6\13\63\11=\242\243\230B\14\32\64"
  "\11=\242#\246f\314\0\65\11=\242\303\30,\6\15\66\11=\242\303\30\264\311\14\67\12=\242\243\30"
  "\223\5\263\0\70\11=\242\303&\273\311\14\71\11=\242\303&\63\6\15:\6\31\247#\22;\7\242\242"
  "'&\1<\10\253\242+\222\226\0=\6\35\246\243Z>\10\253\242#\226\222\4\77\13=\242\243\230\64"
  "\212\203B\0@\12\65\242\303\64\211H\210\5A\11=\242\303&\273\251\5B\12=\242\303&\253\304d"
  "\6C\10=\242\303\230c\1D\11=\242\203\22\323\255\2E\12=\242\303\30\244\4\203\5F\12=\242"
  "\303\30\244\4\23\1G\11=\242\303\230B\223\31H\11=\242#\246vS\13I\11\273\242c\22\312e"
  "\0J\12=\242\63\243L\26\231\0K\16=\242#&\212$\311\42\241\224X\0L\10=\242#\230\307"
  "\2M\13=\242#\66\231Dd\332\2N\14=\242#\66\222DD\63\265\0O\10=\242\303\246\233\1"
  "P\11=\242\303\246v\14\2Q\13=\242\303\246%\42\212L\2R\14=\242\303&;EB\221X\0"
  "S\11=\242\303\30,\6\15T\11=\242\243\24\314\23\0U\10=\242#\246\67\3V\12=\242#\246"
  "[$-\4W\16=\242#\246\22\221D$\21I.\0X\14=\242#&\213\244\245\244\311\2Y\13"
  "=\242#&\213\244\5\63\1Z\11=\242\243\30\313\261\0[\10\273\242\203\224\323\0\134\12=\242#\32"
  "\214\6\243\11]\10\273\242c\224\23\1^\10\35\262+\26I\13_\6\15\242\243\0`\7\33\262#\226"
  "\0a\10\65\242\243x\223\31b\11=\242#\30\264\251\31c\10\65\242\303\230\261\0d\10=\242\63x"
  "S\63e\10\65\242\303&;\26f\13=\242k\22L\253\4c\0g\11E\232\303\246f\14\32h\12"
  "=\242#\30\244\304\264\5i\10\273\242'(\312\62j\11\313\232+(\312\211\0k\15=\242#\30\23"
  "EF)\61Y\0l\10\273\242C\224\227\1m\16\65\242C\62\211H\42\222\210$\42\13n\11\65\242"
  "\203\22\323-\0o\10\65\242\303\246\315\0p\11\65\242\303&;\6\1q\10=\236\303&\63fr\11"
  "\65\242#\242$\314\10s\10\65\242\303X\14\32t\12\65\242#\30\244\4\243\4u\10\65\242#\246\233"
  "\1v\12\65\242#\246-\222\26\2w\16\65\242#&\211H\42\222\210$\62\21x\13\65\242#&\213"
  "\244\245\244\5y\13=\236#\246\26!\6+\0z\12\65\242\243\30\22\305\202\5{\5\0\242\3|\5"
  "\0\242\3}\5\0\242\3~\5\0\242\3\177\5\0\242\3\200\5\0\242\3\201\5\0\242\3\202\5\0\242"
  "\3\203\5\0\242\3\204\5\0\242\3\205\5\0\242\3\206\5\0\242\3\207\5\0\242\3\210\5\0\242\3\211"
  "\5\0\242\3\212\5\0\242\3\213\5\0\242\3\214\5\0\242\3\215\5\0\242\3\216\5\0\242\3\217\5\0"
  "\242\3\220\5\0\242\3\221\5\0\242\3\222\5\0\242\3\223\5\0\242\3\224\5\0\242\3\225\5\0\242\3"
  "\226\5\0\242\3\227\5\0\242\3\230\5\0\242\3\231\5\0\242\3\232\5\0\242\3\233\5\0\242\3\234\5"
  "\0\242\3\235\5\0\242\3\236\5\0\242\3\237\5\0\242\3\240\5\0\242\3\241\5\0\242\3\242\5\0\242"
  "\3\243\5\0\242\3\244\5\0\242\3\245\5\0\242\3\246\5\0\242\3\247\5\0\242\3\250\5\0\242\3\251"
  "\5\0\242\3\252\5\0\242\3\253\5\0\242\3\254\5\0\242\3\255\5\0\242\3\256\5\0\242\3\257\5\0"
  "\242\3\260\5\0\242\3\261\5\0\242\3\262\5\0\242\3\263\5\0\242\3\264\5\0\242\3\265\5\0\242\3"
  "\266\5\0\242\3\267\5\0\242\3\270\5\0\242\3\271\5\0\242\3\272\5\0\242\3\273\5\0\242\3\274\5"
  "\0\242\3\275\5\0\242\3\276\5\0\242\3\277\5\0\242\3\300\5\0\242\3\301\5\0\242\3\302\5\0\242"
  "\3\303\5\0\242\3\304\5\0\242\3\305\5\0\242\3\306\5\0\242\3\307\5\0\242\3\310\5\0\242\3\311"
  "\5\0\242\3\312\5\0\242\3\313\5\0\242\3\314\5\0\242\3\315\5\0\242\3\316\5\0\242\3\317\5\0"
  "\242\3\320\5\0\242\3\321\5\0\242\3\322\5\0\242\3\323\5\0\242\3\324\5\0\242\3\325\5\0\242\3"
  "\326\5\0\242\3\327\5\0\242\3\330\5\0\242\3\331\5\0\242\3\332\5\0\242\3\333\5\0\242\3\334\5"
  "\0\242\3\335\5\0\242\3\336\5\0\242\3\337\5\0\242\3\340\5\0\242\3\341\5\0\242\3\342\5\0\242"
  "\3\343\5\0\242\3\344\5\0\242\3\345\5\0\242\3\346\5\0\242\3\347\5\0\242\3\350\5\0\242\3\351"
  "\5\0\242\3\352\5\0\242\3\353\5\0\242\3\354\5\0\242\3\355\5\0\242\3\356\5\0\242\3\357\5\0"
  "\242\3\360\5\0\242\3\361\5\0\242\3\362\5\0\242\3\363\5\0\242\3\364\5\0\242\3\365\5\0\242\3"
  "\366\5\0\242\3\367\5\0\242\3\370\5\0\242\3\371\5\0\242\3\372\5\0\242\3\373\5\0\242\3\374\5"
  "\0\242\3\375\5\0\242\3\376\5\0\242\3\377\5\0\242\3\0\0\0\4\377\377\0";

const uint8_t triggerDude_small_font[1387] U8G2_FONT_SECTION("triggerDude_small_font") =
  "\340\0\3\3\3\3\3\3\4\5\6\0\377\5\377\5\0\0\347\1\351\5N \5\0\331\0!\5\0\331"
  "\0\42\5\0\331\0#\6i\331\230\4$\7S\337\210H\2%\12-\331I\251\244TR\0&\11-"
  "\331\260\204J\21\3'\7$\331\10\345\1(\13,\331\11EB)\241H\0)\5Q\337\20*\7["
  "\333\210\344\0+\10[\333\211L\42\0,\5\211\331\10-\5\14\335 .\5I\331\10/\7$\331\13"
  "e\3\60\7,\331()\25\61\7k\331\20e\31\62\7,\331\240\331\10\63\10,\331\240Ef\5\64"
  "\10,\331\10\211j\11\65\7,\331\250\321\12\66\7,\331\250\225\12\67\11,\331\240\205b)\0\70\7"
  ",\331(\231\12\71\7,\331(\325\12:\6\231\331\210\4;\7b\331\211I\0<\7k\331\212\244%"
  "=\6\34\333 \22>\10k\331\210\245$\1\77\11,\331\240E\246!\0@\7,\331\250\254\21A\10"
  ",\331(\231D\1B\12,\331\230\204(!\12\0C\7,\331\250\245\21D\11,\331\230\204\224(\0"
  "E\10,\331\250Mb\4F\11,\331\250Mb\61\0G\11,\331\241EF\221\1H\11,\331\10\211"
  "L\242\0I\10+\331\230\204R\6J\7,\331K\23\25K\13,\331\10I\42\242HR\0L\7,"
  "\331\210e#M\10,\331\10\231\224\2N\12,\331\10M$#Q\0O\7,\331()\25P\10,"
  "\331(\331b\0Q\11,\331(\251D$\1R\11,\331(QB\242\0S\7,\331\250\321\12T\10"
  "+\331\230\204\262\0U\7,\331\10i*V\11,\331\10i\212H\0W\10,\331\10)\231\2X\13"
  ",\331\10\211\42\222\220(\0Y\10,\331\10\211j\5Z\11,\331\240E$\61\2[\7j\331\230\244"
  "\10\134\6$\331\10f]\7*\331\220\244\14^\6\23\337\311\0_\6\15\331\230\4`\6R\337\10\5"
  "a\7$\331!\11\7b\10,\331\210\225D\5c\7$\331\250\305\10d\7,\331\63\211\12e\10$"
  "\331(I\42\4f\11,\331\241\305&\61\0g\7,\327(\325\12h\11,\331\210\305J\242\0i\6"
  "a\331\210\10j\7+\327\212\206\10k\13,\331\210E\42\242HR\0l\7*\331\210d\21m\10$"
  "\331\10\231D\1n\10$\331\230\204\224\2o\7$\331(\211\12p\10,\327(\331b\0q\7,\327"
  "(\325\22r\10$\331!\311b\0s\6$\331\60\32t\11,\331\210Mb\61\2u\7$\331\10)"
  "\25v\11$\331\10)E$\0w\10$\331\10\211L\1x\12$\331\10\211\42\222P\0y\10,\327"
  "\10\211j\5z\6$\331 \61{\5\0\331\0|\5\0\331\0}\5\0\331\0~\5\0\331\0\177\5"
  "\0\331\0\200\5\0\331\0\201\5\0\331\0\202\5\0\331\0\203\5\0\331\0\204\5\0\331\0\205\5\0\331"
  "\0\206\5\0\331\0\207\5\0\331\0\210\5\0\331\0\211\5\0\331\0\212\5\0\331\0\213\5\0\331\0\214"
  "\5\0\331\0\215\5\0\331\0\216\5\0\331\0\217\5\0\331\0\220\5\0\331\0\221\5\0\331\0\222\5\0"
  "\331\0\223\5\0\331\0\224\5\0\331\0\225\5\0\331\0\226\5\0\331\0\227\5\0\331\0\230\5\0\331\0"
  "\231\5\0\331\0\232\5\0\331\0\233\5\0\331\0\234\5\0\331\0\235\5\0\331\0\236\5\0\331\0\237\5"
  "\0\331\0\240\5\0\331\0\241\5\0\331\0\242\5\0\331\0\243\5\0\331\0\244\5\0\331\0\245\5\0\331"
  "\0\246\5\0\331\0\247\5\0\331\0\250\5\0\331\0\251\5\0\331\0\252\5\0\331\0\253\5\0\331\0\254"
  "\5\0\331\0\255\5\0\331\0\256\5\0\331\0\257\5\0\331\0\260\5\0\331\0\261\5\0\331\0\262\5\0"
  "\331\0\263\5\0\331\0\264\5\0\331\0\265\5\0\331\0\266\5\0\331\0\267\5\0\331\0\270\5\0\331\0"
  "\271\5\0\331\0\272\5\0\331\0\273\5\0\331\0\274\5\0\331\0\275\5\0\331\0\276\5\0\331\0\277\5"
  "\0\331\0\300\5\0\331\0\301\5\0\331\0\302\5\0\331\0\303\5\0\331\0\304\5\0\331\0\305\5\0\331"
  "\0\306\5\0\331\0\307\5\0\331\0\310\5\0\331\0\311\5\0\331\0\312\5\0\331\0\313\5\0\331\0\314"
  "\5\0\331\0\315\5\0\331\0\316\5\0\331\0\317\5\0\331\0\320\5\0\331\0\321\5\0\331\0\322\5\0"
  "\331\0\323\5\0\331\0\324\5\0\331\0\325\5\0\331\0\326\5\0\331\0\327\5\0\331\0\330\5\0\331\0"
  "\331\5\0\331\0\332\5\0\331\0\333\5\0\331\0\334\5\0\331\0\335\5\0\331\0\336\5\0\331\0\337\5"
  "\0\331\0\340\5\0\331\0\341\5\0\331\0\342\5\0\331\0\343\5\0\331\0\344\5\0\331\0\345\5\0\331"
  "\0\346\5\0\331\0\347\5\0\331\0\350\5\0\331\0\351\5\0\331\0\352\5\0\331\0\353\5\0\331\0\354"
  "\5\0\331\0\355\5\0\331\0\356\5\0\331\0\357\5\0\331\0\360\5\0\331\0\361\5\0\331\0\362\5\0"
  "\331\0\363\5\0\331\0\364\5\0\331\0\365\5\0\331\0\366\5\0\331\0\367\5\0\331\0\370\5\0\331\0"
  "\371\5\0\331\0\372\5\0\331\0\373\5\0\331\0\374\5\0\331\0\375\5\0\331\0\376\5\0\331\0\377\5"
  "\0\331\0\0\0\0\4\377\377\0";


uint16_t last_pinValues;
uint16_t current_pinValues;
bool     keypad_pressed;


const uint8_t masks[8] = {1, 2, 4, 8, 16, 32, 64, 128};


//COLORS
const uint32_t playA = strip.Color(17, 201, 168);// normal mode trigs PLAYING
const uint32_t playB = strip.Color(166, 237, 198);// AB mode A trigs PLAYING
const uint32_t playD = strip.Color(0, 219, 100);// Pattern selection --> selected// Bank selection -> selected
const uint32_t playE = strip.Color(250, 100, 10);// Pattern selection screen selected
const uint32_t stepSeqCvOutBlank = strip.Color(49, 55, 232); // if selected track is NOT on this cvOut
const uint32_t recModeActive = strip.Color(255, 0, 0);
const uint32_t playBtn = strip.Color(45, 101, 168);


//UTILITIES
unsigned long lastDebounceTime_millis = 0;
unsigned long last_btn_DebounceTime_millis = 0;

unsigned long last_pads_DebounceTime_micros[TRACKS];
unsigned long pads_micros[TRACKS];


// ENCODERS STATES
bool fired_1 = false;
bool fired_2 = false;
bool enc1_clk = false;
bool enc2_clk = false;
bool last_encoder1_Btn_State = 1;
bool last_encoder2_Btn_State = 1;

// ALT
bool ALT_pushed = false;

uint8_t selectedSeqStep_byte = 0;
uint8_t selectedSeqStep_bitPos = 0;

// REMOTE BTNS
bool last_remoteBtn_Back_State = 0;
bool last_remoteBtn_Play_State = 0;
bool last_remoteBtn_Rec_State = 0;
bool last_remoteBtn_Alt_State = 0;
// SCREENS BTNS
bool last_patternBank_ButtonState = 0;
bool last_AbLength_ButtonState = 0;
bool last_muteSolo_ButtonState = 0;
bool last_fillShift_ButtonState = 0;
bool last_deleteClear_ButtonState = 0;


// TRIGDUR

const uint16_t trig_durations[6] = {250, 500, 1000, 2000, 5000, 10000}; // microseconds - default 2000 (2ms)
byte           trig_durations_Cursor = 3;
uint16_t       trig_DURATION = trig_durations[trig_durations_Cursor];

//CLOCK
//byte          tick = 0;
volatile bool clockPulse = HIGH;
volatile bool glidePulse = HIGH;

uint16_t  global_Step_16PPQN = 0;
uint16_t  last_global_Step_16PPQN = global_Step_16PPQN;

uint16_t  track_Step_16PPQN[TRACKS];

byte      global_Step_4PPQN = 0;
byte      last_global_Step_4PPQN = global_Step_4PPQN;
byte      global_Step_Counter16 = 0;
bool      displayFirstStep_1 = 1;


float  bpm = 120.0; // default BPM
//uint16_t bpm = 120;

unsigned long  step_micros = 31250; // default @ 120 bpm (16PPQN)
unsigned long  clock_time_16PPQN = 0;
unsigned long  last_clock_time_16PPQN = clock_time_16PPQN;


//PLAY - PAUSE - RECORDING
bool  PLAYING = 0;
bool  PLAY_pushed = 0;
bool  recMode = 0;

bool MICROTIMING = 0;

//PATTERN + BANK
byte  currentBank = 0;
byte  currentBankLED = 0;         // A ENLEVER QUAND STOCKAGE SUFFISANT POUR BANKS
byte  currentPattern = 0;
bool  patternChange_triggered = false;
byte  nextPattern = 0;

// PATTERN EDIT
const char* const bank_pattern_EditAction[] = {"RELOAD", "SAVE", "COPY", "ERASE"};
byte  bank_pattern_EditAction_Cursor = 0;
bool  patternToEdit_readyToAction = 0;  // 0 si pas clické sur "ACTION > COPY / ERASE", 1 si clické
bool  bankToEdit_readyToAction = 0;
byte  patternSelected = 255;
byte  bankSelected = 255;
byte  patternDestination = 255;
byte  bankDestination = 255;


//DELETE - CLEAR BUTTON
//bool  deleteClear_ButtonState = 0;

const char* const clear_trigsOrPitch_param[] = {"BOTH", "TRIGS", "PITCH"};
byte clear_trigsOrPitch_cursor = 0;

// CLEAR
const char* const clear_SelectTrack_param[] = {"BOTH", "A", "B"};
byte clear_SelectTrack_Cursor = 0;

//TRACK
byte selected_Track = 0;

// PATTERN LENGTH
const byte  pattern_LengthArray[6] = {4, 8, 16, 32, 64, 128};
bool        pattern_Length_Locked = 1;

// ROLL MODE
const byte     rollRates[11] = {1, 2, 3, 4, 6, 8, 12, 16, 24, 32, 64};
byte           rollRates_cursor = 3;
unsigned long  rollRate_micros = 0;
bool           rollMode = 0;

//FILL
byte   fillRates[7] = {16, 12, 8, 6, 4, 3, 2}; // "1/4", "1/8", "1/16", "1/32"
byte   fillRate_Cursor = 0;


//SCREEN DISPLAYED
byte screenConfig = 1;

bool  trigsOut_BitVal[DATA_WIDTH]; // 16 (voir pour mettre 14 -> tracks + clockOut / resetOut)
bool  trigsIn_BitVal[TRACKS];
bool  padPushed[TRACKS];
bool  padLogPushed[TRACKS];
bool  btnsIn_BitVal[4];

unsigned long button_micros[TRACKS];

// STEP SEQUENCER
byte     stepSeq_page = 0;
byte     stepSeq_global_Step_4PPQN = 0;
byte     stepSeq_clickedStep = 255;

unsigned int decimalValue_seq = 0;
unsigned int decimalValue_seq2 = 0;


bool update_shiftRegisters = false;    // permet de jouer des trigs venant des sequences
bool delayBeforeTrig = false;
bool timeFlag_arp = false;


// CHROMA MODE
const char* const chromaMode_UIs[] = {"KEYBOARD", "ARPEGGIATOR", "TRANSPOSE", "GLIDE", "CV OUT"};
// TESTER enum chromaMode_UI = {KEYBOARD, ARPEGGIATOR, TRANSPOSE, GLIDE,  CVOUT};
const char* const chromaMode_Glide_params_labels[] = {"TRACK", "STEP"};
byte chromaMode_cursor = 0;
bool chromaMode_KBoard = 1;
bool chromaMode_Arp = 0;
bool chromaMode_Transpose = 0;
bool chromaMode_Glide = 0;
bool GLIDE_paramSelection_cursor = 0; // type bool à changer si + de 2 params
bool GLIDE_paramSelection_active = 0; // si on a cliqué on non sur un param GLIDE pour le modifier
bool chromaMode_CVOutSelect = 0;

// CHROMA MODE - TRANSPOSE
//int transposeValue = 0; // -2 -> 5

const char* const chromaMode_Arp_params_labels[] = {"RATE", "TYPE"};
byte chromaMode_Arp_params = 0; // 0 = pas dans PARAMS, 1 = RATE, 2 = STYLE
const byte arp_Rate[5] = {1, 2, 4, 8, 16};
byte arp_Rate_Cursor = 2;
const char* const arp_Style[] = {"Up", "Down", "U+D"};
byte arp_Style_Cursor = 0;
byte arpNotes[ARP_MAX_STEPS];
byte arpSteps = 0;
byte arpCounter = 0;
bool newArp = 0;
bool arp_latch = 0;
bool arp_latch_active = 0; // si arp_latch == ON et pads puched
bool arp_latch_pads[TRACKS];
unsigned long arp_Time = 0;


//SETTINGS
bool  SYNC_mode_internal = 1;
bool  replicateBars = 1;
byte  goTo_copy = false;


bool enc1_Range_10 = 0;
bool enc2_Range_10 = 0;
bool pitch_Range_12 = 0;


const char* const settings_titles[] = {
  "LOOP MODE",
  "FIRSTSTEP",

  "CVin 1",
  "CVin 2",

  "COPY BARS",
  "SYNC",
  "BPM",
  "ClkIn DIV",
  "ClkOut DIV",
  "QUANTIZE",
  "SHUFFLE",

  "LFO1 OUTPUT",
  "LFO1 WAVE",
  "LFO1 RATE",
  "LFO1 GAIN",
  "LF01 PHASE",

  "LFO2 OUTPUT",
  "LFO2 WAVE",
  "LFO2 RATE",
  "LFO2 GAIN",
  "LFO2 PHASE",

  "PTN CHANGE", // patternChange_Quantized : 1 / 0

  "TRIG LENGTH",
  "LED BRIGHT."
};


byte settings_titles_Cursor = 0;


const unsigned int rampUp[LFO_WAVE_RESOLUTION] {
  300, 503, 707, 910, 1114, 1318, 1521, 1725, 1928, 2132, 2336, 2539, 2743, 2946, 3150, 3354, 3557, 3761, 3964, 4168, 4372, 4575, 4779, 4982,
  5186, 5390, 5593, 5797, 6000, 6204, 6408, 6611, 6815, 7018, 7222, 7426, 7629, 7833, 8036, 8240, 8444, 8647, 8851, 9054, 9258, 9462, 9665, 9869,
  10072, 10276, 10480, 10683, 10887, 11090, 11294, 11498, 11701, 11905, 12108, 12312, 12516, 12719, 12923, 13126, 13330, 13534, 13737, 13941, 14144,
  14348, 14552, 14755, 14959, 15162, 15366, 15570, 15773, 15977, 16180, 16384, 16588, 16791, 16995, 17198, 17402, 17606, 17809, 18013, 18216, 18420,
  18624, 18827, 19031, 19234, 19438, 19642, 19845, 20049, 20252, 20456, 20660, 20863, 21067, 21270, 21474, 21678, 21881, 22085, 22288, 22492, 22696,
  22899, 23103, 23306, 23510, 23714, 23917, 24121, 24324, 24528, 24732, 24935, 25139, 25342, 25546, 25750, 25953, 26157, 26360, 26564, 26768, 26971,
  27175, 27378, 27582, 27786, 27989, 28193, 28396, 28600, 28804, 29007, 29211, 29414, 29618, 29822, 30025, 30229, 30432, 30636, 30840, 31043, 31247,
  31450, 31654, 31858, 32061, 32265, 32468, 32672, 32876, 33079, 33283, 33486, 33690, 33894, 34097, 34301, 34504, 34708, 34912, 35115, 35319, 35522,
  35726, 35930, 36133, 36337, 36540, 36744, 36948, 37151, 37355, 37558, 37762, 37966, 38169, 38373, 38576, 38780, 38984, 39187, 39391, 39594, 39798,
  40002, 40205, 40409, 40612, 40816, 41020, 41223, 41427, 41630, 41834, 42038, 42241, 42445, 42648, 42852, 43056, 43259, 43463, 43666, 43870, 44074,
  44277, 44481, 44684, 44888, 45092, 45295, 45499, 45702, 45906, 46110, 46313, 46517, 46720, 46924, 47128, 47331, 47535, 47738, 47942, 48146, 48349,
  48553, 48756, 48960, 49164, 49367, 49571, 49774, 49978, 50182, 50385, 50589, 50792, 50996, 51200, 51403, 51607, 51810, 52014, 52200
};

const unsigned int rampDown[LFO_WAVE_RESOLUTION] {
  52200, 52014, 51810, 51607, 51403, 51200, 50996, 50792, 50589, 50385, 50182, 49978, 49774, 49571, 49367, 49164, 48960, 48756, 48553, 48349, 48146,
  47942, 47738, 47535, 47331, 47128, 46924, 46720, 46517, 46313, 46110, 45906, 45702, 45499, 45295, 45092, 44888, 44684, 44481, 44277, 44074, 43870,
  43666, 43463, 43259, 43056, 42852, 42648, 42445, 42241, 42038, 41834, 41630, 41427, 41223, 41020, 40816, 40612, 40409, 40205, 40002, 39798, 39594,
  39391, 39187, 38984, 38780, 38576, 38373, 38169, 37966, 37762, 37558, 37355, 37151, 36948, 36744, 36540, 36337, 36133, 35930, 35726, 35522, 35319,
  35115, 34912, 34708, 34504, 34301, 34097, 33894, 33690, 33486, 33283, 33079, 32876, 32672, 32468, 32265, 32061, 31858, 31654, 31450, 31247, 31043,
  30840, 30636, 30432, 30229, 30025, 29822, 29618, 29414, 29211, 29007, 28804, 28600, 28396, 28193, 27989, 27786, 27582, 27378, 27175, 26971, 26768,
  26564, 26360, 26157, 25953, 25750, 25546, 25342, 25139, 24935, 24732, 24528, 24324, 24121, 23917, 23714, 23510, 23306, 23103, 22899, 22696, 22492,
  22288, 22085, 21881, 21678, 21474, 21270, 21067, 20863, 20660, 20456, 20252, 20049, 19845, 19642, 19438, 19234, 19031, 18827, 18624, 18420, 18216,
  18013, 17809, 17606, 17402, 17198, 16995, 16791, 16588, 16384, 16180, 15977, 15773, 15570, 15366, 15162, 14959, 14755, 14552, 14348, 14144, 13941,
  13737, 13534, 13330, 13126, 12923, 12719, 12516, 12312, 12108, 11905, 11701, 11498, 11294, 11090, 10887, 10683, 10480, 10276, 10072, 9869, 9665,
  9462, 9258, 9054, 8851, 8647, 8444, 8240, 8036, 7833, 7629, 7426, 7222, 7018, 6815, 6611, 6408, 6204, 6000, 5797, 5593, 5390, 5186, 4982, 4779,
  4575, 4372, 4168, 3964, 3761, 3557, 3354, 3150, 2946, 2743, 2539, 2336, 2132, 1928, 1725, 1521, 1318, 1114, 910, 707, 503, 300
};

const unsigned int sine[LFO_WAVE_RESOLUTION] {
  26250, 26887, 27523, 28159, 28794, 29427, 30058, 30686, 31313, 31936, 32555, 33171, 33783, 34390, 34992, 35589, 36181, 36766, 37345, 37917, 38483, 39041,
  39591, 40133, 40667, 41192, 41708, 42215, 42713, 43200, 43677, 44144, 44599, 45044, 45478, 45900, 46310, 46708, 47093, 47466, 47827, 48174, 48508, 48829,
  49136, 49429, 49709, 49974, 50225, 50461, 50683, 50890, 51083, 51260, 51422, 51569, 51701, 51818, 51919, 52005, 52075, 52130, 52169, 52192, 52200, 52192,
  52169, 52130, 52075, 52005, 51919, 51818, 51701, 51569, 51422, 51260, 51083, 50890, 50683, 50461, 50225, 49974, 49709, 49429, 49136, 48829, 48508, 48174,
  47827, 47466, 47093, 46708, 46310, 45900, 45478, 45044, 44599, 44144, 43677, 43200, 42713, 42215, 41708, 41192, 40667, 40133, 39591, 39041, 38483, 37917,
  37345, 36766, 36181, 35589, 34992, 34390, 33783, 33171, 32555, 31936, 31313, 30686, 30058, 29427, 28794, 28159, 27523, 26887, 26250, 25613, 24977, 24341,
  23706, 23073, 22442, 21814, 21187, 20564, 19945, 19329, 18717, 18110, 17508, 16911, 16319, 15734, 15155, 14583, 14017, 13459, 12909, 12367, 11833, 11308,
  10792, 10285, 9787, 9300, 8823, 8356, 7901, 7456, 7022, 6600, 6190, 5792, 5407, 5034, 4673, 4326, 3992, 3671, 3364, 3071, 2791, 2526, 2275, 2039, 1817, 1610,
  1417, 1240, 1078, 931, 799, 682, 581, 495, 425, 370, 331, 308, 300, 308, 331, 370, 425, 495, 581, 682, 799, 931, 1078, 1240, 1417, 1610, 1817, 2039, 2275,
  2526, 2791, 3071, 3364, 3671, 3992, 4326, 4673, 5034, 5407, 5792, 6190, 6600, 7022, 7456, 7901, 8356, 8823, 9300, 9787, 10285, 10792, 11308, 11833, 12367,
  12909, 13459, 14017, 14583, 15155, 15734, 16319, 16911, 17508, 18110, 18717, 19329, 19945, 20564, 21187, 21814, 22442, 23073, 23706, 24341, 24977, 25613
};

const unsigned int triangle[LFO_WAVE_RESOLUTION] {
  26250, 26655, 27061, 27466, 27872, 28277, 28683, 29088, 29494, 29899, 30305, 30710, 31116, 31521, 31927, 32332,
  32738, 33143, 33548, 33954, 34359, 34765, 35170, 35576, 35981, 36387, 36792, 37198, 37603, 38009, 38414, 38820,
  39225, 39630, 40036, 40441, 40847, 41252, 41658, 42063, 42469, 42874, 43280, 43685, 44091, 44496, 44902, 45307,
  45713, 46118, 46523, 46929, 47334, 47740, 48145, 48551, 48956, 49362, 49767, 50173, 50578, 50984, 51389, 51795,
  52200, 51795, 51389, 50984, 50578, 50173, 49767, 49362, 48956, 48551, 48145, 47740, 47334, 46929, 46523, 46118,
  45713, 45307, 44902, 44496, 44091, 43685, 43280, 42874, 42469, 42063, 41658, 41252, 40847, 40441, 40036, 39630,
  39225, 38820, 38414, 38009, 37603, 37198, 36792, 36387, 35981, 35576, 35170, 34765, 34359, 33954, 33548, 33143,
  32738, 32332, 31927, 31521, 31116, 30710, 30305, 29899, 29494, 29088, 28683, 28277, 27872, 27466, 27061, 26655,
  26250, 25845, 25439, 25034, 24628, 24223, 23817, 23412, 23006, 22601, 22195, 21790, 21384, 20979, 20573, 20168,
  19763, 19357, 18952, 18546, 18141, 17735, 17330, 16924, 16519, 16113, 15708, 15302, 14897, 14491, 14086, 13680,
  13275, 12870, 12464, 12059, 11653, 11248, 10842, 10437, 10031, 9626, 9220, 8815, 8409, 8004, 7598, 7193,
  6788, 6382, 5977, 5571, 5166, 4760, 4355, 3949, 3544, 3138, 2733, 2327, 1922, 1516, 1111, 705,
  300, 705, 1111, 1516, 1922, 2327, 2733, 3138, 3544, 3949, 4355, 4760, 5166, 5571, 5977, 6382,
  6788, 7193, 7598, 8004, 8409, 8815, 9220, 9626, 10031, 10437, 10842, 11248, 11653, 12059, 12464, 12870,
  13275, 13680, 14086, 14491, 14897, 15302, 15708, 16113, 16519, 16924, 17330, 17735, 18141, 18546, 18952, 19357,
  19763, 20168, 20573, 20979, 21384, 21790, 22195, 22601, 23006, 23412, 23817, 24223, 24628, 25034, 25439, 25845
};



//EEPROM
#define EEPROM_INIT  0            // 0x08060000 -- 134610944 -- 1 BIT pour savoir si EEPROM Init ou non
#define BANK_ADR_0   1            // 0x08060001 -- 134610945 -- PATTERNS * BIT (12)
#define PATT_ADR_0   16           // 0x08060010 -- 134610960
#define PATT_ADR_1   4766         // 0x08060DF8 --  
#define PATT_ADR_2   9516         // 0x08061BE0 -- 
#define PATT_ADR_3   14266        // 0x080629C8 -- 
#define PATT_ADR_4   19016        // 0x080637B0 -- 
#define PATT_ADR_5   23766        // 0x08064598 -- 
#define PATT_ADR_6   28516        // 0x08065380 -- 
#define PATT_ADR_7   33266        // 0x08066168 -- 
/*#define PATT_ADR_8   38016        // 0x08066F50 --
  #define PATT_ADR_9   42766        // 0x08067D38 --
  #define PATT_ADR_10  47516        // 0x08068B20 --
  #define PATT_ADR_11  52266        // 0x08069908 --*/

const int patterns_addresses[PATTERNS] {
  PATT_ADR_0,
  PATT_ADR_1,
  PATT_ADR_2,
  PATT_ADR_3,
  PATT_ADR_4,
  PATT_ADR_5,
  PATT_ADR_6,
  PATT_ADR_7/*,
  PATT_ADR_8,
  PATT_ADR_9,
  PATT_ADR_10,
  PATT_ADR_11*/
};


// LFOs
const char* const LFOs_output[] = {"CvOut1", "CvOut2", "CvOut3", "CvOut4"};
const char* const LFOs_waves[] = {"RAMPUP", "RAMPDN", "SINE", "TRI", "RANDOM"};
const char* const LFOs_rates[] = {"/8", "/6", "/4", "/3", "/2", "=", "x2", "x3", "x4", "x6", "x8", "x16", "x32"};

struct lfo_Params {
  unsigned long rate_micros = (step_micros * 4) / LFO_WAVE_RESOLUTION;
  unsigned int  counter = 0;
  unsigned int randomVal; 
} LFO[2];  // ex. LFO[0].counter;


// CVinputs
const char* const cvIn_modulation_Target[] = {"PATTERN", "BANK", "STEP", "PAT.LGTH", "1stStep", "L1 WAVE", "L1 RATE", "L2 WAVE", "L2 RATE"};

struct CVin_params {
  bool  enable = 0;
  byte  dest_Cursor = 0;
  byte  dest_Selected = 0;
} CVin[2];


// DACs
struct dac_Params {
  //bool            timeFlag_glide;
  bool            glideActive;
  bool            gliding;
  bool            glide_dir;
  unsigned long   glide_duration_micros;
  unsigned int    incrementValue;
  unsigned int    startPitch;
  unsigned int    endPitch;
  unsigned int    currentPitch;
} DaC[4]; // ex. DaC[0].glide_duration_micros

bool timeFlag_glide;

/*
  struct globalParams {
  float   internal_BPM;                               //2
  bool    quantizeBeat;                               //1
  }
*/

// PATTERN
struct patternStructure {
  int8_t  shiftValue[TRACKS];                         // 12
  bool    glide_mode[TRACKS];                         // 12
  byte    glide_steps[TRACKS];                        // 12 /* nbr de steps : longueur portamento pour chaque cvOut track (4) => 0 à 16 */
  byte    fill_rate[TRACKS];                          // 12
  byte    pattern_Length;                             // 1    en 4PPQN
  byte    track_Length[TRACKS];                       // 12   en 4PPQN
  byte    AB_State[TRACKS];                           // 12
  byte    cvOut_Tracks[4];                            // 4    /* 0 à 11 = track  ---  12 = none  ---  13 = LFO[0]  ---  14 = LFO[1] */

  bool    track_isGate[TRACKS]; // 12

  byte    Trigs[TRACKS_AB][MAX_STEPS_16PPQN / 8];     // 1536
  byte    Pitch[TRACKS_AB][MAX_STEPS_4PPQN];          // 3072
  int8_t  transposeValue[TRACKS_AB];                  // 24  /* -2 -> +5 */

  bool    tracksRetrig;                               // 1    1 = RETRIG  ---  0 = FREE
  bool    lfo_enable[2];                              // 2
  byte    lfo_dest[2];                                // 2
  byte    lfo_wave[2];                                // 2
  byte    lfo_rate[2];                                // 2
  byte    lfo_gain[2];                                // 2
  int8_t  lfo_phase[2];                               // 2

  uint16_t internal_BPM;                               //2
  byte    clkIn_Div;                                  //1
  byte    clkOut_Div;                                 //1
  bool    quantizeBeat;                               //1
  byte    shuffle;                                    //1

  byte    track_has_Trigs[TRACKS_AB / 8];             // 3  - track_has_Trigs[(0000.0000) (0000.0000) (0000.0000)]
  byte    mute_solo_States[TRACKS_AB / 8];            // 3  - mute[(0000.0000) (0000].[0000) (0000.0000)]solo
  byte    fillState[2];                               // 2  - fillState[(0000.0000) (0000./*0000*/)]
  byte    firstStep;                                  // 1

  bool    allAorB;                                    // 1      ///////////////// à virer ////////////
  bool    glide_active;                               // 1
  bool    shift_active;                               // 1
  bool    mute_active;                                // 1
  bool    allMute_active;                             // 1
  bool    solo_active;                                // 1
  bool    fill_active;                                // 1
};                                                    // total = 4710  -> (on arrondi la taille d'un pattern à 4750 pour le stockage EEPROM)
//bank[currentBank].pattern[currentPattern].track_isGate[TRACKS];


// BANK
struct bankStructure {
  bool   notEmpty_Pattern[PATTERNS];                  // 12
  byte   songPatterns[16];                            // 16
  struct patternStructure pattern[PATTERNS];          // 3549*12 = 42588
} bank[BANKS];                                        // total = 42616 (42588 + 12 + 16)

// arrondi à 42928 (3575 * 12) + 12 + 16



bool BPM_isGlobal = 1;          // il faudra ajouter ce setting dans SETTINGS screen
bool Trigs_Undo_enable = 0;
bool Pitch_Undo_enable = 0;
byte Trigs_Undo_buffer[TRACKS_AB][MAX_STEPS_16PPQN / 8];
byte Pitch_Undo_buffer[TRACKS_AB][MAX_STEPS_4PPQN];


// SONG MODE
bool songMode_Activated;
byte songMode_current_Step_pot; // cursor
byte songMode_current_Step_PLAYING;


byte step_seq_ab[TRACKS];
bool read_Seq_dont_play_this[TRACKS];

bool pitchMode;
bool glideMode;



const char* const notesNames[96] = {
  "C0", "C#0", "D0", "D#0", "E0", "F0", "F#0", "G0", "G#0", "A0", "A#0", "B0",
  "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
  "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
  "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
  "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
  "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5",
  "C6", "C#6", "D6", "D#6", "E6", "F6", "F#6", "G6", "G#6", "A6", "A#6", "B6",
  "C7", "C#7", "D7", "D#7", "E7", "F7", "F#7", "G7", "G#7", "A7", "A#7", "B7"
};


const unsigned int notesValues[96] = { // default pitch = 26300
  15900, 16350, 16850, 17150, 17550, 18000, 18450, 18850, 19300, 19800, 20200, 20650, // 0  - à calibrer
  21100, 21550, 22050, 22350, 22750, 23200, 23650, 24050, 24500, 25000, 25400, 25850, //12
  26300, 26700, 27100, 27500, 27900, 28300, 28700, 29200, 29700, 30100, 30550, 31000, //24
  31450, 31850, 32250, 32750, 33150, 33550, 34050, 34400, 34900, 35350, 35800, 36200, //36
  36650, 37050, 37500, 37950, 38400, 38800, 39300, 39650, 40100, 40500, 40950, 41400, //48
  41800, 42200, 42700, 43100, 43500, 43900, 44400, 44800, 45200, 45650, 46050, 46500, //60
  47000, 47350, 47800, 48200, 48600, 49100, 49500, 50000, 50400, 50800, 51250, 51700, //72
  52150, 52500, 52950, 53450, 53850, 54350, 54750, 55250, 55650, 56050, 56500, 56950 //84 - à calibrer
};


byte pitchPointer = 24;
byte pitchPointer_Pot = 24;
byte pitchPointer_Pad = 24;
byte octaveValue = 24;


byte modulated_currentPattern;
byte modulated_currentBank;
uint16_t modulated_pattern_Length;
byte modulated_firstStep;
uint16_t modulated_global_Step;
byte modulated_lfo1Wave;
byte modulated_lfo1Rate;
byte modulated_lfo2Wave;
byte modulated_lfo2Rate;


bool update_padLeds = true;
bool update_padLeds_off = false;

//bool update_screen = true;
bool update_screen_INIT = true;
bool update_screen_TOP = true;     // si on recrée JUSTE TOP ROW (steps / avancement)
bool update_screen_POTS_DIAL_L = true;  // si on recrée JUSTE LEFT POTS PARAMS (dial / click)
bool update_screen_POTS_DIAL_R = true;  // si on recrée JUSTE RIGHT POTS PARAMS (dial / click)
bool update_screen_POTS_CLK_L = true;  // si on recrée JUSTE LEFT POTS PARAMS (dial / click)
bool update_screen_POTS_CLK_R = true;  // si on recrée JUSTE RIGHT POTS PARAMS (dial / click)
bool update_screen_PADS = true;    // si on recrée JUSTE certains éléments (au click sur pads par ex.)
bool update_screen_ALT_BTN = true;


unsigned long last_sr_roll_time1, last_sr_roll_time2 = 0;

unsigned long beat_micros_realtime = 0;


bool canPass1 = 0;
bool canPass2 = 0;

bool saving_pattern;

bool patternChange_Quantized = 0;


bool last_encoder1_A_State;
bool last_encoder2_A_State;
unsigned long last_enc_DebounceTime_millis[2];
int8_t enc1_dir = 0;
int8_t enc2_dir = 0;


int pitchOut;

//bool padTapped = false; // true si on tape un pad en appuyany sur ALT

//volatile unsigned long CLOCK_EXT_COUNTER = 0;


bool confirmed_CvOutReassign = false;
bool trigger_cvOutReassign_process = false;
byte cvOutputPad;


bool needToCheck_Arp = 0;
bool allowToCheck_Arp = 0;
unsigned long datime = 0;
unsigned long last_datime = 0;
unsigned long last_pad_push = 0;
byte scan_pads_Arp = 0;

bool fillDataRecording;
uint16_t fillDataRecording_startStep;


//unsigned int timeInitial_global = 0;
//unsigned int time10 = 0;


bool canResetScreen = false; // ALT BTN




void setup(void) {
  u8g2.begin();

  strip.begin();
  strip.show();
  strip.setBrightness(led_brightness);

  Dac.begin();

  pinMode(SR_Input_p_load, OUTPUT);  //DATA LOAD PIN (PL)
  pinMode(SR_Input_data, INPUT);    //DATA IN PIN (Q7)
  pinMode(SR_Input_clock, OUTPUT); //CLOCK OUT PIN (CP)

  pinMode(SR_Output_latch, OUTPUT);
  pinMode(SR_Output_clock, OUTPUT);
  pinMode(SR_Output_data, OUTPUT);

  pinMode(altBtnLed, OUTPUT);

  pinMode(patternBank_Button, INPUT);
  pinMode(AbLength_Button, INPUT);
  pinMode(muteSolo_Button, INPUT);
  pinMode(fillShift_Button, INPUT);
  pinMode(deleteClear_Button, INPUT);

  pinMode(LED_DATA, OUTPUT);

  pinMode(clockInPin, INPUT);
  pinMode(resetInPin, INPUT);

  digitalWrite(SR_Input_clock, LOW);
  digitalWrite(SR_Input_p_load, HIGH);

  pinMode(encoder1_A_Pin, INPUT_PULLUP);
  pinMode(encoder1_B_Pin, INPUT_PULLUP);
  pinMode(encoder1_Btn, INPUT_PULLUP);
  pinMode(encoder2_A_Pin, INPUT_PULLUP);
  pinMode(encoder2_B_Pin, INPUT_PULLUP);
  pinMode(encoder2_Btn, INPUT_PULLUP);

  pinMode(CV_IN_1, INPUT);
  pinMode(CV_IN_2, INPUT);

  last_encoder1_A_State = digitalRead(encoder1_A_Pin);
  last_encoder2_A_State = digitalRead(encoder2_A_Pin);

  for (byte i = 0; i < PATTERNS; ++i) {
    reset_Pattern(currentBank, i);
  }

  if (EEPROM.read(EEPROM_INIT) != 0xFF) {
    loadBank(0);
  }

  for (byte track = 0; track < TRACKS; ++track) {
    step_seq_ab[track] = bank[currentBank].pattern[currentPattern].AB_State[track];
    track_Step_16PPQN[track] = 0; // bank[currentBank].pattern[currentPattern].firstStep ??
  }

  global_Step_4PPQN = bank[currentBank].pattern[currentPattern].firstStep; // régler ça (voir plus haut aussi (track_Step_16PPQN[track] = 0;)

  current_pinValues = 0;

  last_pinValues = current_pinValues;

  //step_micros = calc_step_micros();
  loadPattern_params();

  CLOCK_INT_timer.setInterval(step_micros, do_MASTERClock);

  if (!SYNC_mode_internal) {
    CLOCK_INT_timer.disableTimer();
    attachInterrupt(digitalPinToInterrupt(clockInPin), do_MASTERClock, FALLING);
  }

  GLIDE_timer.setInterval(1000, do_GLIDEClock);

  if (bank[currentBank].pattern[currentPattern].glide_active)
    GLIDE_timer.enableTimer();
  else
    GLIDE_timer.disableTimer();

  //update_LFO_Rate_micros(); //si CLOCK EXT on n'a pas encore la bonne valeur de step_micros
  //LFOs_enable_disable();
  //get_RollRateCalc();


  startUpSequence();

  drawPage_mainScreen();
}




void loop(void) {
  //timeInitial_global = micros();

  clockBeats();

  read_shift_registers();

  make595Play_Trigs(decimalValue_TOTAL());

  makeDACplayNotes_from_Sequences();

  makeStripDisplayRemote();

  //read_shift_registers();

  //poll_encoders();
  set_UI_Config();

  //refresh_Screen();


  /*catch_Mode_Selection_Change();

    poll_encoders();

    detect_remoteBtnsPush();*/

  //time10 = micros() - timeInitial_global;
}
