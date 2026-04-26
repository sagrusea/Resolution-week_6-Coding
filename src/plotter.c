#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HEIGHT 30
#define WIDTH 100

#ifdef _WIN32
#include <windows.h>
#define SLEEP(t) Sleep(t * 1000)
#define CLEAR_SCREEN() system("cls")
#else
#include <unistd.h>
#define SLEEP(t) sleep(t)
#define CLEAR_SCREEN() system("clear")
#endif

int make_screen_array(char *screen_buffer);
int draw_symbol(int ball_x, int ball_y, char *screen_buffer, char symbol);
double ask_for_data(char *question);
int draw_graph(double a, double b, double c, char type, double zoom, double offset_x);
int draw_min_max(int *bounds, char *screen_buffer);
int enter_function(char function);

int main() {
    char menu[] = "Graph plotter menu:\n1.Enter Function\n2.Add function\n3.Show Graph\n4.Quit\n";
    char sub_menu[] = "1.Linear or Quadratic\n2.Sin\n3.Cos\n4.Tan\n";
    char option;
    printf("%s", menu);
    while (1) {
        scanf(" %c", &option);
        while (getchar() != '\n');

        switch (option) {
            case '1':
                CLEAR_SCREEN();
                printf("%s", sub_menu);
                char function;
                scanf(" %c", &function);
                while (getchar() != '\n');
                enter_function(function);
                break;
            case '2':
                break;
            case '4':
                return 0;
            case 'q':
                return 0;
            default:
                CLEAR_SCREEN();
                printf("%c is undefined\n", option);
                printf("%s", menu);
                break;
        }
    }
    return 0;
}

int enter_function(char function) {
    printf("note: set a = 0 for linear functions\n");
    double a = ask_for_data("a");
    double b = ask_for_data("b");
    double c = ask_for_data("c");
    double zoom = 0.2;
    double offset_x = 0;
    
    // terminal setup
    printf("\x1b[8;%d;%dt", HEIGHT + 1, WIDTH + 2); // terminal size
    printf("\x1b[?25l");
    CLEAR_SCREEN();
    
    char *screen_buffer = malloc((WIDTH + 1) * HEIGHT + 1);
    make_screen_array(screen_buffer);
    while (1) {
        CLEAR_SCREEN();
        draw_graph(a, b, c, function, zoom, offset_x);
        printf("\ncontrols: A/D pan, +/- zoom, q quit");
        
        char key;
        scanf(" %c", &key);
        if (key == 'q') break;
        if (key == 'a') offset_x -= zoom * 10;
        if (key == 'd') offset_x += zoom * 10;
        if (key == '+') zoom *= 0.8;
        if (key == '-') zoom *= 1.2;

    }


    CLEAR_SCREEN();

    return 0;
}

int make_screen_array(char *screen_buffer) {
    screen_buffer[0] = '\0';
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            strcat(screen_buffer, " ");
        }
        if (y < HEIGHT - 1) {
            strcat(screen_buffer, "\n");
        }
    }
    return 0;
}

int draw_symbol(int ball_x, int ball_y, char *screen_buffer, char symbol) {
    if (ball_x < 0 || ball_x >= WIDTH || ball_y < 0 || ball_y >= HEIGHT) {
        return 1;
    }

    ball_y = -(ball_y - (HEIGHT - 1));
    int location = ball_y * (WIDTH + 1) + ball_x;
    screen_buffer[location] = symbol;

    printf("\x1b[H");
    printf("%s", screen_buffer);
    fflush(stdout);

    return 0;
}

double ask_for_data(char *question) {
    while (1){
        char input_buffer[10];

        printf("Enter %s:", question);
        fgets(input_buffer, 10, stdin);

        char *temp_pointer = strchr(input_buffer, '\n'); //remove new line
        if (temp_pointer) *temp_pointer = '\0';

        double output = strtod(input_buffer, &temp_pointer);

        if (*temp_pointer == '\0') {
            return output;
        } else {
            printf("Please enter a number!\n");
        }
    }
}

int *calc_y_bounds(double a, double b, double c, char type, double zoom, double offset_x) {
    int *arr = malloc(2 * sizeof(int));
    
    arr[0] = INT_MAX;
    arr[1] = INT_MIN;
    
    for (int i = 0; i < WIDTH; i++) {
        double y_val;
        double x_map = (i - WIDTH / 2.0) * zoom + offset_x;

        if (type == '2') y_val = a * sin(b * x_map) + c;
        else if (type == '3') y_val = a * cos(b * x_map) + c;
        else if (type == '4') y_val = a * tan(b * x_map) + c;
        else y_val = a * x_map * x_map + b * x_map + c;

        int y = (int)y_val;
        if (y > arr[1]) arr[1] = y;
        if (y < arr[0]) arr[0] = y;
    }
    return arr;
}

int draw_graph(double a, double b, double c, char type, double zoom, double offset_x) {
    char *screen_buffer = malloc((WIDTH + 1) * HEIGHT + 1);

    make_screen_array(screen_buffer);
    int *bounds = calc_y_bounds(a, b, c, type, zoom, offset_x);

    int x_axis_location;
    if (bounds[0] == bounds[1]) {
        x_axis_location = (HEIGHT / 2) - b;
    } else {
        double x_axis_ratio = (0.0 - bounds[0]) / (bounds[1] - bounds[0]);
        x_axis_location = (int)((HEIGHT - 1) * x_axis_ratio);
    }
    if (x_axis_location < 0) x_axis_location = 0;
    if (x_axis_location > HEIGHT - 1) x_axis_location = HEIGHT - 1;

    for (int i = 0; i < WIDTH; i++) {
        draw_symbol(i, x_axis_location, screen_buffer, '-');
    }

    for (int i = 0; i < WIDTH; i++) {
        double x_map = (i - WIDTH / 2.0) * zoom + offset_x;
        double y;

        if (type == '2') y = a * sin(b * x_map) + c;
        else if (type == '3') y = a * cos(b * x_map) + c;
        else if (type == '4') y = a * tan(b * x_map) + c;
        else y = a * x_map * x_map + b * x_map + c;

        int normal_y;

        if (bounds[0] == bounds[1]) {
            normal_y = HEIGHT / 2;
        } else {
            normal_y = (int)((HEIGHT - 1) * ((y - bounds[0]) / (bounds[1] - bounds[0])));
        }
        if (normal_y < 0) normal_y = 0;
        if (normal_y > HEIGHT - 1) normal_y = HEIGHT - 1;

        draw_symbol(i, normal_y, screen_buffer, 'o');
    }

    draw_min_max(bounds, screen_buffer);

    free(screen_buffer);
    free(bounds);

    return 0;
}

int draw_min_max(int *bounds, char *screen_buffer) {
    int min = bounds[0], max = bounds[1];
    char min_str[20], max_str[20];

    if (min == max) {
        snprintf(max_str, sizeof(max_str), "%d", max);

        int location = (HEIGHT / 2) * (WIDTH + 1);
        size_t flat_len = strlen(max_str);
        if (flat_len > WIDTH) {
            flat_len = WIDTH;
        }
        memcpy(&screen_buffer[location], max_str, flat_len);

        printf("\x1b[H");
        printf("%s", screen_buffer);
        fflush(stdout);

        return 0;
    }

    snprintf(min_str, sizeof(min_str), "%d", min);
    snprintf(max_str, sizeof(max_str), "%d", max);

    size_t max_len = strlen(max_str);
    if (max_len > WIDTH) {
        max_len = WIDTH;
    }
    memcpy(&screen_buffer[0], max_str, max_len);

    int location = (HEIGHT - 1) * (WIDTH + 1);
    size_t min_len = strlen(min_str);
    if (min_len > WIDTH) {
        min_len = WIDTH;
    }
    memcpy(&screen_buffer[location], min_str, min_len);

    printf("\x1b[H");
    printf("%s", screen_buffer);
    fflush(stdout);

    return 0;
}