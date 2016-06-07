#include<stdio.h>
#include<math.h>

#include "image_filter.c"

main()
{
    int width, height, bpp, i, j, k, w, h, b;
    unsigned char* rgb = stbi_load( "landscape1.png", &width, &height, &bpp, 3 );
    //rgb is now three bytes per pixel, width*height size. Or NULL if load failed.

    //Raw memory for the filtered image
    unsigned char img_filtered[500][848][3];
    apply_any_filter((unsigned char (*)[848][3]) rgb, img_filtered, &gaussian_filter);

    stbi_write_png("landscape_filtered_c.png", width, height, 3, img_filtered, 3*width);
    printf("done\n");
    stbi_image_free(rgb);
}
