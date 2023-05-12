#include "what3words.h"

#define EARTH_RADIUS 6371.0 // Earth radius in kilometers

double degreesToRadians(double degrees) {
    return degrees * M_PI / 180.0;
}

//Haversine formula implementation
double calculateDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = degreesToRadians(lat2 - lat1);
    double dLon = degreesToRadians(lon2 - lon1);

    double a = sin(dLat / 2) * sin(dLat / 2) +
        cos(degreesToRadians(lat1)) * cos(degreesToRadians(lat2)) *
        sin(dLon / 2) * sin(dLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    double distance = EARTH_RADIUS * c;
    return distance;
}

// Function to write response data received from CURL
size_t writeCallback(void* contents, size_t size, size_t nmemb, char* buffer) {
    size_t realsize = size * nmemb;
    strcat(buffer, (char*)contents);
    return realsize;
}

void coordinatesToGeolocation(double latitude, double longitude) {
    CURL* curl;
    CURLcode res;
    char url[512];
    char response[4096] = { 0 };
    char words[256];

    snprintf(url, sizeof(url), "https://api.what3words.com/v3/convert-to-3wa?coordinates=%.6f,%.6f&key=%s", latitude, longitude, APIKEY);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            printf("Response:\n%s\n", response);

            // Extract geolocation words from the response using strstr and sscanf
            char* words_start = strstr(response, "\"words\":\"");
            if (words_start) {
                words_start += strlen("\"words\":\"");
                char* words_end = strstr(words_start, "\"");
                if (words_end) {
                    strncpy(words, words_start, words_end - words_start);
                    words[words_end - words_start] = '\0';

                    printf("Geolocation for %.6f, %.6f: words = %s\n", latitude, longitude, words);
                }
                else {
                    fprintf(stderr, "Failed to extract geolocation words from the response.\n");
                }
            }
            else {
                fprintf(stderr, "Failed to extract geolocation information from the response.\n");
            }
        }
        curl_easy_cleanup(curl);
    }
}


void geolocationToCoordinates(const char* location) {
    CURL* curl;
    CURLcode res;
    char url[254];
    char response[4096] = { 0 };
    double latitude, longitude;

    snprintf(url, sizeof(url), "https://api.what3words.com/v3/convert-to-coordinates?words=%s&key=%s", location, APIKEY);

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else {
            printf("Response:\n%s\n", response);

            // Extract coordinates from the response using strstr and sscanf
            char* coordinates_start = strstr(response, "\"coordinates\":{\"lng\":");
            if (coordinates_start) {
                coordinates_start += strlen("\"coordinates\":{\"lng\":");
                char* longitude_end = strstr(coordinates_start, ",");
                if (longitude_end) {
                    char longitude_str[32];
                    strncpy(longitude_str, coordinates_start, longitude_end - coordinates_start);
                    longitude_str[longitude_end - coordinates_start] = '\0';
                    sscanf(longitude_str, "%lf", &longitude);

                    char* latitude_start = longitude_end + strlen("\",\"lat\":");
                    char* latitude_end = strstr(latitude_start, "}");
                    if (latitude_end) {
                        char latitude_str[32];
                        strncpy(latitude_str, latitude_start, latitude_end - latitude_start);
                        latitude_str[latitude_end - latitude_start] = '\0';
                        sscanf(latitude_str, "%lf", &latitude);

                        printf("Coordinates for %s: latitude = %.6f, longitude = %.6f\n", location, latitude, longitude);
                    }
                    else {
                        fprintf(stderr, "Failed to extract latitude from the response.\n");
                    }
                }
                else {
                    fprintf(stderr, "Failed to extract longitude from the response.\n");
                }
            }
            else {
                fprintf(stderr, "Failed to extract coordinates from the response.\n");
            }
        }
        curl_easy_cleanup(curl);
    }
}
