#include <Arduino.h>

#define BUTTON_IN 16  // Кнопка підключена до GPIO 16

volatile uint32_t button_counter = 0; 
uint32_t last_count = 0; 

// Змінна для збереження часу останнього підтвердженого натискання (в мікросекундах)
// uint64_t потрібен, бо esp_timer_get_time() повертає 64-бітне число
volatile uint64_t last_interrupt_time = 0; 

// Швидка функція обробки переривань (ISR)
void IRAM_ATTR button_isr() {
  uint64_t interrupt_time = esp_timer_get_time(); // Отримуємо поточний час у мікросекундах
  
  // Якщо з моменту останнього натискання пройшло більше ніж 50 000 мікросекунд (50 мс)
  if (interrupt_time - last_interrupt_time > 1000000) {
    button_counter++;
    last_interrupt_time = interrupt_time; // Оновлюємо час останнього спрацьовування
  }
}

void setup() {
  pinMode(BUTTON_IN, INPUT_PULLUP);
  Serial.begin(115200);

  // Прив'язка переривання
  attachInterrupt(digitalPinToInterrupt(BUTTON_IN), button_isr, FALLING);
}

void loop() {
  if (button_counter != last_count) {
    last_count = button_counter;
    Serial.print("Button Pressed! Count: ");
    Serial.println(last_count);
  }

  delay(10);
}


