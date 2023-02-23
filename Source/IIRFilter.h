#pragma once

#include <cstddef>
#include <stdint.h>
#include <iostream>

class IIRFilter
{
public:
    template <size_t B, size_t A>
    IIRFilter(const double (&b)[B], const double (&_a)[A]) : lenB(B), lenA(A - 1)
    {
        i_b = 0;
        i_a = 0;

        x = new double[lenB]();
        y = new double[lenA]();

        coeff_b = new double[2 * lenB - 1];
        coeff_a = new double[2 * lenA - 1];

        // normalize
        double a0 = _a[0];
        const double *a = &_a[1];
        for (uint8_t i = 0; i < 2 * lenB - 1; i++)
        {
            coeff_b[i] = b[(2 * lenB - 1 - i) % lenB] / a0;
        }
        for (uint8_t i = 0; i < 2 * lenA - 1; i++)
        {
            coeff_a[i] = a[(2 * lenA - 2 - i) % lenA] / a0;
        }
    }

    ~IIRFilter()
    {
        delete[] x;
        delete[] y;
        delete[] coeff_a;
        delete[] coeff_b;
    }

    double process_sample(double value)
    {
        x[i_b] = value;
        double b_terms = 0;
        double *b_shift = &coeff_b[lenB - i_b - 1];
        for (uint8_t i = 0; i < lenB; i++)
        {
            b_terms += x[i] * b_shift[i];
        }
        double a_terms = 0;
        double *a_shift = &coeff_a[lenA - i_a - 1];
        for (uint8_t i = 0; i < lenA; i++)
        {
            a_terms += y[i] * a_shift[i];
        }
        double filtered = b_terms - a_terms;
        y[i_a] = filtered;
        i_b++;
        if (i_b == lenB)
        {
            i_b = 0;
        }
        i_a++;
        if (i_a == lenA)
        {
            i_a = 0;
        }
        return filtered;
    }

    void process(double *input, double *output, int block_size)
    {
        for (int i = 0; i < block_size; i++)
        {
            output[i] = process_sample(input[i]);
        }
    }

private:
    const uint8_t lenB, lenA;
    uint8_t i_b;
    uint8_t i_a;
    double *x;
    double *y;
    double *coeff_b;
    double *coeff_a;
};

class Complementary
{
public:
    template <size_t N>
    Complementary(const double(&b)[N], const double(&b_)[N], const double(&_a)[N])
    {
        double accumulator[N] = { 0.0 };
        for (int i = 0; i < N; i++)
        {
            accumulator[i] = b[i] + b_[i];
        }

        F  = new IIRFilter(b, _a);
        F_ = new IIRFilter(b_, _a);
        A  = new IIRFilter(accumulator, _a);
    }

    void process_sample_complementary(double input_data, double* output_data)
    {
        output_data[0] = F->process_sample(input_data);
        output_data[1] = F_->process_sample(input_data);
    }

    void process_complementary(double* input, double** output, int block_size)
    {
        for (int i = 0; i < block_size; i++)
        {
            output[0][i] = F->process_sample(input[i]);
            output[1][i] = F_->process_sample(input[i]);
        }
    }

    double process_sample_allpass(double input_data)
    {
        return A->process_sample(input_data);
    }

    void process_allpass(double* input, double* output, int block_size)
    {
        for (int i = 0; i < block_size; i++)
        {
            output[i] = A->process_sample(input[i]);
        }
    }
    
private:
    IIRFilter* F;
    IIRFilter* F_;
    IIRFilter* A;
};