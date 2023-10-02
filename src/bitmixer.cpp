#include "../include/bitmixer.h"
#include <filesystem>
#include <tuple>

namespace fs = std::filesystem;
namespace cl = cimg_library;

// Struct, class, enum declarations

/// @brief Output stream for CImage images with << operator
class cio_stream{
    int streamed_count = 0;
    fs::path destination;

    cio_stream() = delete;
public:

    cio_stream(fs::path x) : destination{x}{}

    std::optional<cio_stream> create_image_output(const fs::path fp_o) {
        if (!fs::exists(fp_o) || !fs::is_directory(fp_o)) {
            std::cerr << "Recombine conditions not met\n";
            return std::nullopt;
        }
        cio_stream result{fp_o};
    }
    
    cio_stream& operator <<(const cl::CImg<unsigned char> &t) {
        t.save(destination.c_str(), streamed_count++);
        return *this;
    }
};


// Functions

int recombine_image(const fs::path &, const fs::path &);
int recombine_directory(const fs::path &, const fs::path &);

int recombine(const char *t, const char * o){

    fs::path fp_target(t), fp_output(o);
    int code = EXIT_FAILURE;

    if (!fs::exists(fp_target) || !fs::exists(fp_output) || !fs::is_directory(fp_output)) {
        // TODO : Assert file-dir or dir-dir process
        // TODO : Implement recursive building of path
        std::cerr << "Recombine conditions not met\n";
        return code;
    }
    if (fs::is_regular_file(fp_target)) {
        code = recombine_image(fp_target, fp_output);
    }
    else {
        code = recombine_directory(fp_target, fp_output);
    }

    return code;
}

int test_func(const fs::path & t, const fs::path & ){
    cl::CImg<unsigned char> image(t.c_str());
    cimg_foroff(image,off) {image[off] = off % 255;}
    cl::CImgDisplay main_disp(image,"Result of your actions");
    
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }    
    
    return 0;
}

cl::CImg<unsigned char> init_image(cl::CImg<unsigned char> t, const unsigned char masks[3]){
    cimg_forC(t, c){
        cimg_forXY(t, x, y){
            t(x,y,0,c) &= masks[c];
        }
    }
    return t;
}

void recombine_iterative(cl::CImg<unsigned char> t, cio_stream out,
                         const unsigned char masks[3]) {
    // IMPLEMENT THIS GOOFY AAAH ALGORITHM
}

int recombine_img(const cl::CImg<unsigned char> & original, const fs::path & destination){
    cl::CImg<unsigned char> combined(original);
    cio_stream out_stream{destination};
    cl::CImgDisplay main_disp(combined,"Result of your actions");

    auto current = init_image(original, MASKS);
    
    return 0;
}

int recombine_directory(const fs::path &, const fs::path &){
    std::clog << "RECOMBINED DIRECTORY" << std::endl;
    return 0;
}

std::optional<unsigned char> parse_planes(const char* query){
    unsigned char result = 0;
    for (auto it = query; *it != '0'; it++)
    {
        char c = *it;
        if (c < '0' || c > '7') {
            std::cerr << "ERROR : Invalid bitplane demand\n";
            return std::nullopt;
        }
        result |= (1u << ('7' - '0'));
    }
    
    return result;
}

using channels = std::tuple<cl::CImg<unsigned char>, cl::CImg<unsigned char>,
                            cl::CImg<unsigned char>>;

