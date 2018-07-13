#include <Arduino.h>

#define PI 3.1415926535897932384626433832795

const int WELL_RADIUS = 1000; // in milimeters
int WATER_LEVEL_MIN = 10000; // in milimeters from sensor
int WATER_LEVEL_MAX = 20000; // in milimeters from sensor
const int VALUES_TO_KEEP = 7*24*60; 
int history[VALUES_TO_KEEP]; // array of last weeks worh of data. 
int circ_index = 0;
int MINUTE = 60000; //ms

void read_sd_card() {
    memcpy(history, history, sizeof(history)*sizeof(int) ); //used to load history into memeory
}

int water_volume(int level) {
    int volume = WELL_RADIUS*PI*(WATER_LEVEL_MIN-level);
    return volume;
}
int get_value(int* array, int pos){
    return array[(pos+circ_index)%VALUES_TO_KEEP];
}

void update_array(int new_value, int* array) {
    array[circ_index] = new_value;
    Serial.println(array[circ_index]);
    circ_index = (circ_index+1)%VALUES_TO_KEEP;

}

void measure() {
    //TODO add sensor logic
    int new_water_level = 8;

    update_array(new_water_level, history);

}

void save_data(int* hist_array, int MAX, int MIN) {
    //TODO add save to sd_card
    
    
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
      // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while (!Serial) {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    Serial.println("Started");
    read_sd_card(); // load history into memory after reboot
}

void loop() {
    Serial.println("Updating...");
    delay(MINUTE/20);
    measure(); // get new data
    save_data(history, WATER_LEVEL_MAX, WATER_LEVEL_MIN); // save data in case of a crash
    display_data(history); // show data on display

}