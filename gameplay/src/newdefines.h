#ifndef NEWDEFINES_H
#define NEWDEFINES_H

/*
Types
*/

typedef int DWORD;
typedef unsigned char CHAR;
typedef unsigned char BYTE;
typedef const char* const CHAR_PTR;
typedef const char* const BYTE_PTR;
//typedef unsigned int BOOL;
typedef unsigned int _BOOL;
typedef float FLOAT;
typedef float CLAMP;

#define VOID void
//#define NULL nullptr
#define TRUE 1
#define FALSE 0

union PACK
{
    struct
    {
        FLOAT fX, fY, fZ, fW;
    };
    struct
    {
        CLAMP fR, fG, fB, fA;
    };
    struct
    {
        CLAMP fU1, fV1, fU2, fV2;
    };
    struct
    {
        DWORD qOffsetX, qSizeX, qOffsetY, qSizeY;
    };
    
    FLOAT rfPS[4];
    DWORD rqPD[4];
};

/*
Limits
*/

#define DWORD_MIN (0x0)
#define DWORD_MAX (~0x0)
#define FLOAT_MIN (1E-37)
#define FLOAT_MAX (1E+37)
#define CLAMP_MIN (0.0f)
#define CLAMP_MAX (1.0f)

/*
Macros
*/

#define SIZEOF(x) sizeof(x)
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))
#define STATIC_ASSERT(x) static_assert((x), __FILE__)

#ifdef DEBUG

VOID _Assert(
    const _BOOL&
    );

#define ASSERT(x) _Assert((_BOOL)(x))

#else

#define ASSERT(x)

#endif

#endif