#ifndef bqtAveragingtHH
#define bqtAveragingtHH

#include <cmath>

/* Method for calculating average */
class AveragingMethod
{
    const char *name;
public:
    AveragingMethod(const char *n) : name(n) { }
    const char *GetName() const { return name; }
    virtual void Reset() = 0;
    virtual void Cumulate(double value) = 0;
    virtual double GetValue() const = 0;
    virtual ~AveragingMethod(){}
};

/* Average */
class Averaging : public AveragingMethod
{
    unsigned n;
    double sum;
public:
    Averaging() : AveragingMethod("normal"), n(0), sum(0) { }
    virtual void Reset() { n=0; sum=0; }
    virtual void Cumulate(double value) { sum += value; ++n;
    //printf("Now %g, %u\n", sum, n);
    }
    void Cumulate(double value, unsigned weight) { sum += value*weight; n+=weight; }
    virtual double GetValue() const { return sum / n; }
};

/* Harmonic mean average */
class HarmonicAveraging : public AveragingMethod
{
    unsigned n;
    double sum;
public:
    HarmonicAveraging() : AveragingMethod("harmonic"), n(0), sum(0) { }
    virtual void Reset() { n=0; sum=0; }
    virtual void Cumulate(double value) { sum += 1/value; ++n; }
    virtual double GetValue() const { return n / sum; }
};

/* Geometric mean average */
class GeometricAveraging : public AveragingMethod
{
    unsigned n;
    double sum;
public:
    GeometricAveraging() : AveragingMethod("geometric"), n(0), sum(1) { }
    virtual void Reset() { n=0; sum=1; }
    virtual void Cumulate(double value) { sum *= value; ++n; }
    virtual double GetValue() const { return std::pow(sum, 1.0 / n); }
};

/* Biased mean average */
class BiasedAveraging : public AveragingMethod
{
    double min,max,bias;
    Averaging ka;
    bool first;
public:
    // bias: -1=min, 0=avg, 1=max
    BiasedAveraging(double b, const char *n="biased")
        : AveragingMethod(n), min(),max(), bias(b), first(true) { }
    virtual ~BiasedAveraging() { }
    virtual void Reset() { first=true; ka.Reset(); }
    virtual void Cumulate(double value)
    {
        if(first) { min=max=value; first=false; }
        else { if(value < min)min=value; if(value > max)max=value; }
        ka.Cumulate(value);
    }
    virtual double GetValue() const
    {
        if(first) return sqrt((double)-1); //nan
        const double avg = ka.GetValue();
        const double b = (bias+1)/2;
        return min * (  b)
             + max * (1-b)
             + (avg - (max+min)/2) * (1 - std::pow(2 * b - 1, 2));
    }
};

/* Optimistic average */
class OptimisticAveraging : public BiasedAveraging
{
public:
    OptimisticAveraging(double value=1.3) : BiasedAveraging(1/value - 1, "optimistic") { }
};

/* Pessimistic average */
class PessimisticAveraging : public BiasedAveraging
{
public:
    PessimisticAveraging(double value=1.3) : BiasedAveraging(1 - 1/value, "pessimistic") { }
};

#endif
