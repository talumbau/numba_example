#include<stdio.h>
#include<math.h>

#include "image_filter.c"

main()
{
    int width, height, bpp, i, j, k, w, h, b;
    //unsigned char (*)[1364][3] ptr;
    //unsigned char* rgb = stbi_load( "austin_flights.png", &width, &height, &bpp, 3 );
    unsigned char* rgb = stbi_load( "landscape1.png", &width, &height, &bpp, 3 );
    //rgb is now three bytes per pixel, width*height size. Or NULL if load failed.

    double gKernel[5][5];
    create_filter(gKernel);
    for(i= 0; i < 5; ++i){
        printf("%f %f %f %f %f\n", gKernel[i][0], gKernel[i][1], gKernel[i][2], gKernel[i][3], gKernel[i][4]);
    }

    unsigned char img_filtered[500][848][3];

    printf("before apply \n");
    //apply_filter((unsigned char (*)[1364][3]) rgb, img_filtered);
    //apply_any_filter((unsigned char (*)[1364][3]) rgb, img_filtered, &gaussian_filter);
    apply_any_filter2((unsigned char (*)[848][3]) rgb, img_filtered, &gaussian_filter2);
    printf("after apply \n");

    stbi_write_png("landscape_filtered_c.png", width, height, 3, img_filtered, 3*width);
    printf("done\n");
    stbi_image_free(rgb);
}
