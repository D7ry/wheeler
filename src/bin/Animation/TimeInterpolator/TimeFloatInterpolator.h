#pragma once
/**
 * Wraps around a float value and linearly interpolates it over time when applicable.
 * The float stays at a fixed value when not interpolating and can be read using GetValue().
 * 
 * Interpolator puts itself into a TimeFloatInterpolatorManager on construction and removes 
 * itself on destruction; TimeFloatInterpolatorManager is a static class that handles updating 
 * all instances of TimeFloatInterpolator.
 */
class TimeFloatInterpolator
{
private:
	std::atomic<double> value;
	std::atomic<double> target;
	std::atomic<double> duration;
	std::atomic<double> elapsedTime;

public:
	TimeFloatInterpolator(double initialValue);
	TimeFloatInterpolator();
	~TimeFloatInterpolator();

	void InterpolateTo(double targetValue, double interpolDuration);

	// Update the interpolator's value based on a delta. Only TimeFloatInterpolatorManager may call it.
	void Update(double dt);

	double GetValue() const;

	void ForceFinish();

	void SetValue(double value);

	void ForceValue(double value);
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
