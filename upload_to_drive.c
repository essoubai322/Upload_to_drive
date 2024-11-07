#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Add your client details here
#define CLIENT_ID "221357323036-akcsrtljm4f3fmgpf2dosfrd6lf3qdjk.apps.googleusercontent.com"
#define CLIENT_SECRET "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"
#define REFRESH_TOKEN "1//04a522-ehpCw0CgYIARAAGAQSNwF-L9Ir-pCofucuglfY9zw2_uKfln8H0xj3vmDXI54vxqJvnZgsF843jQy4xXRVfpmdDnIKFYc"
#define TOKEN_URI "https://oauth2.googleapis.com/token"

struct response {
    char *data;
    size_t size;
};

size_t write_callback(void *ptr, size_t size, size_t nmemb, struct response *res) {
    size_t total_size = size * nmemb;
    res->data = realloc(res->data, res->size + total_size + 1);
    if (res->data == NULL) {
        printf("Memory allocation failed\n");
        return 0;
    }
    memcpy(res->data + res->size, ptr, total_size);
    res->size += total_size;
    res->data[res->size] = '\0';
    return total_size;
}

char *refresh_access_token(const char *refresh_token) {
    CURL *curl;
    CURLcode res;
    struct response response_data = { .data = NULL, .size = 0 };
    char post_fields[1024];

    snprintf(post_fields, sizeof(post_fields), "client_id=%s&client_secret=%s&refresh_token=%s&grant_type=refresh_token", CLIENT_ID, CLIENT_SECRET, refresh_token);

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, TOKEN_URI);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Token refresh failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return NULL;
        }

        // Print the response (which contains the new access token)
        printf("Response: %s\n", response_data.data);

        // Parse the JSON response (assuming you know the format)
        // Example: {"access_token": "new-access-token", "expires_in": 3600, "token_type": "Bearer"}
        // You can extract the access token from the response and return it.

        // In this case, you might want to use a JSON parser, like `cJSON` to extract the access token.

        // Clean up
        curl_easy_cleanup(curl);
        curl_global_cleanup();

        return response_data.data; // Or you can return only the access token
    }
    return NULL;
}

int main() {
    // Refresh the token if needed
    char *new_access_token = refresh_access_token(REFRESH_TOKEN);
    if (new_access_token) {
        printf("New Access Token: %s\n", new_access_token);
        // Now you can use this new token to upload the file
    } else {
        fprintf(stderr, "Failed to refresh access token\n");
    }

    return 0;
}
