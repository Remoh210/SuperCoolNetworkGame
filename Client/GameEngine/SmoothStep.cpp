#include "SmoothStep.h"

#include <iostream>

float clamp(float x, float lowerlimit, float upperlimit) {
	if (x < lowerlimit)
		x = lowerlimit;
	if (x > upperlimit)
		x = upperlimit;
	return x;
}

float smoothstep(float edge0, float edge1, float x) {
	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	float retvalue = x * x * (3 - 2 * x);

	std::cout << "retvalue = " << retvalue << std::endl;

	return retvalue;
}