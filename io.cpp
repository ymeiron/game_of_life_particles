#include <algorithm>
#include <boost/program_options.hpp>
#include <hdf5.h>
#include <hdf5_hl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "common.h"

constexpr hid_t fake_file = -99999;

Writer::Writer(const std::string& filename, bool write_coords_once) :
    write_coords_once(write_coords_once),
    coordinates_written(false)
    {
        if (filename.size())
            file_id = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        else file_id = fake_file;
    }

Writer::~Writer()
{
    if (file_id == fake_file) return;
    H5Fclose(file_id);
}

void Writer::operator()(const std::vector<Particle>& particles, const int step = 0)
{
    if (file_id == fake_file) return;
    constexpr int rank = 1;
    const hsize_t dims[rank] = {particles.size()};

    const std::string group_name = "Step#" + std::to_string(step);
    H5Gcreate2(file_id, group_name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    std::vector<int> status(particles.size());
    std::ranges::transform(particles, begin(status), [](const Particle& p){ return static_cast<int>(p.status); });
    H5LTmake_dataset(file_id, (group_name +"/STATUS").c_str(), rank, dims, H5T_NATIVE_INT, status.data());

    if (!write_coords_once || !coordinates_written) {
        std::vector<double> data(particles.size());
        
        std::ranges::transform(particles, begin(data), [](const Particle& p){ return p.x; });
        H5LTmake_dataset(file_id, (group_name +"/X").c_str(), rank, dims, H5T_NATIVE_DOUBLE, data.data());

        std::ranges::transform(particles, begin(data), [](const Particle& p){ return p.y; });
        H5LTmake_dataset(file_id, (group_name +"/Y").c_str(), rank, dims, H5T_NATIVE_DOUBLE, data.data());

        std::ranges::transform(particles, begin(data), [](const Particle& p){ return p.vx; });
        H5LTmake_dataset(file_id, (group_name +"/VX").c_str(), rank, dims, H5T_NATIVE_DOUBLE, data.data());

        std::ranges::transform(particles, begin(data), [](const Particle& p){ return p.vy; });
        H5LTmake_dataset(file_id, (group_name +"/VY").c_str(), rank, dims, H5T_NATIVE_DOUBLE, data.data());
        
        coordinates_written = true;
    }


    H5Fflush(file_id, H5F_SCOPE_LOCAL);
}

Config parse_program_options(int argc, char** argv)
{
    namespace po = boost::program_options;
    Config config;
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("input,i", po::value<std::string>(&config.input), "Input file name (in ASCII format)")
            ("output,o", po::value<std::string>(&config.output)->default_value("output.h5part"), "Output file name (in HDF5 format)")
            ("extent", po::value<double>(&config.extent)->default_value(0), "Value of extent")
            ("tmax,T", po::value<int>(&config.tmax), "Time of simulation end")
            ("write_coords_once", po::bool_switch(&config.write_coords_once), "Only write coordinates in the first step")
            ("no_output", po::bool_switch(&config.no_output), "Do not save output, just do the simulation")
        ;

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            exit(0);
        }

        if (vm.count("tmax") == 0) {
            std::cerr << "Error: Value of tmax not set.\n";
            exit(1);
        }

        return config;

    }
    catch(std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        exit(1);
    }
    catch(...) {
        std::cerr << "Error: Exception occured when parsing command line arguments.\n";
        exit(1);
    }
}

std::vector<Particle> read_input_file(std::string filename)
{
    std::ifstream f(filename, std::ios::in);
    if (!f) {
        std::cout << "Error: problem opening file\n";
        exit(1);
    }

    std::vector<Particle> data;
    do {
        Particle p;
        f >> p.x;
        f >> p.y;
        f >> p.vx;
        f >> p.vy;
        int status_int;
        f >> status_int;
        p.status = status_int ? Particle::Status::active : Particle::Status::inactive;
        data.push_back(p);
    } while (!f.eof());

    return data;
}