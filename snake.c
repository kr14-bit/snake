#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define SNAKE_CHAR 'O'  // Snake body
#define FOOD_CHAR '*'   // Snake Food
#define BORDER_CHAR '#' // Border walls
#define MAX_LENGTH 100  // Max length of snake

void play_game(int *final_score, iint *won);

int main() {
    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int final_score, won;

    while (1) {
      play_game(&final_score, &won);

      // This displays the 'game over' or 'you win' screen
      clear();
      int max_x, max_y;
      getmaxyx(stdscr, max_y, max_x);

      char msg_game_over[20];
      if (won) {
          strcpy(msg_game_over, "YOU WIN!");
      } else {
          strcpy(msg_game_over, "GAME OVER!");
      }

      char msg_score[50];
      char msg_exit[] = "Press CTRL-C to Exit";
      char msg_play_again[] = "Press SPACE to Play Again";

      snprintf(msg_score, sizeof(msg_score), "SCORE: %d", final_score);

      mvprintw(max_y / 2 - 2, (max_x - strlen(msg_game_over)) / 2, "%s", msg_game_over);
      mvprintw(max_y / 2 - 1, (max_x - strlen(msg_score)) / 2, "%s", msg_score);
      mvprintw(max_y / 2, (max_x - strlen(msg_play_again)) / 2, "%s", msg_play_again);
      mvprintw(max_y - max_y / 4, (max_x - strlen(msg_exit)) / 2, "%s", msg_exit);

      refresh()

      // Wait for SPACE or CTRL-C
      int ch;
      while ((ch = getch()) != ' ') {
        if (ch == ERR) continue; // No input, keep waiting
      }
  }

  endwin();
  return 0;
}

void play_game(int *final_score, int *won) {
    int max_x, max_y, x, y, food_x, food_y, snake_len = 3;
    int dx = 1, dy = 0;
    int snake_x[MAX_LENGTH], snake_y[MAX_LENGTH];
    int ch, game_over = 0;

    timeout(100);
    getmaxyx(stdscr, max_y, max_x);
    
    // Ensure minimum pit size of 20x20 (22x22 with borders)
    if (max_x < 22 || max_y < 22) {
        clear();
        mvprintw(0, 0, "Terminal too small! Need at least 22x22");
        refresh();
        getch();
        *final_score = 0;
        *won = 0;
        return;
    }
    
    // Calculate win condition: half the perimeter of the border
    int win_length = (max_x + max_y - 4) / 2;

    // Snake with 3 segments (head + 2 body parts)
    x = max_x / 2;
    y = max_y / 2;
    snake_x[0] = x;
    snake_y[0] = y;
    snake_x[1] = x - 1;
    snake_y[1] = y;
    snake_x[2] = x - 2;
    snake_y[2] = y;

    srand(time(NULL));
    do {
        food_x = rand() % (max_x - 2) + 1;
        food_y = rand() % (max_y - 2) + 1;
    } while ((food_x == x && food_y == y) || 
             (food_x == x - 1 && food_y == y) || 
             (food_x == x - 2 && food_y == y));

    *won = 0;

    while (!game_over) {
        clear();

