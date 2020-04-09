#ifndef FILTER_H
#define FILTER_H

class Filter
{
public:
    Filter();
    void init(float value);
    void setGain(float value);
    float filter(float value);

private:
    float gain;
    float prevValue;    
};

#endif /* FILTER_H */