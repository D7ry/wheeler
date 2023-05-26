#pragma once
class TimeFloatInterpolator
{
private:
	double value;
	double target;
	double duration;
	double elapsedTime;

public:
	TimeFloatInterpolator(double initialValue);
	TimeFloatInterpolator();
	~TimeFloatInterpolator();

	void InterpolateTo(double targetValue, double interpolDuration);

	void Update(double dt);

	double GetValue() const;
};

////PROMPT:
////Great. here's the design scheme of an interpolator based on time:
////we have class `TimeFloatInterpolator` and a manager class `TimeFloatInterpolatorManager`, which is also a static class. `TimeFloatInterpolator`'s constructor puts itself into the manager class' storage, which is an std:unordered_set<TimeFloatInterpolator*>; TimeFloatInterpolator's destructor removes itself from the manager class.
////
////TimeFloatInterpolator contains the following method:
////public:
////	void InterpolateTo(float target, float duration);
////	void Update(float dt);
////	float GetValue() const;
////InterpolateTo() starts a new job for the interpolator which will slowly change its value towards target for DURATION. 
////
////now, first code time floatinterpolator, then timefloatinterpolatormanager.
