// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
// See the LICENSE file in the project root for more information.

/*=============================================================================
**
** Source: test1.c
**
** Purpose: Test to ensure that acosh return the correct values
** 
** Dependencies: PAL_Initialize
**               PAL_Terminate
**               Fail
**               fabs
**
**===========================================================================*/

#include <palsuite.h>

// binary64 (double) has a machine epsilon of 2^-52 (approx. 2.22e-16). However, this 
// is slightly too accurate when writing tests meant to run against libm implementations
// for various platforms. 2^-50 (approx. 8.88e-16) seems to be as accurate as we can get.
//
// The tests themselves will take PAL_EPSILON and adjust it according to the expected result
// so that the delta used for comparison will compare the most significant digits and ignore
// any digits that are outside the double precision range (15-17 digits).

// For example, a test with an expect result in the format of 0.xxxxxxxxxxxxxxxxx will use
// PAL_EPSILON for the variance, while an expected result in the format of 0.0xxxxxxxxxxxxxxxxx
// will use PAL_EPSILON / 10 and and expected result in the format of x.xxxxxxxxxxxxxxxx will
// use PAL_EPSILON * 10.
#define PAL_EPSILON 8.8817841970012523e-16

#define PAL_NAN     sqrt(-1.0)
#define PAL_POSINF -log(0.0)
#define PAL_NEGINF  log(0.0)

/**
 * Helper test structure
 */
struct test
{
    double value;     /* value to test the function with */
    double expected;  /* expected result */
    double variance;  /* maximum delta between the expected and actual result */
};

/**
 * validate
 *
 * test validation function
 */
void __cdecl validate(double value, double expected, double variance)
{
    double result = acosh(value);

    /*
     * The test is valid when the difference between result
     * and expected is less than or equal to variance
     */
    double delta = fabs(result - expected);

    if (delta > variance)
    {
        Fail("acosh(%g) returned %20.17g when it should have returned %20.17g",
             value, result, expected);
    }
}

/**
 * validate
 *
 * test validation function for values returning NaN
 */
void __cdecl validate_isnan(double value)
{
    double result = acosh(value);

    if (!_isnan(result))
    {
        Fail("acosh(%g) returned %20.17g when it should have returned %20.17g",
             value, result, PAL_NAN);
    }
}

/**
 * main
 * 
 * executable entry point
 */
int __cdecl main(int argc, char **argv)
{
    struct test tests[] = 
    {
        /* value                   expected                variance */
        {  1,                      0,                      PAL_EPSILON },
        {  1.0510897883672876,     0.31830988618379067,    PAL_EPSILON },        // expected:  1 / pi
        {  1.0957974645564909,     0.43429448190325183,    PAL_EPSILON },        // expected:  log10(e)
        {  1.2095794864199787,     0.63661977236758134,    PAL_EPSILON },        // expected:  2 / pi
        {  1.25,                   0.69314718055994531,    PAL_EPSILON },        // expected:  ln(2)
        {  1.2605918365213561,     0.70710678118654752,    PAL_EPSILON },        // expected:  1 / sqrt(2)
        {  1.3246090892520058,     0.78539816339744831,    PAL_EPSILON },        // expected:  pi / 4
        {  1.5430806348152438,     1,                      PAL_EPSILON * 10 },
        {  1.7071001431069344,     1.1283791670955126,     PAL_EPSILON * 10 },   // expected:  2 / sqrt(pi)
        {  2.1781835566085709,     1.4142135623730950,     PAL_EPSILON * 10 },   // expected:  sqrt(2)
        {  2.2341880974508023,     1.4426950408889634,     PAL_EPSILON * 10 },   // expected:  log2(e)
        {  2.5091784786580568,     1.5707963267948966,     PAL_EPSILON * 10 },   // expected:  pi / 2
        {  5.05,                   2.3025850929940457,     PAL_EPSILON * 10 },   // expected:  ln(10)
        {  7.6101251386622884,     2.7182818284590452,     PAL_EPSILON * 10 },   // expected:  e
        {  11.591953275521521,     3.1415926535897932,     PAL_EPSILON * 10 },   // expected:  pi
        {  PAL_POSINF,             PAL_POSINF,             0 },
    };

    /* PAL initialization */
    if (PAL_Initialize(argc, argv) != 0)
    {
        return FAIL;
    }

    for (int i = 0; i < (sizeof(tests) / sizeof(struct test)); i++)
    {
        validate(tests[i].value, tests[i].expected, tests[i].variance);
    }

    validate_isnan(PAL_NAN);

    PAL_Terminate();
    return PASS;
}
