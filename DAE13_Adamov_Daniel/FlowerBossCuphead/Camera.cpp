#include "pch.h"
#include "Camera.h"
#include <iostream>
#include "utils.h"

Camera::Camera(float screenWidth, float screenHeight)
	: m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_CameraPos{ 0.f, 0.f }
{
}

void Camera::Aim(float levelW, float levelH, const Vector2f& trackCenter)
{
	m_CameraPos = Vector2f{trackCenter.x - m_ScreenWidth / 2, trackCenter.y - m_ScreenHeight / 2};

	if (m_CameraPos.x + m_ScreenWidth > levelW)
	{
		m_CameraPos.x = levelW - m_ScreenWidth;
	}
	if (m_CameraPos.x < 0)
	{
		m_CameraPos.x = 0;
	}
	if (m_CameraPos.y + m_ScreenHeight - 100.f > levelH)
	{
		m_CameraPos.y = levelH - m_ScreenHeight + 100.f;
	}
	if (m_CameraPos.y < 0)
	{
		m_CameraPos.y = 0;
	}
	
	glPushMatrix();
		glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0.f);
}

void Camera::DrawBorderOverlay(float lineHeight) const
{
	utils::SetColor(Color4f{ 0, 0, 0, 1 });
	utils::FillRect(m_CameraPos.x, m_CameraPos.y + m_ScreenHeight - lineHeight, m_ScreenWidth, lineHeight);
	utils::FillRect(m_CameraPos.x, m_CameraPos.y, m_ScreenWidth, lineHeight);
}

Vector2f Camera::Reset() const
{
	glPopMatrix();
	return Vector2f();
}

Rectf Camera::GetCuurentCameraBounds() const
{
	return Rectf{ m_CameraPos.x, m_CameraPos.y, m_ScreenWidth, m_ScreenHeight };
}
