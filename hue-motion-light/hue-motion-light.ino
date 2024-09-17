#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include "secrets.h"
#include "util.h"

/*
 * Required Secrets
 * 
 * String HUE_IP              : IP address of hue lights
 * String HUE_USERNAME        : meethue API username
 * const char* WIFI_SSID      : Wifi name
 * const char* WIFI_PASSWORD  : Wifi password
 */

// #define DEBUG 1
#define SENSOR_PIN 35
#define KNOB_PIN 32


#define LIGHT_ON_DELAY 300000 // 30 minutes

enum State {
    OFF,
    ON,
    CHECK
};

unsigned int state = OFF;

// Global variables
int pot_value;
bool found_motion;

HueApi hueApi(HUE_IP, HUE_USERNAME);

struct Timer {
    unsigned long start_millis;
    unsigned long duration;
};

struct Light {
    bool is_on; // Are lights on or off
    unsigned int brightness; // Percentage
};

struct Light light;

// Check if a timer has finished
bool is_timer_finished(Timer t) {
    unsigned long curr_millis = millis();
    return ((curr_millis - t.start_millis) > t.duration);
}

// Reset a timer
void reset_timer(Timer *t) {
    t->start_millis = millis();
}

struct Timer on_timer, check_timer;

void connect_to_wifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD); 
    Serial.print("Connecting to WiFi network...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }

    Serial.println("Connected!");

}

void set_on_state() {
    state = ON;
    light.is_on = true;
    hueApi.setOnState(1, true);
    hueApi.setOnState(2, true);
    reset_timer(&on_timer);
}

void set_off_state() {
    state = OFF;
    light.is_on = false;
    hueApi.setOnState(1, false);
    hueApi.setOnState(2, false);
}

void set_check_state() {
    state = CHECK;
    reset_timer(&check_timer);
}

void setup() {
    // Initialise pins
    pinMode(SENSOR_PIN, INPUT);
    pinMode(KNOB_PIN, INPUT);
   
    // Initialise timers
    on_timer.duration = 60000 * 20; // 60000; // 1 minute
    check_timer.duration = 60000 * 3;

    light.is_on = false;
    light.brightness = 50;
    
    // Start monitoring on port 9600
    Serial.begin(9600);

    connect_to_wifi();

    hueApi.setBrightness(1, 80);
    hueApi.setBrightness(2,80);
}


void loop() {
    switch (state) {
        case OFF:
            // Read motion sensor
            found_motion = (digitalRead(SENSOR_PIN) == HIGH);
            if (found_motion) {
                set_on_state();
            }
            break;
        case ON:
            // Move to CHECK after ON delay milliseconds
            if (is_timer_finished(on_timer)) {
                set_check_state();
            }
            break;

        case CHECK:
            // Check for motion to reset the delay
            found_motion = (digitalRead(SENSOR_PIN) == HIGH);
            if (found_motion) {
                set_on_state();
            }
            
            // No motion after check timer... Turn off
            if (is_timer_finished(check_timer)) {
                set_off_state();
            }

            break;
    }

    delay(500);
}
