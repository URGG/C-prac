#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_ROWS 100
#define MAX_COLS 100
#define MAX_LINE 256
#define EMPTY '-'
#define IMPASSIBLE 'X'
#define FOX 'F'
#define SQUIRREL 's'
#define FOX_BREED_TIME 8
#define SQUIRREL_BREED_TIME 4
#define FOX_STARVE_TIME 10

typedef struct {
    char type;
    int breed_count;
    int starve_count;
    int moved_this_turn;
} Cell;

typedef struct {
    Cell grid[MAX_ROWS][MAX_COLS];
    int rows;
    int cols;
    int step;
} Board;

typedef struct {
    int dr;
    int dc;
} Direction;

static const Direction DIRECTIONS[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

int load_board(const char *filename, Board *board);
void init_board(Board *board);
void copy_board(const Board *src, Board *dest);
void print_board(const Board *board);
void print_board_with_stats(const Board *board);
int validate_board_file(const char *filename);
int is_valid_cell(char c);
int is_valid_position(const Board *board, int row, int col);
void simulate_step(Board *board);
void clear_moved_flags(Board *board);
void process_starvation(Board *board);
void move_foxes(Board *board);
void move_squirrels(Board *board);
void move_animal(Board *board, int r, int c);
int try_fox_eat(Board *board, int r, int c);
int try_move_to_empty(Board *board, int r, int c, char animal_type);
void breed_animals(Board *board);
int try_breed_animal(Board *board, int r, int c);
void place_offspring(Board *board, int r, int c, char parent_type);
int count_animals(const Board *board, char type);
int count_adjacent_type(const Board *board, int r, int c, char type);
void get_adjacent_cells(const Board *board, int r, int c, char type, int *positions, int *count);
int get_empty_adjacent_cells(const Board *board, int r, int c, int *positions);
int get_random_direction(const int *valid_dirs, int count);
void clear_cell(Cell *cell);
void copy_cell(const Cell *src, Cell *dest);
void increment_breed_count(Cell *cell);
void increment_starve_count(Cell *cell);
void reset_after_breeding(Cell *cell);
void reset_after_eating(Cell *cell);
void print_statistics(const Board *board);
void print_header(const char *title);
void print_separator(void);
void print_usage(const char *program_name);
int parse_arguments(int argc, char *argv[], char **filename, int *max_steps, int *delay_ms);

int main(int argc, char *argv[]) {
    Board board;
    char *filename;
    int max_steps = 50;
    int delay_ms = 200;

    srand(time(NULL));

    if (!parse_arguments(argc, argv, &filename, &max_steps, &delay_ms)) {
        return 1;
    }

    if (!load_board(filename, &board)) {
        return 1;
    }

    print_header("INITIAL BOARD");
    print_board_with_stats(&board);
    print_separator();

    for (int step = 1; step <= max_steps; step++) {
        int foxes = count_animals(&board, FOX);
        int squirrels = count_animals(&board, SQUIRREL);

        if (foxes == 0 && squirrels == 0) {
            printf("\n>>> All animals died. Simulation ended at step %d. <<<\n", step - 1);
            break;
        }

        if (foxes == 0) {
            printf("\n>>> All foxes died. Only squirrels remain. <<<\n");
            break;
        }

        if (squirrels == 0) {
            printf("\n>>> All squirrels died. Foxes will starve soon. <<<\n");
        }

        usleep(delay_ms * 1000);
        simulate_step(&board);


        char title[64];
        snprintf(title, sizeof(title), "STEP %d", step);
        print_header(title);
        print_board_with_stats(&board);
        print_separator();
    }

    printf("\nSimulation complete.\n");
    return 0;
}

void init_board(Board *board) {
    board->rows = 0;
    board->cols = 0;
    board->step = 0;

    for (int r = 0; r < MAX_ROWS; r++) {
        for (int c = 0; c < MAX_COLS; c++) {
            clear_cell(&board->grid[r][c]);
        }
    }
}

int load_board(const char *filename, Board *board) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return 0;
    }

    init_board(board);
    char line[MAX_LINE];

    while (fgets(line, MAX_LINE, fp) && board->rows < MAX_ROWS) {
        int len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }

        if (len == 0) continue;

        if (board->cols == 0) {
            board->cols = len;
        } else if (len != board->cols) {
            fprintf(stderr, "Error: Inconsistent row length at line %d\n", board->rows + 1);
            fprintf(stderr, "  Expected %d columns, got %d\n", board->cols, len);
            fclose(fp);
            return 0;
        }

        for (int c = 0; c < len; c++) {
            char ch = line[c];
            if (!is_valid_cell(ch)) {
                fprintf(stderr, "Error: Invalid character '%c' at row %d, col %d\n",
                        ch, board->rows + 1, c + 1);
                fprintf(stderr, "  Valid characters: F (fox), s (squirrel), - (empty), X (impassible)\n");
                fclose(fp);
                return 0;
            }

            board->grid[board->rows][c].type = ch;
            board->grid[board->rows][c].breed_count = 0;
            board->grid[board->rows][c].starve_count = 0;
            board->grid[board->rows][c].moved_this_turn = 0;
        }

        board->rows++;
    }

    fclose(fp);

    if (board->rows == 0) {
        fprintf(stderr, "Error: Empty board file\n");
        return 0;
    }

    printf("Successfully loaded board: %d rows x %d cols\n", board->rows, board->cols);
    return 1;
}

void copy_board(const Board *src, Board *dest) {
    dest->rows = src->rows;
    dest->cols = src->cols;
    dest->step = src->step;

    for (int r = 0; r < src->rows; r++) {
        for (int c = 0; c < src->cols; c++) {
            copy_cell(&src->grid[r][c], &dest->grid[r][c]);
        }
    }
}

void print_board(const Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            putchar(board->grid[r][c].type);
        }
        putchar('\n');
    }
}

void print_board_with_stats(const Board *board) {
    print_board(board);
    printf("\n");
    print_statistics(board);
}

int is_valid_cell(char c) {
    return (c == EMPTY || c == IMPASSIBLE || c == FOX || c == SQUIRREL);
}

int is_valid_position(const Board *board, int row, int col) {
    return (row >= 0 && row < board->rows && col >= 0 && col < board->cols);
}

int validate_board_file(const char *filename) {
    Board temp;
    return load_board(filename, &temp);
}

void simulate_step(Board *board) {
    clear_moved_flags(board);
    move_foxes(board);
    move_squirrels(board);
    process_starvation(board);
    breed_animals(board);
    board->step++;
}

void clear_moved_flags(Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            board->grid[r][c].moved_this_turn = 0;
        }
    }
}

void process_starvation(Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            Cell *cell = &board->grid[r][c];
            if (cell->type == FOX && cell->starve_count >= FOX_STARVE_TIME) {
                clear_cell(cell);
                cell->type = EMPTY;
            }
        }
    }
}

void move_foxes(Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            Cell *cell = &board->grid[r][c];

            if (cell->type != FOX || cell->moved_this_turn) {
                continue;
            }

            if (try_fox_eat(board, r, c)) {
                continue;
            }

            if (try_move_to_empty(board, r, c, FOX)) {
                increment_starve_count(&board->grid[r][c]);
            } else {
                increment_starve_count(cell);
                increment_breed_count(cell);
                cell->moved_this_turn = 1;
            }
        }
    }
}

void move_squirrels(Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            Cell *cell = &board->grid[r][c];

            if (cell->type != SQUIRREL || cell->moved_this_turn) {
                continue;
            }

            if (!try_move_to_empty(board, r, c, SQUIRREL)) {
                increment_breed_count(cell);
                cell->moved_this_turn = 1;
            }
        }
    }
}

int try_fox_eat(Board *board, int r, int c) {
    int squirrel_positions[8];
    int num_squirrels = 0;

    get_adjacent_cells(board, r, c, SQUIRREL, squirrel_positions, &num_squirrels);

    if (num_squirrels == 0) {
        return 0;
    }

    int choice = get_random_direction(squirrel_positions, num_squirrels);
    int dir_idx = squirrel_positions[choice];
    int nr = r + DIRECTIONS[dir_idx].dr;
    int nc = c + DIRECTIONS[dir_idx].dc;

    Cell *fox = &board->grid[r][c];
    Cell *target = &board->grid[nr][nc];

    copy_cell(fox, target);
    target->moved_this_turn = 1;
    reset_after_eating(target);
    increment_breed_count(target);

    clear_cell(fox);
    fox->type = EMPTY;

    return 1;
}

int try_move_to_empty(Board *board, int r, int c, char animal_type) {
    int empty_positions[8];
    int num_empty = get_empty_adjacent_cells(board, r, c, empty_positions);

    if (num_empty == 0) {
        return 0;
    }

    int choice = get_random_direction(empty_positions, num_empty);
    int dir_idx = empty_positions[choice];
    int nr = r + DIRECTIONS[dir_idx].dr;
    int nc = c + DIRECTIONS[dir_idx].dc;

    Cell *animal = &board->grid[r][c];
    Cell *target = &board->grid[nr][nc];

    copy_cell(animal, target);
    target->moved_this_turn = 1;
    increment_breed_count(target);

    clear_cell(animal);
    animal->type = EMPTY;

    return 1;
}

void breed_animals(Board *board) {
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            Cell *cell = &board->grid[r][c];

            if (cell->type != FOX && cell->type != SQUIRREL) {
                continue;
            }

            try_breed_animal(board, r, c);
        }
    }
}

int try_breed_animal(Board *board, int r, int c) {
    Cell *cell = &board->grid[r][c];
    int breed_time = (cell->type == FOX) ? FOX_BREED_TIME : SQUIRREL_BREED_TIME;

    if (cell->breed_count < breed_time) {
        return 0;
    }

    int empty_positions[8];
    int num_empty = get_empty_adjacent_cells(board, r, c, empty_positions);

    if (num_empty == 0) {
        return 0;
    }

    int choice = get_random_direction(empty_positions, num_empty);
    int dir_idx = empty_positions[choice];
    int nr = r + DIRECTIONS[dir_idx].dr;
    int nc = c + DIRECTIONS[dir_idx].dc;

    place_offspring(board, nr, nc, cell->type);
    reset_after_breeding(cell);

    return 1;
}

void place_offspring(Board *board, int r, int c, char parent_type) {
    Cell *offspring = &board->grid[r][c];
    offspring->type = parent_type;
    offspring->breed_count = 0;
    offspring->starve_count = 0;
    offspring->moved_this_turn = 1;
}

int count_animals(const Board *board, char type) {
    int count = 0;
    for (int r = 0; r < board->rows; r++) {
        for (int c = 0; c < board->cols; c++) {
            if (board->grid[r][c].type == type) {
                count++;
            }
        }
    }
    return count;
}

int count_adjacent_type(const Board *board, int r, int c, char type) {
    int count = 0;

    for (int i = 0; i < 4; i++) {
        int nr = r + DIRECTIONS[i].dr;
        int nc = c + DIRECTIONS[i].dc;

        if (is_valid_position(board, nr, nc) && board->grid[nr][nc].type == type) {
            count++;
        }
    }

    return count;
}

void get_adjacent_cells(const Board *board, int r, int c, char type, int *positions, int *count) {
    *count = 0;

    for (int i = 0; i < 4; i++) {
        int nr = r + DIRECTIONS[i].dr;
        int nc = c + DIRECTIONS[i].dc;

        if (is_valid_position(board, nr, nc) && board->grid[nr][nc].type == type) {
            positions[(*count)++] = i;
        }
    }
}

int get_empty_adjacent_cells(const Board *board, int r, int c, int *positions) {
    int count = 0;

    for (int i = 0; i < 4; i++) {
        int nr = r + DIRECTIONS[i].dr;
        int nc = c + DIRECTIONS[i].dc;

        if (is_valid_position(board, nr, nc) && board->grid[nr][nc].type == EMPTY) {
            positions[count++] = i;
        }
    }

    return count;
}

int get_random_direction(const int *valid_dirs, int count) {
    if (count == 0) return -1;
    return rand() % count;
}

void clear_cell(Cell *cell) {
    cell->type = EMPTY;
    cell->breed_count = 0;
    cell->starve_count = 0;
    cell->moved_this_turn = 0;
}

void copy_cell(const Cell *src, Cell *dest) {
    dest->type = src->type;
    dest->breed_count = src->breed_count;
    dest->starve_count = src->starve_count;
    dest->moved_this_turn = src->moved_this_turn;
}

void increment_breed_count(Cell *cell) {
    cell->breed_count++;
}

void increment_starve_count(Cell *cell) {
    if (cell->type == FOX) {
        cell->starve_count++;
    }
}

void reset_after_breeding(Cell *cell) {
    cell->breed_count = 0;
}

void reset_after_eating(Cell *cell) {
    cell->starve_count = 0;
}

void print_statistics(const Board *board) {
    int foxes = count_animals(board, FOX);
    int squirrels = count_animals(board, SQUIRREL);
    int empty = count_animals(board, EMPTY);
    int impassible = count_animals(board, IMPASSIBLE);
    int total = board->rows * board->cols;

    printf("Foxes: %d | Squirrels: %d | Empty: %d | Impassible: %d | Total: %d\n",
           foxes, squirrels, empty, impassible, total);
}

void print_header(const char *title) {
    printf("\n========================================\n");
    printf("  %s\n", title);
    printf("========================================\n");
}

void print_separator(void) {
    printf("----------------------------------------\n");
}

void print_usage(const char *program_name) {
    printf("Usage: %s <board_file> [max_steps] [delay_ms]\n", program_name);
    printf("\n");
    printf("Arguments:\n");
    printf("  board_file  - Path to game board file (required)\n");
    printf("  max_steps   - Maximum simulation steps (default: 50)\n");
    printf("  delay_ms    - Delay between steps in ms (default: 200)\n");
    printf("\n");
    printf("Board file format:\n");
    printf("  F - Fox\n");
    printf("  s - Squirrel\n");
    printf("  - - Empty space\n");
    printf("  X - Impassible space\n");
}

int parse_arguments(int argc, char *argv[], char **filename, int *max_steps, int *delay_ms) {
    if (argc < 2) {
        fprintf(stderr, "Error: Missing board file argument\n\n");
        print_usage(argv[0]);
        return 0;
    }

    *filename = argv[1];

    if (argc >= 3) {
        *max_steps = atoi(argv[2]);
        if (*max_steps <= 0) {
            fprintf(stderr, "Error: max_steps must be positive\n");
            return 0;
        }
    }

    if (argc >= 4) {
        *delay_ms = atoi(argv[3]);
        if (*delay_ms < 0) {
            fprintf(stderr, "Error: delay_ms cannot be negative\n");
            return 0;
        }
    }

    return 1;
}