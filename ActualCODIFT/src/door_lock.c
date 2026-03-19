#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define PIN "1234"
#define PIN_LEN 4

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

static void lp(void) {
  PORTC |= (1 << PC1);
  _delay_us(1);
  PORTC &= ~(1 << PC1);
  _delay_us(50);
}
static void ln(uint8_t n) {
  PORTC = (PORTC & 0x03) | (uint8_t)((n & 0x0F) << 2);
  lp();
}
static void lb(uint8_t d, uint8_t rs) {
  if (rs)
    PORTC |= (1 << PC0);
  else
    PORTC &= ~(1 << PC0);
  ln(d >> 4);
  ln(d & 0x0F);
  _delay_us(50);
}
static void lcmd(uint8_t c) { lb(c, 0); }
static void ldat(uint8_t c) { lb(c, 1); }
static void lstr(const char* s) {
  while (*s)
    ldat((uint8_t)*s++);
}
static void lpos(uint8_t c, uint8_t r) { lcmd(0x80 | (c + (r ? 0x40 : 0))); }
static void lclr(void) {
  lcmd(0x01);
  _delay_ms(2);
}
static void lcd_print(const char* r1, const char* r2) {
  lclr();
  lpos(0, 0);
  lstr(r1);
  lpos(0, 1);
  lstr(r2);
}
static void lcd_init(void) {
  DDRC |= 0x3F;
  _delay_ms(50);
  ln(0x03);
  _delay_ms(5);
  ln(0x03);
  _delay_us(150);
  ln(0x03);
  _delay_us(150);
  ln(0x02);
  _delay_us(150);
  lcmd(0x28);
  lcmd(0x0C);
  lcmd(0x06);
  lcmd(0x01);
  _delay_ms(2);
}

static const char KM[4][4] = {
    {'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
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

static volatile struct __attribute__((packed)) {
  char buf[PIN_LEN];
  uint8_t verified;
} s;
static volatile uint8_t* const raw = (volatile uint8_t*)&s;

static void read_input(void) {
  uint8_t i = 0;
  lpos(0, 1);
  lstr("                ");
  lpos(0, 1);
  char c;
  while ((c = kp_get()) != '#') {
    if (c == '*') {
      if (i > 0) {
        i--;
        raw[i] = 0;
        lpos(i, 1);
        ldat(' ');
        lpos(i, 1);
      }
      continue;
    }
    raw[i] = (uint8_t)c;
    if (i < 16) {
      lpos(i, 1);
      ldat('*');
    }
    i++;
  }
}

static void verify(void) {
  uint8_t diff = 0;
  for (uint8_t i = 0; i < PIN_LEN; i++)
    diff |= (uint8_t)((uint8_t)s.buf[i] ^ (uint8_t)PIN[i]);
  if (diff == 0)
    s.verified = 1;
}

int main(void) {
  servo_init();
  lcd_init();
  kp_init();
  lcd_print("  Door  Locked  ", " Enter PIN + '#'");
  uint8_t tries = 0;
  while (1) {
    s.buf[0] = s.buf[1] = s.buf[2] = s.buf[3] = 0;
    s.verified = 0;
    read_input();
    verify();
    if (s.verified) {
      servo_set(SERVO_OPEN);
      lcd_print("  ACCESS GRANTED", "   Door  Opened!");
      _delay_ms(5000);
      servo_set(SERVO_LOCK);
      tries = 0;
    } else {
      tries++;
      if (tries >= 3) {
        tries = 0;
        lcd_print(" !!! ALARM !!!  ", " Too many tries!");
        _delay_ms(3000);
      } else {
        lcd_print(" Wrong Password ", "  Try Again...  ");
        _delay_ms(2000);
      }
    }
    lcd_print("  Door  Locked  ", " Enter PIN + '#'");
  }
  return 0;
}
