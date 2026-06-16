# Terminal Dino Game

A terminal-based endless runner game written in C++ for POSIX-compliant systems (macOS and Linux). The player controls a character that must jump over moving ground obstacles and avoid mid-air obstacles.

## Gameplay Demonstration

Here is a recording of the game running in the terminal:

https://github.com/never954/dinogame_on_terminal/blob/main/gameplay.mov

<video src="gameplay.mov" width="100%" controls></video>

## Compiling and Running

### Prerequisites
- A C++ compiler (such as `g++` or `clang++`)
- A POSIX-compliant terminal environment (required for `termios` input manipulation)

### Compilation
Compile the source code using a C++11 compliant compiler:
```bash
g++ -std=c++11 dinogame.cpp -o DinoGame
```

### Execution
Run the compiled executable:
```bash
./DinoGame
```

## Controls and Gameplay

- **Spacebar**: Jump to avoid ground obstacles (`#`).
- **q**: Quit the game.
- **Rules**:
  - The player is represented by `@`.
  - Ground obstacles are represented by `#`.
  - Airborne obstacles (birds) are represented by `7`. Jumping while an airborne obstacle is overhead results in a collision.

## Technical Architecture

### Non-blocking Input Polling
To keep the game responsive, the program overrides the standard terminal input behavior using `termios`. By disabling canonical mode (`ICANON`) and echo (`ECHO`), keyboard inputs are read immediately without requiring the player to press Enter. The `kbhit()` helper uses `fcntl` with `O_NONBLOCK` to poll for inputs asynchronously without halting the game loop.

### Timing and Thread Control
The game loop regulates its frame rate using thread-based sleep utilities from the standard library:
- **Header:** `<thread>` and `<chrono>` are used.
- **Mechanism:** At the end of each game loop iteration, `std::this_thread::sleep_for(std::chrono::milliseconds(50))` suspends the main execution thread for 50 milliseconds.
- **Purpose:** This controls the game's tick rate (locking it to approximately 20 ticks/frames per second). It prevents the CPU from running at 100% utilization while ensuring the game speed remains consistent across different hardware platforms.
