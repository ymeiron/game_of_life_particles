#pragma once

#include <cmath>
#include <hdf5.h>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

struct Config {
    std::string input, output;
    double extent;
    int tmax;
    bool write_coords_once;
    bool no_output;
};

struct Particle {
    double x, y, vx, vy;
    enum class Status { inactive, active };
    Status status;
};

class Writer {
    hid_t file_id;
    bool write_coords_once;
    bool coordinates_written;
public:
    explicit Writer(const std::string& filename, bool write_coords_once = false);
    ~Writer();
    void operator()(const std::vector<Particle>& particles, const int step);
};

Config parse_program_options(int argc, char** argv);

std::vector<Particle> read_input_file(std::string filename);

std::vector<int> get_active_neighbor_count(const std::vector<Particle>& particles, const double radius, const double extent);