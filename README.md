🚦 Four-Way Emergency Traffic Light System

This project simulates a smart traffic light control system designed to manage four intersecting roads while prioritizing emergency vehicles (such as ambulances, fire trucks, or police cars).
The system uses microcontroller logic to dynamically adjust traffic signals, ensuring efficient flow and quick clearance for emergency response.

🧩 System Overview

The traffic control logic continuously monitors each lane using infrared or ultrasonic sensors to detect approaching vehicles.
When an emergency vehicle is detected on a particular lane, the system immediately switches all other signals to red and gives green priority to that lane.
Once the emergency vehicle passes, normal traffic sequencing resumes automatically.

🛠️ Technologies Used

a. Microcontroller	Arduino Uno Development Board
b. Programming Language	C / C++ (Arduino IDE)
c. Sensors	IR Sensor (Vehicle Detection)
d. Indicators	LEDs (Red, Yellow, Green)
e. Power Supply	5V DC
f. Tools	Arduino IDE, Proteus

⚙️ How It Works

a. Each of the four lanes has a traffic light controlled by the microcontroller.
b. Sensors detect the presence of vehicles at each lane.
c. When an emergency vehicle is detected (via a dedicated sensor or signal input), the system:
d. Turns all other lanes red
e. Gives green priority to the emergency lane
f. After the emergency vehicle passes, the normal timing cycle resumes.

🔋 Features

a. Intelligent lane prioritization for emergency vehicles

b. Fully automated control with minimal human input

c. Reduces traffic congestion and response time

d. Can be expanded to real-world intersections with wireless communication modules
