/*
// Created by aysul on 18/02/24.
*/

#include "../include/flowpoint.h"

FlowPoint::FlowPoint()
{
    pos = Vector2(0.0f,0.0f);
    vel = Vector2(0.0f,0.0f);
}

FlowPoint::FlowPoint(Vector2 p)
{
    pos = p;
    vel = Vector2(0.0f,0.0f);
}

FlowPoint::FlowPoint(Vector2 p, Vector2 v)
{
    pos = p;
    vel = v;
}