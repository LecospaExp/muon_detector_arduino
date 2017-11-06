// for neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


// for Queue
#include <QList.h>
#include "QList.cpp"
// basic pin assignment
#define PinA 6
#define PinB 7
#define PinC 8
#define PinD 9
#define Buzzer 5
#define TRG 2
#define LED 3

// song 
const int toneTable[7][5]={
    { 66, 131, 262, 523, 1046},  // C Do
    { 74, 147, 294, 587, 1175},  // D Re
    { 83, 165, 330, 659, 1318},  // E Mi
    { 88, 175, 349, 698, 1397},  // F Fa
    { 98, 196, 392, 784, 1568},  // G So
    {110, 220, 440, 880, 1760},  // A La
    {124, 247, 494, 988, 1976}   // B Si
};

int happy_tone[62] = {3,3,4,5,5,4,3,2,1,1,2,3,3,2,2,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1,2,2,3,1,2,3,4,3,1,2,3,4,3,2,1,2,5,3,3,4,5,5,4,3,2,1,1,2,3,2,1,1};
int happy_series[62] = {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
int songEnd = 62;
//Parameter
#define LED_size 88
#define LED_group_size 11
#define MaxSoundBuffer 5
#define LED_decay_factor 1.1


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_size, LED, NEO_GRB + NEO_KHZ800);
QList<int>   queue;
char led[11][3];
int CountStopTimer = 0;
int songIdx = 0;


void setup()
{
  Serial.begin(57600);
  Serial.println("HelloWorld! Taiwan number one!");
  //sets all the address pins as outputs
  pinMode(PinA, INPUT); 
  pinMode(PinB, INPUT); 
  pinMode(PinC, INPUT); 
  pinMode(PinD, INPUT); 
  pinMode(Buzzer, OUTPUT); 
  pinMode(TRG, INPUT); 
  attachInterrupt(digitalPinToInterrupt(TRG), trigger, RISING) ;

  for (int i = 0; i < LED_group_size; ++i)
  {
    led[i][0] = 0;
    led[i][1] = 0;
    led[i][2] = 0;
  }
}

void loop()
{
  
  if(queue.size()>0){
    int newEvtCH = queue.front();
    if(newEvtCH<1||newEvtCH>9){
      return;
    }
    queue.pop_front();
   
    Serial.println(newEvtCH);

    if(queue.size()>MaxSoundBuffer&&CountStopTimer==0)CountStopTimer = MaxSoundBuffer;
    if(CountStopTimer>0){
      CountStopTimer--;
    }else{
      if(songIdx<songEnd)tone(Buzzer, toneTable[happy_tone[songIdx++]-1][happy_series[songIdx]+1]);
      else songIdx = 0;    
    }
    
  }

  // led decay
  for (int i = 0; i < LED_group_size; ++i)
  {
    led[i][0]/=LED_decay_factor;
    led[i][1]/=LED_decay_factor;
    led[i][2]/=LED_decay_factor;
    for (int j = 0; j < 8; ++j)
    {
      pixels.setPixelColor(j+i*8, pixels.Color(led[i][0],led[i][1],led[i][2]));  
    }
  }
  pixels.show();
  if(CountStopTimer==0){
    delay(100);
    noTone(Buzzer);
  }
  delay(50);
}

void trigger()
{
  int channel = (int)!digitalRead(PinD)*8+(int)!digitalRead(PinC)*4+(int)!digitalRead(PinB)*2+(int)!digitalRead(PinA)*1;
  if(channel!=0)queue.push_back(channel);
}



