#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <zip.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <dirent.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    return fwrite(ptr, size, nmemb, stream);
}

void download_file(const char *url, const char *filename) {
    CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        fp = fopen(filename, "wb");
        if (!fp) {
            perror("Cannot open file for writing");
            curl_easy_cleanup(curl);
            return;
        }

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        fclose(fp);
        curl_easy_cleanup(curl);
    } else {
        fprintf(stderr, "Failed to init curl\n");
    }
}

void extract_zip(const char *archive_filepath) {
    int err;
    zip_t *za = zip_open(archive_filepath, 0, &err);
    if (!za) {
        fprintf(stderr, "Failed to open zip file %s (error %d)\n", archive_filepath, err);
        return;
    }

    zip_int64_t num_entries = zip_get_num_entries(za, 0);
    for (zip_int64_t i = 0; i < num_entries; i++) {
        const char *filename = zip_get_name(za, i, 0);
        if (filename[strlen(filename) - 1] == '/') {
            continue;  // Skip directories
        }

        zip_file_t *zf = zip_fopen_index(za, i, 0);
        char filepath[1024];
        sprintf(filepath, "./%s", filename);
        FILE *fout = fopen(filepath, "wb");
        if (!fout) {
            fprintf(stderr, "Error opening %s for writing.\n", filepath);
            zip_fclose(zf);
            continue;
        }

        char buffer[4096];
        zip_int64_t bytes_read;
        while ((bytes_read = zip_fread(zf, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, bytes_read, fout);
        }

        fclose(fout);
        zip_fclose(zf);
    }

    zip_close(za);
}

int main() {
    const char *url = "https://drive.google.com/uc?export=download&id=1rUIZmp10lXLtCIH3LAZJzRPeRks3Crup";
    const char *filename = "library.zip";
    const char *directory = "/home/soal_2/library";
    
    
    download_file(url, filename);
    extract_zip(filename);
    

    return 0;
}
