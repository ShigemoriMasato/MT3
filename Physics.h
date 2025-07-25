#pragma once
#include "MyMath.h"

//バネ
void SpringMotion(Spring& spring, DecaltMotionBall& ball);

//円運動
void CircleMotion(CircleMotionBall& ball);

//振り子
void PendulumMotion(Pendulum& pendulum, Ball* ball);

//円錐振り子
void ConicalPendulumMotion(ConicalPendulum& conicalPendulum, Ball* ball);