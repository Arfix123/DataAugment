#ifndef BITMIXER_H

#define BITMIXER_H

//INCLUDES
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>

#define cimg_use_jpeg
#include "CImg.h"

namespace cl = cimg_library;
namespace fs = std::filesystem;

// Struct, clalss, enum declarations
enum source_type
{
    UNDEFINED,
    GRAYSCALE,
    RGB
};

enum augment_type
{
    SMALL,
    BIG,
    CUSTOM
};

// TODO : HARDCODING solution OMEGALUL
inline std::vector<std::vector<unsigned char>> MASKS = {
    {},                                                                         // 1 - empty
    {},                                                                         // 2 - empty
    {0b11000001, 0b11000010, 0b11000100, 0b11001000, 0b11010000, 0b11100000},   // 3 - 6
    {0b11110000, 0b11101000, 0b11100100, 0b11100010, 0b11100001, 0b11011000,    // 4 - 15
     0b11010100, 0b11010010, 0b11010001, 0b11001100, 0b11001010, 0b11001001,
     0b11000110, 0b11000101, 0b11000011},
    {0b11111000, 0b11110100, 0b11110010, 0b11110001,                            // 5 - 20
     0b11101100, 0b11101010, 0b11101001, 0b11100110,
     0b11100101, 0b11100011, 0b11011100, 0b11011010,
     0b11011001, 0b11010110, 0b11010101, 0b11010011,
     0b11001110, 0b11001101, 0b11001011, 0b11000111},
    {0b11111100, 0b11111010, 0b11111001, 0b11110110, 0b11110101, 0b11110011,    // 6 - 15
     0b11101110, 0b11101101, 0b11101011, 0b11100111, 0b11011110, 0b11011101,
     0b11011011, 0b11010111, 0b11001111},
    {0b11111110, 0b11111101, 0b11111011, 0b11110111, 0b11101111, 0b11011111}    // 7 - 6
};


// FUNCTION DECLARATIONS

std::optional<int> parse_int(const char* query);

/// @brief Recombine target bitplanes according to the BIRD algorithm
/// @param target source filepath
/// @param output output filepath
/// @param augmentation_rate augmentation rate
/// @param pure_flag whether to include the original images in the output
/// @return int - 0 on success, 1 on failure
int recombine(const char *, const char *, int, bool);

/// @brief Simple test function
int custom_display(cl::CImg<unsigned char>, const fs::path &);

#endif // !BITMIXER_H


