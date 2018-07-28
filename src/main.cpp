#include <Arduino.h>
#include <NewPing.h>

#define PI 3.1415926535897932384626433832795

// setup sonar
#define SONAR_TRIG 2
#define SONAR_ECHO 1
NewPing sonar(SONAR_TRIG, SONAR_ECHO, 600);

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
    //SerialUSB.println(array[circ_index]);
    circ_index = (circ_index+1)%VALUES_TO_KEEP;

}

void measure() {
    delay(500);
    SerialUSB.print("Ping: ");
    SerialUSB.print(sonar.ping_cm());
    SerialUSB.println("cm");
    int new_water_level = 8;

    update_array(new_water_level, history);

}

void save_data(int* hist_array, int MAX, int MIN) {
    //TODO add save to sd_card
    
    
}



void display_data(int* data) {
    int volume_now = water_volume(data[0]);
  //  SerialUSB.println(volume_now);

 //   for(int i = 0; i < 10; i++) // print last ten values
  //      {
   //     SerialUSB.println(data[i]);
    //    }
    //TODO add display logic;
}

void setup() {
      // Open Serial communications and wait for port to open:
    SerialUSB.begin(115200);
    pinMode(SONAR_ECHO,INPUT_PULLUP);
    
    //while (!Serial) {
    //    ; // wait for Serial port to connect. Needed for native USB port only
    //}    
    while (!SerialUSB) ;
    SerialUSB.println("Started");
    

    read_sd_card(); // load history into memory after reboot
}

void loop() {
    //SerialUSB.println("Updating...");
    //delay(MINUTE/30);
    measure(); // get new data
    save_data(history, WATER_LEVEL_MAX, WATER_LEVEL_MIN); // save data in case of a crash
    display_data(history); // show data on display

}