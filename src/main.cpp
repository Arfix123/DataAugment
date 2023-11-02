#include <iostream>
#include <unistd.h>
#include <optional>
#include "bitmixer.h"

int main(int argc, char *argv[])
{
    int opt = 0;
    int result = EXIT_SUCCESS;
    bool continue_flag = true;
    bool adjacent_flag = false;
    char* TARGET = nullptr, *OUTPUT = nullptr;
    std::optional<int> target_bitplanes = std::nullopt;
    while ((opt = getopt(argc, argv, "f:o:n:N:ahq")) != -1 && continue_flag)
    {
        std::optional<unsigned char> parse_result;
        switch (opt)
        {
        case 'h':
            std::cout << "Usage: " << argv[0] << " -f <source> -o <output> -n <bitplanes> [-a] [-h] [-q]"
                            "\n\t-f <source> : Source image or folder containing images to be processed"
                            "\n\t-o <output> : Output folder for the augmented images"
                            "\n\t-n <bitplanes> : Amount of bitplanes to extract from the source images"
                            "\n\t-a : Turn on adjacent extraction constraint"
                            "\n\t-q : Turn on quiet mode"
                            "\n\t-h : Display this help message"
                            << std::endl;
            return EXIT_SUCCESS;
            break;
        case 'q':
            QUIET_FLAG = true;
            break;
        case 'f':
            if (TARGET) {
                std::cerr << "Unexpected option : Source already provided\n";
                continue_flag = false;
                result = EXIT_FAILURE;
                break;
            }
            TARGET = optarg;
            break;
        case 'o':
            if (OUTPUT) {
                std::cerr << "Unexpected option : Output already provided\n";
                continue_flag = false;
                result = EXIT_FAILURE;
                break;
            }
            OUTPUT = optarg;
            break;
        case 'n':
            target_bitplanes = parse_int(optarg);
            break;
        case 'a':
            adjacent_flag = true;
            break;
        default:
            std::cerr << "Invalid option\n";
            continue_flag = false;
            result = EXIT_FAILURE;
            break;
        }
    }

    if (!TARGET || !OUTPUT || !target_bitplanes) {
        std::cerr << "Missing required arguments or unable to parse\n";
        return EXIT_FAILURE;
    }

    // MAGIC HAPPENS HERE
    if (result == EXIT_FAILURE) {
        std::cerr << "SETUP FAILURE\n";
        return result;
    }

    result = recombine(TARGET, OUTPUT, *target_bitplanes, adjacent_flag);
    
    if (result == EXIT_FAILURE) {
        std::cerr << "RECOMBINE FAILURE\n";
        return result;
    }
    std::cout << "RECOMBINE SUCCESS\n";
    return result;
}

