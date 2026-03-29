#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define PIN "1234"
#define PIN_LEN 4
#define MAX_TRIES 3

#define SERVO_LOCK 16
#define SERVO_OPEN 31

static void servo_init(void) {
  DDRD |= (1 << PD3);
  OCR2A = 249;
  OCR2B = SERVO_LOCK;
  TCCR2A = (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B = (1 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);
}
static void servo_set(uint8_t v) { OCR2B = v; }

static void uart_init(void) {
  UBRR0H = 0;
  UBRR0L = 103;
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
static void uart_char(char c) {
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}
static void uart_str(const char* s) {
  while (*s)
    uart_char(*s++);
}
static void uart_ln(const char* s) {
  uart_str(s);
  uart_char('\r');
  uart_char('\n');
}

static const char KM[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

static void kp_init(void) {
  DDRD |= 0xF0;
  PORTD |= 0xF0;
  DDRB &= ~0x0F;
  PORTB |= 0x0F;
}

static char kp_scan(void) {
  for (uint8_t r = 0; r < 4; r++) {
    PORTD = (PORTD | 0xF0) & ~(uint8_t)(0x10 << r);
    _delay_us(5);
    uint8_t c = ~PINB & 0x0F;
    PORTD |= 0xF0;
    if (c)
      for (uint8_t i = 0; i < 4; i++)
        if (c & (1 << i)) {
          _delay_ms(20);
          return KM[r][i];
        }
  }
  return 0;
}

static char kp_get(void) {
  char k;
  do {
    k = kp_scan();
  } while (!k);
  while (kp_scan())
    _delay_ms(5);
  return k;
}

#define LCD_ADDR 0x27

void i2c_init(void) {
  TWSR = 0x00;
  TWBR = 72;
}

void i2c_start(void) {
  TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)))
    ;
}

void i2c_stop(void) {
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

void i2c_write(uint8_t data) {
  TWDR = data;
  TWCR = (1 << TWINT) | (1 << TWEN);
  while (!(TWCR & (1 << TWINT)))
    ;
}

#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04
#define RS 0x01

void lcd_write(uint8_t data) {
  i2c_start();
  i2c_write(LCD_ADDR << 1);
  i2c_write(data | LCD_BACKLIGHT);
  i2c_stop();
}

void lcd_pulse(uint8_t data) {
  lcd_write(data | ENABLE);
  _delay_us(1);
  lcd_write(data & ~ENABLE);
  _delay_us(50);
}

void lcd_send_nibble(uint8_t nibble, uint8_t mode) {
  uint8_t data = (nibble & 0xF0) | mode;
  lcd_pulse(data);
}

void lcd_cmd(uint8_t cmd) {
  lcd_send_nibble(cmd & 0xF0, 0);
  lcd_send_nibble((cmd << 4) & 0xF0, 0);
}

void lcd_data(uint8_t data) {
  lcd_send_nibble(data & 0xF0, RS);
  lcd_send_nibble((data << 4) & 0xF0, RS);
}

void lcd_init(void) {
  _delay_ms(50);

  lcd_send_nibble(0x30, 0);
  _delay_ms(5);
  lcd_send_nibble(0x30, 0);
  _delay_us(150);
  lcd_send_nibble(0x30, 0);

  lcd_send_nibble(0x20, 0);

  lcd_cmd(0x28);
  lcd_cmd(0x0C);
  lcd_cmd(0x06);
  lcd_cmd(0x01);
  _delay_ms(2);
}

void lcd_clear(void) {
  lcd_cmd(0x01);
  _delay_ms(2);
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  uint8_t addr = (row == 0) ? 0x80 : 0xC0;
  lcd_cmd(addr + col);
}

void lcd_print(const char* str) {
  while (*str)
    lcd_data(*str++);
}

static volatile struct __attribute__((packed)) {
    char buf[PIN_LEN];
    uint8_t verified;
} s;

static volatile uint8_t* const raw = (volatile uint8_t*)&s;

void read_input(void) {
  uint8_t i = 0;

  lcd_clear();
  lcd_print("Enter PIN:");
  lcd_set_cursor(0, 1);

  uart_ln("Enter PIN + #:");

  char c;
  while ((c = kp_get()) != '#') {
    if (c == '*') {
      if (i > 0) {
        i--;
        raw[i] = 0;
      }
      continue;
    }
    raw[i] = (uint8_t)c;
    uart_char('*');
    lcd_data('*');
    i++;
  }
}

void verify(void) {
  uint8_t diff = 0;
  for (uint8_t i = 0; i < PIN_LEN; i++)
    diff |= (uint8_t)(s.buf[i] ^ PIN[i]);

  if (diff == 0)
    s.verified = 1;
}

int main(void) {
  servo_init();
  uart_init();
  kp_init();
  i2c_init();
  lcd_init();

  uart_ln("=== Door Lock ===");

  lcd_print("Door LOCKED");

  uint8_t tries = 0;

  while (1) {
    s.buf[0] = s.buf[1] = s.buf[2] = s.buf[3] = 0;
    s.verified = 0;

    read_input();
    verify();

    if (s.verified) {
      servo_set(SERVO_OPEN);

      lcd_clear();
      lcd_print("ACCESS GRANTED");

      uart_ln("ACCESS GRANTED");
      _delay_ms(3000);

      servo_set(SERVO_LOCK);
      tries = 0;
    } else {
      tries++;

      if (tries >= MAX_TRIES) {
        lcd_clear();
        lcd_print("!!! ALARM !!!");

        uart_ln("ALARM!");
        _delay_ms(3000);
        tries = 0;
      } else {
        lcd_clear();
        lcd_print("Wrong PIN");

        uart_ln("Wrong PIN");
        _delay_ms(1500);
      }
    }

    lcd_clear();
    lcd_print("Door LOCKED");
  }
}
