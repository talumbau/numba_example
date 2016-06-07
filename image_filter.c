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


void example_image(unsigned char  img[][9][3]){
    int w, h, b, i, j, k;
    h = 12;
    w = 9;
    b = 3;
    //char img[12][9][3];
    //char (*img)[9][3]=(char (*)[9][3]) img_raw;

    for(j=0; j<h; ++j){
        for(i=0; i<w; ++i) {
            if (i/3 == 0) {
                img[j][i][0] = 0;
                img[j][i][1] = 0;
                img[j][i][2] = 255;
            }
            if (i/3 == 1) {
                img[j][i][0] = 0;
                img[j][i][1] = 255;
                img[j][i][2] = 0;
            }
            if (i/3 == 2) {
                img[j][i][0] = 255;
                img[j][i][1] = 0;
                img[j][i][2] = 0;
            }
        }
    }

    //return img
}

void modify_image(unsigned char  img[][1364][3]){
    int w, h, b, i, j, k, offset_h, offset_v;
    //h = 1364;
    //w = 1364;
    h = 12;
    w = 9;
    b = 3;
    //char img[12][9][3];
    //char (*img)[9][3]=(char (*)[9][3]) img_raw;
    offset_h = 200;
    offset_v = 400;

    for(j=0; j<h; ++j){
        for(i=0; i<w; ++i) {
            if (i/3 == 0) {
                img[j + offset_h][i+offset_v][0] = 0;
                img[j + offset_h][i+offset_v][1] = 0;
                img[j + offset_h][i+offset_v][2] = 255;
            }
            if (i/3 == 1) {
                img[j + offset_h][i+offset_v][0] = 0;
                img[j + offset_h][i+offset_v][1] = 255;
                img[j + offset_h][i+offset_v][2] = 0;
            }
            if (i/3 == 2) {
                img[j + offset_h][i+offset_v][0] = 255;
                img[j + offset_h][i+offset_v][1] = 0;
                img[j + offset_h][i+offset_v][2] = 0;
            }
        }
    }
}


void apply_filter(unsigned char  img[][1364][3], unsigned char  img_final[][1364][3]){
    int w, h, b, i, j, k, offset_h, offset_v, m, n;
    int start_left, end_left, start_top, end_top;
    double sum;
    h = 1364;
    w = 1364;
    b = 3;

    start_left = 2;
    end_left = w - 2;
    start_top = 2;
    end_top = h - 2;

    double gKernel[5][5];
    create_filter(gKernel);

    for(j=start_top; j<end_top; ++j){
        for(i=start_left; i<end_left; ++i) {
            //convolve for each of r, g, b channels
            for(k=0; k<3; k++){
                sum = 0.;
                //for each row of the filter
                //printf("before starting filter\n");
                for(m=-2; m<3; m++){
                    //for each column of the filter
                    for(n=-2; n<3; n++){
                        sum += gKernel[m+2][n+2] * img[j + m][i + n][k];
                    }
                }
                img_final[j][i][k] = sum;
            }
        }
    }

}


void gaussian_filter(unsigned char img_read[][1364][3], unsigned char img_write[][1364][3]){
    //int m, n, k;
    int p, q, r;
    double sum;
    double gKernel[5][5];
    create_filter(gKernel);

    for(r=0; r<3; r++){
        sum = 0.;
        //for each row of the filter
        //printf("before starting filter\n");
        //for(p=-2; p<3; p++){
        for(p=0; p<5; p++){
            //for each column of the filter
            //for(q=-2; q<3; q++){
            for(q=0; q<5; q++){
                sum += gKernel[p][q] * img_read[p][q][r];
            }
        }
        img_write[2][2][r] = sum;
        //img_write[2][2][r] = 255;
    }

}


void gaussian_filter2(unsigned char img_read[][1364][3], unsigned char img_write[][1364][3], int y, int x){
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



void apply_any_filter(unsigned char  img[][1364][3],
                      unsigned char  img_final[][1364][3],
                      void (*filter)(unsigned char [][1364][3], unsigned char [][1364][3])){ 
    int w, h, b, i, j, k, offset_h, offset_v, m, n;
    int start_left, end_left, start_top, end_top;
    unsigned char ***img_chunk;
    unsigned char ***img_chunk_final;
    double sum;
    h = 1364;
    w = 1364;
    b = 3;

    start_left = 2;
    end_left = w - 2;
    start_top = 2;
    end_top = h - 2;

    for(j=start_top; j<end_top; ++j){
        for(i=start_left; i<end_left; ++i) {
            //convolve for each of r, g, b channels
            // addr( & arr[ row ][ col ] ) = addr( arr ) + [ sizeof( int ) * COLS * row ]
            //                                              + [ sizeof( int ) * col ]
            //img_chunk = (unsigned char ***) &img + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk_final = (unsigned char ***) &img_final + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk = unsigned char ***) &img_final[j][i];
            //img_chunk_final = (unsigned char ***) &img_final[j][i];
            //filter(img_chunk, img_chunk_final);
            filter((unsigned char (*)[1364][3]) img[j-2][k-2],
                   (unsigned char (*)[1364][3]) img_final[j-2][k-2]);
        }
    }

}


double take_basic_callback(double x, double y, int(*filter)(int, int)){
    double ans;
    ans = x + y + filter((int) x, (int) y);
    printf("ans is %f", ans);
    return ans;
}


double take_callback_unsafe(double x, double y, void *address){
    double ans;
    int(*filter)(int, int) = (int(*)(int, int)) address;
    ans = filter((int) x, (int) y);
    return ans + x + y;
}



void apply_any_filter2(unsigned char  img[][1364][3],
                      unsigned char  img_final[][1364][3],
                      void (*filter)(unsigned char [][1364][3], unsigned char [][1364][3], int, int)){ 
    int w, h, b, i, j, k, offset_h, offset_v, m, n;
    int start_left, end_left, start_top, end_top;
    unsigned char ***img_chunk;
    unsigned char ***img_chunk_final;
    double sum;
    h = 1364;
    w = 1364;
    b = 3;

    start_left = 2;
    end_left = w - 2;
    start_top = 2;
    end_top = h - 2;

    for(j=start_top; j<end_top; ++j){
        for(i=start_left; i<end_left; ++i) {
            //convolve for each of r, g, b channels
            // addr( & arr[ row ][ col ] ) = addr( arr ) + [ sizeof( int ) * COLS * row ]
            //                                              + [ sizeof( int ) * col ]
            //img_chunk = (unsigned char ***) &img + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk_final = (unsigned char ***) &img_final + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk = unsigned char ***) &img_final[j][i];
            //img_chunk_final = (unsigned char ***) &img_final[j][i];
            //filter(img_chunk, img_chunk_final);
            //filter((unsigned char (*)[1364][3]) img[j-2][k-2],
                   //(unsigned char (*)[1364][3]) img_final[j-2][k-2], j, i);
            filter(img, img_final, j, i);
        }
    }

}


void apply_any_filter3(unsigned char  img[][1364][3],
                      unsigned char  img_final[][1364][3],
                      void *address_of_filter){ 

    void (*filter)(unsigned char [][1364][3], unsigned char [][1364][3], int, int) = (void (*)(unsigned char [][1364][3], unsigned char [][1364][3], int, int)) address_of_filter;
    int w, h, b, i, j, k, offset_h, offset_v, m, n;
    int start_left, end_left, start_top, end_top;
    unsigned char ***img_chunk;
    unsigned char ***img_chunk_final;
    double sum;
    h = 1364;
    w = 1364;
    b = 3;

    start_left = 2;
    end_left = w - 2;
    start_top = 2;
    end_top = h - 2;

    printf("here we are\n");

    for(j=start_top; j<end_top; ++j){
        for(i=start_left; i<end_left; ++i) {
            //convolve for each of r, g, b channels
            // addr( & arr[ row ][ col ] ) = addr( arr ) + [ sizeof( int ) * COLS * row ]
            //                                              + [ sizeof( int ) * col ]
            //img_chunk = (unsigned char ***) &img + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk_final = (unsigned char ***) &img_final + sizeof(char) * 1364 * j + sizeof(char) * i;
            //img_chunk = unsigned char ***) &img_final[j][i];
            //img_chunk_final = (unsigned char ***) &img_final[j][i];
            //filter(img_chunk, img_chunk_final);
            //filter((unsigned char (*)[1364][3]) img[j-2][k-2],
                   //(unsigned char (*)[1364][3]) img_final[j-2][k-2], j, i);
            filter(img, img_final, j, i);
        }
    }

    printf("some data %d", (int) img_final[4][4][0]);
}

void write_png(unsigned char img[][1364][3], int width, int height){
    stbi_write_png("fftest.png", width, height, 3, img, 3*width);
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

