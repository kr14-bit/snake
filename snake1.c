#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#define SNAKE_CHAR 'O'      // Snake body
#define FOOD_CHAR  '*'      // Snake food
#define BORDER_CHAR '#'     // Border walls
#define MAX_LENGTH 200      // Max length of snake

// Size of the snake pit (must be at least 20x20)
#define PIT_WIDTH  40
#define PIT_HEIGHT 20

void play_game(int *final_score, int *won);

// -----------------------------------------------------
// main: handles game loop and "play again" screen
// -----------------------------------------------------
int main(void) {
    // Seed RNG once for the whole program
    srand((unsigned int)time(NULL));

    initscr();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);

    int final_score, won;

    while (1) {
        play_game(&final_score, &won);

        // Show game over / win screen
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

        mvprintw(max_y / 2 - 2, (max_x - (int)strlen(msg_game_over)) / 2, "%s", msg_game_over);
        mvprintw(max_y / 2 - 1, (max_x - (int)strlen(msg_score)) / 2, "%s", msg_score);
        mvprintw(max_y / 2,     (max_x - (int)strlen(msg_play_again)) / 2, "%s", msg_play_again);
        mvprintw(max_y - max_y / 4, (max_x - (int)strlen(msg_exit)) / 2, "%s", msg_exit);

        refresh();

        // Wait for SPACE (play again). CTRL-C will just kill program.
        int ch;
        while ((ch = getch()) != ' ') {
            // block until they press space
        }
    }

    // Technically unreachable, but good style:
    endwin();
    return 0;
}

// -----------------------------------------------------
// Helper: place food at a random empty spot
// -----------------------------------------------------
static void place_food(int *food_x, int *food_y,
                       int left, int top, int pit_width, int pit_height,
                       int snake_x[], int snake_y[], int length) {
    int valid = 0;
    while (!valid) {
        *food_x = left + 1 + rand() % (pit_width  - 2);
        *food_y = top  + 1 + rand() % (pit_height - 2);
        valid = 1;
        for (int i = 0; i < length; i++) {
            if (snake_x[i] == *food_x && snake_y[i] == *food_y) {
                valid = 0;
                break;
            }
        }
    }
}

// -----------------------------------------------------
// play_game: runs one full snake game
// -----------------------------------------------------
void play_game(int *final_score, int *won) {
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // Ensure pit fits into terminal; shrink if needed
    int pit_width  = PIT_WIDTH;
    int pit_height = PIT_HEIGHT;

    if (pit_width > max_x - 2)  pit_width = max_x - 2;
    if (pit_height > max_y - 2) pit_height = max_y - 2;

    // Top-left corner of pit (so it’s roughly centered)
    int left   = (max_x - pit_width) / 2;
    int top    = (max_y - pit_height) / 2;
    int right  = left + pit_width - 1;
    int bottom = top  + pit_height - 1;

    // Half of the perimeter of the border
    // Perimeter cells = 2*pit_width + 2*(pit_height - 2)
    // Half of that = pit_width + pit_height - 2
    int target_length = pit_width + pit_height - 2;

    // Snake state
    int snake_x[MAX_LENGTH];
    int snake_y[MAX_LENGTH];
    int length = 3;  // initial length

    // Start roughly in the middle, moving to the right
    int start_x = left + pit_width / 2;
    int start_y = top  + pit_height / 2;
    for (int i = 0; i < length; i++) {
        snake_x[i] = start_x - i;
        snake_y[i] = start_y;
    }

    int dir_x = 1;  // moving right initially
    int dir_y = 0;

    // Food
    int food_x, food_y;
    place_food(&food_x, &food_y, left, top, pit_width, pit_height,
               snake_x, snake_y, length);

    // Game state flags
    *won = 0;
    *final_score = 0;
    int game_over = 0;

    // Make input non-blocking so snake keeps moving
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    while (!game_over) {
        clear();

        // Draw border
        for (int x = left; x <= right; x++) {
            mvaddch(top,    x, BORDER_CHAR);
            mvaddch(bottom, x, BORDER_CHAR);
        }
        for (int y = top + 1; y < bottom; y++) {
            mvaddch(y, left,  BORDER_CHAR);
            mvaddch(y, right, BORDER_CHAR);
        }

        // Draw food
        mvaddch(food_y, food_x, FOOD_CHAR);

        // Draw snake
        for (int i = 0; i < length; i++) {
            mvaddch(snake_y[i], snake_x[i], SNAKE_CHAR);
        }

        // Show length / win target
        mvprintw(top - 1, left, "Length: %d / %d", length, target_length);

        refresh();

        // Snake speed (adjust this to make it faster/slower)
        usleep(120000);  // 0.12 seconds

        // Read input (non-blocking)
        int ch = getch();
        switch (ch) {
            case KEY_UP:
                if (dir_y != 1) { dir_x = 0; dir_y = -1; }
                break;
            case KEY_DOWN:
                if (dir_y != -1) { dir_x = 0; dir_y = 1; }
                break;
            case KEY_LEFT:
                if (dir_x != 1) { dir_x = -1; dir_y = 0; }
                break;
            case KEY_RIGHT:
                if (dir_x != -1) { dir_x = 1; dir_y = 0; }
                break;
            default:
                break;
        }

        // New head position
        int new_head_x = snake_x[0] + dir_x;
        int new_head_y = snake_y[0] + dir_y;

        // Check wall collision
        if (new_head_x <= left || new_head_x >= right ||
            new_head_y <= top  || new_head_y >= bottom) {
            game_over = 1;
            *won = 0;
            break;
        }

        // Check self collision
        for (int i = 0; i < length; i++) {
            if (snake_x[i] == new_head_x && snake_y[i] == new_head_y) {
                game_over = 1;
                *won = 0;
                break;
            }
        }
        if (game_over) break;

        // Move body: from tail to head
        for (int i = length - 1; i > 0; i--) {
            snake_x[i] = snake_x[i - 1];
            snake_y[i] = snake_y[i - 1];
        }

        // Place new head
        snake_x[0] = new_head_x;
        snake_y[0] = new_head_y;

        // Check if snake ate food
        if (new_head_x == food_x && new_head_y == food_y) {
            if (length < MAX_LENGTH) {
                // Grow by one: duplicate last segment
                snake_x[length] = snake_x[length - 1];
                snake_y[length] = snake_y[length - 1];
                length++;
            }

            // Place new food
            place_food(&food_x, &food_y, left, top, pit_width, pit_height,
                       snake_x, snake_y, length);
        }

        // Win condition
        if (length >= target_length) {
            *won = 1;
            game_over = 1;
        }
    }

    // Score = how many food pieces eaten (length - initial length)
    *final_score = length - 3;

    // Put input back to blocking for the "game over" screen in main
    nodelay(stdscr, FALSE);
}
