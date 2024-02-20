#include <cmath>
#include <iostream>
#include <utility>

#include <GL/gl.h>
#include <GL/glut.h>

#include <conways_game_of_life/board.hpp>
#include <conways_game_of_life/conways_game_of_life.hpp>
#include <conways_game_of_life/parameters.hpp>

namespace cgl = conways_game_of_life;

int fps = DEFAULT_FPS;
cgl::ConwaysGameOfLife<COLUMNS, ROWS> game;

int main(int argc, char *argv[])
{
  // Window initialization
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  glutCreateWindow("Conway's Game of Life");

  // Register callbacks
  glutDisplayFunc(display_callback);
  glutReshapeFunc(reshape_callback);
  glutTimerFunc(0, timer_callback, 0);
  glutSpecialFunc(special_keys_callback);
  glutKeyboardFunc(keyboard_callback);
  glutMouseFunc(mouse_callback);

  // Initialize game
  init();

  // Run the main loop
  glutMainLoop();

  return 0;
}

void init()
{
  glClearColor(cgl::bg_red, cgl::bg_green, cgl::bg_blue, 1.0f);
}

void display_callback()
{
  glClear(GL_COLOR_BUFFER_BIT);

  // Draw the game board
  drawCells(game.getGrid());

  // Draw the grid of cells
  drawGrid();

  glutSwapBuffers();
}

void reshape_callback(int width, int height)
{
  glViewport(0, 0, (GLsizei)width, (GLsizei)height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, COLUMNS, 0.0, ROWS, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void timer_callback(int value)
{
  // Re-run the display callback
  glutPostRedisplay();

  // Re-initialize this timer according to the fps
  glutTimerFunc(1000 / fps, timer_callback, value);

  // Update the game board
  game.update();
}

void special_keys_callback(int key, int mouseX, int mouseY)
{
  switch (key)
  {
    // Up arrow
    case GLUT_KEY_UP:
      // Increment the FPS
      fps += FPS_STEP;
      if (fps >= MAX_FPS) { fps = MAX_FPS; }
      break;

    // Down arrow
    case GLUT_KEY_DOWN:
      // Decrement the FPS
      fps -= FPS_STEP;
      if (fps <= MIN_FPS) { fps = MIN_FPS; }
      break;

    default:
      break;
  }
}

void keyboard_callback(unsigned char key, int mouseX, int mouseY)
{
  switch (key)
  {
    // Spacebar
    case ' ':
      // Pause animation
      game.togglePause();
      break;
    // C key
    case 'c':
      // Clear the board
      game.clear();
      break;

    default:
      break;
  }
}

//! \brief Convert the coordinates of the mouse in the window to the coordinates of the cell in the grid.
//!
//! \param [in] mouseX The X coordinate of the mouse cursor in the window.
//! \param [in] mouseY The Y coordinate of the mouse cursor in the window.
//! \return std::pair<int, int> The X and Y coordinates of the grid cell that the mouse coordinates fall into.
std::pair<int, int> convert_mouse_coords(int mouseX, int mouseY)
{
  int x, y;
  x = std::floor(mouseX / CELL_SIZE_PX);
  y = std::floor((WINDOW_HEIGHT - mouseY) / CELL_SIZE_PX);
  return std::make_pair(x, y);
}

void mouse_callback(int button, int state, int mouseX, int mouseY)
{
  switch (button)
  {
    // Left mouse button
    case GLUT_LEFT_BUTTON:
      if (state == GLUT_DOWN)
      {
        std::pair<int, int> coords = convert_mouse_coords(mouseX, mouseY);

        // Set the clicked cell to alive
        game.setCell(coords.first, coords.second);
      }
      break;

    // Right mouse button
    case GLUT_RIGHT_BUTTON:
      if (state == GLUT_DOWN)
      {
        std::pair<int, int> coords = convert_mouse_coords(mouseX, mouseY);

        // Set the clicked cell to dead
        game.unsetCell(coords.first, coords.second);
      }
      break;

    default:
      break;
  }
}

void drawGrid()
{
  for(int x = 0; x < COLUMNS; x++)
  {
    for(int y = 0; y < ROWS; y++)
    {
      drawUnit(x, y);
    }
  }
}

void drawUnit(int x, int y)
{
  glLineWidth(1.0f);
  glColor3f(cgl::line_red, cgl::line_green, cgl::line_blue);

  glBegin(GL_LINE_LOOP);

  glVertex2f(x, y);
  glVertex2f(x + 1, y);
  glVertex2f(x + 1, y + 1);
  glVertex2f(x, y + 1);

  glEnd();
}

void drawCells(std::shared_ptr<const std::vector<cgl::CellState>> grid)
{
  std::pair grid_size = game.getSize();
  for (int x = 0; x < grid_size.first; x++)
  {
    for (int y = 0; y < grid_size.second; y++)
    {
      int index = y * grid_size.first + x;
      drawCell(x, y, (*grid)[index]);
    }
  }
}

void drawCell(int x, int y, cgl::CellState state)
{
  // Set the drawing color based on the state of the cell
  if (state == cgl::CellState::ON)
  {
    glColor3f(cgl::cell_on_red, cgl::cell_on_green, cgl::cell_on_blue);
  }
  else if (state == cgl::CellState::OFF)
  {
    glColor3f(cgl::cell_off_red, cgl::cell_off_green, cgl::cell_off_blue);
  }

  // Draw a rectangle at the position of the cell
  glRectd(x, y, x+1, y+1);
}