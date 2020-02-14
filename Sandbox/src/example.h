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

		gRotate =    { { cos(0.f),   -sin(0.f),    0.f  },
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
		gRotate =    { { cos(rad),   -sin(rad),    0.f  },
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