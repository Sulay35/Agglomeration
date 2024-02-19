/*
// Created by aysul on 18/02/24.
*/
#include "../include/cell.h"

Cell::Cell(Vector2 p, Vector2 v, int st)
{
    pos = p;
    vel = v;
    state = st;
}

Cell::Cell(Vector2 p)
{
    pos = p;
    vel = Vector2(-1 + rand()%3, -1 +rand()%3);
    state = (int)rand()%2;
}

Cell::Cell(){
    pos.x = 0.0f;
    pos.y = 0.0f;
    vel = Vector2(-1 + rand()%3, -1 +rand()%3);
    state = (int)rand()%2;
}

/*
 * Update the cell : acceleration is added to velocity, and limits are applied
 *
 */
void Cell::Update(double dt){
    vel.add(&acc);

    acc.limit(maxAcc);
    vel.limit(maxSpeed);

    pos.add(&vel);
}

void Cell::keep_in_bounds(double minX, double maxX, double minY, double maxY){
    if(pos.x > maxX)
        pos.x = minX;
    else if(pos.x < minX)
        pos.x = maxX;

    if(pos.y > maxY)
        pos.y = minY;
    else if(pos.y < minY)
        pos.y = maxY;
}

void Cell::applyForce(Vector2 *force){
    acc.add(force);
}
