# UWindVisualization

**UWindVisualization** is a terminal-based C++ simulation that visualizes airflow through urban environments using particles, flow fields, recirculation zones, and vortex streets. The simulation renders particles in real time using ASCII art, with colors representing wind speed.

This project blends computational fluid dynamics concepts with creative data visualization, built entirely from scratch without any game engines or third-party visualization libraries.

## Why I Built It

While working as an FX Artist on *Ghostbusters: Frozen Empire*, I frequently had to simulate snowfall in windy urban environments. These simulations were often repetitive, computationally expensive, and required manual tweaking to get the right look shot-to-shot. I began thinking:

> “What if there were a more physically accurate and reusable way to simulate wind behavior?”

This project is the result — an exploration of airflow around obstacles like buildings, recirculation bubbles, and Von Kármán vortex streets using a lightweight, code-first approach.

## What It Does

- Simulates wind flow through a 2D grid representing a cityscape
- Supports buildings as obstacles that influence flow
- Models key airflow phenomena:
  - Flow fields
  - Recirculation zones
  - Vortex streets
- Visualizes particles in real time using ASCII characters in the terminal
- Colors particles based on wind velocity magnitude

## Technologies

- C++: Core simulation and logic
- Terminal/CLI: Text-based visualization using ASCII art

## Preview
![UWind Demo 001](/assets/images/UWind-demo-001.gif)
![UWind Demo 002](/assets/images/UWind-demo-002.gif)
![UWind Demo 003](/assets/images/UWind-demo-003.gif)
