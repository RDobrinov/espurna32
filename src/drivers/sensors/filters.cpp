#include "config/hardware.h"

#if SENSOR_SUPPORT

#include <vector>
#include "drivers/sensors/filters.hpp"

/* Filters */

unsigned char BaseFilter::size() { return _size; };

// Last Filter
void LastFilter::add(double value) {
    _value = value;
}

unsigned char LastFilter::count() {
    return 1;
}

void LastFilter::reset() {
    _value = 0;
}

double LastFilter::result() {
    return _value;
}

void LastFilter::resize(unsigned char size) {}

// Max Filter
void MaxFilter::add(double value) {
    if (value > _max) _max = value;
}

unsigned char MaxFilter::count() {
    return 1;
}

void MaxFilter::reset() {
    _max = 0;
}

double MaxFilter::result() {
    return _max;
}

void MaxFilter::resize(unsigned char size) {}

// Median Filter
MedianFilter::~MedianFilter() {
    if (_data) delete _data;
}

void MedianFilter::add(double value) {
    if (_pointer <= _size) {
        _data[_pointer] = value;
        _pointer++;
    }
}

unsigned char MedianFilter::count() {
    return _pointer;
}

void MedianFilter::reset() {
    if (_pointer > 0) {
        _data[0] = _data[_pointer-1];
        _pointer = 1;
    } else {
        _pointer = 0;
    }
}

double MedianFilter::result() {

    double sum = 0;

    if (_pointer > 2) {

        for (unsigned char i = 1; i <= _pointer - 2; i++) {

            // For each position,
            // we find the median with the previous and next value
            // and use that for the sum

            double previous = _data[i-1];
            double current = _data[i];
            double next = _data[i+1];

            if (previous > current) std::swap(previous, current);
            if (current > next) std::swap(current, next);
            if (previous > current) std::swap(previous, current);

            sum += current;

        }

        sum /= (_pointer - 2);

    } else if (_pointer > 0) {

        sum = _data[0];

    }

    return sum;

}

void MedianFilter::resize(unsigned char size) {
    if (_size == size) return;
    _size = size;
    if (_data) delete _data;
    _data = new double[_size+1];
    for (unsigned char i=0; i<=_size; i++) _data[i] = 0;
    _pointer = 0;
}

// Moving Average Filter
void MovingAverageFilter::add(double value) {
    _sum = _sum + value - _data[_pointer];
    _data[_pointer] = value;
    _pointer = (_pointer + 1) % _size;
}

unsigned char MovingAverageFilter::count() {
    return _pointer;
}

void MovingAverageFilter::reset() {}

double MovingAverageFilter::result() {
    return _sum;
}

void MovingAverageFilter::resize(unsigned char size) {
    if (_size == size) return;
    _size = size;
    if (_data) delete _data;
    _data = new double[_size];
    for (unsigned char i=0; i<_size; i++) _data[i] = 0;
    _pointer = 0;
    _sum = 0;
}
/* End Filters */
#endif /* SENSOR_SUPPORT */