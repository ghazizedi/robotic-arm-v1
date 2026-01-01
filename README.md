# Neo V1 (6-DOF Robotic Arm)

## Overview

This project is a Version 1 (V1) robotic arm designed and built from scratch using hobby-grade actuators, custom 3D-printed mechanical components, and embedded control electronics. The arm is manually controlled using two analog joysticks, with mode switching via joystick push buttons, allowing multiple degrees of freedom to be controlled with limited input hardware.

![Neo V1](media/images/IMG_4748.png)

The primary goal of this project was to gain hands-on experience with the full robotics engineering pipeline: mechanical design, electronics, power management, embedded programming, debugging, and iterative improvement.

This project serves as a foundational platform for future upgrades toward industrial-style manipulation, autonomy, and AI-assisted perception. 

### Demo Video
![Watch the demo](media/videos/IMG_4490.mp4)

## System Architecture

### Mechanical
- **6 total joints**
  - **3 × MS24 high-torque servos** (base, shoulder, elbow)
  - **3 × SG90 micro servos** (wrist axes + gripper)
- Fully custom 3D-printed arm links, joints, and gripper

### Electronics
- **Arduino Nano** (main microcontroller)
- **PCA9685 16-channel servo driver**
  - Handles PWM generation externally
  - Allows stable simultaneous control of multiple servos
- **External 5V 3A power supply**
  - Dedicated servo power
  - Prevents voltage drops and Arduino resets
- Breadboard-based prototyping (V1 implementation)

### Control Interface
- **2 analog joysticks**
  - Each joystick controls two servo axes (X/Y)
- **Joystick push button used for mode switching**
  - Enables control of all 6 servos using minimal input hardware

## Control Logic

- Joystick analog values are read using the Arduino’s ADC
- Inputs are mapped to servo angle commands
- Commands are sent via **I²C** to the PCA9685
- A push button cycles between control modes:
  - Mode 1: Base + Shoulder
  - Mode 2: Elbow + Wrist
  - Mode 3: Wrist rotation + Gripper

This approach mirrors real robotic systems where **context-based input switching** is used to manage complex systems with limited controls.

## CAD & Mechanical Design (Fusion 360)

All mechanical components were designed in **Autodesk Fusion** using a parametric modeling workflow.

### Key Design Challenges
- **Tolerances & clearances**
  - Adjusting hole sizes for screws and servo shafts
  - Accounting for 3D printer dimensional inaccuracies
- **Load paths & torque distribution**
  - Understanding how torque propagates through arm links
  - Placing higher-torque servos closer to the base
- **Servo mounting**
  - Designing pockets to prevent rotation
  - Allowing easy installation and removal
- **Gripper (claw) design**
  - Achieving sufficient opening range
  - Preventing binding at mechanical limits
  - Designing screw bosses that do not crack under load

The final design required multiple **print–test–redesign iterations**, reinforcing the importance of iterative engineering.

## What I Learned

### Robotics & Mechatronics
- Separation of logic power vs actuator power
- Why servos must not be powered directly from microcontroller rails
- Benefits of external PWM controllers (PCA9685)
- Early intuition for joint-space vs task-space motion

### Mechanical Engineering
- Tolerance stack-ups in multi-link mechanisms
- Clearance vs rigidity tradeoffs
- Load balancing and torque scaling with arm length
- Fastener selection and stress concentration in 3D-printed parts

### Embedded Systems
- I²C communication and addressing
- ADC noise and joystick dead-zone handling
- Button debouncing
- Ground reference importance for stable control signals

### Software Engineering
- State-based control logic
- Modular code structure
- Mapping and scaling analog inputs
- Writing code designed for iteration and expansion

### Engineering Process
- Debugging hardware often dominates development time
- Many failures originate in mechanical design, not software
- Designing for maintainability is as important as functionality
- Trial-and-error is a core part of real engineering work

## Challenging & Time-Consuming Components

### Gripper Design
- Achieving sufficient grip force without stalling the servo
- Maintaining symmetric motion
- Preventing mechanical binding

### Structural Stability
- Reducing wobble without over-constraining joints
- Reinforcing printed parts while minimizing added mass

### Power Management
- Diagnosing unintended servo motion
- Learning the effects of shared grounds
- Properly isolating power domains

### Control Usability
- Designing intuitive joystick mappings
- Implementing safe and predictable mode switching
- Avoiding accidental state transitions during operation

## Limitations of V1

- Servo backlash and gear slop
- Limited positional accuracy and repeatability
- Structural flex under load
- Open-loop control (no feedback)
- Breadboard wiring is not mechanically robust
- Manual operation only

These limitations directly inform the next design iteration.

## Future Improvements (V2 and Beyond)

### Actuation & Mechanics
- Replace servos with **stepper motors**
- Integrate **gear reduction systems**
  - Planetary gearboxes
  - Cycloidal drives for high torque and low backlash
- Redesign arm proportions for improved load distribution
- Increase base mass and stiffness

### Electronics
- Custom **PCB** for power distribution and control
- Add encoders for closed-loop control
- Upgrade power system for higher current capability

### Software & Control
- Motion smoothing and acceleration limits
- Inverse kinematics
- Trajectory planning and interpolation
- Software safety limits and soft end-stops

### Autonomy & AI
- Add camera module
- Vision-based object detection
- Machine learning for object classification
- Autonomous pick-and-place routines
- Integration with **ROS 2**

### Manufacturing Quality
- Reduced tolerance stack-ups
- Improved alignment features
- Structural ribs and optimized print orientation
- Fewer fasteners with better mechanical interfaces

## Media & Documentation
CAD Files: 
Wiring Diagram:



