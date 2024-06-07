#include <Robo_square.h>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
klepeta klepeto;
// robot goes to first position
void StopMotors()
{
  rb::Manager::get().motor(rb::MotorId::M1).speed(0);
  rb::Manager::get().motor(rb::MotorId::M2).speed(0);
  rb::Manager::get().motor(rb::MotorId::M3).speed(0);
  rb::Manager::get().motor(rb::MotorId::M4).speed(0);
}
void MoveToGrabBaterry()
{
  Backward(200, 30);
  Straight(200, 30, 3000);
}
enum Side
{
  REDSIDE,
  BLUESIDE
};
enum BoxColor
{
  RED,
  BLUE,
  IDK
};

BoxColor Box_1_c = IDK;
BoxColor Box_2_c = IDK;
BoxColor Box_3_c = IDK;
BoxColor Box_4_c = IDK;
BoxColor Box_5_c = IDK;
BoxColor Box_6_c = IDK;
BoxColor Box_7_c = IDK;
BoxColor Box_8_c = IDK;

Side side = REDSIDE;


bool baterry_loaded = false;
bool skip_baterries = false;

BoxColor ReciveData()
{
  BoxColor box = IDK;
  while (true)
  {
    if (Serial.available() > 0)
    {
      String data = Serial.readStringUntil('\n');
      if (data == "red")
      {
        box = RED;

        break;
      }
      if (data == "blue")
      {
        box = BLUE;
        break;
      }
    }
    delay(10);
  }
  return box;
}
void ColisionDetector(){
  while (true)
  {
    if (man.ultrasound(0).measure() > 0 && man.ultrasound(0).measure() < 400)
    {
      enemy = FRONT;
     // Serial.println("enemy front");
    }
    if (man.ultrasound(1).measure() > 0 && man.ultrasound(1).measure() < 400)
    {
      enemy = BACK;
      //Serial.println("enemy back");
    }
    if(man.ultrasound(1).measure() >= 400 && man.ultrasound(0).measure() >= 400)
    {
      enemy = NO;
      //Serial.println("no enemy");
    }
  }
}

void EnemyStandingInFront(){
  int time = 0;
  while (time <30)
  {
    delay(1000);
    time += 1000;
  }
  if(Box_1_c != IDK){
    //do nothing
  }
  if( Box_1_c == IDK){
    skip_baterries = true;

  }

}

void GetRestBoxColor()
{
  if (Box_8_c == BLUE)
  {
    Box_1_c = RED;
  }
  else if (Box_8_c == RED)
  {
    Box_1_c = BLUE;
  }
  if (Box_7_c == BLUE)
  {
    Box_2_c = RED;
  }
  else if (Box_7_c == RED)
  {
    Box_2_c = BLUE;
  }
  if (Box_6_c == BLUE)
  {
    Box_3_c = RED;
  }
  else if (Box_6_c == RED)
  {
    Box_3_c = BLUE;
  }
  if (Box_5_c == BLUE)
  {
    Box_4_c = RED;
  }
  else if (Box_5_c == RED)
  {
    Box_4_c = BLUE;
  }
}

void setup()
{
  // Get the manager instance as a singleton
  auto &man = rb::Manager::get();
  // Install the manager
  man.install();
  // for setting autostop parameters
  SmartServoBus::AutoStopParams par;
  par.max_diff_centideg = 1000;
  par.max_diff_readings = 1;
  // Set the serial communication baud rate to 115200
  Serial.begin(115200);

  servoBus.begin(2, UART_NUM_1, GPIO_NUM_27);
  servoBus.setAutoStopParams(par);
  servoBus.setAutoStop(0, false);



  // while (true)
  // {
  //   if (Serial.available() > 0)
  //   {
  //     String data = Serial.readStringUntil('\n');
  //     if (data == "ready")
  //     {
  //       man.leds().green(true);
  //       break;
  //     }
  //   }
  //   delay(10);
  // }

  while (true)
  {
    if (man.buttons().up() == 1)
    {
      side = REDSIDE;
      man.leds().yellow(true);
      man.leds().blue(false);
    }
    if (man.buttons().down() == 1)
    {
      side = BLUESIDE;
      man.leds().blue(true);
      man.leds().yellow(false);
    }
    if (man.buttons().on() == 1)
    {
      break;
    }

    delay(10);
  }
  std::thread t1(ColisionDetector);
  std::thread t2(EnemyStandingInFront);
  // startovaci tlacitko
  while (true)
  {
    if (man.buttons().up() == 1)
    {
      break;
    }

    delay(10);
  }
 // t1.~thread();


  ///////////////////////////////////////////////////////////////////////
  if (side == BLUESIDE)
  {
    man.stupidServo(0).setPosition(-2); // 90 deg left
    delay(1000);
    man.stupidServo(1).setPosition(2);
    man.stupidServo(2).setPosition(1); // pozice magnetu pro brani baterek

    // Straight(1000,999999,15000);
    // while (true)
    // {
    //   /* code */
    //   delay(100);
    // }
    

    Straight(1000, Box_8, 15000);
    Serial.println("givecolor");
    delay(500);
    Box_8_c = ReciveData();

    Straight(1000, Box_7 - Box_8, 15000);
    Serial.println("givecolor");
    delay(500);
    Box_8_c = ReciveData();

    Straight(1000, Box_6 - Box_7, 15000);
    Serial.println("givecolor");
    delay(500);
    Box_7_c = ReciveData();

    Straight(1000, Box_5 - Box_6, 15000);
    Serial.println("givecolor");
    delay(500);
    Box_6_c = ReciveData();

    GetRestBoxColor();

    if (Box_8_c == BLUE)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 15000);

      man.stupidServo(0).setPosition(-2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_8);
      man.stupidServo(2).setPosition(-2);
    }

    if (Box_7_c == BLUE)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 15000);

      man.stupidServo(0).setPosition(-2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_7);
      man.stupidServo(2).setPosition(-2);
    }

    if (Box_6_c == BLUE)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 15000);

      man.stupidServo(0).setPosition(-2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(2).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_6);
      man.stupidServo(2).setPosition(-2);
    }
    if (Box_5_c == BLUE)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 15000);

      man.stupidServo(0).setPosition(-2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_5);
      man.stupidServo(2).setPosition(-2);
    }

    BackwardUntillWall();
    Straight(1000, 150, 12000);
    StopMotors();
    Turn(90);
    BackwardUntillWall();
    klepeto.Move(open);
    Straight(1000, 800, 13000);
    StopMotors();
//////////////////////////////////////////////////////////////////////////////////////////////







  }
  ////////////////////////////////////////////////////////
  if (side == REDSIDE)
  {
     man.stupidServo(0).setPosition(2); // 90 deg right
    delay(1000);
    man.stupidServo(1).setPosition(2);
    man.stupidServo(2).setPosition(1); // pozice magnetu pro brani baterek

    // Straight(1000,999999,15000);
    // while (true)
    // {
    //   /* code */
    //   delay(100);
    // }
    

    Straight(1000, 630, 5000);
    StopMotors();
    delay(1000);
    Serial.println("givecolor");
    delay(500);
    Box_8_c = ReciveData();

    Straight(1000, 200, 5000);
    StopMotors();
    delay(1000);
    Serial.println("givecolor");
    delay(500);
    Box_7_c = ReciveData();

    Straight(1000, 200, 5000);
    StopMotors();
    delay(1000);
    Serial.println("givecolor");
    delay(500);
    Box_6_c = ReciveData();

    Straight(1000, 200, 5000);
    StopMotors();
    delay(1000);
    Serial.println("givecolor");
    delay(500);
    Box_5_c = ReciveData();
    //TurnLeft(1);

    GetRestBoxColor();
    
    if (Box_8_c == RED)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 5000);
      StopMotors();

      man.stupidServo(0).setPosition(2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(-2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_8-100);
      StopMotors();
      man.stupidServo(2).setPosition(-2);
      delay(1000);
    }

    if (Box_7_c == RED)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 5000);
      StopMotors();

      man.stupidServo(0).setPosition(2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(-2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_7-100);
      StopMotors();
      man.stupidServo(2).setPosition(-2);
      delay(1000);
    }

    if (Box_6_c == RED)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 5000);
      StopMotors();

      man.stupidServo(0).setPosition(2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(-2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_6-100);
      StopMotors();
      man.stupidServo(2).setPosition(-2);
      delay(1000);
    }
    if (Box_5_c == RED)
    {
      Straight(1000, Baterry_6 - currenrt_x_pos, 5000);
      StopMotors();

      man.stupidServo(0).setPosition(2); // 90 deg left
      delay(1000);
      man.stupidServo(1).setPosition(1.4);
      man.stupidServo(1).disable(); // pozice magnetu pro brani baterek
      delay(1000);

      MoveToGrabBaterry();

      man.stupidServo(1).setPosition(1.55); // zvedne rameno
      delay(800);
      man.stupidServo(0).setPosition(-2); // otoci se ramenem na drohou stranu
      delay(1000);

      Backward(1000, currenrt_x_pos - Box_5-100);
      StopMotors();
      man.stupidServo(2).setPosition(-2);
      delay(1000);
    }

    BackwardUntillWall();
    Straight(1000, 450, 2000);
    StopMotors();
    TurnLeft(90);
    BackwardUntillWall();
    //klepeto.Move(open);
    Straight(1000, 800, 3000);
    StopMotors();

   
    // konec ifu podle strany hriste
  }
  t1.join();
  t2.join();
}

void loop()
{
  // Serial.print("L: ");
}