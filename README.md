
# CLI Minesweeper

A command-line implementation of the classic Minesweeper game written in C. This project allows users to enjoy the game directly in their terminal, featuring bomb placement, player actions, and board management.

## Features
- **Randomized Bomb Placement**: Bombs are randomly distributed across the game board with customizable chances.
- **Intuitive Controls**: Simple input-based interaction to flag or reveal cells.
- **Dynamic Gameplay**: The board dynamically updates based on user actions.

## How to Play

1. **Compile the Program**:
   Use `gcc` to compile the source code:
   ```bash
   gcc -o minesweeper main.c
   ```

2. **Run the Game**:
   Execute the compiled program:
   ```bash
   ./minesweeper
   ```

3. **Input Format**:
   - The game accepts input in the following format:
     ```
     <Action> <Column:Hex> <Row:Hex>
     ```
     - **Action**:
       - `C` for clicking (revealing a cell).
       - `F` for flagging or unflagging a cell.
     - **Column**: The column number in hexadecimal (e.g., `0`, `1`, `A`, `F`).
     - **Row**: The row number in hexadecimal (e.g., `0`, `1`, `A`, `F`).

   - **Examples**:
     - `C A 3` → Click the cell in column `A`, row `3`.
     - `F 5 B` → Place or remove a flag on the cell in column `5`, row `B`.

4. **Gameplay Instructions**:
   - Your goal is to flag all the bombs without triggering any.
   - Use the input format above to interact with the board.
   - The game ends when you either flag all bombs correctly or click on a bomb.

## Code Overview
- **Game Board Generation**: The function `generate_table()` initializes the game board with bombs and empty spaces.
- **User Interaction**: The `input` struct and associated logic handle player actions.
- **Recursive Mechanics**: Minesweeper's revealing logic is implemented with stack-based recursion for uncovering adjacent cells.

## Requirements
- **C Compiler**: GCC or any C99-compliant compiler.
- **Terminal Emulator**: Any terminal capable of executing CLI programs.

## Future Enhancements
- Support for different board sizes.
- Enhanced visuals using ASCII art.
- Additional difficulty levels.

## Contributing
Feel free to fork the repository and submit pull requests for improvements or bug fixes.
