#define NORTH_REDLIGHT 11
#define NORTH_YELLOWLIGHT 12
#define NORTH_GREENLIGHT 13
#define NORTH_IRSENSOR A0
#define NORTH_SOUNDSENSOR A4

#define EAST_REDLIGHT 4
#define EAST_YELLOWLIGHT 3
#define EAST_GREENLIGHT 2
#define EAST_IRSENSOR A3
#define EAST_SOUNDSENSOR A7

#define SOUTH_REDLIGHT 10
#define SOUTH_YELLOWLIGHT 9
#define SOUTH_GREENLIGHT 8
#define SOUTH_IRSENSOR A1
#define SOUTH_SOUNDSENSOR A5

#define WEST_REDLIGHT 5
#define WEST_YELLOWLIGHT 6
#define WEST_GREENLIGHT 7
#define WEST_IRSENSOR A2
#define WEST_SOUNDSENSOR A6

// Define states for the traffic light system
enum TrafficState {
  NORMAL_TRAFFIC,
  EMERGENCY_WARNING,
  EMERGENCY_LANE_ACTIVE,
  EMERGENCY_CLEAR,
  IR_SENSOR_ACTIVE
};

TrafficState currentState = NORMAL_TRAFFIC;
bool emergencyActive[4] = {false}; // Tracks emergency state for each lane
bool irSensorActive = false; // Flag to track IR sensor activation

void setup() {
  // Initialize pin modes for traffic lights and sensors
  pinMode(NORTH_REDLIGHT, OUTPUT);
  pinMode(NORTH_YELLOWLIGHT, OUTPUT);
  pinMode(NORTH_GREENLIGHT, OUTPUT);
  pinMode(NORTH_IRSENSOR, INPUT);
  pinMode(NORTH_SOUNDSENSOR, INPUT);

  pinMode(EAST_REDLIGHT, OUTPUT);
  pinMode(EAST_YELLOWLIGHT, OUTPUT);
  pinMode(EAST_GREENLIGHT, OUTPUT);
  pinMode(EAST_IRSENSOR, INPUT);
  pinMode(EAST_SOUNDSENSOR, INPUT);

  pinMode(SOUTH_REDLIGHT, OUTPUT);
  pinMode(SOUTH_YELLOWLIGHT, OUTPUT);
  pinMode(SOUTH_GREENLIGHT, OUTPUT);
  pinMode(SOUTH_IRSENSOR, INPUT);
  pinMode(SOUTH_SOUNDSENSOR, INPUT);

  pinMode(WEST_REDLIGHT, OUTPUT);
  pinMode(WEST_YELLOWLIGHT, OUTPUT);
  pinMode(WEST_GREENLIGHT, OUTPUT);
  pinMode(WEST_IRSENSOR, INPUT);
  pinMode(WEST_SOUNDSENSOR, INPUT);

 
  
}

void loop() {
  // Read sensor values
  int NORTHIRSENSORVALUE = digitalRead(NORTH_IRSENSOR);
  int EASTIRSENSORVALUE = digitalRead(EAST_IRSENSOR);
  int SOUTHIRSENSORVALUE = digitalRead(SOUTH_IRSENSOR);
  int WESTIRSENSORVALUE = digitalRead(WEST_IRSENSOR);

  int NORTHSOUNDSENSORVALUE = digitalRead(NORTH_SOUNDSENSOR);
  int EASTSOUNDSENSORVALUE = analogRead(EAST_SOUNDSENSOR);
  int SOUTHSOUNDSENSORVALUE = digitalRead(SOUTH_SOUNDSENSOR);
  int WESTSOUNDSENSORVALUE = analogRead(WEST_SOUNDSENSOR);

  // Check for emergency vehicle detection in each lane
  if (NORTHSOUNDSENSORVALUE == HIGH) {
    emergencyActive[0] = true; // North lane emergency detected
  } else {
    emergencyActive[0] = false;
  }

  if (EASTSOUNDSENSORVALUE > 500) {
    emergencyActive[1] = true; // East lane emergency detected
  } else {
    emergencyActive[1] = false;
  }

  if (SOUTHSOUNDSENSORVALUE == HIGH) {
    emergencyActive[2] = true; // South lane emergency detected
  } else {
    emergencyActive[2] = false;
  }

  if (WESTSOUNDSENSORVALUE > 500) {
    emergencyActive[3] = true; // West lane emergency detected
  } else {
    emergencyActive[3] = false;
  }

  // Check for normal traffic flow using IR sensors
  if (!emergencyActive[0] && !emergencyActive[1] && !emergencyActive[2] && !emergencyActive[3]) {
    if (NORTHIRSENSORVALUE == HIGH || EASTIRSENSORVALUE == HIGH || SOUTHIRSENSORVALUE == HIGH || WESTIRSENSORVALUE == HIGH) {
      irSensorActive = true;
    } else {
      irSensorActive = false;
    }
  } else {
    irSensorActive = false; // Ensure IR sensor activation is false during emergency
  }

 

  // State machine to handle traffic light states based on emergency detection
  switch (currentState) {
    case NORMAL_TRAFFIC:
      if (emergencyActive[0] || emergencyActive[1] || emergencyActive[2] || emergencyActive[3]) {
        currentState = EMERGENCY_WARNING;
        
      } else if (irSensorActive ) {
        currentState = IR_SENSOR_ACTIVE;
      }

      break;

    case IR_SENSOR_ACTIVE:
      // Implement your normal traffic flow logic here
      normalTrafficFlow();

      // Reset IR sensor activation flag after handling the state
      irSensorActive = false;
      
      // Return to normal traffic state
      currentState = NORMAL_TRAFFIC;
      break;

    case EMERGENCY_WARNING:
      // Activate yellow lights on all lanes
      activateYellowLights();
      delay(200); // Wait for 200ms

      // Determine emergency lanes and set lights accordingly
      for (int i = 0; i < 4; ++i) {
        if (emergencyActive[i]) {
          activateEmergencyLaneLights(i);
        }
      }
      
      currentState = EMERGENCY_LANE_ACTIVE;
      break;

    case EMERGENCY_LANE_ACTIVE:
      // Wait for emergency vehicle to pass
      delay(200); // Wait for 200ms

      // Check if the emergency lane is still active
      if (emergencyActive[0] || emergencyActive[1] || emergencyActive[2] || emergencyActive[3]) {
        // Emergency lane is still active, continue emergency handling
        for (int i = 0; i < 4; ++i) {
          if (emergencyActive[i]) {
            activateEmergencyLaneLights(i);
          }
        }
      } else {
        // No longer emergency, switch to clear state
        currentState = EMERGENCY_CLEAR;
        delay(200);
        
      // Check if any IR Sensor is turned on to resume normal traffic flow
        if(irSensorActive){
        currentState = IR_SENSOR_ACTIVE;
        deactivateYellowLights();
        } else {
          activateRedLights();
        }
      }
      break;

    case EMERGENCY_CLEAR:
      // Check if all emergencies are cleared
      bool allClear = true;
      for (int i = 0; i < 4; ++i) {
        if (emergencyActive[i]) {
          allClear = false;
          break;
        }
      }
      
      if (allClear) {
        // Return to normal traffic flow
        activateYellowLights();
        delay(200); // Wait for 200ms
        
        
        normalTrafficFlow();
        currentState = NORMAL_TRAFFIC;
      }
      break;
  }
}

// Function to deactivate yellow lights on all lanes
void deactivateYellowLights(){
  digitalWrite(NORTH_YELLOWLIGHT, LOW);
  digitalWrite(EAST_YELLOWLIGHT, LOW);
  digitalWrite(SOUTH_YELLOWLIGHT, LOW);
  digitalWrite(WEST_YELLOWLIGHT, LOW);

  digitalWrite(NORTH_GREENLIGHT, LOW);
  digitalWrite(EAST_GREENLIGHT, LOW);
  digitalWrite(SOUTH_GREENLIGHT, LOW);
  digitalWrite(WEST_GREENLIGHT, LOW);

  digitalWrite(NORTH_REDLIGHT, LOW);
  digitalWrite(EAST_REDLIGHT, LOW);
  digitalWrite(SOUTH_REDLIGHT, LOW);
  digitalWrite(WEST_REDLIGHT, LOW);
}
// Function to activate yellow lights on all lanes
void activateYellowLights() {
  digitalWrite(NORTH_YELLOWLIGHT, HIGH);
  digitalWrite(EAST_YELLOWLIGHT, HIGH);
  digitalWrite(SOUTH_YELLOWLIGHT, HIGH);
  digitalWrite(WEST_YELLOWLIGHT, HIGH);

  digitalWrite(NORTH_GREENLIGHT, LOW);
  digitalWrite(EAST_GREENLIGHT, LOW);
  digitalWrite(SOUTH_GREENLIGHT, LOW);
  digitalWrite(WEST_GREENLIGHT, LOW);

  digitalWrite(NORTH_REDLIGHT, LOW);
  digitalWrite(EAST_REDLIGHT, LOW);
  digitalWrite(SOUTH_REDLIGHT, LOW);
  digitalWrite(WEST_REDLIGHT, LOW);
}
void activateRedLights(){
  digitalWrite(NORTH_YELLOWLIGHT, LOW);
  digitalWrite(EAST_YELLOWLIGHT, LOW);
  digitalWrite(SOUTH_YELLOWLIGHT, LOW);
  digitalWrite(WEST_YELLOWLIGHT, LOW);

  digitalWrite(NORTH_GREENLIGHT, LOW);
  digitalWrite(EAST_GREENLIGHT, LOW);
  digitalWrite(SOUTH_GREENLIGHT, LOW);
  digitalWrite(WEST_GREENLIGHT, LOW);

  digitalWrite(NORTH_REDLIGHT, HIGH);
  digitalWrite(EAST_REDLIGHT, HIGH);
  digitalWrite(SOUTH_REDLIGHT, HIGH);
  digitalWrite(WEST_REDLIGHT, HIGH);
}

// Function to activate green light for the emergency lane
void activateEmergencyLaneLights(int lane) {
  // Turn off yellow lights for the detected emergency lane
  switch (lane) {
    case 0: // North lane
      digitalWrite(NORTH_YELLOWLIGHT, LOW);
      digitalWrite(NORTH_REDLIGHT, LOW);
      digitalWrite(NORTH_GREENLIGHT, HIGH);
      break;
    case 1: // East lane
      digitalWrite(EAST_YELLOWLIGHT, LOW);
      digitalWrite(EAST_REDLIGHT, LOW);
      digitalWrite(EAST_GREENLIGHT, HIGH);
      break;
    case 2: // South lane
      digitalWrite(SOUTH_YELLOWLIGHT, LOW);
      digitalWrite(SOUTH_REDLIGHT, LOW);
      digitalWrite(SOUTH_GREENLIGHT, HIGH);
      break;
    case 3: // West lane
      digitalWrite(WEST_YELLOWLIGHT, LOW);
      digitalWrite(WEST_REDLIGHT, LOW);
      digitalWrite(WEST_GREENLIGHT, HIGH);
      break;
    default:
      break;
  }

  // Turn red lights off for all other lanes
  for (int i = 0; i < 4; ++i) {
    if (i != lane) {
      switch (i) {
        case 0:
          digitalWrite(NORTH_REDLIGHT, LOW);
          break;
        case 1:
          digitalWrite(EAST_REDLIGHT, LOW);
          break;
        case 2:
          digitalWrite(SOUTH_REDLIGHT, LOW);
          break;
        case 3:
          digitalWrite(WEST_REDLIGHT, LOW);
          break;
        default:
          break;
      }
    }
  }
}

// Function to handle normal traffic flow
void normalTrafficFlow(){
// CONDITION 1: ASSUME THAT VEHICLES ARE COMING FROM ANY OF THE FOUR LANES
    digitalWrite(NORTH_REDLIGHT, LOW);
    digitalWrite(EAST_REDLIGHT, LOW);
    digitalWrite(NORTH_GREENLIGHT, HIGH);
    digitalWrite(EAST_GREENLIGHT, HIGH);
    digitalWrite(SOUTH_GREENLIGHT, LOW);
    digitalWrite(WEST_GREENLIGHT, LOW);
    digitalWrite(SOUTH_REDLIGHT, HIGH);
    digitalWrite(WEST_REDLIGHT, HIGH);

    delay(200);

    digitalWrite(NORTH_GREENLIGHT, LOW);
    digitalWrite(EAST_GREENLIGHT, LOW);
    digitalWrite(NORTH_YELLOWLIGHT, HIGH);
    digitalWrite(EAST_YELLOWLIGHT, HIGH);
    digitalWrite(SOUTH_REDLIGHT, LOW);
    digitalWrite(WEST_REDLIGHT, LOW);
    digitalWrite(SOUTH_YELLOWLIGHT, HIGH);
    digitalWrite(WEST_YELLOWLIGHT, HIGH);

    delay(200);

    digitalWrite(NORTH_YELLOWLIGHT, LOW);
    digitalWrite(EAST_YELLOWLIGHT, LOW);
    digitalWrite(NORTH_REDLIGHT, HIGH);
    digitalWrite(EAST_REDLIGHT, HIGH);
    digitalWrite(SOUTH_YELLOWLIGHT, LOW);
    digitalWrite(WEST_YELLOWLIGHT, LOW);
    digitalWrite(SOUTH_GREENLIGHT, HIGH);
    digitalWrite(WEST_GREENLIGHT, HIGH);
    
    delay(200);

}



