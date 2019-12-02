#include "graph.h"
#include <random>

class BarabasiAlbert {
    private:
        random_device device;
        mt19937 generator;
        void start_random_cycle (Graph &graph);
        void start_random_cycle_stubs (Graph &graph, Vector &stubs);
        void write_degree_sequence(const Graph &graph, const string &main_name) const;
        void write_time_series(const Graph &graph, const uint &t, const string &main_name) const;
    public:
        uint n0, m0;
        BarabasiAlbert (const uint &n0_, const uint &m0_);
        void growth_preferential (const uint &t_max);
        void growth_random (const uint &t_max);
        void nogrowth_preferential (const uint &t_max);
};