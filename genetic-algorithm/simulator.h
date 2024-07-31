#ifndef __SIMULATOR_H
#define __SIMULATOR_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>

/*
The following file implements a simulator for the game 2048
It takes a vector of integers as a parameter, which are 
supposed to represent the moves done by the player, and returns
the final state of the board. The integers of the vector mean the
following moves:

0 - up
1 - right
2 - down
3 - left

The board is represented as a 64-bit integer. Each 4 consecutive bits
are making up one tile of the board. One important difference between
the original game 2048 and this simulator is the fact that this simulator
does not use the numbers 2, 4, 8, 16 etc., but rather the numbers 1, 2, 3 etc.
This makes it possible to represent all powers of two up to 2^15 = 32768 using
4 bits only. Unlike in the original version of the game, where two tiles with
the values "2" merged together result into 4, in this simulator, two tiles 
with the same value merged together result into a tile with a value of 1 greater
than the original value (2 + 2 = 3).

There are two versions of the game:

Version 1: In this version of the game, only tiles with the value 2 pop up 
in random positions of the board after each move of the player.

Version 2: In this version, tiles pop up in tandom positions of the board.
Its value is 2 with a probability of 90%, and 4 with a probability of 10%.
*/

void print_board(const uint64_t board) {
  for (int i=0;i<4;i++) {
    for (int j=0;j<4;j++) {
      std::cout << ((board >> (16 * i + 4 * j)) & 0xF) << "\t";
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}

uint64_t move_up(uint64_t actual_state) {
  for (int j=0;j<4;j++) {
    // pointer to the last tile that was not merged
    int last = 0;
    
    for (int i=1;i<4;i++) {
      uint64_t value_at_pos = ((actual_state >> (16 * i + 4 * j)) & 0xFUL);
      uint64_t value_at_last = ((actual_state >> (16 * last + 4 * j)) & 0xFUL);

      if (value_at_pos) {
        
        // set the value at the current position to zero
        actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * j));

        if (value_at_last) {
          if (value_at_last == value_at_pos) {
            // increase the value at last by one
            actual_state = actual_state + (1UL << (16 * last + 4 * j));
            last++;
          }

          else {
            // new pointer to an unmerged tile is found
            last++;

            // set the value at position "last" to the value at the current position
            actual_state = actual_state ^ (value_at_pos << (16 * last + 4 * j));
          }
        }

        else {
          // set the value at position "last" to the value at the current position
          actual_state = actual_state ^ (value_at_pos << (16 * last + 4 * j));
        }
      }
    }
  }

  // std::cout << "moved up\n";
  // print_board(actual_state);

  return actual_state;
}

uint64_t move_down (uint64_t actual_state) {
  for (int j=0;j<4;j++) {
    // pointer to the last tile that was not merged
    int last = 3;
    
    for (int i=2;i>=0;i--) {
      uint64_t value_at_pos = ((actual_state >> (16 * i + 4 * j)) & 0xFUL);
      uint64_t value_at_last = ((actual_state >> (16 * last + 4 * j)) & 0xFUL);

      if (value_at_pos) {

        // set the value at the current position to zero
        actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * j));

        if (value_at_last) {
          if (value_at_last == value_at_pos) {
            // increase the value at last by one
            actual_state = actual_state + (1UL << (16 * last + 4 * j));
            last--;
          }

          else {
            // new pointer to an unmerged tile is found
            last--;

            // set the value at position "last" to the value at the current position
            actual_state = actual_state ^ (value_at_pos << (16 * last + 4 * j));
          }
        }

        else {
          // set the value at position "last" to the value at the current position
          actual_state = actual_state ^ (value_at_pos << (16 * last + 4 * j));
        }
      }
    }
  }

  // std::cout << "moved down\n";
  // print_board(actual_state);

  return actual_state;
}

uint64_t move_right (uint64_t actual_state) {
  for (int i=0;i<4;i++) {
    // pointer to the last tile that was not merged
    int last = 3;
    
    for (int j=2;j>=0;j--) {
      uint64_t value_at_pos = ((actual_state >> (16 * i + 4 * j)) & 0xFUL);
      uint64_t value_at_last = ((actual_state >> (16 * i + 4 * last)) & 0xFUL);

      if (value_at_pos) {

        // set the value at the current position to zero
        actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * j));

        if (value_at_last) {
          if (value_at_last == value_at_pos) {
            // increase the value at last by one
            actual_state = actual_state + (1UL << (16 * i + 4 * last));
            last--;
          }

          else {
            // new pointer to an unmerged tile is found
            last--;

            // set the value at position "last" to the value at the current position
            actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * last));
          }
        }

        else {
          // set the value at position "last" to the value at the current position
          actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * last));
        }
      }
    }
  }

  // std::cout << "moved right\n";
  // print_board(actual_state);

  return actual_state;
}


uint64_t move_left (uint64_t actual_state) {
  for (int i=0;i<4;i++) {
    // pointer to the last tile that was not merged
    int last = 0;
    
    for (int j=1;j<4;j++) {
      uint64_t value_at_pos = ((actual_state >> (16 * i + 4 * j)) & 0xFUL);
      uint64_t value_at_last = ((actual_state >> (16 * i + 4 * last)) & 0xFUL);

      if (value_at_pos) {

        // set the value at the current position to zero
        actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * j));

        if (value_at_last) {
          if (value_at_last == value_at_pos) {
            // increase the value at last by one
            actual_state = actual_state + (1UL << (16 * i + 4 * last));
            last++;
          }

          else {
            // new pointer to an unmerged tile is found
            last++;

            // set the value at position "last" to the value at the current position
            actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * last));
          }
        }

        else {
          // set the value at position "last" to the value at the current position
          actual_state = actual_state ^ (value_at_pos << (16 * i + 4 * last));
        }
      }
    }
  }

  // std::cout << "moved left\n";
  // print_board(actual_state);

  return actual_state;
}

uint64_t add_tile_at_rand_position (uint64_t actual_state, uint64_t value_of_tile, std::mt19937 &rng) {
  // first count the number of empty slots
  int counter = 0;
  for (int i=0;i<16;i++) {
    if (((actual_state >> (4*i)) & 0xF) == 0) counter++;
  }

  // if there are no empty slots then it's not possible to add a new tile, so just exit
  if (counter == 0) return actual_state;

  // now add a new tile to a random position
  int position_to_be_added = std::uniform_int_distribution<int>(1, counter)(rng);
  counter = 0;
  
  for (int i=0;i<16;i++) {
    if (((actual_state >> (4*i)) & 0xF) == 0) counter++;

    if (counter == position_to_be_added) {
      return actual_state ^ (value_of_tile << (4*i));
    }
  }

  // never gets reached
  return 0;
}

uint64_t set_up_board(std::mt19937 &rng) {
  // set up a new board such that there are two tiles with
  // the value "2" at two random positions (not necessarily distinct)

  int pos1 = std::uniform_int_distribution<int>(0, 15)(rng);
  int pos2 = std::uniform_int_distribution<int>(0, 15)(rng);

  return ((1UL << (4*pos1)) | (1UL << (4*pos2)));
}

uint64_t simulator_version_1 (std::vector<int> &sequence_of_moves) {
  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  uint64_t new_board = set_up_board(rng);
  uint64_t prev;

  // std::cout << "initial state of the board\n";
  // print_board(new_board);

  for (int &x : sequence_of_moves) {
    prev = new_board;

    if (x == 0) new_board = move_up(new_board);
    else if (x == 1) new_board = move_right(new_board);
    else if (x == 2) new_board = move_down(new_board);
    else new_board = move_left(new_board);

    // if the move didn't change the state of the board then don't add a tile
    if (new_board != prev) {
      new_board = add_tile_at_rand_position(new_board, 1, rng);

      // std::cout << "added tile\n";
      // print_board(new_board);
    }
  }

  return new_board;
}

uint64_t simulator_version_2 (std::vector<int> &sequence_of_moves) {
  std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
  uint64_t new_board = set_up_board(rng);
  uint64_t prev;

  for (int &x : sequence_of_moves) {
    prev = new_board;

    if (x == 0) new_board = move_up(new_board);
    else if (x == 1) new_board = move_right(new_board);
    else if (x == 2) new_board = move_down(new_board);
    else new_board = move_left(new_board);

    // if the move didn't change the state of the board then don't add a tile
    if (new_board != prev) {
      if (std::uniform_int_distribution<int>(0, 9)(rng) == 0) {
        new_board = add_tile_at_rand_position(new_board, 2, rng);
      }

      else {
        new_board = add_tile_at_rand_position(new_board, 1, rng);
      }
    }
  }

  return new_board;
}

// signed main() {
//   std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
//   std::vector<int> moves;
//   for (int i=0;i<20;i++) {
//     moves.push_back(std::uniform_int_distribution<int>(0, 3)(rng));
//   }

//   simulator_version_1(moves);
// }

#endif