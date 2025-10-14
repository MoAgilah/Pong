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
- [ ] Add menus and score display  
- [ ] Add networked or local multiplayer  
- [ ] Experiment with additional backends (SDL, Vulkan, etc.) once supported by [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine)  

---

## 💡 Credits

Built and maintained by [**MoAgilah**](https://github.com/MoAgilah)  

Powered by [**SFMLEngine**](https://github.com/MoAgilah/SFMLEngine) — the modular, backend-agnostic engine powering all current and upcoming projects including [**SuperMarioWorldClone**](https://github.com/MoAgilah/SuperMarioWorldClone).


