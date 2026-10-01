#include "PositionClass.h"

PositionClass::PositionClass()
{
	m_frameTime = 0;
	m_rotationY = 0;
	m_leftTurnSpeed = 0;
	m_rightTurnSpeed = 0;
}

PositionClass::PositionClass(const PositionClass& other)
{
}

PositionClass::~PositionClass()
{
}

void PositionClass::SetFrameTime(float time)
{
	m_frameTime = time;
	return;
}

void PositionClass::GetRotation(float& y)
{
	y = m_rotationY;
	return;
}

void PositionClass::TurnLeft(bool keydown)
{
	if (keydown)
	{
		m_leftTurnSpeed += m_frameTime * 1.5f;

		if (m_leftTurnSpeed > (m_frameTime * 200.0f))
		{
			m_leftTurnSpeed = m_frameTime * 200.0f;
		}
	}
	else
	{
		m_leftTurnSpeed -= m_frameTime * 1.0f;

		if (m_leftTurnSpeed < 0.0f)
		{
			m_leftTurnSpeed = 0.0f;
		}
	}

	m_rotationY -= m_leftTurnSpeed;
	if (m_rotationY < 0.0f)
	{
		m_rotationY += 360.0f;
	}

	return;
}

void PositionClass::TurnRight(bool keydown)
{
	if (keydown)
	{
		m_rightTurnSpeed += m_frameTime * 1.5f;

		if (m_rightTurnSpeed > (m_frameTime * 200.0f))
		{
			m_rightTurnSpeed = m_frameTime * 200.0f;
		}
	}
	else
	{
		m_rightTurnSpeed -= m_frameTime * 1.0f;

		if (m_rightTurnSpeed < 0.0f)
		{
			m_rightTurnSpeed = 0.0f;
		}
	}

	m_rotationY += m_rightTurnSpeed;
	if (m_rotationY > 360.0f)
	{
		m_rotationY -= 360.0f;
	}

	return;
}


