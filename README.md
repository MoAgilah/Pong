# Pong

A minimalist Pong clone built to **demonstrate** usage of **SFMLEngine** — a backend-agnostic C++ engine core with an SFML backend.

This project exists to:

- Show how to structure a simple game on top of your engine  
- Validate interface design and engine usability  
- Serve as a reference for future projects like *SuperMarioWorldClone*

---

## 🎯 Goals & Purpose

- Provide a clean, straightforward example of using **SFMLEngine**  
- Test engine subsystems (windowing, input, rendering, game loop) in a small, contained codebase  
- Help onboard new users by lowering the barrier to experimenting with the engine  

---
## 📦 Download & Run

You can download the latest standalone executable here:

👉 [**Download Standalone Executable**](https://raw.githubusercontent.com/MoAgilah/Pong/master/Bin/Pong.zip)

After downloading:

- Extract the `.zip` file.  
- Open the folder and run the executable:
  - **Windows:** `Pong.exe`
- Use the menus and control instructions below to start playing.

---

## 🕹️ How to Start / Mode Select

When you launch the game:

1. The **Loading Screen** will appear briefly while assets are initialised.  
2. You’ll then enter the **Main Menu**, where you can choose:
   - **Player vs A.I**
   - **Player vs Player**
   - **A.I vs A.I**
   - **Player vs Wall** (Practice Mode)

Use **Up / Down** and **Enter** to navigate menus.  
Some menus also use **Left / Right** for side or difficulty selection.

### Menu Flow
- Select your **game mode** → choose **player sides** → select **difficulty** → start match.  
- Once selections are made, you’ll transition to the game court (or practice court for wall mode).

---

## 🌍 Global Actions

| Action                  | Key          |
|--------------------------|--------------|
| Open / Close Pause Menu  | **Space**    |
| Confirm / Select         | **Enter**    |
| Navigate Up / Down       | **↑ / ↓**    |
| Navigate Left / Right    | **← / →**    |
| Exit Game (any time)     | **Esc**      |

---

## ⏸️ Pause Menu

A global **Pause Menu** is available during gameplay.

**Controls:**
- **Up / Down** – navigate options  
- **Enter** – confirm selection  

**Options:**
- **Resume** – return to the match  
- **To Title** – return to the main menu  
- **Quit** – close the game  

---

## 🎮 Player Controls

### Player 1
| Action           | Key          |
|------------------|--------------|
| Move Up          | **W**        |
| Move Down        | **S**        |
| Fast Shot        | **Left Shift** |
| Slow Shot        | **Left Ctrl** |
| Left Spin        | **A**        |
| Right Spin       | **D**        |
| Top Spin         | **Q**        |
| Back Spin        | **Z**        |

### Player 2
| Action           | Key          |
|------------------|--------------|
| Move Up          | **I**        |
| Move Down        | **K**        |
| Fast Shot        | **Right Shift** |
| Slow Shot        | **Right Ctrl** |
| Left Spin        | **J**        |
| Right Spin       | **L**        |
| Top Spin         | **U**        |
| Back Spin        | **M**        |

---

## 🤖 AI Modes

In **AI vs AI** or **Player vs AI**, the A.I. dynamically adjusts:
- **Reaction time**, **fatigue**, and **stress** based on match flow.  
- **Shot selection** (fast, slow, spin, fake, or corner shots) driven by difficulty level.  

You can observe A.I. behavior as a demonstration of decision-making and shot prediction built atop the `SFMLEngine` framework.

---

## 🏟️ Practice Mode

- **Player vs Wall** allows endless rallies against a single side wall.  
- Rally count is tracked as your score.  
- Ideal for testing paddle control and ball physics.

---

> “Pong demonstrates the flexibility of the SFMLEngine — from dynamic menus to adaptive AI behavior — all while keeping gameplay simple and fun.”


## 🧱 Project Structure
```
Pong/
├── Game/
│ ├── Code/ # Game-specific source code (entities, logic, main loop)
│ └── Resources/ # Game-specific assets (textures, sounds, etc.)
├── SFMLEngine/ # Submodule: the SFMLEngine repository
└── README.md
```


---

## ⚙️ Getting Started

### 1. Clone with Submodules
```
git clone https://github.com/MoAgilah/Pong.git
cd Pong
git submodule update --init --recursive
```
### 2. Build

#### 🧰 Using Visual Studio
Simply open the solution (if provided) and build your desired configuration (**Debug** or **Release**).

## 🧩 How It Works

**Pong** is built on top of the [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine) framework, using its **EngineInterface** for all engine-level systems — rendering, input, and windowing — without depending directly on SFML.

---

### 🧱 Architecture Overview

- **Game/Code/** — Contains all game logic such as ball movement, paddle control, collision detection, and score handling.  
- **sfmlengine/** — Submodule reference to the engine core providing backend-agnostic functionality via SFML.  
- **Resources/** — Contains textures, fonts, and other visual or audio assets.  

---

### 🔗 Integration Points

1. The game creates an engine instance through the **EngineSFMLBackEnd** factory.  
2. All game entities interact only with **EngineInterface** types.  
3. The engine handles rendering, input events, and timing through its backend abstraction.

## 🎮 Gameplay Overview

Classic **Pong** gameplay — two paddles, one ball, simple physics, and a scoring system.  
Designed as a lightweight demonstration of real-time input handling, object updates, and rendering through [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine).

---

## 🛠️ Future Extensions

- [ ] Add sound effects using the engine’s audio system  
- [ ] Introduce AI-controlled opponent (tying into [**AI-Game-Controller**](https://github.com/MoAgilah/AI-Game-Controller) concepts)  
- [ ] Add networked or local multiplayer  
- [ ] Experiment with additional backends (DirectX, etc.) once supported by [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine)  

---

## 👤 Author
**Mohamed Agilah**  
🎓 Games Programmer & AI Developer  
🌐 [Portfolio Website](https://moagilah.com/)  
📧 Contact: agilahmohamed@gmail.com  


