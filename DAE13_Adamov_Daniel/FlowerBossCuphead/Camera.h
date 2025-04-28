#pragma once
class Camera
{
public:
	// Rule of 0
	explicit Camera(float screenWidth, float screenHeight);

	void Aim(float levelW, float levelH, const Vector2f& trackCenter);
	void DrawBorderOverlay(float lineHeight) const;
	Vector2f Reset() const;

private:
	const float m_ScreenWidth;
	const float m_ScreenHeight;

	Vector2f m_CameraPos;

};

