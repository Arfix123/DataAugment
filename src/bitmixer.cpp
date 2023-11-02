#include "bitmixer.h"
#include <cassert>
#include <charconv>
#include <filesystem>
#include <optional>
#include <tuple>

namespace fs = std::filesystem;

// Struct, class, enum declarations

/// @brief Output stream for CImage images with << operator
class cio_stream{
    int streamed_count = 0;
    fs::path destination;

    cio_stream() = delete;
public:

    cio_stream(fs::path x) : destination{x / "recombined.jpg"}{}

    static std::optional<cio_stream> create_image_output(const fs::path fp_o) {
        if (!fs::exists(fp_o) || !fs::is_directory(fp_o)) {
            std::cerr << "Could not initialize image output stream\n";
            return std::nullopt;
        }
        cio_stream result {fp_o};
        return result;
    }
    
    cio_stream& operator <<(const cl::CImg<unsigned char> &t) {
        try {
            t.save(destination.c_str(), streamed_count++);
            return *this;
        }
        catch(const cl::CImgIOException& e) {
            throw e;
        }
    }

    const fs::path& get_destination() const {
        return destination;
    }
};

struct recombine_result {
    int result;
    std::optional<std::string> message;
};


// Functions

recombine_result recombine_image(const std::string&, cio_stream&,
                                 const int&, const bool&);

int recombine_fs(const fs::path& t, const fs::path& o, int plane, bool adjacent_flag){

    if (plane > 7 || !fs::exists(t) || !fs::exists(t) || !fs::is_directory(o)) {
        // TODO : Determine whether file-dir or dir-dir process
        // TODO : Implement recursive building of path
        std::cerr << "Recombine conditions not met\n";
        return -1;
    }

    auto output_stream = cio_stream::create_image_output(o);
    if (!output_stream) {
        std::cerr << "Could not initialize output stream to " << o << "\n";
        return -1;
    }
    
    if (fs::is_regular_file(t)) {
        auto rr = recombine_image(t, *output_stream, plane, adjacent_flag);
        if (rr.result != 0) {
            std::cerr << "Could not recombine image " << t;
            if (rr.message) std::cerr << " : " << *rr.message << "\n";
            else std::cerr << "\n";
            return -1;
        }
    }
    else {

        for (auto& p : fs::directory_iterator(t)) {
            std::string filepath = p.path().string();
            if (!QUIET_FLAG)
                std::cout << "Processing " << filepath << "\n";
         
            auto rr = recombine_image(filepath, *output_stream, plane, adjacent_flag);
            if (rr.result != 0) {
                std::cerr << "Could not recombine image " << filepath;
                if (rr.message) std::cerr << " : " << *rr.message << "\n";
                else std::cerr << "\n";
                return -1;
            }

        }
    }

    return 0;
}

int recombine(const char *t, const char *o, int p, bool a){
    return recombine_fs(t, o, p, a);
}

template <typename T>
int custom_display(cl::CImg<T> given, const fs::path &o){
    T MASK = 0b10000000;

    fs::path fp_output(o);
    // cimg_forXYC(image, x, y, c) { if (40 < x && x < 80) image(x, y, c) = 255u;}
    cimg_for(given, ptr, unsigned char){*ptr &= MASK;}
    cl::CImgDisplay main_disp(given,"Result of your actions");
    
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }    
    
    return 0;
}


recombine_result recombine_image(const std::string& filepath, cio_stream& iout,
                                 const int& plane, const bool& is_adjacent) {
    cl::CImg<unsigned char> given{};
    try {
        given.load(filepath.c_str());
    }
    catch(const cl::CImgIOException& e) {
        return {-1, "Error loading image : " + filepath};
    } 
    
    const std::vector<unsigned char>& held = MASKS[plane - 1];
    if (is_adjacent)
        std::cout << "not implemented lol";
    
    auto temp = given;

    for (const auto &mask : held)
    {
        cimg_forXYC(temp, x, y, c){
            temp(x, y, c) = given(x, y, c) & mask;
        }
        iout << temp;
    }
    return {0, std::nullopt};
};

// UTILITIES

std::optional<int> parse_int(const char* query){
    // Parse using std::from_chars
    int result;
    auto [p, ec] = std::from_chars(query, query + strlen(query), result);
    if (ec != std::errc()) {
        std::cerr << "Could not parse integer\n";
        return std::nullopt;
    }
    return result;
}