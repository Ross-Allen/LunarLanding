#include <cmath>
#include "LunarLander.hpp"

//Vector definition

Lander::Lander() {
  gravity.x = 0;
  gravity.y = 0;
  angle = 0;
  velocity.x = 0;
  velocity.y = 0;
  fuel = 0;
  position.x = 0;
  position.y = 0;
  maxTouchdownSpeed = 0;
  crashed = false;
  landed = false;
  rotatedThisTurn = 0;
  fuelBurnedThisTurn = 0;
}

Lander::Lander(Vect iG, double iangle, Vect ivelocity, double ifuel, Vect iposition, double imax, bool icrashed, bool ilanded) {
  thrust.x = 0.0;
  thrust.y = 0.0;
  gravity = iG;
  angle = iangle;
  velocity = ivelocity;
  fuel = ifuel;
  position = iposition;
  maxTouchdownSpeed = imax;
  crashed = icrashed;
  landed = ilanded;
}

//accessors
double Lander::getAngle() {
  return angle;
}

Vect Lander::getVelocity() {
  return velocity;
}

double Lander::getFuel() {
  return fuel;
}

Vect Lander::getPosition() {
  return position;
}

double Lander::getMTS() {
  return maxTouchdownSpeed;
}

bool Lander::getCrashed() {
  return crashed;
}

bool Lander::getLanded() {
  return landed;
}

//controls
void Lander::rotateLeft(double rotateAngle) {
  if (rotatedThisTurn <= (PI / 12)) {
    if (rotateAngle < 0) {
      rotateAngle = (-1 * rotateAngle);
    }
    rotatedThisTurn += rotateAngle;
    return;
  }
  return;
}
// max rotation per sec is pi/12
// rotateAngle should be a
// positive number.  This function
// affects only the variable
// rotatedThisTurn.

void Lander::rotateRight(double rotateAngle) {
  if (rotatedThisTurn >= (-PI / 12) && rotateAngle > 0) {
    if (rotateAngle < 0) {
      rotateAngle = (-1 * rotateAngle);
    }
    rotatedThisTurn -= rotateAngle;
  }
}
// max rotation per sec is pi/12
// rotateAngle should be a
// positive number.  This function
// affects only the variable
// rotatedThisTurn.

void Lander::burn(double fuelAmount) {
  if (fuelBurnedThisTurn <= 50 && fuelAmount > 0) {
    fuelBurnedThisTurn += fuelAmount;
  }
  if(fuelBurnedThisTurn > 50){
    fuelBurnedThisTurn = 50;
  }
}
// max fuelBurn per sec is 50;
// fuelAmount should be a
// positive number.  This function
// affects only the variable
// fuelBurnedThisTurn

void Lander::timeUpdate() {
  fuel -= fuelBurnedThisTurn;
  angle += rotatedThisTurn;

  thrust.x = cos(angle) * fuelBurnedThisTurn;
  thrust.y = sin(angle) * fuelBurnedThisTurn;

  if (fuel == 0) {
    thrust.x = 0;
    thrust.y = 0;
  }
  velocity.x = velocity.x + thrust.x + gravity.x;
  velocity.y = velocity.y + thrust.y + gravity.y;
  position.x = position.x + velocity.x;
  position.y = position.y + velocity.y;


  if (position.y < 0) {
    position.y = 0;
  }
  int tempAngle = (angle * (180 / PI));

  if (position.y == 0) {

    if (velocity.y >= ((-1) * maxTouchdownSpeed) && tempAngle == 90) {
      landed = true;
    } else {
      landed = false;
      crashed = true;
    }

  }

  rotatedThisTurn = 0;
  fuelBurnedThisTurn = 0;
  // this function uses
  // rotatedThisturn and
  // fuelBurnedThisTurn to simulate
  // the passage of 1 second of
  // time. this function updates
  // angle, thrust, velocity,
  // position, fuel, crashed, and
  // landed.  it also resets
  // rotatedThisTurn and
  // fuelBurnedThisTurn
}

