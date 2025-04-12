#pragma once
class Camera
{
public:
	Camera(float screenWidth, float screenHeight);
	void Aim(float levelW, float levelH, const Vector2f& trackCenter);
	void DrawBorderOverlay(float lineHeight) const;
	Vector2f Reset();

private:
	float m_ScreenWidth;
	float m_ScreenHeight;

	Vector2f m_CameraPos;

};

