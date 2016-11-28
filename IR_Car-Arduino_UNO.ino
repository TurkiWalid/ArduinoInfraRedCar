#include <IRremote.h>
#include <Servo.h>

#define LEDpower 13
#define LEDup 10
#define LEDdown 11
#define LEDleft 9
#define LEDright 12
#define Acup 5
#define Acdown 6
#define IRpin 2
#define serv 7
#define dleft A1
#define dright A3
#define dcenter A2

int ini=1,cont=1,butdown=0,dir=0,i,pw=1;
IRrecv ir_recv(IRpin);
decode_results results;
unsigned long time,ecl=500;
Servo Drive_wheel;

long Power;
long Up;
long Down;
long Upleft;
long Downleft;
long Upright;
long Downright;

void setup()
{
  Serial.begin(9650);

  Drive_wheel.attach(serv);
  Drive_wheel.write(105);

  ir_recv.enableIRIn(); // Start the receiver  
  for(i=5;i<14;i++)
      pinMode(i,OUTPUT);
  pinMode(IRpin,INPUT);

  //init 

   for (i=9;i<14;i++)  digitalWrite(i,HIGH);
                 delay(400);
   for (i=9;i<14;i++)  digitalWrite(i,LOW);

   Save_Conf(&Power,&Up,&Down,&Upleft,&Upright,&Downleft,&Downright); ini=0;
  ///init
}

void loop() 
{     
      GO();        
        if (Obstacle()) {digitalWrite(butdown,LOW);Change_Dir();}
  if (ir_recv.decode(&results))
    {  time = millis();
              //Serial.println(results.value,HEX);
        if (results.value==Up) 
        	{digitalWrite(Acup,HIGH); digitalWrite(Acdown,LOW); butdown=Acup;}
        else if (results.value==Down )
        	{digitalWrite(Acdown,HIGH); digitalWrite(Acup,LOW); butdown=Acdown;}
        else if (results.value==Upright )
                {Drive_wheel.write(140); digitalWrite(Acup,HIGH); digitalWrite(Acdown,LOW);  butdown=Acup;dir=1;}
        else if (results.value==Downright )
                {Drive_wheel.write(140); digitalWrite(Acup,LOW); digitalWrite(Acdown,HIGH); butdown=Acdown;dir=1;}
        else if (results.value==Upleft )
                {Drive_wheel.write(65); digitalWrite(Acup,HIGH); digitalWrite(Acdown,LOW); butdown=Acup;dir=1;}
        else if (results.value==Downleft )
                {Drive_wheel.write(65); digitalWrite(Acup,LOW); digitalWrite(Acdown,HIGH); butdown=Acdown;dir=1;}
        else if (results.value==Power)
               {Stay_inline(ir_recv,Power);delay(200);}
      ir_recv.resume(); 

    }
      if (butdown && millis()-time>120)
	      {
                    digitalWrite(butdown,LOW);   
                    if (dir) Drive_wheel.write(105);
	      butdown=0;
              dir=0;
	      }
}

int Stay_inline(IRrecv ir_recv,long Power)
{
  Play_Leds();
  digitalWrite(LEDpower,HIGH);
  while(1)
  {
    /*if (ir_recv.decode(&results) &&(results.value==Power))
      {digitalWrite(LEDpower,LOW);return 0;}*/

      //Put here  : Speed reg + Go (exmpl "delay(80); digitalWrite(Acup,LOW); delay(50); digitalWrite(Acup,HIGH);")

      if (inlin(dcenter))
      {
        if (inlin(dleft)) Drive_wheel.write(115);
        else if (inlin(dright)) Drive_wheel.write(85);
        else Drive_wheel.write(105);
      }
      else
        {
        if (inlin(dleft)) Drive_wheel.write(140);
        else if (inlin(dright)) Drive_wheel.write(65);
        else Play_Leds();
        }
  }
}

int inlin(uint8_t det)
{
     if (analogRead(det)>=500) return 1;
      else return 0; 
}

void Play_Leds()
{
delay(400);
digitalWrite(LEDup,HIGH);
delay(200);
digitalWrite(LEDup,LOW);
digitalWrite(LEDleft,HIGH);
delay(200);
digitalWrite(LEDleft,LOW);
digitalWrite(LEDdown,HIGH);
delay(200);
digitalWrite(LEDdown,LOW);
digitalWrite(LEDright,HIGH);
delay(200);
digitalWrite(LEDright,LOW);
delay(100);
}

void Change_Dir()
{
  Drive_wheel.write(100);
  digitalWrite(Acdown,HIGH);
  delay(500);
  digitalWrite(Acdown,LOW);
  Drive_wheel.write(135);
  digitalWrite(Acup,HIGH);
  delay(500);
  digitalWrite(Acup,LOW);
  Drive_wheel.write(100);
  delay(200);
}

void GO()
{
  if(ecl+200<millis())
        {
         ecl=millis();
         if (pw){digitalWrite(LEDpower,LOW);pw=0;}
          else  {digitalWrite(LEDpower,HIGH);pw=1;}
        }
}

int Obstacle ()
{
   if (analogRead(A0)>=615) return 1;
       else return 0;
}

void Save_Conf(long *Power,long *Up,long *Down,long *Upleft,long *Upright,long *Downleft,long *Downright)
{

      while(cont)
      {
        digitalWrite(LEDpower,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Power=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDpower,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDup,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Up=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDup,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDdown,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Down=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDdown,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDleft,HIGH);
        digitalWrite(LEDup,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Upleft=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDleft,LOW);
      digitalWrite(LEDup,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDleft,HIGH);
        digitalWrite(LEDdown,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Downleft=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDleft,LOW);
      digitalWrite(LEDdown,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDright,HIGH);
        digitalWrite(LEDup,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Upright=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDup,LOW);
      digitalWrite(LEDright,LOW);
      delay(400);
      ir_recv.resume();

      while(cont)
      {
        digitalWrite(LEDright,HIGH);
        digitalWrite(LEDdown,HIGH);
          if (ir_recv.decode(&results) && cont)
            {
              *Downright=results.value;
              cont=0;
            }
      }
      cont=1;
      digitalWrite(LEDdown,LOW);
      digitalWrite(LEDright,LOW);
      delay(400);
      ir_recv.resume();
      cont=1;
      Serial.println("Exit Init Success !" );  
}