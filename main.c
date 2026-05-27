#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define RED   0
#define GREEN 1
#define BLUE  2

int width, height, channels;

int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};



unsigned char mat_get(unsigned char*, int, int, int);
void mat_set(unsigned char*, int, int, int, unsigned char);
unsigned char *convert_grey_scale(unsigned char*);
int element_wise_multiplication(unsigned char*, int x, int y, int[3][3]);
unsigned char *sobel_technique(unsigned char*);


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
    unsigned char *sketch = sobel_technique(grey_scale);
    stbi_write_png(
        "output/sketch.png",
        width,
        height,
        1,
        sketch,
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

int element_wise_multiplication(unsigned char *grey_scale, int x, int y, int GX[3][3]) {
    int sum = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int neighbour_x = x + j - 1;
            int neighbour_y = y + i - 1;
            int mat_value = grey_scale[neighbour_y * width + neighbour_x];
            sum += mat_value * GX[i][j];
        }
    }
    return sum;
}

unsigned char *sobel_technique(unsigned char *grey_scale) {
    unsigned char *sketch = (unsigned char *) malloc(width * height);
    memset(sketch, 0, width * height);
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int gx = element_wise_multiplication(grey_scale, j, i, GX);
            int gy = element_wise_multiplication(grey_scale, j, i, GY);
            double g = sqrt(
                        (double)(gx * gx) +
                        (double)(gy * gy)
                    );
            if (g > 255)
                g = 255;
            sketch[i * width + j] = (unsigned char)g;
        }
    }
    return sketch;
}