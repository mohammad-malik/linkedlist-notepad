# Linked List based Notepad (on CLI)

This project is a console-based notepad application developed as part of a Data Structures assignment. Unlike traditional text editors that use strings, this application leverages **2D doubly linked lists** to store and manage text data. Implemented in C++, it provides essential notepad features in a console environment, including text manipulation, saving, loading, and spell-checking with suggestions for corrections.

## Features

- **Console-Based Text Editing with Linked List Storage**:
  - Text is stored character-by-character using a 2D doubly linked list instead of strings or arrays.
  - Each character is stored in a node, and linked nodes form both horizontal (line-wise) and vertical (line-separated) sequences.
  - Supports insertion, deletion, and modification of text in a dynamic structure that grows as you type.

- **Spell Checker**:
  - Checks each word against a dictionary stored in a vector-based structure.
  - Misspelled words trigger suggestions, including letter substitution, omission, insertion, and reversal.
  - Outputs suggested corrections in real-time within a reserved section of the console interface.

- **Custom Cursor Navigation**:
  - Implements a custom cursor independent of the default console cursor, allowing flexible movement across characters and lines within the 2D linked list.
  - Cursor updates on each key press, displaying changes dynamically without requiring the `Enter` key.

## Data Structures

- **2D Doubly Linked List**:
  - This project emphasizes a unique approach by using a 2D doubly linked list for text storage, where each node represents a single character. Links connect characters horizontally (within the same line) and vertically (across lines).
  - This structure allows for efficient text manipulation and provides a strong understanding of how linked lists can be applied beyond conventional usage.

- **Dictionary Stored in a Vector**:
  - The dictionary for spell-checking is loaded into a `vector<vector<char>>` for efficient storage and quick access.

## Requirements

This application requires a Unix-like environment with `ncurses` installed for console management. Follow the instructions below to set up and run the application.

## Installation and Setup

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/mohammad-malik/linkedlist-notepad.git
   cd linkedlist-notepad
   ```

2. **Install ncurses** (if not already installed):
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libncurses5-dev libncursesw5-dev

   # MacOS
   brew install ncurses
   ```

3. **Compile the Program**:
   Use `g++` to compile the `main.cpp` and `dependencies.cpp` files.
   ```bash
   g++ main.cpp dependencies.cpp -lncurses -o console_notepad
   ```

4. **Run the Program**:
   ```bash
   ./console_notepad
   ```

## Usage

- **Text Input**:
  - Type to insert text in the notepad, which is stored in the linked list structure.
  - Use `Backspace` to delete text nodes.

- **Navigation**:
  - Navigate through the text using arrow keys (`Up`, `Down`, `Left`, `Right`), moving the custom cursor over linked list nodes.

- **Save and Load**:
  - Press `Ctrl + S` to save the text in `save.txt`, storing it character-by-character.
  - Press `Ctrl + L` to load text from `save.txt`, rebuilding the linked list structure.

- **Spell Checker**:
  - The spell checker activates each time a space is typed, analyzing the previous word stored in the linked list.
  - Misspelled words display suggested corrections in the lower console section (25% of the screen).

- **Exit**:
  - Press `Esc` to quit, clearing all nodes in the linked list.

## File Structure

- `main.cpp`: Contains the main program loop, handling user input, cursor control, and interactions with the linked list.
- `dependencies.cpp`: Implements the 2D doubly linked list structure for text management and the spell-checking methods.
- `dictionary.txt`: A dictionary file with words for spell-checking (ensure this file is in the directory).
- `saved.txt`: A text file that the application writes to.
- `Makefile`: Used for quick build and run.


## Example Usage

```
$ make
# Console opens with 75% of the screen for text input, 25% for spell-check suggestions.
# Type, use Ctrl+S to save, Ctrl+L to load, and Esc to exit.
```

## Known Issues

- **Performance with Large Text**: The linked list structure may slow down with large amounts of text; potential improvements include more efficient node management.
- **Compatibility**: Some terminals may require additional configuration for optimal display.

## License

This project is open-source and available under the MIT License. See `LICENSE` for details.
