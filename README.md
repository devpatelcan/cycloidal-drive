# Cycloidal Drive

<a href="https://www.youtube.com/watch?v=WIKjf5lulUA">
  <img src="https://img.youtube.com/vi/WIKjf5lulUA/maxresdefault.jpg" width="600">
</a>


[Click here or on the image to watch the video](https://youtu.be/WIKjf5lulUA)


I built a cycloidal drive that will later function as a joint in a custom 5-degree-of-freedom robotic arm. The system achieves a 20:1 gear reduction, handling up to 5.2 Nm of torque while keeping backlash under 0.6°. This setup delivers the high torque and precise output control necessary for robotic actuators.


---



## How It Works

### Mechanical
* 3D printed parts out of PETG 
* 2 eccentric disks suspended by 2x 6802 load-bearings
<img width="621" height="400" alt="inside drive" src="https://github.com/user-attachments/assets/f369bbd7-c73e-43e3-a039-edb488c9fbb4" />


### Electrical
* **Microcontroller:** ESP32-WROVER Board, BLDC SimpleFOC Board (Clone)
* **Sensors:** AS5600 Magnetic Encoder
* **Actuators:** 5010 360 KV BLDC Motor
* **Power:** 12VDC 20A PSU

### Software
* **Languages:** C++
* Upon startup, the motor first initializes FOC. The magnetic encoder aligns its 0° position with the BLDC electrical 0. After calibration, the motor remains fixed at 0° while running current and velocity PID to minimize energy loss and maintain position. A command (ex, "M3.14" - M activates motor, 3.14 is the angular position) can be sent into serial monitor to adjust its position. Simultaneously, the ESP32-WROVER reads encoder and active current sense data from FOC board, and uses Teleplot to graph parameters (i.e, Angle, Velocity, Q axis current, etc).

<img width="1657" height="706" alt="teleplot plots" src="https://github.com/user-attachments/assets/c02faf8a-6c43-4079-a40c-8b9308b850ff" />

---

## Hardware Components

| Component | Function |
| :--- | :--- |
| **ESP32-WROVER Board** | Main microcontroller, handling serial communication between ESC and computer to enable live telemetry of parameters (angle, velocity, etc). |
| **5010 360KV BLDC Motor** | Input motor for cycloidal drive. |
| **12VDC 20A PSU** | Provides power to all electrical components. |
| **AS5600** | I2C magnetic encoder to track the motor's angular position. |
| **Brushless DC Motor ESC (FOC)** | Electronic speed controller (ESC) for BLDC via FOC. |

---

## Challenges and Fixes
* **Issue:** Experienced slight stuttering during closed loop motion.
* **Fix:** Temporarily swapped to MT6816 magnetic encoder via SPI communication.

* **Issue:** Jamming between cycloidal disk teeth and ring gear lobes due to compactness of gearbox.
* **Fix:** Set the pressure angle to minimum lowest amount allowed mathematically.

* **Issue:** Motor overheating during closed loop operation.
* **Fix:** Introduced PID for direct axis current so that it is as close to 0 as possible while spinning.

* **Issue:** Motor stutters upon holding position while usijg PWM for FOC.
* **Fix:** Swapped to I²C vommunication via AS5600 encoder to increase signal return frequency. 
---

## More Pictures and Videos


<p align="center">
  <video src="https://github.com/user-attachments/assets/7b36ad69-4149-4719-a4bc-b8de98f3c1cd" width="100%" autoplay loop muted playsinline></video>
</p>
Drive operating in open loop conditions (without an encoder).

<br>

<p align="center">
  <video src="https://github.com/user-attachments/assets/dbe917e2-9807-45cd-9588-70bf32901362" width="100%" autoplay loop muted playsinline></video>
</p>
BLDC operating in closed loop conditions controlled by voltage (spinning freely like a drone motor).










