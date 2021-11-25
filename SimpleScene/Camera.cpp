#include "Camera.h"
#include "glfw3.h"
#include "windows.h"
#define _USE_MATH_DEFINES
#include "math.h"

SCamera cam = { 0, 0, 2.0f, 70, -40 };

void CameraApply()
{
    glRotatef(-cam.xRot, 1, 0, 0);
    glRotatef(-cam.zRot, 0, 0, 1);
    glTranslatef(-cam.x, -cam.y, -cam.z);
}

void CameraRotation(float xAngle, float zAngle)
{
    cam.xRot += xAngle;
    if (cam.xRot < 0) cam.xRot = 0;
    if (cam.xRot > 180) cam.xRot = 180;

    cam.zRot += zAngle;
    if (cam.zRot < 0) cam.zRot += 360;
    if (cam.zRot > 360) cam.zRot -= 360;
}

void CameraMoveByMouse(int centerX, int centerY, float speed)
{
    ShowCursor(false);
    POINT cur;

    static POINT base{ centerX, centerY };
    GetCursorPos(&cur);
    CameraRotation((base.y - cur.y) * speed, (base.x - cur.x) * speed);

    SetCursorPos(base.x, base.y);
}

void CameraMoveDirection(int forwardMove, int rightMove, float speed)
{
    float moveAngle = -cam.zRot / 180 * M_PI;
    float flightAngle = cam.xRot / 180 * M_PI;

    if (forwardMove > 0)
    {
        moveAngle += rightMove > 0 ? M_PI_4 : (rightMove < 0 ? -M_PI_4 : 0);
        flightAngle += M_PI;
    }

    if (forwardMove < 0)
        moveAngle += M_PI + (rightMove > 0 ? -M_PI_4 : (rightMove < 0 ? M_PI_4 : 0));
	
    if (forwardMove == 0)
    {
        moveAngle += rightMove > 0 ? M_PI_2 : -M_PI_2;
        flightAngle = M_PI_2;

        if (rightMove == 0) speed = 0;
    }

    if (speed != 0)
    {
        cam.x += sin(moveAngle) * speed;
        cam.y += cos(moveAngle) * speed;
        cam.z += cos(flightAngle) * speed;
    }
}

void SetCamPos(float x, float y, float z)
{
    cam.x = x;
    cam.y = y;
    cam.z = z;
}

void SetCamPos(float pos[3])
{
    cam.x = pos[0];
    cam.y = pos[1];
    cam.z = pos[2];
}
