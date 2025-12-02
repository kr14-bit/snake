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


