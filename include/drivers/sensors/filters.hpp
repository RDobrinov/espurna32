#if SENSOR_SUPPORT
#include <Arduino.h>

class BaseFilter {

    public:
        virtual void add(double value);
        virtual unsigned char count();
        virtual void reset();
        virtual double result();
        virtual void resize(unsigned char size);
        unsigned char size();

    protected:
        unsigned char _size;

};

class LastFilter : public BaseFilter {

    public:

        void add(double value);
        unsigned char count();
        void reset();
        double result();
        void resize(unsigned char size);

    protected:
        double _value = 0;

};

class MaxFilter : public BaseFilter {

    public:

        void add(double value);
        unsigned char count();
        void reset();
        double result();
        void resize(unsigned char size);
    protected:
        double _max = 0;

};

class MedianFilter : public BaseFilter {

    public:
        ~MedianFilter();
        void add(double value);
        unsigned char count();
        void reset();
        double result();
        void resize(unsigned char size);

    protected:
        unsigned char _pointer = 0;
        double * _data = NULL;

};

class MovingAverageFilter : public BaseFilter {

    public:

        void add(double value);
        unsigned char count();
        void reset();
        double result();
        void resize(unsigned char size);
    protected:
        unsigned char _pointer = 0;
        double _sum = 0;
        double * _data = NULL;

};
#endif /* SENSOR_SUPPORT */ 