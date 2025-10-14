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
