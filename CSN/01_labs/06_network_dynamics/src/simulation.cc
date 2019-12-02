#include "barabasi_albert.h"

void print_usage() {
    cerr << "usage: ./simulation type n0 m0 t_max" << endl;
    cerr << "\ttype: all gp gr np" << endl;
}

int main (int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "ERROR: some parameters are not specified." << endl;
        print_usage();
        return 1;
    }
    string type = argv[1];
    int n0 = atoi(argv[2]);
    int m0 = atoi(argv[3]);
    int t_max = atoi(argv[4]);

    BarabasiAlbert ba (n0, m0);
    if (type == "all") {
        ba.growth_preferential(t_max);
        ba.growth_random(t_max);
        ba.nogrowth_preferential(t_max);
    } else if (type == "gp") {
        ba.growth_preferential(t_max);
    } else if (type == "gr") {
        ba.growth_random(t_max);
    } else if (type == "np") {
        ba.nogrowth_preferential(t_max);
    } else {
        cerr << "ERROR: Non-defined type." << endl;
        print_usage();
    }    
}