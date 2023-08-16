#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <ctime>
#include <cmath>
#include <string>
#include <Windows.h>
char direction = 'R';
const int gridsize = 50;
int snakelength = 5;
int prevtime = 0;
int currenttime = 0;
float ratio;
float gridscale = 1.0 / gridsize;
bool pause = false;
bool isescpress = false;
bool iskeypress = true;

class unitobject {
public:
	float LocX = 0.0, LocY = 0.0;
	float unitW = 1.0 / gridsize * 1.0, unitH = 1.0 / gridsize * 1.0;
	float R = 1.0, G = 1.0, B = 1.0, A = 0.5;
	void spawn() {
		LocX = ((rand() % (gridsize * 2) - gridsize) / (float)gridsize);
		LocY = ((rand() % (gridsize * 2) - gridsize) / (float)gridsize);
		LocY += 1.0 / gridsize;
		R = 0.9;
		G = 0.1;
		B = 0.1;
	}
	void draw() {
		glColor4f(R, G, B, A);
		glBegin(GL_QUADS);
		glVertex3f(LocX, LocY, 0.0);
		glVertex3f(LocX + unitW, LocY, 0.0);
		glVertex3f(LocX + unitW, LocY - unitH, 0.0);
		glVertex3f(LocX, LocY - unitH, 0.0);
		glEnd();
	}
}unit;
unitobject snake[gridsize * gridsize];
//unitobject snakeshadow[gridsize * 2 * gridsize * 2];
unitobject bait;
unitobject baitshadow;
unitobject stones[gridsize * gridsize];
unitobject backg[gridsize * gridsize];
void error_callback(int error, const char* description) {
	fprintf(stderr, "Error: %s\n", description);
}
void setbuffersizecallbackfunc(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (width > height) {
		ratio = (float)width / (float)height;
		glOrtho(-ratio, ratio, -1.0, 1.0, 1.0, -1.0);
	}
	if (width < height) {
		ratio = (float)height / (float)width;
		glOrtho(-1.0, 1.0, -ratio, ratio, 1.0, -1.0);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
float collisiondet(float aX, float aY, float bX, float bY) {
	float dx = aX - bX;
	float dy = aY - bY;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}
void shiftarray() {
	for (int i = snakelength; i >= 0; i--) {
		snake[i + 1].LocX = snake[i].LocX;
		snake[i + 1].LocY = snake[i].LocY;
	}
}
void selfcollision() {
	for (int i = 1; i <= snakelength; i++) {
		if (collisiondet(snake[0].LocX, snake[0].LocY, snake[i].LocX, snake[i].LocY) * 1.1 <= 1.0 / gridsize) {
			printf("self collusion");
			Beep(600, 300);
			Beep(550, 300);
			Beep(500, 300);
			Beep(450, 300);
			Beep(400, 800);
			snakelength = 5;
			direction = 'R';
			snake[0].LocX = 2 * gridscale;
			snake[0].LocY = 0;
			snake[1].LocX = 1 * gridscale;
			snake[1].LocY = 0;
			snake[2].LocX = 0 * gridscale;
			snake[2].LocY = 0;
			snake[3].LocX = -1 * gridscale;
			snake[3].LocY = 0;
			snake[4].LocX = -2 * gridscale;
			snake[4].LocY = 0;
		}
	}
}
void baitshadowspawn() {
	baitshadow = bait;
	//baitshadow.LocX += (float)gridscale / 5.0;
	//baitshadow.LocY -= (float)gridscale / 5.0;
	baitshadow.R = 0.5;
	baitshadow.G = 0.1;
	baitshadow.B = 0.1;
}
int main() {
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())printf("GLFW Initialization failed!");
	GLFWwindow* window = glfwCreateWindow(500, 500, "Snake 0.4 Remastered :)", NULL, NULL);
	glfwSetWindowSizeCallback(window, setbuffersizecallbackfunc);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSwapInterval(0);
	glfwSetWindowSize(window, 800, 800);
	glfwSetWindowPos(window, 100, 30);
	glfwWindowHint(GLFW_SAMPLES, 16);
	if (!window)printf("An error occured during GLFW Window creating!");
	srand(time(NULL));
	prevtime = glfwGetTime();
	for (int i = 0; i < gridsize * gridsize; i++)
	{
		if (rand() % 100 == 0)
		{
			stones[i].spawn();
		}
	}
	direction = 'R';
	snake[0].LocX = 2 * gridscale;
	snake[0].LocY = 0;
	snake[1].LocX = 1 * gridscale;
	snake[1].LocY = 0;
	snake[2].LocX = 0 * gridscale;
	snake[2].LocY = 0;
	snake[3].LocX = -1 * gridscale;
	snake[3].LocY = 0;
	snake[4].LocX = -2 * gridscale;
	snake[4].LocY = 0;
	//stones->spawn();
	bait.spawn();
	baitshadowspawn();
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) isescpress = true;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE) {
			if (pause == false && isescpress == true) {
				pause = true;
				isescpress = false;
			}
			else if (pause == true && isescpress == true) {
				pause = false;
				isescpress = false;
			}
		}
		if (pause == false) {

			if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS && direction != 'D' && iskeypress == false) {
				direction = 'U';
				iskeypress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS && direction != 'U' && iskeypress == false) {
				direction = 'D';
				iskeypress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS && direction != 'L' && iskeypress == false) {
				direction = 'R';
				iskeypress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS && direction != 'R' && iskeypress == false) {
				direction = 'L';
				iskeypress = true;
			}
			if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
				currenttime = glfwGetTime() * 50;
			}

			else currenttime = glfwGetTime() * 10;
			printf("%d\n", currenttime);
			if (prevtime != currenttime) 
			{
				if (direction == 'U') {
					selfcollision();
					shiftarray();
					snake[0].LocY += gridscale;
					iskeypress = false;
				}
				if (direction == 'D') {
					selfcollision();
					shiftarray();
					snake[0].LocY -= gridscale;
					iskeypress = false;
				}
				if (direction == 'R') {
					selfcollision();
					shiftarray();
					snake[0].LocX += gridscale;
					iskeypress = false;
				}
				if (direction == 'L') {
					selfcollision();
					shiftarray();
					snake[0].LocX -= gridscale;
					iskeypress = false;
				}
				if (snake[0].LocX >= 1)snake[0].LocX = -1;//Hitwall
				if (snake[0].LocX <= -1-gridscale)snake[0].LocX = 1 - gridscale;
				if (snake[0].LocY >= 1+gridscale)snake[0].LocY = -1 + gridscale;
				if (snake[0].LocY <= -1)snake[0].LocY = 1.0;
				prevtime = currenttime;
				if (collisiondet(snake[0].LocX, snake[0].LocY, bait.LocX, bait.LocY) * 1.1 <= gridscale) {//Snake and bait collision detection
					bait.spawn();
					baitshadowspawn();
					for (int i = 1; i <= snakelength; i++) {//Bait respawn collision check on snake
						if (collisiondet(snake[i].LocX, snake[i].LocY, bait.LocX, bait.LocY) * 1.1 <= gridscale) {
							bait.spawn();
							baitshadowspawn();
							//printf("respawn");
						}
					}
					snakelength += 1;
					Beep(500, 50);
				}
				for (int i = 0; i < gridsize * gridsize; i++) {//Bait respawn collision check with stones
					if (collisiondet(bait.LocX, bait.LocY, stones[i].LocX, stones[i].LocY) * 1.1 <= gridscale) {
						bait.spawn();
						baitshadowspawn();
						printf("respawn");
					}
				}
				//for (int i = 0; i < gridsize * gridsize; i++) {//Snake and stones collision check
				//	if (collisiondet(snake[0].LocX, snake[0].LocY, stones[i].LocX, stones[i].LocY) * 1.1 <= gridscale) {
				//		snakelength = 5;
				//		snake[0].LocX = 0;
				//		snake[0].LocY = 0;
				//		direction = 'R';
				//	}
				//}
			}
		}
		glClearColor(0.1, 0.1, 0.1, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		//glScalef(0.9, 0.9, 0.0);
		//glRotatef(25, 0.0, 0.0, 1.0);

		glBegin(GL_QUADS);//Background
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(-1.0, 1.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(1.0, -1.0, 0.0);
		glVertex3f(-1.0, -1.0, 0.0);
		glEnd();
		for (int i = 0; i < gridsize; i++)
		{
			stones[i].R = 0.5;
			stones[i].G = 0.6;
			stones[i].B = 0.5;
			//stones[i].draw();
		}
		for (int i = 0; i <= snakelength; i++) {
			//snake[i].R = i * (0.8 / snakelength);
			//snake[i].G = i * (1.0 / snakelength);
			//snake[i].B = i * (0.8 / snakelength);
			snake[i].draw();

		}
		baitshadow.draw();
		if (currenttime % 2 == 0)bait.draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}