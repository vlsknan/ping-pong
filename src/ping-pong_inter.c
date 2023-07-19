#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 80
#define HEIGHT 25
#define LEFT_RACKET_X 10
#define RIGHT_RACKET_X 70

int racket_left_top = HEIGHT / 2 - 1;
int racket_left_middle = HEIGHT / 2;
int racket_left_bottom = HEIGHT / 2 + 1;

int racket_right_top = HEIGHT / 2 - 1;
int racket_right_middle = HEIGHT / 2;
int racket_right_bottom = HEIGHT / 2 + 1;

int ball_x = WIDTH / 2;
int ball_y = HEIGHT / 2;

int score_left = 0;   // левый
int score_right = 0;  // правый

void print_field(int racket_left_top, int racket_left_middle, int racket_left_bottom, int racket_right_top,
                 int racket_right_middle, int racket_right_bottom);
void racket_up(int player);
void racket_down(int player);
void move_ball();
void calculate_score();
int collision_racket(int state);
int collision_wall(int state);

int main() {
    initscr();
    cbreak();
    noecho();
    scrollok(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    clear();
    int state = 2;
    while (1) {
        print_field(racket_left_top, racket_left_middle, racket_left_bottom, racket_right_top,
                    racket_right_middle, racket_right_bottom);
        move_ball(state);
        char menu;
        usleep(60000);
        menu = getch();
        calculate_score();
        if (score_left == 21) {
            printw("The player on the right won with a score: %d: %d\n", score_left, score_right);
            refresh();
            return 0;
        }
        if (score_right == 21) {
            printw("The player on the left won with a score: %d : %d\n", score_left, score_right);
            refresh();
            return 0;
        }

        if (collision_racket(state) != 0) {
            state = collision_racket(state);
        }

        if (collision_wall(state) != 0) {
            state = collision_wall(state);
        }

        switch (menu) {
            case 'a':
                racket_up(0);
                break;
            case 'z':
                racket_down(0);
                break;
            case 'k':
                racket_up(1);
                break;
            case 'm':
                racket_down(1);
                break;
            case 'q':
                return 0;
        }
    }
    return 0;
}

// отрисовка поля, ракеток, мяча и счета
void print_field(int racket_left_top, int racket_left_middle, int racket_left_bottom, int racket_right_top,
                 int racket_right_middle, int racket_right_bottom) {
    printw("Press q to exit\n");
    for (int i = 0; i < HEIGHT; i++) {
        for (int k = 0; k < WIDTH; k++) {
            if (i == 0 || i == HEIGHT - 1) {
                printw("-");
            } else if ((i == racket_left_top || i == racket_left_middle || i == racket_left_bottom) &&
                       (k == LEFT_RACKET_X)) {
                printw("|");
            } else if ((i == racket_right_top || i == racket_right_middle || i == racket_right_bottom) &&
                       (k == RIGHT_RACKET_X)) {
                printw("|");
            } else if (i == ball_y && k == ball_x) {
                printw("o");
            } else if ((i != 0 && i != HEIGHT) && ((k == WIDTH - 1) || k == 0)) {
                printw("|");
            } else {
                printw(" ");
            }
        }
    }
    printw("%d : %d", score_left, score_right);
    printw("\n");
}

void racket_up(int player) {
    if (player == 0) {
        if (racket_left_top != 1) {
            racket_left_top = racket_left_top - 1;
            racket_left_middle = racket_left_middle - 1;
            racket_left_bottom = racket_left_bottom - 1;
        }
    }
    if (player == 1) {
        if (racket_right_top != 1) {
            racket_right_top = racket_right_top - 1;
            racket_right_middle = racket_right_middle - 1;
            racket_right_bottom = racket_right_bottom - 1;
        }
    }
}

void racket_down(int player) {
    if (player == 0) {
        if (racket_left_bottom != HEIGHT - 2) {
            racket_left_top = racket_left_top + 1;
            racket_left_middle = racket_left_middle + 1;
            racket_left_bottom = racket_left_bottom + 1;
        }
    }
    if (player == 1) {
        if (racket_right_bottom != HEIGHT - 2) {
            racket_right_top = racket_right_top + 1;
            racket_right_middle = racket_right_middle + 1;
            racket_right_bottom = racket_right_bottom + 1;
        }
    }
}

void move_ball(int state) {
    switch (state) {
        case 1:
            ball_x++;
            ball_y--;
            break;
        case 2:
            ball_x++;
            break;
        case 3:
            ball_x++;
            ball_y++;
            break;
        case 4:
            ball_x--;
            ball_y--;
            break;
        case 5:
            ball_x--;
            break;
        case 6:
            ball_x--;
            ball_y++;
            break;
    }
}

void calculate_score() {
    if (ball_x == 1) {
        score_right++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
    } else if (ball_x == WIDTH - 1) {
        score_left++;
        ball_x = WIDTH / 2;
        ball_y = HEIGHT / 2;
    }
}

int collision_racket(int state) {
    if ((ball_x == LEFT_RACKET_X + 1 && ball_y == racket_left_top) ||
        (ball_x - 1 == LEFT_RACKET_X && ball_y + 1 == racket_left_top)) {
        if ((state == 5 || state == 2) &&
            ((ball_y == racket_left_top - 1) && (ball_x == LEFT_RACKET_X + 1))) {
            return state;
        }
        return 1;  // rigth + up
    } else if (ball_x == LEFT_RACKET_X + 1 && ball_y == racket_left_middle) {
        return 2;  // right
    } else if ((ball_x == LEFT_RACKET_X + 1 && ball_y == racket_left_bottom) ||
               (ball_x - 1 == LEFT_RACKET_X && ball_y - 1 == racket_left_bottom)) {
        if ((state == 5 || state == 2) &&
            ((ball_y == racket_left_bottom + 1) && (ball_x == LEFT_RACKET_X + 1))) {
            return state;
        }
        return 3;  // bottom + right
    } else if ((ball_x == RIGHT_RACKET_X - 1 && ball_y == racket_right_top) ||
               (ball_x + 1 == RIGHT_RACKET_X && ball_y + 1 == racket_right_top)) {
        if ((state == 5 || state == 2) &&
            ((ball_y == racket_right_top - 1) && (ball_x == RIGHT_RACKET_X - 1))) {
            return state;
        }
        return 4;  // left + up
    } else if (ball_x == RIGHT_RACKET_X - 1 && ball_y == racket_right_middle) {
        return 5;  // left
    } else if ((ball_x == RIGHT_RACKET_X - 1 && ball_y == racket_right_bottom) ||
               (ball_x + 1 == RIGHT_RACKET_X && ball_y - 1 == racket_right_bottom)) {
        if ((state == 5 || state == 2) &&
            ((ball_y == racket_right_bottom + 1) && (ball_x == RIGHT_RACKET_X - 1))) {
            return state;
        }
        return 6;  // left + bottom
    }
    return 0;
}

int collision_wall(int state) {
    // обработка удара об стенку
    if (ball_y == 1 && state == 1) {
        return 3;  // rigth + up
    } else if (ball_y == HEIGHT - 2 && state == 3) {
        return 1;  // right
    } else if (ball_y == 1 && state == 4) {
        return 6;  // bottom + right
    } else if (ball_y == HEIGHT - 2 && state == 6) {
        return 4;  // left + up
    }
    return 0;
}