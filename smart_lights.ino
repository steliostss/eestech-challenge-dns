//Question C
// Add these to smart_amph
#define ROW_SEATS 9

int persons = 0;

loop() {
    //...

    // place after update in persons variable
    if (persons % ROW_SEATS == 1 && entree) {
        enable_row(persons/ROW_SEATS);
    } else if (persons % ROW_SEATS == 0 && exitee) {
        disable_row((persons+1)/ROW_SEATS)
    }
    //...
}