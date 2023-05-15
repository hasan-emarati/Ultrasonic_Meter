
#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>
////////////////////////////////////////////
typedef unsigned char byte;
flash byte char0[8]={0,10,31,31,14,4,0,0};
////////////////////////////////////////////
long int overflow=0;
long int overflowm=0;
unsigned long int  Counter=0;
unsigned long int  Counterm=0;
float distance=0.0;
float distancem=0.0;
unsigned char lcd[16];
///////////////////////////////////////////////////
interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
overflow++;       
TCNT0=0x00;
}
/////////////////////////////////////////////////////
void define_char(byte flash *pc,byte char_code){
byte i,a;
a=(char_code<<3)|0x40;
for (i=0; i<8; i++) lcd_write_byte(a++,*pc++);
}
//////////////////////////////////////////////////

void main(void)
{



DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (1<<DDA0);
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (1<<DDD3) | (1<<DDD2) | (1<<DDD1) | (1<<DDD0);
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (1<<DDC0);
PORTD=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: 1000/000 kHz
// Mode: Normal top=0xFF
// OC0 output: Disconnected
// Timer Period: 0/256 ms
TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (1<<CS01) | (0<<CS00);
TCNT0=0x00;
OCR0=0x00;

TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (1<<TOIE0);

lcd_init(16);

// Global enable interrupts
define_char(char0,0);
#asm("sei")




lcd_gotoxy(0,0);
lcd_puts(" hasan emarati ");
lcd_gotoxy(0,0);
lcd_putchar(0);
lcd_gotoxy(15,0);
lcd_putchar(0);
delay_ms(1000);

lcd_clear();
lcd_puts("Ultrasonic meter ");
delay_ms(3000);
lcd_clear();


while (1)
     {
     
      PORTA.0=1;
      delay_us(10);
      PORTA.0=0;

      while(PINA.1==0){};   
      
      overflow=0;
      TCNT0=0;
      TCCR0=0x02;
      while(PINA.1==1){};
      TCCR0=0x00;   
      
      Counter=(overflow*256)+TCNT0;    
      distance=(Counter/2)*0.03432 ;
       if(Counter<30000) {   
       lcd_clear();
       lcd_gotoxy(0,0);
       lcd_putsf("distance=");
       lcd_gotoxy(9,0);
       ftoa(distance,1,lcd);
       lcd_puts(lcd);
       if (PINC.1==1){delay_ms(1000);}
       lcd_putsf("cm");
       delay_ms(100);
      
       
      //////////////////////////////////
      
      Counterm=(overflowm*256)+TCNT0;    
      distancem=(Counterm/2)*34.32 ;
      
      if (Counter>30001)
      {
       lcd_clear();
       lcd_gotoxy(0,0);
       lcd_putsf("distance=");
       lcd_gotoxy(9,0);
       ftoa(distancem,1,lcd);
       lcd_puts(lcd);
       lcd_putsf("m");
       delay_ms(100);  
      }
      }
      if (Counter>30000)
      {  
      PORTC.0=1;
      lcd_gotoxy(2,1);
      lcd_putsf("out of range    ");
      delay_ms(100); 
      }
      if (Counter<30000) 
      {
      lcd_clear();
      PORTC.0=0;
      }
     }
}
