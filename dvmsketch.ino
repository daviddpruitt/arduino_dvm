// Reading delay time in millis
#define DELAY_TIME 547

// ADC reference voltage
#define VREF 5.00
// resistor ratio 20.93ish
// ADC scale value divider and calibration
#define VSCALE_0 (4.84)
#define VSCALE_2 (4.845)
#define VSCALE_4 (4.84)
#define VSCALE_6 (4.81)

#define A_SCALE 10
#define V_TO_A_GAIN_0  .0415
#define OFFSET_3      1.16
#define V_TO_A_GAIN_1  .27

// old values for the high gain cirtcuit
//#define OFFSET_5      1.21
//#define V_TO_A_GAIN_2  .26
#define OFFSET_5      .67
#define V_TO_A_GAIN_2  1.57

// V to AMPS
#define V_AMP 5.4054
#define OFFSET 2.49

// ADC max value
#define ADC_MAX 1023

// ADC int to volts
#define ADC_SCALE (VREF / 1023)

// convert from int to voltage
#define ADC_SCALE_0 ((VREF / 1023) * VSCALE_0)
#define ADC_SCALE_2 ((VREF / 1023) * VSCALE_2)
#define ADC_SCALE_6 ((VREF / 1023) * VSCALE_6)

// global variables
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;
int millisec = 0;

void setup() {
  // setup serial communication
  Serial.begin(9600);
  // use lower voltage vref
  //analogReference(INTERNAL2V56);
  // set pins to input
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);

  // print header
  Serial.println("date,amps0,mw0,amps1,mw1,amps2,mw2");

}
void looper() {
  // print time
  time(millis());
  Serial.println();
  //grab 10,000 samples
  long test = 0;

  for (int i = 0; i < 1000; ++i) {
    test += analogRead(2);
  }
  Serial.println(test / 1000);
  time(millis());
  Serial.println();

}

void loop() {
  // print time
  time(millis());

  /*****
   *  first two pins
   *
   * all the sequences work essentially the same
   * first read and average 1000 samples
   * scale the values appropriately to voltage
   * then convert the voltage to amperage for the current sensor
   * obtain watts from volts and amps
   * finally print out the readings
   */

  long V0 = 0;
  long V1 = 0;
  for (int i = 0; i < 1000; ++i) {
    V0 += analogRead(0);
    V1 += analogRead(1);
  }

  V0 /= 1000;
  V1 /= 1000;

  float Volts_0 = ADC_SCALE_0 * 1.0 * V0;
  float Volts_1 = V1 * ADC_SCALE * V_TO_A_GAIN_0;
  float Amps_1 = Volts_1 * A_SCALE * 1000;
  float Watts_1 = (Volts_0-Volts_1) * Amps_1;
  Serial.print(",");Serial.print(Amps_1);Serial.print(",");Serial.print(Watts_1);
  /******
   * Second two pins
   */

  long V2 = 0;
  long V3 = 0;
  for (int i = 0; i < 1000; ++i) {
    V2 += analogRead(4);
    V3 += analogRead(5);
  }

  V2 /= 1000;
  V3 /= 1000;

  float Volts_2 = ADC_SCALE_2 * 1.0 * V2;
  float Volts_3 = V3 * ADC_SCALE; // *;
  float Amps_3 = (Volts_3 - OFFSET_3) *  V_TO_A_GAIN_1 * 1000;
  float Watts_3 = Volts_2 * Amps_3;
  Serial.print(",");Serial.print(Amps_3);Serial.print(",");Serial.print(Watts_3);

  // Last pins
  long V4 = 0;
  long V5 = 0;
  for (int i = 0; i < 1000; ++i) {
    V4 += analogRead(8);
    V5 += analogRead(9);
  }

  V4 /= 1000;
  V5 /= 1000;

  float Volts_4 = ADC_SCALE_6 * 1.0 * V4;
  float Volts_5 = V5 * ADC_SCALE;// * ;
  float Amps_5 = (Volts_5 - OFFSET_5) * V_TO_A_GAIN_2 * 1000;
  float Watts_5 = Volts_4 * Amps_5;
  Serial.print(",");Serial.print(Amps_5);Serial.print(",");Serial.print(Watts_5);

  Serial.println();

  // wait for a bit before reading again
  delay(DELAY_TIME);
}

// time printing code
// adapted from
// https://forum.arduino.cc/index.php?topic=38158.0

// defs for time
// macros from DateTime.h
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)

/* Useful Macros for getting elapsed time */
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

void time(long val){
  int milliseconds = val % 1000;
  val = val / 1000;
  int days = elapsedDays(val);
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);

  // digital clock display of current time
  //printDigits(days);

  if (days < 10)
    Serial.print('0');
  if (days < 100)
    Serial.print('0');
  Serial.print(days,DEC);
  printDigits(hours);
  printDigits(minutes);
  printDigits(seconds);

  if (milliseconds < 10)
    Serial.print('0');
  if (milliseconds < 100)
    Serial.print('0');
  Serial.print(milliseconds,DEC);
}

void printDigits(byte digits){
  // utility function for digital clock display: prints colon and leading 0
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits,DEC);
}
