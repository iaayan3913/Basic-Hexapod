# 8-DOF DIY Quadruped Robot

A low-cost, 3D-printable quadruped robot powered by an **Arduino** and the **PCA9685 servo driver**.  
This project features a suite of pre-programmed animations such as waving, pushups, and bowing.

![Wave animation](Images_Videos/vid7.gif)

---

## 📋 Table of Contents
- [Features](#-features)
- [Hardware Requirements](#️-hardware-requirements)
- [Wiring Guide](#-wiring-guide)
- [Software Setup](#-software-setup)
- [Calibration](#️-calibration)

---

## 🌟 Features
- **8 Degrees of Freedom**  
  - 4 legs with **Hip** and **Knee** joints
- **Object-Oriented Control**  
  - Custom C++ classes for joint abstraction and mirroring
- **Animations**
  - 👋 **Wave** – Balances on 3 legs and waves
  - 💤 **Sleep / Wake** – Smooth transitions between poses
  - 💪 **Pushups** – Demonstrates torque coordination
  - 🙇 **Bow & Sit** – Dog-like tricks
- **Interactive Calibration**  
  - Serial-based tuning of servo limits

---

## 🛠️ Hardware Requirements
- **Microcontroller:** Arduino Uno R3 (or compatible)
- **Servo Driver:** PCA9685 16-Channel 12-bit PWM Driver (I2C)
- **Actuators:**  
  - 8× SG90 Micro Servos  
  - *(MG90S recommended for metal gears)*
- **Power Supply:**  
  - 5V **2A or higher** (USB wall adapter or power bank)

⚠️ **Important:**  
Do **NOT** power servos directly from the Arduino 5V pin.

- **Chassis:**  
  - Custom 3D-printed parts (see `/Final_Prints` folder)

---

## 🔌 Wiring Guide

This project uses the **Adafruit_PWMServoDriver** library.

### PCA9685 → Arduino
| PCA9685 | Arduino Uno |
|-------|-------------|
| VCC | 5V |
| GND | GND |
| SDA | A4 |
| SCL | A5 |


---

### Servo Channel Mapping

| Leg | Hip Channel | Knee Channel |
|----|------------|--------------|
| Front Left (FL) | 0 | 4 |
| Front Right (FR) | 1 | 5 |
| Back Left (BL) | 2 | 6 |
| Back Right (BR) | 3 | 7 |


---

## 💾 Software Setup
1. **Install Arduino IDE**  
   - Download from: https://www.arduino.cc/en/software

2. **Install Required Library**
   - Open Arduino IDE  
   - Go to **Tools → Manage Libraries**
   - Search for **Adafruit PWM Servo Driver**
   - Install the latest version

3. **Upload the Code**
   - Open a code from the `/Codes` folder
   - Connect Arduino via USB
   - Select the correct **Board** and **Port**
   - Click **Upload**

---
## Conclusion from this project 
This project has allowed me to experiment with many different features of a spider robot. I have experimented with inverse kinmatics as well as controlling servos from PWM signals alone. However some of the setbacks of this model were the CAD model and the servos. The model does not fully secure the hip mount to the servo and causing it to fall off in high load situations. The lack of tolerance also creates alot of friction causing the servo to struggle further under load. These are also the SG90 servos which are notoriously weak. An improvement would be to use the metal geared servos or scale this up to account for larger stronger servos.

The next stage would be to increase the degrees of freedom and make a more articulated spider robot.

---

## ⚙️ Calibration

Each servo behaves slightly differently. If the legs are not straight or symmetrical, adjust the pulse limits:

```cpp
// Located at the top of the .ino file
#define SERVOMIN  110  // Minimum pulse length (≈ 0°)


#define SERVOMAX  550  // Maximum pulse length (≈ 180°)
