//
//
// hello.ftdi.44.echo.c
//
// 115200 baud FTDI character echo, with flash string
//
// set lfuse to 0x7E for 20 MHz xtal
//
// Neil Gershenfeld
// 12/8/10
//
// (c) Massachusetts Institute of Technology 2010
// Permission granted for experimental and personal use;
// license for commercial sale available from MIT.
//

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 100 // bit delay for 115200 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay
#define position_delay() _delay_ms(500)
#define PWM_delay() _delay_us(25) // PWM delay

#define serial_port PORTB
#define serial_direction DDRB
#define serial_pins PINB
#define serial_pin_in (1 << PB0)
#define serial_pin_out (1 << PB1)


#define servo_port PORTA
#define servo_direction DDRA
#define servo_pin (1<<PA6)

#define red_led_port PORTA
#define red_led_direction DDRA
#define red (1 << PA3)

#define green_led_port PORTA
#define green_led_direction DDRA
#define green (1 << PA2)

#define blue_led_port PORTA
#define blue_led_direction DDRA
#define blue (1 << PA1)

#define max_buffer 25

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   while (pin_test(*pins,pin))
      //
      // wait for start bit
      //
      ;
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
   //
   // send character in txchar on port pin
   //    assumes line driver (inverts bits)
   //
   // start bit
   //
   clear(*port,pin);
   bit_delay();
   //
   // unrolled loop to write data bits
   //
   if bit_test(txchar,0)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,1)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,2)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,3)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,4)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,5)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,6)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   if bit_test(txchar,7)
      set(*port,pin);
   else
      clear(*port,pin);
   bit_delay();
   //
   // stop bit
   //
   set(*port,pin);
   bit_delay();
   //
   // char delay
   //
   bit_delay();
   }

void put_string(volatile unsigned char *port, unsigned char pin, char *str) {
   //
   // print a null-terminated string
   //
   static int index;
   index = 0;
   do {
      put_char(port, pin, str[index]);
      ++index;
      } while (str[index] != 0);
   }
   

void accendiLed(int r, int g, int b){
	
	
	// /put_string(&serial_port, serial_pin_out, "Accendi led");
	
	if (r == 0) {
		set(red_led_port,red);
	} else {
 		clear(red_led_port,red);
	}
	if (g == 0) {
		set(green_led_port,green);
	} else {
		clear(green_led_port,green);
	}
	if (b == 0) {
		set(blue_led_port,blue);
	} else {
		clear(blue_led_port,blue);
	}
}

void muoviServo(int gradi){
	
	put_char(&serial_port, serial_pin_out, 'A');
	OCR1A = 500+(100*gradi);
	position_delay();
}
   
void doCommand(char cmd, char param){
	if (cmd == '0'){
		int p = atoi(&param);
		//led
		if (p==0) accendiLed(0,0,0);
		if (p==1) accendiLed(255,0,0);
		if (p==2) accendiLed(0,255,0);
		if (p==3) accendiLed(0,0,255);
		
	}
	if (cmd == '1'){
		int p = atoi(&param);
		muoviServo(p);
		// if (param == 0){
		// 	muoviServo(0);
		// } else {
		// 	muoviServo(1);
		// }
		
	}

}

int main(void) {
   //
   // main
   //
   static char chr;
   char* buffer;
   static int index;
   static char cmd;
   
      // initialize LED pins
   
   set(red_led_port, red);
   output(red_led_direction, red);
   set(green_led_port, green);
   output(green_led_direction, green);
   set(blue_led_port, blue);
   output(blue_led_direction, blue);
   //
   
   buffer = malloc(sizeof(char) * max_buffer);
   // set clock divider to /1
   //
   CLKPR = (1 << CLKPCE);
   CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
   //
   // initialize output pins
   // set up timer 1
   //
   TCCR1A = (1 << COM1A1) | (0 << COM1A0); // clear OC1A on compare match
   TCCR1B = (0 << CS12) | (1 << CS11) | (0 << CS10) | (1 << WGM13); // prescaler /8, phase and frequency correct PWM, ICR1 TOP
   ICR1 = 20000; // 20 ms frequency
   sei();
   //
   //
   set(serial_port, serial_pin_out);
   output(serial_direction, serial_pin_out);
   
   clear(servo_port, servo_pin);
   output(servo_direction, servo_pin);
   //
   // main loop
   //
   index = 0;
   put_string(&serial_port, serial_pin_out, "Init.");
   while (1) {
      get_char(&serial_pins, serial_pin_in, &chr);
      if (chr=='\n'){
	      char t='t';
	      put_char(&serial_pins, serial_pin_out, t);
      }
      if (chr=='#') {
	      index=0;
	      get_char(&serial_pins, serial_pin_in, &chr);
              cmd = chr;
	      get_char(&serial_pins, serial_pin_in, &chr);
	      doCommand(cmd, chr);
      }
   }
}