# AI Rules - Breakout Game Coding Standards

## 📁 PROJECT STRUCTURE

### Directory Organization
```
src/
├── class/          - All class definitions (.hpp files)
├── enum/           - All enum definitions (.hpp files)  
├── functions/      - Utility functions and constants (.hpp files)
```

**RULE 1.1**: All header files MUST use `.hpp` extension
**RULE 1.2**: Classes go in `src/class/`, enums in `src/enum/`, utility functions in `src/functions/`
**RULE 1.3**: One class per file, filename MUST match class name exactly (e.g., `Ball.hpp` for `Ball` class)

## 🏗️ FILE STRUCTURE

### Header File Template
```cpp
#pragma once
#include <raylib.h>
// ... other system includes
#include "../functions/Consts.hpp"
#include "../functions/Utils.hpp"
// ... other local includes

class ClassName {
public:
    // Public members and methods
private:
    // Private members and methods
};
```

**RULE 2.1**: ALL header files MUST start with `#pragma once`
**RULE 2.2**: Include `<raylib.h>` first for graphics-related classes
**RULE 2.3**: Group includes: system libraries first, then local includes
**RULE 2.4**: Use relative paths with `../` for cross-directory includes
**RULE 2.5**: Public section MUST come before private section

## 🏷️ NAMING CONVENTIONS

### Classes
**RULE 3.1**: Class names use PascalCase (e.g., `Ball`, `Paddle`, `ItemManager`)
**RULE 3.2**: Struct names use PascalCase (e.g., `Block`, `Gift`)

### Variables and Members
**RULE 3.3**: Private member variables use camelCase (e.g., `rect`, `blocks`, `vx`, `vy`)
**RULE 3.4**: Local variables use camelCase (e.g., `modalWidth`, `buttonHeight`)
**RULE 3.5**: Loop counters use single letters: `row`, `col`, `it` for iterators
**RULE 3.6**: Boolean variables use descriptive names (e.g., `hasGift`, `active`, `showEndModal`)

### Functions and Methods
**RULE 3.7**: Method names use camelCase (e.g., `checkCollisions`, `handleInput`, `increaseWidth`)
**RULE 3.8**: Getter methods return references: `getRect()` returns `Rectangle&`
**RULE 3.9**: Action methods use verb names: `update()`, `show()`, `reset()`
**RULE 3.10**: Boolean query methods use `is` prefix: `isEmpty()`, `isOut()`

### Constants
**RULE 3.11**: Constants use SCREAMING_SNAKE_CASE (e.g., `WINDOW_WIDTH`, `BALL_SIZE`)
**RULE 3.12**: All constants MUST be in `Consts` namespace in `Consts.hpp`
**RULE 3.13**: Use `constexpr` for compile-time constants

### Enums
**RULE 3.14**: Enum classes use PascalCase (e.g., `EndState`)
**RULE 3.15**: Enum values use SCREAMING_CASE (e.g., `NONE`, `VICTORY`, `DEFEAT`)

## 🎯 VARIABLE PREFIXES AND SUFFIXES

### Coordinate Variables
**RULE 4.1**: Position variables: `x`, `y` coordinates
**RULE 4.2**: Velocity variables: `vx`, `vy` for velocity components
**RULE 4.3**: Size variables: `width`, `height` or `W`, `H` in constants

### Collection Variables
**RULE 4.4**: Container variables use plural nouns (e.g., `blocks`, `gifts`)
**RULE 4.5**: Iterator variables use `it` (e.g., `for (auto it = blocks.begin()...)`)

### UI Variables
**RULE 4.6**: Modal variables use `modal` prefix (e.g., `modalWidth`, `modalX`)
**RULE 4.7**: Button variables use `button` prefix (e.g., `buttonWidth`, `buttonX`)

## 🏛️ CLASS DESIGN PRINCIPLES

### Constructor Rules
**RULE 5.1**: Initialize basic properties in constructor body
**RULE 5.2**: Use member initializer lists for const members or complex initialization
**RULE 5.3**: Parameterized constructors for configurable classes (e.g., `Blocks(int rows, int cols)`)

### Method Organization
**RULE 5.4**: Public methods order: constructor, main actions (`update`, `show`), utilities, getters
**RULE 5.5**: Core game loop methods: `update()`, `show()`, `reset()`
**RULE 5.6**: Collision methods: `checkCollisions()` for inter-object collision detection

### Member Variables
**RULE 5.7**: Use `Rectangle rect{}` for graphical objects (raylib convention)
**RULE 5.8**: Velocity as separate `float vx, vy` variables for 2D movement
**RULE 5.9**: Boolean flags for state: `active`, `hasGift`, etc.

## 🔧 CODING PATTERNS

### Memory Management
**RULE 6.1**: Use stack allocation for game objects when possible
**RULE 6.2**: Use `std::vector` for dynamic collections
**RULE 6.3**: Pointer parameters for object interaction (e.g., `Ball* ball`, `Paddle* paddle`)

### Collision Detection
**RULE 6.4**: Use raylib's `CheckCollisionRecs()` for rectangle collision
**RULE 6.5**: Implement `rebound()` method for physics response
**RULE 6.6**: Remove objects using iterator pattern with `pop_back()` swap

### Game Loop Integration
**RULE 6.7**: Every game object MUST have: `update()`, `show()`, `reset()` methods
**RULE 6.8**: Use `reset()` to return objects to initial state
**RULE 6.9**: Handle input in dedicated `handleInput()` methods

## 📦 NAMESPACE USAGE

### Consts Namespace
**RULE 7.1**: ALL constants MUST be in `Consts` namespace
**RULE 7.2**: Access constants with `Consts::` prefix (e.g., `Consts::WINDOW_WIDTH`)
**RULE 7.3**: Group related constants (window, paddle, ball, block, gift constants)

### Utils Namespace
**RULE 7.4**: Utility functions MUST be in `Utils` namespace
**RULE 7.5**: Use `inline` for header-only utility functions
**RULE 7.6**: UI helper functions belong in Utils (e.g., `showModal()`)

## 🎮 RAYLIB INTEGRATION

### Drawing Rules
**RULE 8.1**: All drawing MUST happen in `show()` methods
**RULE 8.2**: Use `DrawRectangleRec(rect, color)` for rectangular objects
**RULE 8.3**: Color constants: `WHITE`, `SKYBLUE`, `PURPLE`, `GOLD`, `BLACK`, `LIGHTGRAY`

### Input Handling
**RULE 8.4**: Use `IsKeyDown()` for continuous input (movement)
**RULE 8.5**: Use `IsMouseButtonPressed()` for discrete input (clicks)
**RULE 8.6**: Support both arrow keys and WASD: `KEY_LEFT`/`KEY_A`, `KEY_RIGHT`/`KEY_D`

### Game Loop
**RULE 8.7**: Main loop structure: update logic, then render with `BeginDrawing()`/`EndDrawing()`
**RULE 8.8**: Use `WindowShouldClose()` for proper window handling
**RULE 8.9**: Clear background with `ClearBackground({0, 12, 33, 61})` (project's dark blue)

## 🔄 OBJECT LIFECYCLE

### Initialization Pattern
**RULE 9.1**: Dynamic objects: constructor → `reset()` → ready to use
**RULE 9.2**: Static objects: constructor → configure → ready to use

### Update Pattern
**RULE 9.3**: Update order: physics → input → collisions → cleanup
**RULE 9.4**: Objects MUST update their own state in `update()` method
**RULE 9.5**: Cross-object interactions via method parameters

### Cleanup Pattern
**RULE 9.6**: Use `clear()` for containers in `reset()` methods
**RULE 9.7**: Reset position and state variables to initial values
**RULE 9.8**: Maintain object relationships after reset

## 📊 DATA STRUCTURES

### Collections
**RULE 10.1**: Use `std::vector` for dynamic object collections
**RULE 10.2**: Use structs for simple data grouping (e.g., `Block`, `Gift`)
**RULE 10.3**: Embed data in structs, not pointers (e.g., `Rectangle rect` not `Rectangle* rect`)

### State Management
**RULE 10.4**: Use enum classes for typed state (e.g., `EndState::VICTORY`)
**RULE 10.5**: Boolean flags for binary states (e.g., `showEndModal`, `hasGift`)
**RULE 10.6**: Numeric counters for quantities (e.g., `rows`, `cols`)

## 🎯 PHYSICS AND MOVEMENT

### Position Updates
**RULE 11.1**: Update position: `rect.x += vx; rect.y += vy;`
**RULE 11.2**: Boundary checking after position update
**RULE 11.3**: Clamp positions to valid ranges

### Collision Response
**RULE 11.4**: Calculate overlap to determine collision direction
**RULE 11.5**: Separate objects before changing direction
**RULE 11.6**: Use dedicated direction methods: `setDirectionUp()`, `setDirectionDown()`, etc.

---

## 🎮 QUICK REFERENCE

### Essential Method Names
- `update()` - Update object state
- `show()` - Render object
- `reset()` - Return to initial state
- `checkCollisions()` - Handle collision detection
- `handleInput()` - Process user input
- `getRect()` - Return Rectangle reference

### Essential Patterns
- Iterator removal: `*it = container.back(); container.pop_back();`
- Collision check: `CheckCollisionRecs(rect1, rect2)`
- Random chance: `std::rand() / RAND_MAX < probability`
- Center calculation: `(WINDOW_WIDTH - width) / 2`

### File Naming
- Classes: `ClassName.hpp`
- Enums: `EnumName.hpp`
- Utilities: `Utils.hpp`, `Consts.hpp`

**FOLLOW THESE RULES EXACTLY - NO EXCEPTIONS**
