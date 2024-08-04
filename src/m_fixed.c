#include "m_fixed.h"

#include <limits.h>

fixed_t FixedMul(fixed_t a, fixed_t b)
{
    return (fixed_t) (((int64_t) a * (int64_t) b) >> FRACBITS);
}

fixed_t FixedDiv(fixed_t a, fixed_t b)
{
    if ((D_abs(a) >> 14) >= D_abs(b))
    {
	    return (a^b) < 0 ? INT_MIN : INT_MAX;
    }
    else
    {
        int64_t result;

        result = ((int64_t) a << FRACBITS) / b;

        return (fixed_t) result;
    }
}