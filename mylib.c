// CS 2110 Summer 2017
// AUTHOR: Matthew Krupczak
#include <stdlib.h>
#include "mylib.h"

// VIDEOBUFFERADDRESS = 0x6000000
u16* videoBuffer = (u16*) VIDEOBUFFERADDRESS;

void setPixel(int x, int y, u16 color) {
  videoBuffer[y * 240 + x] = color;
  
}

// from 06-27Demo mylib.c
void drawRect(int row,
	      int col,
	      int height,
	      int width,
	      u16 color) {
  
  for (int r = 0; r < height; r++) {
    DMA[3].src = &color;
    DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
  }
  //
  // Old code w/o DMA
  /* for (int r = row; r < row + height; r++) { */
  /*   for (int c = col; c < col + width; c++) { */
  /*     setPixel(c, r, color); */
  /*   } */
  /* } */

  
}

// SCANLINECOUNTER must be volatile!
void waitForVBlank() {
  while(SCANLINECOUNTER > 160);
  while(SCANLINECOUNTER < 160);
}

void delay(int ticks) {
  // simmiliar to lecture code from a while back
  vs32 x;
  for (int i = 0; i < ticks * 8000; i++) {
    x++;
  }
}

// @TODO make a version of this that will
// cull drawing columns that are near edge
void drawImage3(int r,
		int c,
		int width,
		int height,
		const u16* image) {
  //
  for(int i = 0; i < height; i++) {
    // point to correct pixel in image
    DMA[3].src = image + (i * width);
    // point to correct pixel in video buffer
    DMA[3].dst = videoBuffer + OFFSET(i + r, c, 240);
    DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | width;    
  }
  
}
