#include <Arduino.h>
#include <NewPing.h>


// setup sonar
#define SONAR_TRIG 2
#define SONAR_ECHO 1
NewPing sonar(SONAR_TRIG, SONAR_ECHO, 600);

const int WELL_RADIUS = 100; // in CM
const int WELL_DEPTH = 600; //in CM ->
//-> distance between the lowest point at which the pump is able to intake water and the sensor
int WATER_LEVEL_MIN = 0; // in CM from sensor lowest achived
int WATER_LEVEL_MAX = 0; // in CM from sensor highest achieved

const int VALUES_TO_KEEP = 7*24*60; // length of history
int history[VALUES_TO_KEEP]; // array of last weeks worh of data.

int circ_index = -1; //used for circular array above

//constants
const int MINUTE = 60000; //ms
const float pi = 3.141592653;


void read_sd_card() {
    memcpy(history, history, sizeof(history)*sizeof(int) ); //used to load history into memeory
}

int water_volume(int level) {
    int volume = WELL_RADIUS*pi*level;
    return volume;
}

int get_value(int* array, int pos){
    return array[(pos+circ_index)%VALUES_TO_KEEP];
}

void update_array(int new_value, int* array) {
    circ_index = (circ_index+1)%VALUES_TO_KEEP;
    array[circ_index] = new_value;
    SerialUSB.print("circ_index: ");
    SerialUSB.println(circ_index);
    

}

void measure() {
    delay(500);
    int dist = sonar.ping_cm();



    int new_water_level = WELL_DEPTH - dist;
    WATER_LEVEL_MIN = min(new_water_level, WATER_LEVEL_MIN);
    WATER_LEVEL_MAX = max(new_water_level, WATER_LEVEL_MAX);
    update_array(new_water_level, history);

    SerialUSB.print("Distance: ");
    SerialUSB.print(dist);
    SerialUSB.print("cm; Water level: ");
    SerialUSB.print(new_water_level);
    SerialUSB.println("cm");

}

void save_data(int* hist_array, int MAX, int MIN) {
    //TODO add save to sd_card
    
    
}



void display_data(int* data) {
    int volume_now = water_volume(get_value(data, 0));
    SerialUSB.print("Volume: ");
    SerialUSB.print(volume_now);
    SerialUSB.println(" l");

    for(int i = 0; i < 10; i++) // print last ten values
        {
        SerialUSB.println(data[i]);
        }
     SerialUSB.println("");
      SerialUSB.println("");
    //TODO add display logic;
}

void setup() {
      // Open Serial communications and wait for port to open:
    SerialUSB.begin(115200);
    pinMode(SONAR_ECHO,INPUT_PULLUP); 
    while (!SerialUSB) ;
    SerialUSB.println("Started");
    
    read_sd_card(); // load history into memory after reboot
}

void loop() {
    //SerialUSB.println("Updating...");
    delay(MINUTE/20);
    measure(); // get new data
    save_data(history, WATER_LEVEL_MAX, WATER_LEVEL_MIN); // save data in case of a crash
    display_data(history); // show data on display

}