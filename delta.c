#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} tTuple;

int students = 0;
int COLUMNS; 
int ROWS;
int total_sensors = 0;
tTuple final_sensors[1000];

tTuple find_max(int** heat_map){
    tTuple maxTuple;
    maxTuple.x = 0;
    maxTuple.y = 0;
    int max = 0;
    for (int i=0; i<ROWS; ++i){
        for (int j=0; j<COLUMNS; ++j){
            if (heat_map[i][j] > max) {
              maxTuple.x = i;
              maxTuple.y = j;  
            }
        }
    }
    return maxTuple;
}

void edit_neighbors_on_heat_map(int col, int row, int radius, int** heat_map) {
    //set the borders of the "subgrid" to decrease value on heatmap
    //rows are set here
    int row_from, row_to;    
    if (row < radius)
        row_from = 0;
    else
        row_from = row-radius;

    //stop row is set here
    //if we are too close to the right edge, start from the edge
    if (row > COLUMNS-radius)
        row_to = COLUMNS-1;
    else 
        row_to = row+radius;

    //columns are set here
    //start column is set here
    int col_from, col_to;
    //if we are too close to the left edge, start from the edge
    if (col < radius)
        col_from = 0;
    //else from distance radius
    else 
        col_from = col-radius;

    //stop column is set here
    //if we are too close to the right edge, start from the edge
    if (col > ROWS-radius)
        col_to = COLUMNS-1;
    else 
        col_to = col+radius;
    for (int i=row_from; i<row_to; ++i) {
        for(int j=col_from; j<col_to; ++j){
            --heat_map[i][j];
        }
    }
}

void calculate_sensors(int col, int row, int radius, int** heat_map, int** student_seats) {
    int row_from, row_to;    
    if (row < radius)
        row_from = 0;
    else 
        row_from = row-radius;
    if (row > COLUMNS-radius)
        row_to = COLUMNS-1;
    else 
        row_to = row+radius;

    int col_from, col_to;
    if (col < radius)
        col_from = 0;
    else 
        col_from = col-radius;
    if (col > ROWS-radius)
        col_to = COLUMNS-1;
    else 
        col_to = col+radius;

    for (int i=row_from; i<row_to; ++i) {
        for(int j=col_from; j<col_to; ++j){
            if (student_seats[i][j] == 1){
                edit_neighbors_on_heat_map(i, j, radius, heat_map);
            }
        }
    }
}

void create_heat_map(int radius, int** heat_map, int** student_seats) {
    for (int row=0; row<COLUMNS; ++row){

        int row_from;
        int row_to;

        if (row < radius)
            row_from = 0;
        else 
            row_from = row-radius;
        if (row > COLUMNS-radius)
            row_to = COLUMNS-1;
        else 
            row_to = row+radius;

        for (int col=0; col<ROWS; ++col){
            if (student_seats[row][col] == 0) 
                continue;
            int col_from;
            int col_to;
            if (col < radius)
                col_from = 0;
            else 
                col_from = col-radius;
            if (col > ROWS-radius)
                col_to = COLUMNS-1;
            else 
                col_to = col+radius;
            for (int i=row_from; i<row_to; ++i){
                for (int j=col_from; j<col_to; ++j){
                    ++heat_map[i][j];
                }
            }
        }
    }
}

int main (int argc, char** argv) {

    //Enter COLUMNS of the room:
    scanf("%d", &COLUMNS);
    
    //Enter ROWS of the room:
    scanf("%d", &ROWS);

    int** student_seats = malloc(COLUMNS*sizeof(int*));
    int** heat_map = malloc(COLUMNS*sizeof(int*));

    int i=0;
    for (i=0; i<COLUMNS; ++i) { student_seats[i] = malloc(ROWS*sizeof(int)); }
    for (i=0; i<COLUMNS; ++i) { heat_map[i] = malloc(ROWS*sizeof(int)); }

    int seats = COLUMNS*ROWS;
    int row = 0;
    int col = 0;

    //Enter seats:
    for (i=0; i<seats; ++i){
        scanf("%d", &student_seats[row][col]);

        if (student_seats[row][col] == 1)
            ++students;

        ++col;
        if (col == ROWS) {
            ++row;
            col = 0;
        }
    }

    //Enter radius of sensors:;
    int radius;
    scanf("%d", &radius);

    //DO THINGS HERE----------------------------------------------------
    create_heat_map(radius, heat_map, student_seats);
    while(students > 0) {
        tTuple max = find_max(heat_map);
        //add new sensor
        final_sensors[total_sensors] = max;
        //increase number of sensors
        ++total_sensors;
        //calculate next sensor
        calculate_sensors(max.x, max.y, radius, heat_map, student_seats);
    }

    //------------------------------------------------------------------

    for (int i=0; i<COLUMNS; ++i) {
        for (int j=0; j<ROWS; ++j) {
            printf("%d ", student_seats[i][j]);
        }
        printf("\n");
    }

    printf("\n\n");   

    for (int i=0; i<COLUMNS; ++i) {
        for (int j=0; j<ROWS; ++j) {
            printf("%d ", heat_map[i][j]);
        }
        printf("\n");
    }

    return 0;
}
