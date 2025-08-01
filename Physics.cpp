#include "Physics.h"
#include "imgui.h"
#include "Collition.h"
#include <cmath>

using namespace MyMath;

void SpringMotion(Spring& spring, DecaltMotionBall& ball) {
	Vector3 diff = ball.position - spring.anchor;
	float length = diff.Length();

	if (std::abs(length) >= 1e-4) {
		Vector3 direction = diff.Normalize();
		Vector3 restPosition = spring.anchor * spring.naturalLength;
		Vector3 displacement = length * (ball.position - restPosition);
		Vector3 restoringForce = -spring.stiffness * displacement;
		Vector3 dampingForce = -spring.dampingCoefficient * ball.velocity;
		Vector3 force = restoringForce + dampingForce;
		ball.acceleration = force / ball.mass;
	}

	ball.velocity += ball.acceleration * ball.deltatime;
	ball.position += ball.velocity * ball.deltatime;

	ImGui::Begin("Spring Update");
	ImGui::Text("Ball Position: (%.2f, %.2f, %.2f)", ball.position.x, ball.position.y, ball.position.z);
	ImGui::Text("Ball Velocity: (%.2f, %.2f, %.2f)", ball.velocity.x, ball.velocity.y, ball.velocity.z);
	ImGui::Text("Ball Acceleration: (%.2f, %.2f, %.2f)", ball.acceleration.x, ball.acceleration.y, ball.acceleration.z);
	ImGui::Text("Spring Anchor: (%.2f, %.2f, %.2f)", spring.anchor.x, spring.anchor.y, spring.anchor.z);
	ImGui::Text("Spring Natural Length: %.2f", spring.naturalLength);
	ImGui::End();
}

void CircleMotion(CircleMotionBall& ball) {
	ball.angle += ball.angularVelocity * ball.deltatime;
	ball.position.x = ball.motionRadius * std::cos(ball.angle);
	ball.position.y = ball.motionRadius * std::sin(ball.angle);
}

void PendulumMotion(Pendulum& pendulum, Ball* ball) {
	pendulum.angularAcceleration = -(9.8f / pendulum.length) * std::sin(pendulum.angle);
	pendulum.anfularVelocity += pendulum.angularAcceleration * ball->deltatime;
	pendulum.angle += pendulum.anfularVelocity * ball->deltatime;

	Vector3 p; // ballのposition
	p.x = pendulum.anchor.x + pendulum.length * std::sin(pendulum.angle);
	p.y = pendulum.anchor.y - pendulum.length * std::cos(pendulum.angle);
	p.z = pendulum.anchor.z;

	ball->position = p;
}

void ConicalPendulumMotion(ConicalPendulum& conicalPendulum, Ball* ball) {
	conicalPendulum.angularVelocity = std::sqrt(9.8f / conicalPendulum.length) * std::cos(conicalPendulum.halfApexAngle);
	conicalPendulum.angle += conicalPendulum.angularVelocity * ball->deltatime;

	float radius = std::sin(conicalPendulum.halfApexAngle) * conicalPendulum.length;
	float height = std::cos(conicalPendulum.halfApexAngle) * conicalPendulum.length;

	ball->position.x = conicalPendulum.anchor.x + radius * std::cos(conicalPendulum.angle);
	ball->position.y = conicalPendulum.anchor.y - height;
	ball->position.z = conicalPendulum.anchor.z + radius * std::sin(conicalPendulum.angle);
}

void PlaneReflect(Plane& plane, DecaltMotionBall& ball, float e) {
	ball.velocity += ball.acceleration * ball.deltatime;

	Sphere sphere{};
	sphere.center = ball.position + ball.velocity * ball.deltatime;
	sphere.radius = ball.radius;

	if (IsCollition(sphere, plane)) {
		Vector3 reflected = Reflect(ball.velocity, plane.normal);
		Vector3 projectToNormal = Project(reflected, plane.normal);
		Vector3 movingDirection = reflected - projectToNormal;
		ball.velocity = projectToNormal * e + movingDirection;

		if (ball.velocity.LengthL() < 0.8f) {
			ball.velocity = {};
		}
	}

	ball.position += ball.velocity * ball.deltatime;
}
