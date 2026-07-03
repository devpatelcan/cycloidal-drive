# Cycloidal Drive

[![Watch the video](link-to-video-thumbnail.jpg)](Link-to-your-YouTube-video-or-embed-a-GIF-here)
*Short caption explaining what is happening in the video.*


I built a cycloidal drive that will later function as a joint in a custom 5-degree-of-freedom robotic arm. The system achieves a 20:1 gear reduction, handling up to XXX Nm of torque while keeping backlash under 0.6°. This setup delivers the high torque and precise output control necessary for robotic actuators.


---



## How It Works

### Mechanical
* 3D printed parts out of PETG 
* 2 eccentric disks suspended by 2x 6802 load-bearings
<img width="621" height="400" alt="inside drive" src="https://github.com/user-attachments/assets/f369bbd7-c73e-43e3-a039-edb488c9fbb4" />


### Electrical
* **Microcontroller:**ESP32-WROVER Board, BLDC SimpleFOC Board (Clone)
* **Sensors:** AS5600 Magnetic Encoder
* **Power:** 12VDC 20A PSU

### Software
* **Languages:** C++
* Upon startup, the motor first initializes FOC. The magnetic encoder aligns its 0° position with the BLDC electrical 0. After calibration, the motor remains fixed at 0° while running current and velocity PID to minimize energy loss and maintain position. A command (ex, "M3.14" - M activates motor, 3.14 is the angular position) can be sent into serial monitor to adjust its position. Simultaneously, the ESP32-WROVER reads encoder and active current sense data from FOC board, and uses Teleplot to graph parameters (i.e, Angle, Velocity, Q axis current, etc).

<img width="1657" height="706" alt="teleplot plots" src="https://github.com/user-attachments/assets/c02faf8a-6c43-4079-a40c-8b9308b850ff" />

---

## Bill of Materials (BOM)
A quick list of the main hardware used. You do not need every single screw, just the big ticket items.
* Part A
* Part B
* Part C

---

## Challenges and Fixes
Be honest about what broke and how you fixed it. Engineers love reading about troubleshooting.
* **Issue:** The initial motor drivers burned out under load.
* **Fix:** Swapped to BTS7960 high-current motor drivers and added a heatsink.

---

## How to Run the Code
Provide exact steps for someone to flash your code to their own board if they build this.

1. Clone this repository.
2. Open the `.ino` file in the Arduino IDE.
3. Install the `[Specific Library Name]` library.
4. Upload to board.
