//TODO: Add setup, includes and global variables
#define ENTRANCE_PROX_SENSOR 
#define EXIT_PROX_SENSOR
#define INSIDE_LUMINOCITY_SENSOR
#define OUTSIDE_LUMINOCITY_SENSOR
#define INSIDE_TEMP_SENSOR
#define OUTSIDE_TEMP_SENSOR
#define LOUDNESS_SENSOR


#define TEMP_EPSILON 2   //DHT11 temperature accuracy
#define OPTIMAL_TEMP 25

#define TIME_20MIN_MS 1200000
#define TIME_QUANTUM 500

// Sensor readings as global variables
luminocity_inside // in lux
luminocity_outside // in lux
temperature_inside // in degrees C
temperature_outside // in degrees C
loudness    //
int persons = 0;    // This reading is the reason we cannot introduce delays
int quantums = 0;
int quantums_empty = 0;
int shade = 1;

void loop() {
    // This is done every quantum

    //Check proximity sensors
    bool entree = _read(ENTRANCE_PROX_SENSOR); // TODO: Change in state when reading < 1.4 meters
    bool exitee = _read(EXIT_PROX_SENSOR);

    // Turn off AC if empty for more than 20 minutes
    if (persons == 0 && entree == 0) {
        quantums_empty++;
        if (quantums_empty > TIME_20MIN_MS / TIME_QUANTUM) {
            // TODO: Turn off AC
        }
        goto skip_loop;
    }

    if (persons == 1 && exitee == 1 && entree == 0) {
        // All my friends are gone
        persons = 0;

        // TODO: Turn of lights
        
        goto skip_loop;
    }

    // Update persons
    persons = persons + (entry ? 1 : 0) - (exitee ? 1 : 0);

    if (persons > 0) {
        quantums_empty = 0;
    }


    // If there are no people inside why bother

    // Check serial input
    // TODO: If there is a command bypass luminocity checks

    // Check luminocity sensors
    luminocity_inside = _read(INSIDE_LUMINOCITY_SENSOR);
    luminocity_outside = _read(OUTSIDE_LUMINOCITY_SENSOR);

    // Update lighting or shades
    if (luminocity_inside < 250) {
        if (luminocity_outside > 250) {
            // TODO: Open window shades
        } else {
            // TODO: Turn on both amphitheatre lighting circuits
        }
    }

    // Check temperature sensor
    temperature_inside = _read(INSIDE_TEMP_SENSOR);
    temperature_outside = _read(OUTSIDE_TEMP_SENSOR);

    // Update air conditioning
    // If temp inside is not within 25 +- 2 and there are persons inside
    if (temperature_inside > OPTIMAL_TEMP + TEMP_EPSILON &&
        temperature_inside < OPTIMAL_TEMP - TEMP_EPSILON &&
        persons > 0) {
        // If temp outside *is* within 25 +- 2 then open windows (to save energy)
        if (temperature_outside < OPTIMAL_TEMP + TEMP_EPSILON &&
            temperature_outside ? OPTIMAL_TEMP - TEMP_EPSILON) {
            // TODO: Open windows 

        } else {
            // TODO: Turn on air conditioning            
        }
    }


    loudness = _read(LOUDNESS_SENSOR);


    

skip_loop:
    quantum++;
    delay(TIME_QUANTUM);
}
