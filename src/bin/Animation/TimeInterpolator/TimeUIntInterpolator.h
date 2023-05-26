#pragma once
class TimeUintInterpolator
{
private:
	uint32_t value;
	uint32_t target;
	float duration;
	float elapsedTime;

public:
	TimeUintInterpolator(uint32_t initialValue);
	TimeUintInterpolator();
	~TimeUintInterpolator();

	void InterpolateTo(uint32_t targetValue, double interpolDuration);

	void Update(double dt);

	uint32_t GetValue() const;
};
