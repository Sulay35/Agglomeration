/*
// Created by aysul on 18/02/24.
*/
#ifndef GRID_CELL_H
#define GRID_CELL_H

#include "vector2.h"
#define MAX_SPEED 4.0
#define MAX_ACC 7.0

/**
 * Cell is a living pixel, it has a position and a velocity, a state
*/
class Cell{
public:

    /**
     * @brief This constructor initialize all members attributes
     *
     * @param p initial position
     * @param v initial velocity
     * @param st initial state
     */
    Cell(Vector2 p, Vector2 v, int st);

    /**
     * @brief This constructor initialize the position
     * the cell will have a random velocity
     *
     * @param p initial position
     */
    explicit Cell(Vector2 p);

    /**
     * @brief This constructor sets the initial position to zero
     * the cell will have a random velocity, state.
     */
    Cell();

    /**
     * @brief Update the position and state of the cell
     */
    void Update(double dt);

    /**
     * @brief Keep the cell inbounds
     * @param minX lower X-axis bound
     * @param maxX upper X-axis bound
     * @param minY lower Y-axis bound
     * @param maxY upper Y-axis bound
     */
    void keep_in_bounds(double minX, double maxX, double minY, double maxY);

    /**
     * @brief apply a force to the cell
     * @param force the force to apply
     * @example applyForce(Vector2(0,1,0)) apply a up force
     */
    void applyForce(Vector2 *force);

    /**
     * @var position
     */
    Vector2 pos;

    /**
     * @var velocity
     */
    Vector2 vel;

    /**
     * @var acceleration
     */
    Vector2 acc;

    double maxSpeed = MAX_SPEED;
    double maxAcc = MAX_ACC;

    int state;
};
#endif /*GRID_CELL_H*/
