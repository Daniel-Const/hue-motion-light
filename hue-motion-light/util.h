/**
 * Utility methods for handling strings for API bodies
 */

#include <HTTPClient.h>

class HueApi {

    public:
        String base_url;
        HueApi(String hueIp, String hueUsername) {
            base_url = "https://";
            base_url += hueIp;
            base_url += "/api/";
            base_url += hueUsername;
            base_url += "/lights/";
            // base_url = String("https://" + hueIp + "/api/" + hueUsername + "/lights/");
        }

        void initHttp(HTTPClient* http, int lightId) {
            String request = base_url;
            request += lightId;
            request += "/state/";
            http->begin(request);
            http->addHeader("Content-Type", "text/plain");
        }

        void setBrightness(int lightId, unsigned int brightness) {
            String body = "{";
            body += "\"bri\":";
            body += brightness;
            body += "}";
            sendRequest(lightId, body);
        }

        void setOnState(int lightId, bool is_on) {
            String body = "{";
            body += "\"on\":";
            body += (is_on) ? "true" : "false";
            body += "}";
            sendRequest(lightId, body);
        }

        void sendRequest(int lightId, String body) {
            HTTPClient http;
            initHttp(&http, lightId);
            int response_code = -1;
            while (response_code <= 0) {
                response_code = http.PUT(body);
                if (response_code > 0) {
                    Serial.println("Request made successfully");
                    String response = http.getString();
                    Serial.println(response);
                } else {
                    Serial.println("Error making PUT request...");
                }
            }
        }

};
