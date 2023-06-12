#include <AccelStepper.h>
#define MotorInterfaceType 4

AccelStepper stepper = AccelStepper(MotorInterfaceType, 5,4, 14, 12);\

//временная задержка между шагами мотора в мс
const int move_delay = 300; 

int new_count_rotate = 0;
int old_count_rotate = 0; //200 count шагов = 1 полный оборот
//min value count = 0 || max values count = 21 (15 - достаточно для более спокойного проветривания комнаты)?

int new_value_co2 = 0;
int old_value_co2 = 0;

int value_temp = 0;
int old_value_temp = 0;

void setup() {
  stepper.setMaxSpeed(1000);
  Serial.begin(9600);
}

void loop() {

  if (old_value_co2 != new_value_co2 || value_temp != old_value_temp){
    if (value_temp >= 20 && value_temp <= 26) {
      if (new_value_co2 <= 2500 && new_value_co2 > 400){
        new_count_rotate = ((new_value_co2 * 15) / 2500) - old_count_rotate;
        old_count_rotate += new_count_rotate;
        
        stepper.setSpeed(200 * new_count_rotate); // 200 - 1 оборот = 200 шагов
        stepper.runSpeed();
        delay(1000);
      }
      else if (old_count_rotate <= 15){
        new_count_rotate = 15 - old_count_rotate;
        stepper.setSpeed(200 * new_count_rotate); // 1 оборот = 200 шагов
        stepper.runSpeed();
        old_count_rotate = new_count_rotate;
      }  
    }

    else if (value_temp >= 10 && value_temp < 20){
      if (new_value_co2 <= 2500 && new_value_co2 > 600){ // минимальное значение открытие окна падает до 600
        new_count_rotate = ((new_value_co2 * 7) / 2500) - old_count_rotate;
        old_count_rotate += new_count_rotate;
        
        stepper.setSpeed(200 * new_count_rotate); // 200 - 1 оборот = 200 шагов
        stepper.runSpeed();
        delay(1000);
      }
      else if (old_count_rotate <= 7){
        new_count_rotate = 7 - old_count_rotate;
        stepper.setSpeed(200 * new_count_rotate); // 1 оборот = 200 шагов
        stepper.runSpeed();
        old_count_rotate = new_count_rotate;
      }
      else {
        while (old_count_rotate > 7){
          stepper.setSpeed(-200);
          stepper.runSpeed();
          old_count_rotate-- ;
        }
      }
    }
    else if (value_temp < 10){
      if (new_value_co2 >= 2500){
        while (old_count_rotate > 0){
          stepper.setSpeed(-200);
          stepper.runSpeed();
          old_count_rotate-- ;
        }
        stepper.setSpeed(200 * 7);
        stepper.runSpeed();
        old_count_rotate = 7;
        
        delay(15000);

        stepper.setSpeed(-200 * 7);
        stepper.runSpeed();
        old_count_rotate = 0;
      }
      else{
        while (old_count_rotate > 0){
          stepper.setSpeed(-200);
          stepper.runSpeed();
          old_count_rotate-- ;
        }
      }
    }
  }

  
}
