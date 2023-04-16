#include <algorithm>
#include <boost/program_options.hpp>
#include <cmath>
#include <iostream>
#include <vector>
#include <random>
#include <ranges>

#include "common.h"

void advance_particles(std::vector<Particle>& particles, const double dt = 1, const double extent = 0)
{
    std::ranges::for_each(particles, [&](Particle& p)
    {
        /*const double r   = std::sqrt(p.x*p.x + p.y*p.y);
        const double phi = std::atan2(p.y, p.x);
        const double w = (r > 0) ? 0.01/r : 0;
        double phi_new = phi + w*dt;
        
        p.x = r*std::cos(phi_new);
        p.y = r*std::sin(phi_new);*/

        p.x += p.vx * dt;
        p.y += p.vy * dt;

        if (extent > 0) {
            if      (p.x >  .5*extent) p.x -= extent;
            else if (p.x < -.5*extent) p.x += extent;
            if      (p.y >  .5*extent) p.y -= extent;
            else if (p.y < -.5*extent) p.y += extent;
        }
    });

}

void apply_rules(std::vector<Particle>& particles, const double extent)
{
    constexpr double radius = 1. + std::sqrt(.5);
    auto active_neighbor_count = get_active_neighbor_count(particles, radius, extent);
    for (int i : std::views::iota(0, (int)particles.size())) {
        auto n = active_neighbor_count.at(i);
        auto& p = particles.at(i);
        
        if ((p.status == Particle::Status::active) && ((n < 2) || (n > 3))) p.status = Particle::Status::inactive;
        if ((p.status == Particle::Status::inactive) && (n == 3)) p.status = Particle::Status::active;
    }
}

std::vector<Particle> generate_input(const int n, const double extent)
{
    constexpr int seed = 0;
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> position_distribution(-.5*extent, .5*extent);
    std::uniform_real_distribution<double> velocity_distribution(-1, 1);
    std::bernoulli_distribution bernoulli_distribution(.5);
    std::vector<Particle> particles(n);
    std::ranges::generate(particles, [&]() -> Particle {
        double x  = position_distribution(generator);
        double y  = position_distribution(generator);
        double vx = velocity_distribution(generator);
        double vy = velocity_distribution(generator);
        Particle::Status status = bernoulli_distribution(generator) ? Particle::Status::active : Particle::Status::inactive;
        return Particle {x, y, vx, vy, status};
    });
    return particles;
}

int main(int argc, char** argv)
{
    Config config = parse_program_options(argc, argv);
    if (config.no_output) config.output = "";
    Writer writer(config.output, config.write_coords_once);

    std::vector<Particle> particles;
    if (config.input.size())
        particles = read_input_file(config.input);
    else
        particles = generate_input(100, 10);



    int t = 0;
    int active_count = 0;
    do {
        writer(particles, t);
        active_count = std::ranges::count_if(particles, [](const Particle& p){ return p.status == Particle::Status::active; });
        std::cout << "t = " << t << "  active: " << active_count << "\n";
        advance_particles(particles, 1, config.extent);
        apply_rules(particles, config.extent);
        t++;
    } while ((active_count > 0) && (t < config.tmax));
}