#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

void paint(rgb_t *pixel,rgb_t color);

unsigned char grayscale(rgb_t rgb){
    return ((0.3*rgb.red) + (0.6*rgb.green) + (0.1*rgb.blue));
}


// converts an RGB image in an Gray image
void rgb_to_gray(app_t *newEdit){
    
    for(uint32_t i = 0; i < newEdit->opened_image->infoDIB.height;i++){
        for(uint32_t j = 0;j < newEdit->opened_image->infoDIB.width;j++){
            newEdit->opened_image->rgb[i][j].red = grayscale(newEdit->opened_image->rgb[i][j]);
            newEdit->opened_image->rgb[i][j].green = grayscale(newEdit->opened_image->rgb[i][j]);
            newEdit->opened_image->rgb[i][j].blue = grayscale(newEdit->opened_image->rgb[i][j]);
        }
    }
}

void create_bw_image(app_t *newEdit){
    
    FILE *fp = fopen("new.bmp","rb+");
    rgb_to_gray(newEdit);
    write_image(newEdit);
    
    fclose(fp);
}

//Make an image with TEXT
void image_to_text(app_t *newEdit){

    FILE *fp = fopen("new.bmp","rb+");
    

    char txtPixel[] = {'#','$','%','&','Q','Y','U','O'};
    unsigned char gs;

    for(uint32_t i = 0;i < newEdit->opened_image->infoDIB.height;i++){
        for(uint32_t j = 0; j < newEdit->opened_image->infoDIB.width;j++){
            gs = grayscale(newEdit->opened_image->rgb[i][j]);
            printf("%c",txtPixel[gs/32]);
        }
        printf("\n");
    }

    
    fclose(fp);
}


// this function use Bresenham's Line Algorithm to draw lines
void draw(app_t *newEdit,int x1,int y1,int x2,int y2){

    int32_t dx = abs((int32_t)x2 - (int32_t)x1);
    int32_t sx = x1 < x2 ? 1 : -1;
    int32_t dy = -abs((int32_t)y2 - (int32_t)y1);
    int32_t sy = y1 < y2 ? 1 : -1;
    int32_t err = dx + dy;

    while (1) {
        paint(&newEdit->opened_image->rgb[x1][y1],newEdit->brush_settings.brush_color);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        int32_t e2 = 2 * err;

        if (e2 >= dy) {
            err += dy;
            x1 += sx;
        }

        if (e2 <= dx) {
            err += dx;
            y1 += sy;
        }
    }
    
}


//function that give color to  the pixel for drawing
void paint(rgb_t *pixel,rgb_t color){
    rgb_t temp;
    temp.red = color.red;
    temp.green = color.green;
    temp.blue = color.blue;
    *pixel = temp;
}


// function that draw lines on the image
//INFO:it's draws only simple straight and diagonals lines
void draw_line(app_t *newEdit){
   
    int x1,y1,x2,y2;
    printf("Enter the coordinates\n");
    printf("x1 y1 x2 y2:\n");
    scanf("%d %d %d %d",&x1,&y1,&x2,&y2);

    for(int i = 0; i<=newEdit->brush_settings.brush_size;i++){
        if(x1 == x2){
            draw(newEdit,x1+i,y1,x2+i,y2);
        }else if(y1 == y2){
            draw(newEdit,x1,y1+i,x2,y2+i);
        }else{
            draw(newEdit,x1+i,y1,x2+i,y2);
        }   
    }

    write_image(newEdit);
    
}

//function that draws the square on the image
void draw_square(app_t *newEdit){
    

    int x1,y1,x2,y2;
    printf("Enter the coordinates\n");
    printf("x1 y1 x2 y2:\n");
    scanf("%d %d %d %d",&x1,&y1,&x2,&y2);

    for(int i = 0; i<=newEdit->brush_settings.brush_size;i++){
        draw(newEdit,x1+i,y1,x1+i,y2);
        draw(newEdit,x2+i,y1,x2+i,y2);
        draw(newEdit,x1,y1+i,x2,y1+i);
        draw(newEdit,x1,y2+i,x2,y2+i);
        draw(newEdit,x2+i,y1+i,x2+i,y2+i);
        draw(newEdit,x1+i,y2+i,x2+i,y2+i);
        
    }

    write_image(newEdit);

}


//function that draws the triangle on the image
void draw_triangle(app_t *newEdit){

    int x1,y1,x2,y2,x3,y3;
    printf("Enter the coordinates\n");
    printf("x1 y1 x2 y2 x3 y3:\n");
    scanf("%d %d %d %d %d %d",&x1,&y1,&x2,&y2,&x3,&y3);

    //I used here x2 y2 as the top angle of triangle  
    for(int i = 0; i<=newEdit->brush_settings.brush_size;i++){
        if(x1 == x3){
            draw(newEdit,x1+i,y1,x3+i,y3);
        }else if(y1 == y3){
            draw(newEdit,x1,y1+i,x3,y3+i);
        }
        
        draw(newEdit,x1+i,y1,x2+i,y2);
        draw(newEdit,x3+i,y3,x2+i,y2);
           
    }

    write_image(newEdit);
}

// function where you change the color and the size of the brush
void mod_cl_or_size(app_t *newEdit){
    int num;

    printf("What color do you want your brush to be?\n");
    printf("1.White\n");
    printf("2.Black\n");
    printf("3.Green\n");
    printf("4.Blue\n");
    printf("5.Red\n");
    scanf("%d",&num);

    switch(num){
        case 1:
            newEdit->brush_settings.brush_color.red = 255;
            newEdit->brush_settings.brush_color.green = 255;
            newEdit->brush_settings.brush_color.blue = 255;
            break;
        case 2:
            newEdit->brush_settings.brush_color.red = 0;
            newEdit->brush_settings.brush_color.green = 0;
            newEdit->brush_settings.brush_color.blue = 0;
            break;
        case 3:
            newEdit->brush_settings.brush_color.red = 0;
            newEdit->brush_settings.brush_color.green = 255;
            newEdit->brush_settings.brush_color.blue = 0;
            break;
        case 4:
            newEdit->brush_settings.brush_color.red = 255;
            newEdit->brush_settings.brush_color.green = 0;
            newEdit->brush_settings.brush_color.blue = 0;
            break;
        case 5:
            newEdit->brush_settings.brush_color.red = 0;
            newEdit->brush_settings.brush_color.green = 0;
            newEdit->brush_settings.brush_color.blue = 255;
            break;
        default:
            break;
        
    }

    printf("What size do you want the brush to be?\n");
    printf("1.1 pixel\n");
    printf("2.3 pixels\n");
    printf("3.5 pixels\n");
    scanf("%d",&num);
    switch (num)
    {
    case 1:
        newEdit->brush_settings.brush_size = 1;
        break;
    case 2:
        newEdit->brush_settings.brush_size = 3;
        break;
    case 3:
        newEdit->brush_settings.brush_size = 5;
        break;
    default:
        break;
    }
    
}


// function from where you can change the image
void general_functions(app_t *newEdit){
    
    int num = 1;
    //if you dont change the color it's will be set from the start as black
    newEdit->brush_settings.brush_color.red = 0;
    newEdit->brush_settings.brush_color.green = 0;
    newEdit->brush_settings.brush_color.blue = 0;

    while(num != 0){
        printf("What do you want to change?\n");
        printf("0.Leave\n");
        printf("1.Make it gray\n");
        printf("2.Print the image in text (in terminal)\n");
        printf("3.Draw a line\n");
        printf("4.Draw a square\n");
        printf("5.Draw a triangle\n");
        printf("6.Change the color or the size of the brush\n");
        scanf("%d",&num);

        if(num == 0)break;

        switch (num){
        case 1:
            create_bw_image(newEdit);
            break;
        case 2:
            image_to_text(newEdit);
            break;
        case 3:
            draw_line(newEdit);
            break;
        case 4:
            draw_square(newEdit);
            break;
        case 5:
            draw_triangle(newEdit);
            break;
        case 6:
            mod_cl_or_size(newEdit);
            break;
        default:
            break;
        }
    }

}
