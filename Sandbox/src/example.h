#pragma once
#include <Hazel.h>

#include "imgui/imgui.h"

#include <gl/GL.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class ExampleLayer2 : public Hazel::Layer
{
public:
	ExampleLayer2()
		: Layer("Example")
	{

		position = { 0.f, 0.f , 1.f };

		gTranslate = { { 1.f,   0.f,    0.f  },
					   { 0.f,   1.f,    0.f  },
					   { 0.f,   0.f,    1.f  } };

		gRotate = { { cos(0.f),   -sin(0.f),    0.f  },
					   { sin(0.f),   cos(0.f),     0.f  },
					   { 0.f,          0.f,        1.f  } };
	};

	void trans(float x, float y)
	{
		gTranslate = transpose(gTranslate);
		gTranslate[0][2] = x;
		gTranslate[1][2] = y;
		position = (gTranslate * position);
	}

	void Rotate(float rad)
	{
		gRotate = { { cos(rad),   -sin(rad),    0.f  },
					   { sin(rad),   cos(rad),     0.f  },
					   { 0.f,          0.f,        1.f  } };

		position = (gRotate * position);
	}

	void DrawTriangle()
	{
		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_TRIANGLES);
		glVertex3f(0.f + position.x, 0.1561f + position.y, 0.f);
		glVertex3f(-0.05f + position.x, -0.05f + position.y, 0.f);
		glVertex3f(0.05f + position.x, -0.05f + position.y, 0.f);
		glEnd();
	}
	void DrawXYZ()
	{
		glColor3f(1.f, 0.f, 0.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(-1.0, 0.0, 0.0);
		glEnd();

		glColor3f(0.f, 1.f, 0.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, -1.0, 0.0);
		glEnd();

		glColor3f(0.f, 0.f, 1.f);
		glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, -1.0);
		glEnd();
	}
	void OnUpdate() override
	{
		DrawXYZ();
		DrawTriangle();
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB)) {
			trans(0.002f, 0.002f);
			HZ_TRACE("Tab key is pressed (poll)!");
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_R)) {
			Rotate(0.05);
			HZ_TRACE("Tab key is pressed (poll)!");
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_Y)) {
			position = position * glm::inverse(gTranslate);
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_U)) {
			position = { 0,0,1 };
		}
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World!");
		//ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("Tab key is pressed (event)!");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

private:
	glm::mat3 triangle;
	glm::mat3 gTranslate;
	glm::mat3 gRotate;

	glm::vec3 position;
};



class RigidCircle : public Hazel::Layer
{
public:
	glm::vec2 pos;
	glm::vec2 vel;

	float radius = 0.1f;
	float mass = 1.0f;
public:

	RigidCircle()
	{
		vel = { 1.f, 0.f };
		pos = { 0.f, 0.f };
	}
	void draw()
	{
		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_TRIANGLES);
		glVertex2f(0.f + pos.x, 0.1561f + pos.y);
		glVertex2f(-0.05f + pos.x, -0.05f + pos.y);
		glVertex2f(0.05f + pos.x, -0.05f + pos.y);
		glEnd();

		glColor3f(0.f, 0.f, 1.f);
	}

	void OnUpdate() override
	{
		float dt = 0.016;
		static const glm::vec2 gravity = glm::vec2(0.0f, -9.8f);
		static const float coef_res = 0.7f; // coefficient of restitution
		static const float coef_friction = 0.99f; // friction (not physical)

		// numerical integration
		vel += gravity * dt;
		pos += vel * dt;
		draw();


		// wall collision, friction
		if (1.0f - pos.x <= radius) // right wall
		{
			pos.x = 1.0f - radius;

			if (vel.x >= 0.0f)
				vel.x *= -1.0f * coef_res;
		}

		if (pos.x <= -1.0f + radius) // left wall
		{
			pos.x = -1.0f + radius;

			if (vel.x <= 0.0f)
				vel.x *= -1.0f * coef_res;
		}

		if (pos.y <= -1.0f + radius) // ground
		{
			pos.y = -1.0f + radius;

			if (vel.y <= 0.0f)
				vel.y *= -1.0f * coef_res;

			vel.x *= coef_friction;
		}
	}
};


class Example : public Hazel::Layer
{
public:
	RigidCircle rb0, rb1, rb2;

	Example()
	{
		reset();
	}

	void reset()
	{
		// Initial position and velocity
		rb0.pos = glm::vec2(0.0f, 0.5f);
		rb0.vel = glm::vec2(0.0f, 0.0f);
		rb0.radius = 0.03f;
		rb0.mass = 1.0f;

		rb1.pos = glm::vec2(0.5f, 0.5f);
		rb1.vel = glm::vec2(0.0f, 0.0f);
		rb1.radius = 0.03f;
		rb1.mass = rb0.mass * std::pow(rb1.radius / rb0.radius, 2.0f);

		rb2.pos = glm::vec2(0.8f, 0.5f);
		rb2.vel = glm::vec2(0.0f, 0.0f);
		rb2.radius = 0.03f;
		rb2.mass = rb0.mass * std::pow(rb1.radius / rb0.radius, 2.0f);
	}

	void drawLine(const glm::vec2 & position0, const glm::vec2 & position1)
	{
		glBegin(GL_LINES);
		{
			glColor3f(1.f, 0.f, 0.f);
			glVertex2fv(&position0[0]);
			glColor3f(1.f, 0.f, 0.f);
			glVertex2fv(&position1[0]);
		}
		glEnd();
	}



	float getMagnitude(glm::vec2 v)
	{
		return std::sqrt(v.x * v.x + v.y * v.y);
	}

	void OnUpdate() override
	{
		const float dt = 0.016f * 0.4f;
		const float epsilon = 0.5f;

		// physics update (Temporarily disabled)
		//rb0.update(dt);
		//rb1.update(dt);

		// 상수값
		const glm::vec2 gravity(0.0f, -9.8f);
		const float coeff_k = 50.0f;
		const float coeff_d = 10.0f;

		// update rb1 (Note: rb0 is fixed)
		{
			const float l0 = 0.3f;

			// rb0-rb1 
			const auto distance = getMagnitude(rb1.pos - rb0.pos);
			const auto direction = (rb1.pos - rb0.pos) / distance;// unit vector

			//  stiffness 계산
			glm::dot(rb1.vel - rb0.vel, direction);
			const auto spring_force = direction * -(distance - l0) * coeff_k +
				direction * -glm::dot(rb1.vel - rb0.vel, direction) * coeff_d;

			// 댐핑 계산

			const auto accel = gravity + spring_force / rb1.mass;

			rb1.vel += accel * dt;

		}

		// update rb2
		{
			const float l0 = 0.2f;

			// rb1-rb2 
			const auto distance = getMagnitude(rb2.pos - rb1.pos);
			const auto direction = (rb2.pos - rb1.pos) / distance; // 단위백터 계산

			//  stiffness  계산
			const auto spring_force = direction * -(distance - l0) * coeff_k +
				direction * -glm::dot(rb2.vel - rb1.vel, direction) * coeff_d;

			// 댐핑계산
			const auto accel = gravity + spring_force / rb2.mass;

			rb2.vel += accel * dt;
			rb1.vel -= spring_force / rb1.mass * dt; // reaction
		}

		// 좌표 업데이트
		rb1.pos += rb1.vel * dt;
		rb2.pos += rb2.vel * dt;

		// 선
		drawLine( rb0.pos, rb1.pos);
		drawLine(rb1.pos, rb2.pos);

		rb0.draw();
		rb1.draw();
		rb2.draw();

	}

};