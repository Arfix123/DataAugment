#ifndef BITMIXER_H

#define BITMIXER_H

//INCLUDES
#include <iostream>
#include <optional>
#include "CImg.h"


// Struct, clalss, enum declarations
enum source_type
{
    UNDEFINED,
    GRAYSCALE,
    RGB
};



// FUNCTION DECLARATIONS

std::optional<unsigned char> parse_planes(const char*);

/// @brief Recombine target bitplanes according to the BIRD algorithm
/// @param target source filepath
/// @param output output filepath
/// @return int - 0 on success
int test_func(const char *, const char *);

// GLOBAL VARIABLES

inline unsigned char MASKS[3] = {0, 0, 0};
inline unsigned char &RED = MASKS[0], &GREEN = MASKS[1], &BLUE = MASKS[2];
inline unsigned char GRAY = 0;
inline char* TARGET = nullptr, *OUTPUT = nullptr;
inline bool IS_DIRECTORY;
inline source_type TYPE = UNDEFINED;

#endif // !BITMIXER_H


