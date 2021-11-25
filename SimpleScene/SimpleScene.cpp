#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include "windows.h"
#include "math.h"
#define _USE_MATH_DEFINES
#include "Camera.h"

#define mapH 100
#define mapW 100
#define wallH 20

typedef struct
{
    float x, y, z;
} TCell;

typedef struct
{
    float r, g, b;
} TColor;

TColor whiteCol = { 1, 1, 1 };
TColor floorCol = { 0.7, 0.7, 1 };
TColor wallsCol = { 0.7, 1, 1 };

TCell map[mapW][mapH];
TCell wallsMapW[2][mapW][wallH];
TCell wallsMapH[2][mapH][wallH];

GLuint mapInd[mapW - 1][mapH - 1][6];
GLuint wallsMapWInd[mapW - 1][wallH - 1][6];
GLuint wallsMapHInd[mapH - 1][wallH - 1][6];

int mapIndCnt = sizeof(mapInd) / sizeof(GLuint);

void MapInit()
{
	// Floor //
	for (int i = 0; i < mapW; i++)
        for(int j = 0; j < mapH; j++)
        {
            map[i][j].x = i;
            map[i][j].y = j;
            map[i][j].z = 0;
        }

	for (int i = 0; i < mapW - 1; i ++)
	{
        int pos = i * mapW;
        for (int j = 0; j < mapH - 1; j++)
        {
            mapInd[i][j][0] = pos;
            mapInd[i][j][1] = pos + 1;
            mapInd[i][j][2] = pos + 1 + mapH;

            mapInd[i][j][3] = pos + 1 + mapH;
            mapInd[i][j][4] = pos + mapH;
            mapInd[i][j][5] = pos;

            pos++;
        }
	}

    // Walls for map height//
	for (int k = 0; k < 2; k++)
		for (int i = 0; i < mapH; i++)
			for (int j = 0; j < wallH; j++)
			{
                wallsMapH[k][i][j].x = k * mapW - k;
                wallsMapH[k][i][j].y = i;
                wallsMapH[k][i][j].z = j;
			}

    for (int i = 0; i < mapH - 1; i++)
    {
        int pos = i * wallH;
        for (int j = 0; j < wallH - 1; j++)
        {
            wallsMapHInd[i][j][0] = pos;
            wallsMapHInd[i][j][1] = pos + 1;
            wallsMapHInd[i][j][2] = pos + 1 + wallH;

            wallsMapHInd[i][j][3] = pos + 1 + wallH;
            wallsMapHInd[i][j][4] = pos + wallH;
            wallsMapHInd[i][j][5] = pos;

            pos++;
        }
    }

    // Walls for map width//
	for (int k = 0; k < 2; k++)
        for(int i = 0; i < mapW; i++)
            for(int j = 0; j < wallH; j++)
            {
                wallsMapW[k][i][j].x = i;
                wallsMapW[k][i][j].y = k * mapH - k;
                wallsMapW[k][i][j].z = j;
            }

    for (int i = 0; i < mapW - 1; i++)
    {
        int pos = i * wallH;
        for (int j = 0; j < wallH - 1; j++)
        {
            wallsMapWInd[i][j][0] = pos;
            wallsMapWInd[i][j][1] = pos + 1;
            wallsMapWInd[i][j][2] = pos + 1 + wallH;

            wallsMapWInd[i][j][3] = pos + 1 + wallH;
            wallsMapWInd[i][j][4] = pos + wallH;
            wallsMapWInd[i][j][5] = pos;

            pos++;
        }
    }
}

// Начальный размер окна
int winH = 600, winW = 800;

float spawn[] = {0, 0, 2};

GLFWwindow* window;

// Плоскость - элемент пола
float vert[] = { 1,1,0,  1,-1,0,  -1,-1,0, -1,1,0 };
// Дальность обзора камеры
float camViewRange = 100;

void WindowResize()
{
    glfwGetWindowSize(window, &winW, &winH);
    glViewport(0, 0, winW, winH);
    float k = winW / (float)winH;
    float sz = 0.1;
    glLoadIdentity();
    glFrustum(-k * sz, k * sz, -sz, sz, sz * 2, camViewRange);
}

void SceneInit()
{
    glEnable(GL_DEPTH_TEST);
    WindowResize();
    MapInit();
}

void SceneShow()
{
    glClearColor(0.0f, 0.7f, 0.7f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
		CameraApply();
		
	    glEnableClientState(GL_VERTEX_ARRAY);
			// Floor //
			glVertexPointer(3, GL_FLOAT, 0, map);
            for (int i = 0; i < mapW - 1; i++)
                for (int j = 0; j < mapH - 1; j++)
                {
                    if ((i + j) % 2 == 0) 
                        glColor3f(floorCol.r, floorCol.g, floorCol.b);
                    else             
                        glColor3f(whiteCol.r, whiteCol.g, whiteCol.b);
                	
                    glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, mapInd[i][j]);
                }
	
            // Wall for map height //
            for (int kk = 0; kk < 2; kk++)
            {
                glVertexPointer(3, GL_FLOAT, 0, wallsMapH[kk]);
                for (int i = 0; i < mapH - 1; i++)
                    for (int j = 0; j < wallH - 1; j++)
                    {
                        if ((i + j) % 2 == 0)
                            glColor3f(wallsCol.r, wallsCol.g, wallsCol.b);
                        else
                            glColor3f(whiteCol.r, whiteCol.g, whiteCol.b);

                        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, wallsMapHInd[i][j]);
                    }
            }

            // Wall for map width //
            for (int kk = 0; kk < 2; kk++)
            {
                glVertexPointer(3, GL_FLOAT, 0, wallsMapW[kk]);
                for (int i = 0; i < mapW - 1; i++)
                    for (int j = 0; j < wallH - 1; j++)
                    {
                        if ((i + j) % 2 == 0)
                            glColor3f(whiteCol.r, whiteCol.g, whiteCol.b);
                        else
                            glColor3f(wallsCol.r, wallsCol.g, wallsCol.b);
                            

                        glDrawElements(GL_TRIANGLE_FAN, 6, GL_UNSIGNED_INT, wallsMapWInd[i][j]);
                    }
            }
	    glDisableClientState(GL_VERTEX_ARRAY);
    glPopMatrix();
}

void CameraMove()
{
    if (GetKeyState('R') < 0) SetCamPos(spawn);

    if (!glfwGetWindowAttrib(window, GLFW_FOCUSED)) return;
	
    CameraMoveDirection(
        GetKeyState('W') < 0 ? 1 : (GetKeyState('S') < 0 ? -1 : 0)
        ,GetKeyState('D') < 0 ? 1 : (GetKeyState('A') < 0 ? -1 : 0)
        ,0.2f
    );
	
    CameraMoveByMouse(300, 300, 0.2);
}

int main(void)
{
    // Подключаем OpenGL
    if (!glfwInit())
        return -1;

    // Создаём окно
    window = glfwCreateWindow(winW, winH, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Делаем окно текущим
    glfwMakeContextCurrent(window);

    SceneInit();
	
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        CameraMove();
    	
        SceneShow();

        WindowResize();
    	
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    	
        /* Poll for and process events */
        glfwPollEvents();
    	
        if (GetKeyState(VK_ESCAPE) < 0) break;
    }

    glfwTerminate();
    return 0;
}