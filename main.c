// CS 2110 Summer 2017
// AUTHOR: Matthew Krupczak

#include <stdlib.h>
#include <stdio.h>
// include my weird header stuff
#include "mylib.h"

// include provided text stuff
#include "text.h"

// need to include all pre-baked "images" @TODO
#include "splash.h"
#include "blueskies.h"
#include "gameover.h"

#include "bird.h" // bird is 17x12


// some code


enum STATE {
  SPLASH,
  PLAY,
  GAMEOVER
};

// tells us what state we're in
enum STATE gameState = SPLASH;

char strBuffer[16];


int main() {
  REG_DISPCTL = MODE3 | BG2_ENABLE;
  stateSplash();



  // this below shouldn't happen,
  //     but if it does it just hangs endlessly
  vu16 stahp;
  while(1) {
    stahp++;
  }
  return 0;
}

void stateSplash() {
  gameState = SPLASH;
  waitForVBlank();
  drawImage3(0, 0, 240, 160, splash);

  delay(20);
  while(gameState == SPLASH) {
    if (ANY_KEY_PRESSED) {
      statePlay();
    }
  }  
}


void statePlay() {
  gameState = PLAY;  
  delay(20);
  waitForVBlank();
  drawImage3(0, 0, 240, 160, blueskies);


  
  int pipeWidth = 10;
  int shortPipeHeight = 28;
  int mediumPipeHeight = 56; // default 56
  int longPipeHeight = 84;
  int pipeXPOS = (240 - pipeWidth);

  struct pipe {
    int width;
    int height;
    int yPOS;
  };
  
  // width, height, yPOS
  struct pipe topPipe;
  topPipe.width = pipeWidth;
  topPipe.height = mediumPipeHeight;
  topPipe.yPOS = 0;
  
  struct pipe bottomPipe;
  bottomPipe.width = pipeWidth;
  bottomPipe.height = mediumPipeHeight;
  bottomPipe.yPOS = 160 - mediumPipeHeight;
  
  int birdWidth = 17;
  int birdHeight = 12;
  int birdXPOS = 10;
  int birdYPOS = 74;

  /* int birdXVELO = 0; */
  int birdYVELO = 0;

  int recipBirdGravity = 10;
  int inputDelay = 10;
  
  int counter = 0;

  int score = 0;
  
  bool jumpPressed = false;
  
  /* bool leftPressed = false; */
  /* bool rightPressed = false; */

  sprintf(strBuffer, "Score: %d" , score);
  drawString(1, 20, strBuffer, WHITE);
  
  while (gameState == PLAY) {
    counter++;
    waitForVBlank();
    // Erase old bird on video buffer (if it existed)
    drawRect(birdYPOS,
	     birdXPOS,
	     birdHeight,
	     birdWidth,
	     SKYBLUE);
    //

    // gameover if bird is out of bounds
    if (objectOutOfBounds(birdYPOS, birdXPOS, birdWidth, birdHeight)) {
      stateGameOver();
      break;
    }
    
    // gameover if bird hits top pipe
    if(objectsColliding(birdXPOS,
			birdYPOS,
			birdWidth,
			birdHeight,
			pipeXPOS,
			topPipe.yPOS, // top pipe
			topPipe.width,
			topPipe.height)) {
      //
      stateGameOver();
      break;
    }

    // gameover if bird hits bottom pipe
    if(objectsColliding(birdXPOS,
			birdYPOS,
			birdWidth,
			birdHeight,
			pipeXPOS,
			bottomPipe.yPOS, // bottom pipe
			bottomPipe.width,
			bottomPipe.height)) {
      //
      stateGameOver();
      break;
    }
    
    // debug/easy gameover
    if (KEY_DOWN_NOW(BUTTON_START)) {
      stateGameOver();
      break;
    }
    
    if (KEY_DOWN_NOW(BUTTON_SELECT)) {
      stateSplash();
      break;
    }

    if (KEY_DOWN_NOW(BUTTON_LEFT)) {
      /* leftPressed = true; */
      birdXPOS -= 1;
    }

    if (KEY_DOWN_NOW(BUTTON_RIGHT)) {
      /* rightPressed = true; */
      birdXPOS += 1;
    }    

    // A, B, L, R, UP, DOWN --> jump
    if (ANY_KEY_PRESSED
	&&
	!(KEY_DOWN_NOW(BUTTON_START))
	&&
	!(KEY_DOWN_NOW(BUTTON_SELECT))
	&&
	!(KEY_DOWN_NOW(BUTTON_LEFT))
	&&
	!(KEY_DOWN_NOW(BUTTON_RIGHT))) {
      //
      jumpPressed = true;
    }
	       
    if (jumpPressed) {
      if (counter % inputDelay == 0) {
	// 160 is down and 0 is up
	birdYVELO -= 6;
	jumpPressed = false;
      }
    }


    
    if (counter % recipBirdGravity == 0) {
      // 160 is down and 0 is up
      birdYVELO += 1;
      birdYPOS += birdYVELO;
    }


    // draw the bird
    drawImage3(birdYPOS, birdXPOS, birdWidth, birdHeight, bird);    
    
    delay(1);

    
    pipeXPOS -= PIPEVELO;
    if (pipeXPOS < 0) {
      pipeXPOS += 240;

      score++;
    }

    if (pipeXPOS > 237) {
      // draw the current score:
      /* sprintf(strBuffer, "       %d", score - 1); */
      /* drawString(1, 20, strBuffer, SKYBLUE);    */   

      
      sprintf(strBuffer, "Score: %d" , score);
      drawString(1, 20, strBuffer, WHITE);    

    }

    
    // Only draw if won't cause any graphics glitches
    if (pipeXPOS < (240 - topPipe.width)
	&&
	pipeXPOS > topPipe.width
	&&
	pipeXPOS < (240 - bottomPipe.width)
	&&
	pipeXPOS > (bottomPipe.width)) {
      // bottom pipe    
      drawRect(bottomPipe.yPOS,
	       pipeXPOS,
	       bottomPipe.height,
	       bottomPipe.width,
	       GREEN);

      // top pipe
      drawRect(topPipe.yPOS,
	       pipeXPOS,
	       topPipe.height,
	       topPipe.width,
	       GREEN);
    } else {
      // if the pipe(s) are not being drawn,
      //     we can change the location of the gap.
      //     this is done on a 4-based cycle where the gap
      //     starts in the middle and moves from top to bottom
      //     in a wave-like pattern.

      // gap in middle
      if (score % 2 == 0) {
	topPipe.height = mediumPipeHeight;
	bottomPipe.height = mediumPipeHeight;
	bottomPipe.yPOS = 160 - bottomPipe.height;
      } else {
	// gap near top
	if (score % 4 == 1) {
	  topPipe.height = shortPipeHeight;
	  bottomPipe.height = longPipeHeight;
	  bottomPipe.yPOS = 160 - bottomPipe.height;	  
	}
	// gap near bottom
	if (score % 4 == 3) {
	  topPipe.height = longPipeHeight;
	  bottomPipe.height = shortPipeHeight;
	  bottomPipe.yPOS = 160 - bottomPipe.height;	  
	}
      }
    }
    // remove old bottom pipe
    drawRect(bottomPipe.yPOS,
	     pipeXPOS + bottomPipe.width + PIPEVELO,
	     bottomPipe.height,
	     PIPEVELO,
	     SKYBLUE);
    
    // remove old top pipe
    drawRect(topPipe.yPOS,
	     pipeXPOS + topPipe.width + PIPEVELO,
	     topPipe.height,
	     PIPEVELO,
	     SKYBLUE);
    
  }
}

void stateGameOver() {
  gameState = GAMEOVER;
  delay(1);
  waitForVBlank();  
  drawImage3(0, 0, 240, 160, gameover);
  while(1) {
    if (KEY_DOWN_NOW(BUTTON_SELECT)) {
      stateSplash();
      break;
    }
  }
}

bool objectOutOfBounds(int yPOS, int xPOS, int width, int height) {
  bool output = false;
  
  if (xPOS + width > 240) {
    output = true;
  }

  if (xPOS < 0) {
    output = true;
  }

  if (yPOS + height > 160) {
    output = true;
  }

  if (yPOS < 0) {
    output = true;
  }

  return output;
}

bool objectsColliding(int aXPOS,
		      int aYPOS,
		      int aWidth,
		      int aHeight,
		      int bXPOS,
		      int bYPOS,
		      int bWidth,
		      int bHeight) {
  //
  bool output = false;
  
  if (aXPOS < bXPOS + bWidth
      &&
      aXPOS + aWidth > bXPOS
      &&
      aYPOS < bYPOS + bHeight
      &&
      aYPOS + aHeight > bYPOS)  {
    //
    output = true;
  }
  
  return output;
}

