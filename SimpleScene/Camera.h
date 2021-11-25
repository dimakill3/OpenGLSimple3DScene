#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#pragma once
struct SCamera
{
    float x, y, z;
    float xRot, zRot;
};

void CameraApply();

void CameraRotation(float xAngle, float zAngle);

void CameraMoveByMouse(int centerX, int centerY, float speed);

void CameraMoveDirection(int forwardMove, int rightMove, float speed);

void SetCamPos(float x, float y, float z);

void SetCamPos(float pos[3]);

#endif // CAMERA_H_INCLUDED