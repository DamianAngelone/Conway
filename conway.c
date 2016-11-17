//opengl cross platform includes (final draft)
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <iostream>
#include <ctime>
using namespace std;
#define num 15 //the dimensions of the array.

int cells [num][num]; //main array where main changes occur between "days"
int copyCel [num][num]; //second array, where updates individually occur as the board is analyzed.
int oneCount = 0; //count of alive neighbours.
int value; //returned alive count
bool speed = true; //true when update speed is fast, false when its slow
bool pause = true; //false when updates are paused, true while running
int r1; //hold value for 'red' when deciding on square colour 
int g1; //hold value for 'green' when deciding on square colour
int b1; //hold value for 'blue' when deciding on square colour

/* function used to control all mouse interactions */
void mouse(int btn, int state, int x, int y){
	if(btn == GLUT_LEFT_BUTTON){ //uses left mouse button

		if (state == GLUT_UP){ //activates while mouse button is being raised (after press)
			
			int paintX = 0;
			int paintY = 0;
			float widthLength = (600/(num+2)); //the dimensions (length hand width) of each cell
			for(int i = 0; i < num; i++){
				for(int j = 0; j < num; j++){
					if((i+1)*widthLength < x && (i+2)*widthLength > x){
						if((j+1)*widthLength < y && (j+2)*widthLength > y){
							paintX = i;
							paintY = num - j - 1;

							if(cells[paintX][paintY] == 1){ //if clicked cell was alive, kill it
								printf("\nCell removed!\n");
								cells[paintX][paintY] = 0;
							}else{ //if clicked cell was dead, make it alive
								printf("\nCell added!\n");
								cells[paintX][paintY] = 1;
							}
						}
					}
				}
			}
		}
	}
}

/* function used to control redisplay between "days" for the board */
void FPS(int val){
	if(pause){
		glutPostRedisplay(); //marks the current display as the one that will be continuously repainted
		int n;

		n = speed ? 100: 2000; //sets the different speeds for fast and slow

		glutTimerFunc(n, FPS, 0); //repeated redisplays the screen with the desired time.
	}
}

/* creates the grid the cells will live on */
void createGrid(){

	glPointSize(1.0);
	glColor3f(0,0,0);

	glBegin(GL_POINTS);
	for (float i = 0; i <= num; i+=0.01){
		for (float j = 0; j <= num; j++){
			glVertex2f(i,j);
		}
	}
	for (float i = 0; i <= num; i++){
		for (float j = 0; j <= num; j+=0.01){
			glVertex2f(i,j);
		}
	}
}

/* colours squares to screen, green for alive and red for dead */
void colourSquare(int x, int y, float r, float g, float b){
	glColor3ub(r, g, b);
	glPointSize(1.0);
	glBegin(GL_POINTS);
	for (float i = 0.04; i < 0.96; i+=0.01){
		for (float j = 0.04; j < 0.96; j+=0.01){
			glVertex2f(x+i,y+j); 
		}
	}
}

/* function used to count how many alive cells are neighbouring a cell */
int countCell(int row, int col){

	int count = 0;
    for(int i = row - 1; i <= row + 1; i++) { //runs from before the cell in mention to after it (horizontally)
        if (i >= 0 && i < num){ //checks if number is in bounds of the array (border and corner cases)
            for(int j = col - 1; j <= col + 1; j++){  //runs from before the cell in mention to after it (vertically)
                if (j >= 0 && j < num){ //checks if number is in bounds of the array (border and corner cases)
                    if (i != row || j != col){ //doesn't count the cell that is in questio nas an alive cell
                        if (cells[i][j] == 1){ 
                            count++;
	}}}}}}
	return count;
}

/* uses countCell() to check if the current cell is alive or dead */
void checkArray(){
	for (int j = 0; j < num; j++){
		for (int i = 0; i < num; i++){
			value = countCell(i, j);
			if((cells[i][j] == 1) && (value == 3 || value == 2)){ //remains alive if surrounded by 2 or 3 alive cells.
				copyCel[i][j] = 1;
			}
			else if((cells[i][j] == 0) && (value == 3)){ //becomes alive if surrounded by 3 alive cells.
				copyCel[i][j] = 1;
			}
			else{ //cells dies due to not satisfying the above
				if(cells[i][j] == 1){
					copyCel[i][j] = 0;
				}else{
					copyCel[i][j] = 0;
				}
			}		
		}
	}
	for (int i = 0; i < num; i++){ //copies temp array to the main array to be displayed.
		for (int j = 0; j < num; j++){
			cells[i][j] = copyCel[i][j];
		}
	}
}

/* function used to  update the baord by displaying current alive cells */
void updateBoard(){

	glClearColor(0,0,0,0);

	for (int i = 0; i < num; i++){ //this sets colour of cell depending on it's state
		for (int j = 0; j < num; j++){
			if(cells[i][j] == 1){
				r1 = 0.0;
				g1 = 255.0;
				b1 = 0.0;
			}
			else{
				r1 = 255.0;
				g1 = 0.0;
				b1 = 0.0;
			}
			colourSquare(i, j, r1, g1, b1);
		}
	}
	checkArray();
}

/* displays UI to user */
void display(void){
	glClearColor(0.0,0.0,0.4,0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	createGrid();
	updateBoard();
	glEnd();
	glFlush();
}

/* initializes the array randomly, each cell being either alive or dead */
void initArray(){

	for (int i = 0; i < num; i++){
		for (int j = 0; j < num; j++){
			
			cells[i][j] = rand() % 2;
		}	
	}
}

/* deletes everything in the array, setting every cell to dead */
void clearArray(){

	for (int i = 0; i < num; i++){
		for (int j = 0; j < num; j++){
			
			cells[i][j] = 0;
		}	
	}
}

/* gives functionality to certain keyboard inputs */
void keyboard(unsigned char key, int xIn, int yIn){

	switch (key){
		case 'Q':
		case 'q':
		case 27:
			exit(0);
			break;
		case 'r':
		case 'R':
			initArray();
			printf("\nBoard is randomized!\n");
			break;
		case 'f':
		case 'F':
			speed = !speed;
			if(speed)
				printf("\nSpeed toggled to fast!\n");
			else
				printf("\nSpeed toggled to slow!\n");
			break;
		case 'p':
		case 'P':
			pause = !pause;
			if(pause){
				printf("\nProgram unpaused!\n");
				glutTimerFunc(0, FPS, 0);
			}
			else
				printf("\nProgram paused!\n");
			break;
		case 'c':
		case 'C':
				clearArray();
				printf("\nBoard cleared!\n");
			break;

	}
}

/* main function - program entry point */
int main(int argc, char** argv){
	
	printf("\n          OPTIONS\n");
	printf("***********************************\n");
	printf("* Press 'r' to randomize board.   *\n");
	printf("* Press 'q' to quit.              *\n");
	printf("* Press 'f' to toggle speed.      *\n");
	printf("* Press 'p' to pause game.        *\n");
	printf("* Press 'c' to clear game.        *\n");
	printf("***********************************\n");

	glutInit(&argc, argv);		//starts up GLUT
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200,200);
	srand(time(NULL));
	initArray();
	glutCreateWindow("Conway");	//creates the window
	glutDisplayFunc(display);	//registers "display" as the display callback function
	glutKeyboardFunc(keyboard);
	gluOrtho2D(-1, num + 1, -1, num + 1);
	glutMouseFunc(mouse);
	glutTimerFunc(0, FPS, 0);


	glutMainLoop();			//starts the event loop
	return(0);			//return may not be necessary on all compilers
}	