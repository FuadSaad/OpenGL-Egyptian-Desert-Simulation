# 🐪 Egyptian Desert Simulation in OpenGL

Welcome to the **Egyptian Desert Simulation**, a highly dynamic and interactive 2D graphical project built using **C** and the **OpenGL / GLUT** library. This project visually blends the majestic historical architecture of ancient Egypt with modern infrastructure and bustling dynamic transportation systems.

## 🌟 Key Features

*   **🌍 Interactive Day/Night Cycle:** The scene automatically transitions between Day, Sunset, and Night every 20 seconds.
*   **🏛️ Architectural Marvels:** Features classical Egyptian structures including the Great Pyramids, Stepped Pyramids, Obelisks, and ancient Temples.
*   **🛣️ Smart Traffic System:** Cars, buses, trucks, and motorcycles travel along a vertical road. A functioning traffic light system automatically tells cars to stop at the zebra crossing when red! 
*   **🐪 Camel Crossing AI:** If a camel is crossing the road, nearby vehicles detect it and halt until the path is clear to avoid collisions.
*   **🛳️ River Cruise Ship:** A multi-level cruise ship sails down the Nile River, emitting dynamic particle smoke, and pauses for 5 seconds in front of the main pyramid for sightseeing before continuing its journey.
*   **✈️ Airplane Dynamics:** A commercial jet flies steadily overhead leaving a realistic trailing smoke effect.
*   **🎇 Interactive Fireworks:** Press `R` on your keyboard during the **Night Mode** to trigger colorful, gravity-affected firework particles in the sky.
*   **🌇 Modern Additions:** Functional wind turbines with rotating blades and solar panels demonstrate the fusion of ancient and modern ages.

## ⌨️ Controls

*   `D` / `d` : Manually cycle through Day, Sunset, and Night modes.
*   `R` / `r` : Launch fireworks (Available only in Night mode).
*   `P` / `p` : Play or Pause the entire simulation.
*   `+` / `-` : Increase or decrease the speed of the vehicles.
*   `ESC` : Exit the simulation application.

## 🛠️ Tech Stack & Requirements

*   **Language:** C
*   **Library:** OpenGL / FreeGLUT / GLU
*   **Compiler:** MinGW (GCC) or any standard C compiler with OpenGL linking.

## 🚀 How to Run

1.  Make sure you have `FreeGLUT` configured in your C workspace.
2.  Compile the `main.c` file linking the necessary libraries:
    ```bash
    gcc main.c -o desert_sim -lfreeglut -lglu32 -lopengl32 -lm
    ```
3.  Execute the output file:
    ```bash
    ./desert_sim
    ```

---
*Created as a computer graphics project showcasing 2D transformations, timing functions, intelligent state logic, and particle systems.*
