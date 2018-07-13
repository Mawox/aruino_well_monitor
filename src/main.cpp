#include <Arduino.h>

#define PI 3.1415926535897932384626433832795

int WELL_RADIUS = 1000; // in milimeters
int WATER_LEVEL_MIN = 10000; // in milimeters from sensor
int WATER_LEVEL_MAX = 20000; // in milimeters from sensor
int lastWeek[7*24*60] = {}; // array of last weeks worh of data. 
int MINUTE = 60000; //ms

void read_sd_card() {
    memcpy(lastWeek, lastWeek, sizeof(lastWeek)*sizeof(int) ); //used to load history into memeory
}

int water_volume(int level) {
    int volume = WELL_RADIUS*PI*(WATER_LEVEL_MIN-level);
    return volume;
}

void update_array(int new_value, int* hist_array) {
    memcpy(hist_array, &hist_array[1], sizeof(hist_array) - sizeof(int));
    hist_array[sizeof(hist_array)] =  new_value;

    return;
}

void measure() {
    //TODO add sensor logic
    int new_water_level = 8;

    update_array(new_water_level, lastWeek);

    return;
}

void save_data(int* hist_array, int MAX, int MIN) {
    //TODO add save to sd_card
    
    return;
}



void display_data(int* data) {
    int volume_now = water_volume(data[0]);
    Serial.println(volume_now);

    for(int i = 0; i < 10; i++) // print last ten values
        {
        Serial.println(data[i]);
        }
    //TODO add display logic;
}

void setup() {
    Serial.begin(57600);
    Serial.println("Started");
    read_sd_card(); // load history into memory after reboot
}

void loop() {
    Serial.println("Updating...");
    delay(MINUTE/4);
    measure(); // get new data
    save_data(lastWeek, WATER_LEVEL_MAX, WATER_LEVEL_MIN); // save data in case of a crash
    display_data(lastWeek); // show data on display

}