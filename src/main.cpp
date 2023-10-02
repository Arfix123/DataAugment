#include <iostream>
#include <unistd.h>
#include <optional>
#include "bitmixer.h"

int main(int argc, char *argv[])
{
    int opt = 0;
    int result = EXIT_SUCCESS;
    while ((opt = getopt(argc, argv, "f:o:r:g:b:i:")) != -1 && result == EXIT_SUCCESS)
    {
        std::optional<unsigned char> parse_result;
        switch (opt)
        {
        case 'f':
            if (TARGET) {
                std::cerr << "Unexpected option : Source already provided\n";
                result = EXIT_FAILURE;
                break;
            }
            TARGET = optarg;
            break;
        case 'o':
            if (OUTPUT) {
                std::cerr << "Unexpected option : Processing grayscale\n";
                result = EXIT_FAILURE;
                break;
            }
            OUTPUT = optarg;
            break;
        case 'r':
        case 'g':
        case 'b':
            if (TYPE == GRAYSCALE) {
                std::cerr << "Unexpected option : Processing grayscale\n";
                result = EXIT_FAILURE;
                break;
            }
            TYPE = RGB;
            parse_result = parse_planes(optarg);
            if (parse_result) {
                switch (opt)
                {
                case 'r':
                    RED = *parse_result;
                    break;
                case 'g':
                    GREEN = *parse_result;
                    break;
                case 'b':
                    BLUE = *parse_result;
                    break;
                default:
                    break;
                }
            }
            break;
        case 'i':
            if (TYPE == RGB) {
                std::cerr << "Unexpected option : Processing rgb image\n";
                result = EXIT_FAILURE;
                break;
            }
            TYPE = GRAYSCALE;
            parse_result = parse_planes(optarg);
            GRAY = *parse_result;
            break;
        // TODO option for the result dataset to not contain the original images
        default:
            std::cerr << "Invalid option\n";
            result = EXIT_FAILURE;
            break;
        }
    }

    // TODO Finish error states
    if (TYPE == UNDEFINED) {
        std::cerr << "Failure : No bitplane info given\n";
        result = EXIT_FAILURE;
    }

    // MAGIC HAPPENS HERE
    if (result != EXIT_FAILURE) {
        result = test_func(TARGET, OUTPUT);
    }
    
    std::cout << "FINISHED!" << std::endl;
    return result;
}

