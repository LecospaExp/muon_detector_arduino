// for neopixel
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


// basic pin assignment
#define PinA 8
#define PinB 9
#define PinC 10
#define PinD 11
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
#define LED_decay_factor 10


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LED_size, LED, NEO_GRB + NEO_KHZ800);
int newEvt = 0;
int led[LED_group_size][3];
int songIdx = 0;

void setup()
{
  //sets all the address pins as outputs
  pinMode(PINB, INPUT); 
  pinMode(PinA, INPUT); 
  pinMode(PinB, INPUT); 
  pinMode(PinC, INPUT); 
  pinMode(PinD, INPUT); 
  pinMode(Buzzer, OUTPUT); 
  pinMode(TRG, INPUT); 

  Serial.begin(115200);
  Serial.println("HelloWorld! Taiwan number one!");

  pixels.begin();

  for (int i = 0; i < LED_group_size; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      pixels.setPixelColor(i*8+j, pixels.Color(50, 50, 50));  
    }
    pixels.show();    
    delay(100);
    for (int j = 0; j < 8; ++j)
    {
      pixels.setPixelColor(i*8+j, pixels.Color(0, 0, 0));  
    }
    pixels.show();    
  }






  for (int i = 0; i < LED_group_size; ++i)
  {
    led[i][0] = 0;
    led[i][1] = 0;
    led[i][2] = 0;
  }


  for(int i=0; i<7; i++){
    tone(Buzzer, toneTable[i][3]);
    delay(100);
    noTone(Buzzer);
    delay(50);
  }

  attachInterrupt(0, trigger, RISING) ;
  delay(100);
}

void loop()
{
  int newEvtCH = newEvt;
  newEvt = 0;
  if(newEvtCH>=1&&newEvtCH<=9){
    if(songIdx>=songEnd)songIdx = 0;
    tone(Buzzer, toneTable[happy_tone[songIdx++]-1][happy_series[songIdx]+1]);
    float theta = random(0, 3141)/1000.;
    int r = sin(theta)+abs(sin(theta));
    int g = sin(theta+3.141/3)+abs(sin(theta+3.141/3));
    int b = sin(theta+3.141*2/3)+abs(sin(theta+3.141*2/3));
    led[newEvtCH-1][0] = 32*(r*r);
    led[newEvtCH-1][1] = 32*(g*g);
    led[newEvtCH-1][2] = 32*(b*b);

    led[newEvtCH-2][0] = 32*(r*r);
    led[newEvtCH-2][1] = 32*(g*g);
    led[newEvtCH-2][2] = 32*(b*b);
    
    
  }
  //reset newEvtCH
  newEvtCH=0;

  // led decay
  for (int i = 0; i < LED_group_size; ++i)
  {
    for (int j = 0; j < 8; ++j)
    {
      pixels.setPixelColor(i*8+j, pixels.Color(led[i][0], led[i][1], led[i][2]));  
    }
    led[i][0]/=LED_decay_factor;
    led[i][1]/=LED_decay_factor;
    led[i][2]/=LED_decay_factor;
  }

  pixels.show();
  delay(100);
  noTone(Buzzer);
  delay(20);
//prevent memory outage


}

void trigger()
{
  newEvt=63-PINB;
  Serial.print(newEvt);
}
