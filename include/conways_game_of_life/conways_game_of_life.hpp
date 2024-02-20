#pragma once

#include <conways_game_of_life/board.hpp>

namespace cgl = conways_game_of_life;

//! \brief Initialize the window and game.
void init();

//! \brief Callback for drawing in the window.
void display_callback();

//! \brief Callback for reshaping the window.
void reshape_callback(int width, int height);

//! \brief Callback for the main game loop timer.
//! This sets the FPS of the game.
//!
//! \param [in] value Optional arbitrary value that can be used in the timer.
void timer_callback(int value);

//! \brief Callback for special keys.
//! This callback is used to perform actions when the arrow keys are pressed.
//!
//! \param [in] key The special key which was pressed.
//! \param [in] mouseX The X position of the mouse cursor in the window when the key was pressed.
//! \param [in] mouseY The Y position of the mouse cursor in the window when the key was pressed.
void special_keys_callback(int key, int mouseX, int mouseY);

//! \brief Callback for keyboard keys.
//!
//! \param [in] key The ASCII character of the key which was pressed.
//! \param [in] mouseX The X position of the mouse cursor in the window when the key was pressed.
//! \param [in] mouseY The Y position of the mouse cursor in the window when the key was pressed.
void keyboard_callback(unsigned char key, int mouseX, int mouseY);

//! \brief Callback for mouse button presses.
//! This callback is used to perform actions when the mouse buttons are pressed.
//!
//! \param [in] button The mouse button that was pressed.
//! \param [in] state The state of the mouse button (i.e. pressed or released).
//! \param [in] mouseX The X position of the mouse cursor in the window when the button was pressed.
//! \param [in] mouseY The Y position of the mouse cursor in the window when the button was pressed.
void mouse_callback(int button, int state, int mouseX, int mouseY);

//! \brief Draws the game board grid.
void drawGrid();

//! \brief Draws a single unit of the game board grid.
//!
//! \param [in] x The X coordinate of the unit to draw.
//! \param [in] y The Y coordinate of the unit to draw.
void drawUnit(int x, int y);

//! \brief Draws all of the game board cells.
//!
//! \param [in] grid The grid of cells in the game board to draw.
void drawCells(std::shared_ptr<const std::vector<cgl::CellState>> grid);

//! \brief Draws a single game board cell.
//!
//! \param [in] x The X coordinate of the cell to draw.
//! \param [in] y The Y coordinate of the cell to draw.
//! \param [in] state The state of the cell to draw (alive/dead).
void drawCell(int x, int y, cgl::CellState state);