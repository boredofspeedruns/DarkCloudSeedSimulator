#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <limits>

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

static inline int32_t simulate_value(DcRand &rng, uint32_t divisor_u32) {
    uint32_t r = rng.rand31();
    float f = (float)(int32_t)r;

    float div = (float)(int32_t)divisor_u32;

    float out = f / div;
    return fptosi(out);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout <<
          "Usage: ./dc_seed_sim <seed_hex> <sim_count> [divisor_u32_hex]\n"
          "Example: ./dc_seed_sim 00000001 500000 000186A0\n";
        return 1;
    }

    uint32_t seed = (uint32_t)std::stoul(argv[1], nullptr, 16);
    uint64_t sim_count = (uint64_t)std::stoull(argv[2]);

    uint32_t divisor_u32 = 0x000186A0u;
    if (argc >= 4) divisor_u32 = (uint32_t)std::stoul(argv[3], nullptr, 16);

    if (divisor_u32 == 0) {
        std::cerr << "Error: divisor_u32 cannot be 0\n";
        return 1;
    }

    DcRand rng;
    rng.srand(seed);

    std::unordered_map<int32_t, uint64_t> freq;
    int32_t minV = std::numeric_limits<int32_t>::max();
    int32_t maxV = std::numeric_limits<int32_t>::min();

    for (uint64_t i = 0; i < sim_count; i++) {
        int32_t v = simulate_value(rng, divisor_u32);
        freq[v]++;

        if (v < minV) minV = v;
        if (v > maxV) maxV = v;
    }

    std::vector<std::pair<int32_t,uint64_t>> sorted(freq.begin(), freq.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto &a, const auto &b){ return a.first < b.first; });

    uint64_t unique = (uint64_t)freq.size();
    uint64_t missing = 0;

    int64_t range_len64 = (int64_t)maxV - (int64_t)minV + 1;
    if (range_len64 > 0 && range_len64 <= 50'000'000) {
        for (int32_t v = minV; v <= maxV; v++) {
            if (freq.find(v) == freq.end()) missing++;
            if (v == maxV) break; 
        }
    } else if (range_len64 > 0) {
        missing = (uint64_t)range_len64 - unique;
    }

    std::ofstream out("seed_results.csv");
    out << "Value,Count,Probability\n";

    for (auto &p : sorted) {
        double prob = (double)p.second / (double)sim_count;
        out << p.first << ","
            << p.second << ","
            << std::fixed << std::setprecision(10)
            << prob << "\n";
    }
    out.close();

    std::cout << "seed=" << std::showbase << std::hex << seed << std::dec
              << " (" << seed << ")\n";
    std::cout << "sim_count=" << sim_count << "\n";
    std::cout << "divisor=" << std::showbase << std::hex << divisor_u32 << std::dec
              << " (" << divisor_u32 << ")\n";
    std::cout << "observed_range=[" << minV << ".." << maxV << "]\n";
    std::cout << "unique_values=" << unique << "\n";
    std::cout << "unreached_values_within_range=" << missing
              << " (out of " << (uint64_t)((int64_t)maxV - (int64_t)minV + 1) << ")\n";

    std::cout << "Wrote seed_results.csv\n";
    return 0;
}