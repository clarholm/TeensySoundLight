#include <usb_serial.h>
 
// Matrix size 392 (28x14)
#define Num_Pixels 146
 
#define CMD_NEW_DATA 1
int SDI = 2; 
int CKI = 10;
 
unsigned char display_buffer[Num_Pixels * 3];
 
static unsigned char *ptr;
static unsigned int pos = 0;
 
volatile unsigned char go = 0;
 
void setup() 
{
  pinMode(SDI, OUTPUT);
  pinMode(CKI, OUTPUT); 
  Serial.begin(9600); // USB is always 12 Mbit/sec
}
 
void loop() 
{
  if (0<usb_serial_available()){
    unsigned char b;
    b = usb_serial_getchar();
 
  if (b == CMD_NEW_DATA)  {pos=0; ptr=display_buffer; return;}    
  if (pos == (Num_Pixels*3)) {} else {*ptr=b; ptr++; pos++;}  
  if (pos == ((Num_Pixels*3)-1)) {go=1;}
  }
  if (go==1) {shift_out_data(); go=0;}
        Serial.println(display_buffer[100]);
  
}
 
 
 
 
//############################################################################################################################################################
// Shift out Data                                                                                                                                            #
//############################################################################################################################################################
 
void shift_out_data()
{
 
  for (int i=0; i<Num_Pixels; i++)
  {
    byte r = display_buffer[i*3+0];
    byte g = display_buffer[i*3+1];
    byte b = display_buffer[i*3+2];
    
    for (byte j=0; j<8; j++)
    {
       digitalWrite(CKI, LOW);
       if (r & (byte)(1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH);
    }
    
    for (byte j=0; j<8; j++)
    {
       digitalWrite(CKI, LOW);
       if (g & (1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH); 
    }
    
    for (byte j=0; j<8; j++)
    {
       digitalWrite(CKI, LOW);
       if (b & (1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH);
    }
    
  }
  
  digitalWrite(CKI, LOW);
  delayMicroseconds(800); //Latch Data
 
}
