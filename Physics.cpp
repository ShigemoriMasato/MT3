#include "Physics.h"
#include "imgui.h"
#include <cmath>

void SpringUpdate(Spring& spring, Ball& ball) {
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
