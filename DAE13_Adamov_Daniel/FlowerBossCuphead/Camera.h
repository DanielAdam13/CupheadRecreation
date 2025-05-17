#pragma once
class Camera // The main camera following the player when alive
{
public:
	// Rule of 0
	explicit Camera(float screenWidth, float screenHeight, float scaleX, float scaleY);

	void Aim(float levelW, float levelH, const Vector2f& trackcenter);
	void DrawBorderOverlay(float lineHeight) const;

	void Apply() const;
	Vector2f Reset() const;

	Rectf GetCuurentCameraBounds() const;
	Vector2f GetTrackCenter() const;

private:
	const float m_ScreenWidth;
	const float m_ScreenHeight;

	float m_ScaleX;
	float m_ScaleY;

	Vector2f m_CameraPos;
	Vector2f m_TrackCenter;
};

