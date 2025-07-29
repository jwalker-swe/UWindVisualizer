# UWindVisualizer

**UWindVisualizer** is a C++ CLI tool for understanding wind flow phenomena through hands-on visualization and experimentation.

This project blends computational fluid dynamics concepts with creative data visualization, built entirely from scratch without any game engines or third-party visualization libraries.

## Overview

UWindVisualizer breaks down complex airflow behavior into digestible, real-time ASCII animations. While ASCII rendering isn't the most detailed visualization method, it provides just enough information to clearly understand core fluid dynamics concepts in urban environments.

Using a 2D velocity grid and particle system, you can directly observe:

- How recirculation zones form behind buildings
- How vortex streets develop in the wake of obstacles
- How wind patterns curve and interact with urban geometry
- The relationship between particle behavior and underlying flow fields

## Purpose

This tool serves as an educational instrument for understanding fluid dynamics fundamentals. While sophisticated production solvers automatically handle these phenomena through comprehensive fluid dynamics, they can feel like black boxes when you're trying to understand why certain patterns emerge. UWindVisualizer strips away the complexity to focus on core principles—making the invisible visible.

## Learning Outcomes

By implementing and experimenting with this tool, you can gain intuitive understanding of:

- How velocity gradients create circulation patterns
- Why certain building shapes generate stronger wake effects
- How pressure differentials drive airflow around obstacles
- The physics that drive complex wind simulations

## Features

- Real-time ASCII particle visualization
- 2D velocity field simulation
- Color-coded particles by speed
- Interactive urban geometry setup
- Lightweight and fast iteration

## Technical Details

### Architecture

- Velocity Field: 2D grid-based velocity field using simple finite difference methods
- Particle System: Thousands of advected particles for flow visualization
- Boundary Conditions: No-slip conditions at building walls with velocity interpolation
- Rendering: Real-time ASCII output with ANSI color codes for velocity magnitude

### Key Components

- VelocityField: Manages the underlying flow field and boundary conditions
- ParticleSystem: Handles particle advection and lifecycle management
- Renderer: ASCII visualization with color-coding based on local velocity
- Geometry: Simple building placement and obstacle definition

### Physics Implementation

The simulation uses simplified Navier-Stokes equations focusing on:

- Advection of velocity field
- Pressure projection for incompressible flow
- Vorticity enhancement for visual clarity
- Simple turbulence modeling

## Technologies

- C++: Core simulation and logic
- Terminal/CLI: Text-based visualization using ASCII art

## Preview
![UWind Demo 001](/assets/images/UWind-demo-001.gif)
![UWind Demo 002](/assets/images/UWind-demo-002.gif)
![UWind Demo 003](/assets/images/UWind-demo-003.gif)
