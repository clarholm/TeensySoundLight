
//Change this to YOUR matrix size!!
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
 
  //UART Initialisation
  UCSR0A |= (1<<U2X0);                                
  UCSR0B |= (1<<RXEN0)  | (1<<TXEN0) | (1<<RXCIE0);   
  UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00)             ; 
  UBRR0H = 0;
  UBRR0L = 3; //Baud Rate 0.5 MBit   --> 0% Error at 16MHz :-)
  
  ptr=display_buffer;
  
  //Enable global interrupts
  sei();
}

void loop() 
{
  if (go==1) {shift_out_data(); go=0;}
}

//############################################################################################################################################################
// UART-Interrupt-Prozedur (called every time one byte is compeltely received)                                                                               #
//############################################################################################################################################################

ISR(USART_RX_vect) 
{
  unsigned char b;
  
  b=UDR0;
  
  if (b == CMD_NEW_DATA)  {pos=0; ptr=display_buffer; return;}    
  if (pos == (Num_Pixels*3)) {} else {*ptr=b; ptr++; pos++;}  
  if (pos == ((Num_Pixels*3)-1)) {go=1;}
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
       if (b & (byte)(1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH);
    }
    
    for (byte j=0; j<8; j++)
    {
       digitalWrite(CKI, LOW);
       if (r & (1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH); 
    }
    
    for (byte j=0; j<8; j++)
    {
       digitalWrite(CKI, LOW);
       if (g & (1<<(7-j))) {digitalWrite(SDI, HIGH);} else {digitalWrite(SDI, LOW);}     
       digitalWrite(CKI, HIGH);
    }
    
  }
  
  digitalWrite(CKI, LOW);
  delayMicroseconds(800); //Latch Data

}

//############################################################################################################################################################
//############################################################################################################################################################
//############################################################################################################################################################

