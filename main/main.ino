// Intern Project Code

#include "src/Controller.h"

void setup() {
  Serial.begin(9600);
  
  rgbled_67.fillPixelsBak(0, 2, 1);
  rgbled_68.fillPixelsBak(0, 2, 1);
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);
  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  rgbled_67.setColor(0, 85,32,211);
  rgbled_67.show();

  rgbled_68.setColor(0, 32,211,193);
  rgbled_68.show();
  delay_seconds(2);

}

void loop() {
  // Controller c(LINE_FOLLOW);

  // Offload these functions to the AdvancedController class
  // Move basic functions (move_control, etc.) that control the physical movements to the Controller class
  // Create 
  while(lineFollowMode == true)
  {
    line_sensor0_value = (linefollower_63.readSensor() == 0);
    line_sensor1_value = (linefollower_64.readSensor() == 0);
    if((line_sensor0_value == 1.000000)  &&  (line_sensor1_value == 1.000000)){

        move_control(0, 40 / 100.0 * 255, 0);

        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();

    }
    else if((line_sensor0_value == 0.000000)  &&  (line_sensor1_value == 1.000000)){

        motor_2.run(-50 / 100.0 * 255);
        motor_10.run(-50 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
    else if((line_sensor0_value == 1.000000)  &&  (line_sensor1_value == 0.000000)){

        motor_1.run(50 / 100.0 * 255);
        motor_9.run(50 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }else{

        motor_1.run(0 / 100.0 * 255);
        motor_9.run(0 / 100.0 * 255);

        motor_2.run(0 / 100.0 * 255);
        motor_10.run(0 / 100.0 * 255);

        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

    }
    if (collision_65.isCollision())
    {
      impactCounter = impactCounter + 1;

      while(collision_65.isCollision())
      {
          ;;
      }

      if(impactCounter >= 3)
      {
          lineFollowMode = false;
          obstacleAvoidMode = true;

          rgbled_67.setColor(0, 85,32,211);
          rgbled_67.show();

          rgbled_68.setColor(0, 32,211,193);
          rgbled_68.show();

          Serial.println("Obstacle Avoidance Initiatied");
      }
      delay_seconds(0.4);
    }
  }

  while(obstacleAvoidMode == true)
  {
    if (!barrier_60.isBarried()  &&  !barrier_61.isBarried()  &&  !barrier_62.isBarried())
    {
        move_control(0, 40 / 100.0 * 255, 0);

        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();
    }
    else if(barrier_60.isBarried()  &&  !barrier_62.isBarried()){

        motor_2.run(-30 / 100.0 * 255);
        motor_10.run(-30 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
        else if(barrier_60.isBarried()  &&  barrier_61.isBarried() &&  !barrier_62.isBarried()){

        motor_2.run(-50 / 100.0 * 255);
        motor_10.run(-50 / 100.0 * 255);

        motor_1.run(-10 / 100.0 * 255);
        motor_9.run(-10 / 100.0 * 255);

        rgbled_68.setColor(0, 32,166,211);
        rgbled_68.show();

    }
    else if(!barrier_60.isBarried()  &&   barrier_62.isBarried()){

        motor_1.run(30 / 100.0 * 255);
        motor_9.run(30 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }
        else if(!barrier_60.isBarried() &&  barrier_61.isBarried()  &&   barrier_62.isBarried()){

        motor_1.run(50 / 100.0 * 255);
        motor_9.run(50 / 100.0 * 255);

        motor_2.run(10 / 100.0 * 255);
        motor_10.run(10 / 100.0 * 255);

        rgbled_67.setColor(0, 32,166,211);
        rgbled_67.show();

    }
        else if(!barrier_60.isBarried() &&  barrier_61.isBarried()  &&  !barrier_62.isBarried()){

        motor_1.run(0 / 100.0 * 255);
        motor_9.run(0 / 100.0 * 255);

        motor_2.run(0 / 100.0 * 255);
        motor_10.run(0 / 100.0 * 255);

        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

        handRecMode = true;
        obstacleAvoidMode = false;
    }
  }
  while(handRecMode == true){
      if(barrier_60.isBarried()){
          correctHandMotion = 1;
      }
      if(barrier_61.isBarried()){
          if(correctHandMotion == false){
              passWaveIncorrect = true;
          }
          if(correctHandMotion == 1){
            correctHandMotion = 2;
          }
      }
      if(barrier_62.isBarried()){
          if(correctHandMotion == 2){
              passWaveCorrect = true;
          }  
      }
      if(passWaveCorrect == true){
        rgbled_67.setColor(0, 126,211,33);
        rgbled_67.show();

        rgbled_68.setColor(0, 126,211,33);
        rgbled_68.show();

        delay_seconds(2);
        handRecMode = false;
        passWaveCorrect = false;
        correctHandMotion = 0;
        lineFollowMode = true;
      }
      if(passWaveIncorrect == true){
        rgbled_67.setColor(0, 211,32,32);
        rgbled_67.show();

        rgbled_68.setColor(0, 211,32,32);
        rgbled_68.show();

        delay_seconds(2);
        passWaveIncorrect = false;
        passWaveCorrect = false;
        correctHandMotion = 0;
      }
  }

  _loop();
}
