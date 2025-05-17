#include "pch.h"
#include "Camera.h"
#include <iostream>
#include "utils.h"

Camera::Camera(float screenWidth, float screenHeight, float scaleX, float scaleY)
	: m_ScreenWidth{ screenWidth },
	m_ScreenHeight{ screenHeight },
	m_ScaleX{ scaleX },
	m_ScaleY{ scaleY },
	m_CameraPos{},
	m_TrackCenter{}
{
}

void Camera::Aim(float levelW, float levelH, const Vector2f& trackCenter)
{
	m_TrackCenter = trackCenter;
	m_CameraPos = Vector2f{trackCenter.x * m_ScaleX - m_ScreenWidth / 2, trackCenter.y * m_ScaleY - m_ScreenHeight / 2};
	levelW -= 200.f;

	if (m_CameraPos.x + m_ScreenWidth > levelW)
	{
		m_CameraPos.x = levelW - m_ScreenWidth;
	}
	if (m_CameraPos.x < 0)
	{
		m_CameraPos.x = 0;
	}
	if (m_CameraPos.y + m_ScreenHeight + levelH / 18 > levelH )
	{
		m_CameraPos.y = levelH - m_ScreenHeight - levelH / 18;
	}
	if (m_CameraPos.y < 0)
	{
		m_CameraPos.y = 0;
	}
}

void Camera::DrawBorderOverlay(float lineHeight) const
{
	utils::SetColor(Color4f{ 0, 0, 0, 1 });
	utils::FillRect(m_CameraPos.x, m_CameraPos.y + m_ScreenHeight - lineHeight, m_ScreenWidth, lineHeight);
	utils::FillRect(m_CameraPos.x, m_CameraPos.y, m_ScreenWidth, lineHeight);
}

void Camera::Apply() const
{
	glPushMatrix();
		glTranslatef(-m_CameraPos.x, -m_CameraPos.y, 0.f);
}

Vector2f Camera::Reset() const
{
	glPopMatrix();
	return Vector2f();
}

Rectf Camera::GetCuurentCameraBounds() const
{
	return Rectf{ m_CameraPos.x / m_ScaleX, m_CameraPos.y / m_ScaleY, m_ScreenWidth / m_ScaleX, m_ScreenHeight / m_ScaleY };
}

Vector2f Camera::GetTrackCenter() const
{
	return m_TrackCenter;
}
