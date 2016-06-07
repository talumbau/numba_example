#include<stdio.h>
#include<math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void create_filter(double gKernel[][5])
{
    // set standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
    int x,y,i,j;
    // sum is for normalization
    double sum = 0.0;
    // generate 5x5 kernel
    for (x = -2; x <= 2; x++)
    {
        for(y = -2; y <= 2; y++)
        {
            r = sqrt(x*x + y*y);
            gKernel[x + 2][y + 2] = (exp(-(r*r)/s))/(M_PI * s);
            sum += gKernel[x + 2][y + 2];
        }
    }
    // normalize the Kernel
    for(i = 0; i < 5; ++i)
        for(j = 0; j < 5; ++j)
            gKernel[i][j] /= sum;
 
}


void gaussian_filter(unsigned char img_read[][848][3], unsigned char img_write[][848][3], int y, int x){
    //int m, n, k;
    int p, q, r;
    double sum;
    double gKernel[5][5];
    create_filter(gKernel);

    for(r=0; r<3; r++){
        sum = 0.;
        //for each row of the filter
        //printf("before starting filter\n");
        //for(p=0; p<5; p++){
        for(p=-2; p<3; p++){
            //for each column of the filter
            //for(q=0; q<5; q++){
            for(q=-2; q<3; q++){
                sum += gKernel[p+2][q+2] * img_read[y+p][x+q][r];
            }
        }
        //img_write[2][2][r] = sum;
        img_write[y][x][r] = sum;
    }

}


void apply_any_filter(unsigned char  img[][848][3],
                      unsigned char  img_final[][848][3],
                      void (*filter)(unsigned char [][848][3], unsigned char [][848][3], int, int)){ 
    int w, h, b, i, j, k, offset_h, offset_v, m, n;
    int start_left, end_left, start_top, end_top;
    unsigned char ***img_chunk;
    unsigned char ***img_chunk_final;
    double sum;
    h = 500;
    w = 848;
    b = 3;

    start_left = 2;
    end_left = w - 2;
    start_top = 2;
    end_top = h - 2;

    for(j=start_top; j<end_top; ++j){
        for(i=start_left; i<end_left; ++i) {
            //convolve for each of r, g, b channels
            filter(img, img_final, j, i);
        }
    }

}


void write_png(unsigned char img[][848][3], int width, int height){
    stbi_write_png("landscape_out_python.png", width, height, 3, img, 3*width);
    printf("finished");
}

unsigned char *read_png(char *name, int *width, int *height, int *bpp){
    printf("started reading");
    //unsigned char (*)[1364][3] ptr;
    //#int width, height, bpp, i, j, k, w, h, b;
    unsigned char* rgb = stbi_load(name, width, height, bpp, 3);
    printf("finished reading");
    return rgb;
}

