#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ACS712.h>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include <ctime>
#include "consumo.h"
#include "eficiencia.h"
#include "programs.h"

volatile int STOP=false;

uint8_t website_inputs;
uint8_t output;

/*
for inputs:
-3 = ERROR (plugged off etc)
-2 = program interrupted (manually switched off)
-1 = just finished scheduled program
0 = connected to power and wifi is on /PLUG IS OFF/program0
1 = program1
2 = program2
3 = program3
4 = program4
5 = program5
6 =
7 =
*/

// Initialize hardware and setup
void setup() {
  pinMode(output_pin, OUTPUT);

  Serial.begin(115200);
  plug_state plug0_state = OFF;
  program plug0_program = program0;
}

void loop() {
  // Read inputs or events
  int input; //variable to receive database input for each program, int for now for debugging purposes
  std::cout << "Input?\n";
  std::cin >> input;
  // Update state machine based on inputs/events
  update_state(input);
  handle_program(price_data_instance, power_data_instance);
}

//Function to update state machine
void update_state(int input) {
  switch (plug0_state) {
    case OFF:
      if (input == 0) {
        plug0_state = STANDBY;
        plug0_program = program0;
        printf("plug is on standby\n");
      }
      break;
    case ON:
      if (input == -1) {
        plug0_state = OFF;
        plug0_program = program0;
        printf("plug is off, program finished\n");
      }
      if (input == -2) {
        plug0_state = OFF;
        plug0_program = program0;
        printf("plug is off, program interrupted\n");
      }
      if (input == -3) {
        plug0_state = ERROR;
        printf("plug is off, error\n");
      }
      break;
    case STANDBY:
      if (input == 1) {
        plug0_state = ON;
        plug0_program = program1;
        printf("plug has turned ON\n");
      }
      if (input == 2) {
        plug0_state = ON;
        plug0_program = program2;
        printf("plug has turned ON\n");
      }
      if (input == 3) {
        plug0_state = ON;
        plug0_program = program3;
        printf("plug has turned ON\n");
      }
      if (input == 4) {
        plug0_state = ON;
        plug0_program = program4;
        printf("plug has turned ON\n");
      }
      if (input == 5) {
        plug0_state = ON;
        plug0_program = program5;
        printf("plug has turned ON\n");
      }
      if (input == 6) {
        plug0_state = ON;
        plug0_program = program6;
        printf("plug has turned ON\n");
      }
      if (input == 7) {
        plug0_state = ON;
        plug0_program = program7;
        printf("plug has turned ON\n");
      }
      break;
    case ERROR:
      if (input == -1) {
        plug0_state = OFF;
        printf("plug is off\n");
      }
      break;
  }
}

// Function to handle specific functionalities for each program
// for now each program only turns plug ON according to conditions, later we might want to add variables to save the consumption/eficiency output
void handle_program(price_data& price_data, power_data& power_data) {
  switch (plug0_program) {
    case program0:
      //default, does nothing (plug is OFF)
      output = 0;
      digitalWrite(output_pin, output);//update PINmode
      break;
    case program1:
      while (plug0_state == 1){ //ON manually
        output = 1;
        digitalWrite(output_pin, output);//update PINmode
      }
      break;
    case program2:
        while (plug0_state == 1){ 
            while(price_data.best_schedule == 1){ //ON according to schedule, should be bool -> while condition true
              output = 1;
              digitalWrite(output_pin, output);//update PINmode
            }
            plug0_state = OFF;
        }
      break;
    case program3:
        while(plug0_state == 1){
          while(price_data.energy_price >= set_price){
            output = 1;
            digitalWrite(output_pin, output);//update PINmode
          }
          plug0_state = OFF;
        }
      break;
    case program4:
        while(plug0_state == 1){
          while(price_data.combined_price >= set_comb_price){
            output = 1;
            digitalWrite(output_pin, output);//update PINmode
          }
          plug0_state = OFF;
        }
      break;
    case program5:
        while(plug0_state == 1){
          while(price_data.consumption < set_consumption){
            output = 1;
            digitalWrite(output_pin, output);//update PINmode
          }
          plug0_state = OFF;
        }
      break;
    /*case program6:
      
      break;*/
  }
}