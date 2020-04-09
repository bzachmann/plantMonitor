#include "filter.h"

Filter::Filter():
    gain(0.5f),
    prevValue(0.0f)
{

}

void Filter::init(float value)
{
    prevValue = value;
}

void Filter::setGain(float value)
{
    gain = value;
}

float Filter::filter(float value)
{
    prevValue = prevValue + gain*(value - prevValue);
    return prevValue;
}