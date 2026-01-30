#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <cstdint>

struct DcRand {
    uint32_t state = 1;
    void srand(uint32_t seed) { state = seed; }

    uint32_t rand31() {
        state = state * 1103515245u + 12345u;
        return state & 0x7FFFFFFF;
    }
};

static inline int32_t fptosi(float f) {
    return (int32_t)f;
}

static inline int32_t simulate_value(DcRand &rng) {
    uint32_t r = rng.rand31();

    float f = (float)(int32_t)r;
    float div = 100000.0f;

    float out = f / div;

    return fptosi(out);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout <<
          "Usage: ./dc_seed_sim <seed_hex> <iterations>\n"
          "Example: ./dc_seed_sim 00000001 500000\n";
        return 1;
    }

    uint32_t seed = std::stoul(argv[1], nullptr, 16);
    int iterations = std::stoi(argv[2]);

    DcRand rng;
    rng.srand(seed);

    std::unordered_map<int32_t,int64_t> freq;

    for (int i=0; i<iterations; i++) {
        int32_t v = simulate_value(rng);
        freq[v]++;
    }

    std::vector<std::pair<int32_t,int64_t>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
              [](auto &a, auto &b){ return a.first < b.first; });

    std::ofstream out("seed_results.csv");

    out << "Value,Count,Probability\n";

    for (auto &p : sorted) {
        double prob = (double)p.second / (double)iterations;
        out << p.first << ","
            << p.second << ","
            << std::fixed << std::setprecision(8)
            << prob << "\n";
    }

    out.close();

    std::cout << "Wrote seed_results.csv with "
              << sorted.size() << " unique outcomes\n";
}