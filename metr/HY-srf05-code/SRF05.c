#include <mega8.h>
#include <delay.h>
#include <stdio.h>
#include <alcd.h>
#define buzzer PORTD.7
#define sw_up PIND.6
#define sw_down PIND.5

unsigned char s;
 unsigned long int _time;  
 bit ex_int0;    
  unsigned  char _buf[30];  
  

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
s++;

}




interrupt [EXT_INT1] void ext_int1_isr(void)
{
if(ex_int0==0){
TCCR0=0x02;
ex_int0=1;
MCUCR=0x08;
}
else{
MCUCR=0x0C;
TCCR0=0x00;
_time=s*256+TCNT0;

 sprintf(_buf,"Distance=%d cm ",_time/58);
 lcd_gotoxy(0,0);
 lcd_puts(_buf);         
 delay_us(20);
 
 TCNT0=0x00;
 s=0;
 ex_int0=0;
 }
}

void main(void){
int set_point=30; 
   
char _buf2[17]; 
 DDRD=(1<<2)|(1<<7); //TRIGER AND BUZZER 
 
 PORTD.6=1;PORTD.5=1; //PULLUP SW UP DOWN 


TCCR0=0x00;
TCNT0=0x00;

TIMSK=0x01;


// External Interrupt(s) initialization
// INT0: Off
// INT1: On
// INT1 Mode: Any change
// INT2: Off
GICR|=0x80;
MCUCR=0x0C;
GIFR=0x80;


lcd_init(16);
lcd_clear();

lcd_putsf("Raziyeh Kehtari");
delay_ms(1);
lcd_gotoxy(0,1);
lcd_putsf("Bahare Javadi");
 delay_ms(3000);

#asm("sei")
while(1){
PORTD.2=1;
delay_us(15);
PORTD.2=0;
delay_ms(350);

 if(sw_up==0){
  while(sw_up==0);
  set_point++;
 }    
 if(sw_down==0){
  while(sw_down==0);
  set_point--;
 } 
 
 
  
 
 sprintf(_buf2,"Set point=%d cm ",set_point);
 lcd_gotoxy(0,1);
 lcd_puts(_buf2);
 
 if(_time/58==set_point){ 
 buzzer=1;
 lcd_gotoxy(0,1);
 lcd_putsf("Distance=S.p    ");
 
 }
 else{
  buzzer=0;
 }
 
}

}