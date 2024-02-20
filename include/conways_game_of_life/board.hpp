#pragma once
#include <memory>
#include <utility>
#include <vector>

#include <conways_game_of_life/util.hpp>

namespace conways_game_of_life
{
enum CellState
{
  ON,
  OFF
};

template <int C, int R>
class ConwaysGameOfLife
{
public:
  //! \brief Initialize the Conway's Game of Life board.
  ConwaysGameOfLife()
  {
    grid1_ = std::make_shared<std::vector<CellState>>(C*R);
    grid2_ = std::make_shared<std::vector<CellState>>(C*R);

    pause_ = true;
    grid_ = false;

    current_grid_ = grid_ ? grid1_ : grid2_;
    next_grid_ = grid_ ? grid2_ : grid1_;

    // Initialize all cells off.
    clear();
  }

  //! \brief Get the size of the game board.
  //! \return std::pair<int, int> The number of columns and rows in the board.
  std::pair<int,int> getSize() const { return size_; }

  //! \brief Get the game grid.
  //! \return CellState* The 1D array representing the cells.
  std::shared_ptr<const std::vector<CellState>> getGrid() { return current_grid_; }

  //! \brief Toggle the paused state of the game board.
  //! Toggle updates to the game board. When paused, no new generations will be created.
  void togglePause() { pause_ = !pause_; }

  void setCell(int x, int y) { int index = convert_indices(x, y, C); (*current_grid_)[index] = CellState::ON; }
  void unsetCell(int x, int y) { int index = convert_indices(x, y, C); (*current_grid_)[index] = CellState::OFF; }
  void clear()
  {
    // Set all cells off.
    for (int i = 0; i < C*R; i++)
    {
        (*grid1_)[i] = CellState::OFF;
        (*grid2_)[i] = CellState::OFF;
    }
  }

  //! \brief Update loop of the game.
  //! Update the game grid with the next generation of cells.
  void update()
  {
    if (pause_) { return; }
    // Loop through all cells of the grid
    for (int x = 0; x < C; x++)
    {
      for (int y = 0; y < R; y++)
      {
        int index = convert_indices(x, y, C);

        // Count the number of neighbors that are alive
        int alive = countAliveNeighbors(x, y);

        (*next_grid_)[index] = (*current_grid_)[index];
        if ((*current_grid_)[index] == CellState::OFF)
        {
          // Birth rule
          if (alive == 3) { (*next_grid_)[index] = CellState::ON; }
        }
        else if ((*current_grid_)[index] == CellState::ON)
        {
          // Death rule
          if (alive <= 1 || alive >= 4) { (*next_grid_)[index] = CellState::OFF; }
        }
      }
    }

    // Swap which grid we're modifying
    grid_ = !grid_;
    current_grid_ = grid_ ? grid1_ : grid2_;
    next_grid_ = grid_ ? grid2_ : grid1_;
  }

private:
  //! \brief The size of the grid
  std::pair<int, int> size_ = std::make_pair(C, R);

  //! \brief A grid to store the state of all the cells in the game.
  std::shared_ptr<std::vector<CellState>> grid1_;
  //! \brief A grid to store the state of all the cells in the game.
  std::shared_ptr<std::vector<CellState>> grid2_;

  //! \brief Which grid to use as the current grid, toggles on each update.
  bool grid_;
  //! \brief A pointer to the current grid.
  std::shared_ptr<std::vector<CellState>> current_grid_;
  //! \brief A pointer to the grid used for the next generation.
  std::shared_ptr<std::vector<CellState>> next_grid_;

  //! \brief Pause board updates.
  bool pause_;

  //! \brief Count the number of alive cells adjacent to the current cell.
  //!
  //! \param [in] x The X coordinate of the cell to count the neighbors of.
  //! \param [in] y The Y coordinate of the cell to count the neighbors of.
  //! \return int The number of neighbors adjacent to the current cell.
  int countAliveNeighbors(int x, int y)
  {
    // Initialize counter
    int alive = 0;

    // Loop over X offsets in the range [-1, 1]
    for (int x_off = -1; x_off <= 1; x_off++)
    {
      // Get the x coordinate of the neighbor cell using the x offset
      int neighbor_x = x + x_off;
      // Wrap the value if it is beyond the size of the grid
      if (x + x_off < 0) { neighbor_x = C - 1; }
      if (x + x_off >= C) { neighbor_x = 0; }

      // Loop over Y offsets in the range [-1, 1]
      for (int y_off = -1; y_off <= 1; y_off++)
      {
        // Get the y coordinate of the neighbor cell using the y offset
        int neighbor_y = y + y_off;
        // Wrap the value if it is beyond the size of the grid
        if (y + y_off < 0) { neighbor_y = R - 1; }
        if (y + y_off >= R) { neighbor_y = 0; }

        // Skip the cell if the offsets are both 0, this is the current cell
        if (x_off == 0 && y_off == 0) { continue; }

        // Convert (x, y) coords to 1D array index
        int neighbor_index = convert_indices(neighbor_x, neighbor_y, C);

        // Increment counter if neighbor cell is alive
        if ((*current_grid_)[neighbor_index] == CellState::ON) { alive++; }
      }
    }

    return alive;
  }

};
} // namespace conways_game_of_life