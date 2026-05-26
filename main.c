#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>

#define RED   0
#define GREEN 1
#define BLUE  2

int width, height, channels;


unsigned char mat_get(unsigned char*, int, int, int);
void mat_set(unsigned char*, int, int, int, unsigned char);
unsigned char *convert_grey_scale(unsigned char*);

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("[-]Format: ./sketch <image_path>\n");
        return -1;
    }

    unsigned char *data = stbi_load(
        argv[1],
        &width,
        &height,
        &channels,
        3
    );

    if (data == NULL) {
        printf("[-]Load error");
        return -1;
    }

    channels = 3;

    printf("Width    : %d\n", width);
    printf("Height   : %d\n", height);
    printf("Channels : %d\n\n", channels);

    unsigned char *grey_scale = convert_grey_scale(data);

    stbi_write_png(
        "output/grayscale.png",
        width,
        height,
        1,
        grey_scale,
        width
    );


    return 0;
}

unsigned char mat_get(unsigned char* data, int y, int x, int channel) {
    return data[(y * width + x) * channels + channel];
}

void mat_set(unsigned char *data, int y, int x, int channel, unsigned char val) {
    data[(y * width + x) * channels + channel] = val;
}

unsigned char* convert_grey_scale(unsigned char* data) {
    unsigned char *grey_scale = (unsigned char*) malloc(width * height);
    if (grey_scale == NULL) {
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char r = mat_get(data, i, j, RED);
            unsigned char g = mat_get(data, i, j, GREEN);
            unsigned char b = mat_get(data, i, j, BLUE);

            unsigned char grey_pixel = (unsigned char)(0.299 * r + 0.587 *g + 0.114 * b);

            grey_scale[i * width + j] = grey_pixel;
        }
    }
    return grey_scale;
}