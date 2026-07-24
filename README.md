# Cycloidal Drive
<a href="https://www.youtube.com/watch?v=vjJ8yUB2oT0">
  <img src="https://img.youtube.com/vi/vjJ8yUB2oT0/maxresdefault.jpg" width="600">
</a>

[Click here or on the image to watch the video](https://www.youtube.com/watch?v=vjJ8yUB2oT0)


I built a cycloidal drive that will later function as a joint in a custom 5-degree-of-freedom robotic arm. The system achieves a 20:1 gear reduction, handling atleast 4.53 Nm of torque while keeping backlash under 0.6°. This setup delivers the high torque and precise output control necessary for robotic actuators.


---



## How It Works

### Mechanical
* 3D printed parts out of PETG 
* 2 eccentric disks suspended by 2x 6802 load-bearings
<img width="621" height="400" alt="inside drive" src="https://github.com/user-attachments/assets/f369bbd7-c73e-43e3-a039-edb488c9fbb4" />


### Electrical
* **Microcontroller:** ESP32-WROVER Board, BLDC SimpleFOC Board (Clone)
* **Sensors:** AS5600 Magnetic Encoder (later switched to the MT6816)
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
| Issue | Resolution / Fix |
| :--- | :--- |
| Experienced slight stuttering during closed-loop motion. | Swapped to the **MT6816 magnetic encoder** via SPI communication. Upgraded from the AS5600 (1MHz over I²C) to the MT6816 (16 MHz over SPI) to significantly increase sampling speed. |
| Jamming occurred between cycloidal disk teeth and ring gear lobes due to tight gearbox clearance. | Reduced the pressure angle to the theoretical mathematical minimum to improve tooth mesh clearance. |
| Motor ran hot during continuous closed-loop operation. | Implemented a PID loop for direct-axis current ($I_d$) to actively drive $I_d \approx 0\text{ A}$ during rotation, minimizing unwanted reactive losses. |
| Motor jittered while attempting to hold position using PWM control. | Swapped to $\text{I}^2\text{C}$ communication via the AS5600 encoder to increase overall feedback frequency. |

<br>
Other issues I experienced was during my torque test. Expand the dropdown to view the math behind the theoretical torque. 


<details>
<summary><b>Click to expand theoretical torque calculations</b></summary>

<br>

**1. Input Power ($P_{in}$)**

$$P_{in} = V_{bus} \times I_{bus} = 9\text{V} \times 3\text{A} = 27\text{W}$$

Voltage was capped at 9V ($V_{bus}$), and current was capped at 3A ($I_{bus}$).

---

**2. Motor Phase Current ($I_q$)**

$$P_{in} = \frac{3}{2} \times I_q^2 \times R_{phase}$$

$$27 = \frac{3}{2} \times I_q^2 \times 0.15 \implies I_q = \sqrt{120} \approx 10.95\text{ A}$$

The factor $\frac{3}{2}$ comes from the 3 phases of the BLDC motor, which FOC transforms into a 2-axis coordinate system ($I_q$ and $I_d$). The phase resistance of the BLDC (measured between 2 phase wires) is $0.15\,\Omega$. This yields $I_q = 10.95\text{ A}$, representing a significant phase current through the motor.

---

**3. Motor Shaft Torque ($\tau_{motor}$)**

$$K_t = \frac{60}{2\pi \times KV} = \frac{60}{2\pi \times 360} \approx 0.0265\text{ N}\cdot\text{m/A}$$

$$\tau_{motor} = K_t \times I_q = 0.0265 \times 10.95 \approx 0.290\text{ N}\cdot\text{m}$$

---

**4. Theoretical Output Torque ($\tau_{out}$)**

$$\tau_{out} = \tau_{motor} \times R = 0.290\text{ N}\cdot\text{m} \times 20 = \mathbf{5.81\text{ N}\cdot\text{m}}$$

---

In reality, the measured torque was closer to $\approx 4.56\text{ N}\cdot\text{m}$ ($\approx 78\%$ efficiency). This exceeded what the motor mount could safely handle, so testing was capped at $3\text{ A}$. Consequently, the actuator can deliver output torques higher than $4.53\text{ N}\cdot\text{m}$ if voltage/current limits are increased in the FOC firmware with an upgraded mount.

</details>

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










