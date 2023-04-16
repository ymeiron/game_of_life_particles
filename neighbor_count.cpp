#include <algorithm>
#include <vector>
#include <ranges>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Kd_tree.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/algorithm.h>
#include <CGAL/Fuzzy_sphere.h>
#include <CGAL/Search_traits_2.h>

#include "common.h"

std::vector<int> get_active_neighbor_count(const std::vector<Particle>& particles, const double radius, const double extent)
{
    using K             = CGAL::Simple_cartesian<double>;
    using Point         = K::Point_2;
    using Traits        = CGAL::Search_traits_2<K>;
    using Fuzzy_circle  = CGAL::Fuzzy_sphere<Traits>;
    using Tree          = CGAL::Kd_tree<Traits>;

    auto points_active = particles | std::views::filter([](const Particle& p){ return p.status == Particle::Status::active; })
                                   | std::views::transform([](const Particle& p){ return Point {p.x, p.y}; });

    /* Copy all particles that are a radius from the edge ... only the active ones*/
    std::vector<Point> points_across_boundary;

    if (extent > 0) {
        for (const auto& p : points_active) {
            if (p.x() < -.5*extent + radius) points_across_boundary.emplace_back(p.x() + extent, p.y());
            if (p.y() < -.5*extent + radius) points_across_boundary.emplace_back(p.x(), p.y() + extent);
            if (.5*extent - radius < p.x()) points_across_boundary.emplace_back(p.x() - extent, p.y());
            if (.5*extent - radius < p.y()) points_across_boundary.emplace_back(p.x(), p.y() - extent);
        }
    }

    auto points_all = particles | std::views::transform([](const Particle& p){ return Point {p.x, p.y}; });

    Tree tree(begin(points_active), end(points_active));
    tree.insert(begin(points_across_boundary), end(points_across_boundary));

    std::vector<int> neighbor_count;
    std::vector<Point> neighbors;
    for (int i = 0; const auto& point : points_all) {
        Fuzzy_circle circle(point, radius);
        tree.search(std::back_inserter(neighbors), circle);
        int current_count = neighbors.size();
        if (particles.at(i++).status == Particle::Status::active)
            current_count--;
        neighbor_count.push_back(current_count);
        neighbors.clear();
    }
    return neighbor_count;
}
