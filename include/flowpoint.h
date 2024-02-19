/*
// Created by aysul on 18/02/24.
*/
#ifndef GRID_FLOWPOINT_H
#define GRID_FLOWPOINT_H

#include "vector2.h"

/**
 * @brief A Flowpoint is a fixed point with a velocity. It can affect neighboring entities
 */
class FlowPoint{
public:

    /**
     * @brief Constructor with initial velocity at 0 and initial position specified
     * @param pos initial position
     */
    explicit FlowPoint(Vector2 pos);

    /**
     * @brief Constructor with initial velocity and initial position specified
     * @param pos initial position
     * @param vel initial velocity
     */
    FlowPoint(Vector2 pos, Vector2 vel);

    /**
     * @brief Constructor with initial velocity and initial position at 0
    */
    FlowPoint();

    /** Flowpoint position*/
    Vector2 pos;

    /** Flowpoint velocity*/
    Vector2 vel;
};
#endif /*GRID_FLOWPOINT_H*/
