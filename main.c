#include <stdio.h>
#include "pico/stdlib.h"

const uint TRIGGER = 27;
const uint ECHO = 26;


void ultrasonic_init (uint8_t trigger, uint8_t echo);
float ultrasonic_get_distance_cm (uint8_t trigger, uint8_t echo);

int main() {
  /* Habilito el USB */
  stdio_init_all();
  sleep_ms(1000);
  printf("Listo!\r\n");
  gpio_init (10);
  gpio_set_dir(10, true);
  gpio_init (14);
  gpio_set_dir(14, true);
  gpio_init (12);
  gpio_set_dir(12, true);
  
  ultrasonic_init(TRIGGER, ECHO);

  while (true) {

    /*pone los valores de la distancia de la función del ultrasónicos en una variable*/
    float distance = ultrasonic_get_distance_cm(TRIGGER, ECHO);

    /*Condiciones para cada color, apaga los otros cuando prende uno*/
    if (distance < 10){
      gpio_put (10, true);
      gpio_put (14, false);
      gpio_put (12, false);
    }
    else if (distance < 50){
      gpio_put (14, true);
      gpio_put (10, false);
      gpio_put (12, false);
    }
    else if (distance < 100){
      gpio_put (12, true);
      gpio_put (10, false);
      gpio_put (14, false);
    }

    sleep_ms(1000);
  }
}

void ultrasonic_init (uint8_t trigger, uint8_t echo) {
  /* Habilito el pin de Trigger */
  gpio_init(trigger);  
  /* Configuro el Trigger como salida */
  gpio_set_dir(trigger, true);
  /* Pongo un 0 en el pin de Trigger */
  gpio_put(trigger, false);
  /* Habilito el pin de Echo */
  gpio_init(echo);
  /* Configuro el Echo como entrada */
  gpio_set_dir(echo, false);
}


/*FUNCIÓN DE MEDICIÓN DE DISTANCIA*/
float ultrasonic_get_distance_cm (uint8_t trigger, uint8_t echo) {
  /* Escribo un 1 en el Trigger */
  gpio_put(trigger, true);
  /* Espero 10 us con el pulso encendido */
  sleep_us(10);
  /* Escribo un 0 en el Trigger */
  gpio_put(trigger, false);
  /* Espero a que el pulso llegue al Echo */
  while(!gpio_get(echo));
  /* Mido el tiempo de cuando llega el pulso */
  absolute_time_t from = get_absolute_time();
  /* Espero a que el pulso del Echo baje a cero */
  while(gpio_get(echo));
  /* Mido el tiempo cuando termina el pulso */
  absolute_time_t to = get_absolute_time();
  /* Saco la diferencia de tiempo (el ancho del pulso) en us */
  int64_t pulse_width = absolute_time_diff_us(from, to);
  /* Calculo la distancia */
  float distance = pulse_width / 59.0;
  /*Mando variable distancia fuera de la función*/
  return distance;
}