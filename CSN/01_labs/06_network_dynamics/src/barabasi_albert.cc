#include "barabasi_albert.h"

BarabasiAlbert::BarabasiAlbert (const uint &n0_, const uint &m0_)
    : n0(n0_), m0(m0_), generator(device()) {
    if (m0 > n0)
        cerr << "WARNING: m0 > n0." << endl;
}

void BarabasiAlbert::start_random_cycle (Graph &graph) {
    Vector order = Vector(graph.n_vertices());

    for (uint i = 0; i < graph.n_vertices(); ++i)
        order[i] = i;
    shuffle(order.begin(), order.end(), generator);

    for (uint i = 1; i < order.size(); ++i) {
        graph.add_edge(order[i-1], order[i]);
    }
    if (order.size() > 2)
        graph.add_edge(order[order.size()-1], order[0]);
}

void BarabasiAlbert::start_random_cycle_stubs (Graph &graph, Vector &stubs) {
    Vector order = Vector(graph.n_vertices());

    for (uint i = 0; i < graph.n_vertices(); ++i)
        order[i] = i;
    shuffle(order.begin(), order.end(), generator);

    for (uint i = 1; i < order.size(); ++i) {
        graph.add_edge(order[i-1], order[i]);
        stubs.push_back(order[i-1]);
        stubs.push_back(order[i]);
    }
    if (order.size() > 2) {
        graph.add_edge(order[order.size()-1], order[0]);
        stubs.push_back(order[order.size()-1]);
        stubs.push_back(order[0]);
    }
}

void BarabasiAlbert::write_degree_sequence(const Graph &graph, const string &main_name) const {
    ofstream output(main_name + "_ds.txt");
    graph.write_degree_sequence(output);
    output.close();
}

void BarabasiAlbert::write_time_series(const Graph &graph, const uint &t, const string &main_name) const {
    if (t == 1) {
        ofstream output(main_name + "_ts_1.txt");
        graph.write_limited_degree_sequence(n0+1, output);
        output.close();
    } else if (t == 10) {
        ofstream output(main_name + "_ts_2.txt");
        graph.write_limited_degree_sequence(n0+1, output);
        output.close();
    } else if (t == 100) {
        ofstream output(main_name + "_ts_3.txt");
        graph.write_limited_degree_sequence(n0+1, output);
        output.close(); 
    } else if (t == 1000) {
        ofstream output(main_name + "_ts_4.txt");
        graph.write_limited_degree_sequence(n0+1, output);
        output.close();
    }
}

void BarabasiAlbert::growth_preferential (const uint &t_max) {
    Graph graph(n0);
    Vector stubs(0);
    start_random_cycle_stubs(graph, stubs);
    string main_name = "../data/gp_" + to_string(n0) + "_" + to_string(m0) + "_" + to_string(t_max);

    for (uint t = 1; t < t_max; ++ t) {
        uint min_m0 = min(m0, graph.n_vertices());
        uint node = graph.create_node();
        for (uint i = 0; i < min_m0; ++i) {
            uniform_int_distribution<int> distribution(0,stubs.size()-1);
            uint idx = distribution(generator);
            while (graph.has_connection(node, stubs[idx]))
                idx = distribution(generator);
            graph.add_edge(stubs[idx], node);
            stubs.push_back(stubs[idx]);
            stubs.push_back(node);
        }
        write_time_series(graph, t, main_name);
    }
    write_degree_sequence(graph, main_name);
}

void BarabasiAlbert::growth_random (const uint &t_max) {
    Graph graph(n0);
    start_random_cycle(graph);
    string main_name = "../data/gr_" + to_string(n0) + "_" + to_string(m0) + "_" + to_string(t_max);

    for (uint t = 1; t < t_max; ++ t) {
        uint min_m0 = min(m0, graph.n_vertices());
        uint new_node = graph.create_node();
        for (uint i = 0; i < min_m0; ++i) {
            uniform_int_distribution<int> distribution(0, graph.n_vertices()-1);
            uint old_node = distribution(generator);
            while (graph.has_connection(new_node, old_node))
                old_node = distribution(generator);
            graph.add_edge(old_node, new_node);
        }
        write_time_series(graph, t, main_name);
    }
    write_degree_sequence(graph, main_name);
}

void BarabasiAlbert::nogrowth_preferential (const uint &t_max) {
    Graph graph(n0);
    Vector stubs(0);
    start_random_cycle_stubs(graph, stubs);
    string main_name = "../data/np_" + to_string(n0) + "_" + to_string(m0) + "_" + to_string(t_max);

    uniform_int_distribution<int> distribution(0, n0-1);
    for (uint t = 1; t < t_max; ++ t) {
        uint node = distribution(generator);
        uint min_m0 = min(m0, n0-1-graph.degree(node));
        for (uint i = 0; i < min_m0; ++i) {
            uniform_int_distribution<int> stub_distribution(0, stubs.size()-1);
            uint idx = stub_distribution(generator);
            while (graph.has_connection(node, stubs[idx]))
                idx = stub_distribution(generator);
            graph.add_edge(stubs[idx], node);
            stubs.push_back(stubs[idx]);
            stubs.push_back(node);
        }
        write_time_series(graph, t, main_name);
    }
    write_degree_sequence(graph, main_name);
}