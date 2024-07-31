#include "simulator.h"
#include <algorithm>

const int LENGTH_OF_SEQUENCE = 500;
const int NUM_OF_PASSING_SEQUENCES = 35;
const int INITIAL_NUMBER_OF_SEQUENCES = 5000;
const int NUMBER_OF_SIMULATIONS = 300;
const int NUMBER_OF_MUTATIONS = 1000;

std::vector<std::vector<int>> generate_random_sequences(std::mt19937 &rng) {
    std::vector<std::vector<int>> sequences;

    for (int i=0;i<INITIAL_NUMBER_OF_SEQUENCES;i++) {
        std::vector<int> actual_sequence;

        for (int j=0;j<LENGTH_OF_SEQUENCE;j++) {
            actual_sequence.push_back(std::uniform_int_distribution<int>(0, 3)(rng));
        }
        sequences.push_back(actual_sequence);
    }
    return sequences;
}

int evaluate(uint64_t board) {
    uint64_t mx = 0;
    for (int i=0;i<16;i++) {
        mx = std::max(mx, ((board >> (4 * i)) & 0xF));
    }
    return (1 << mx);
}

std::vector<int> crossover(std::vector<int> &chromosome1, std::vector<int> &chromosome2, std::mt19937 &rng) {
    int position_for_crossover = std::uniform_int_distribution<int>(1, LENGTH_OF_SEQUENCE-1)(rng);
    std::vector<int> new_chromosome(LENGTH_OF_SEQUENCE);
    for (int i=0;i<position_for_crossover;i++) {
        new_chromosome[i] = chromosome1[i];
    }

    for (int i=position_for_crossover;i<LENGTH_OF_SEQUENCE;i++) {
        new_chromosome[i] = chromosome2[i];
    }

    return new_chromosome;
}

std::vector<int> mutation(std::vector<int> chromosome, std::mt19937 &rng) {
    int number_of_changes = std::uniform_int_distribution<int>(1, 3)(rng);
    for (int i=0;i<number_of_changes;i++) {
        chromosome[std::uniform_int_distribution<int>(0, LENGTH_OF_SEQUENCE-1)(rng)] = std::uniform_int_distribution<int>(0, 3)(rng);
    }
    return chromosome;
}

std::vector<std::vector<int>> genetic_algorithm_generation(std::vector<std::vector<int>> &initial_sequences, std::mt19937 &rng) {
    // first run every sequence through a few simulations
    std::vector<std::pair<int, int>> counter(initial_sequences.size());

    for (int i = 0; i < (int)initial_sequences.size(); i++) {
        counter[i].second = i;

        for (int j = 0; j < NUMBER_OF_SIMULATIONS; j++) {
            counter[i].first += evaluate(simulator_version_1(initial_sequences[i]));
        }
    }

    // sort the sequences based on the scores
    std::sort(counter.begin(), counter.end(), std::greater<std::pair<int, int>>());

    std::cout << "this generations' best chromosome reached an average score of ";
    std::cout << (counter[0].first / NUMBER_OF_SIMULATIONS) << "\n";

    // pick the top $NUM_OF_PASSING_SEQUENCES$ candidates and run crossover
    std::vector<std::vector<int>> new_sequences(NUM_OF_PASSING_SEQUENCES * NUM_OF_PASSING_SEQUENCES + NUMBER_OF_MUTATIONS);

    for (int i=0;i<NUM_OF_PASSING_SEQUENCES;i++) {
        for (int j=0;j<NUM_OF_PASSING_SEQUENCES;j++) {
            new_sequences[i * NUM_OF_PASSING_SEQUENCES + j] = crossover(initial_sequences[counter[i].second], initial_sequences[counter[j].second], rng);
        }
    }

    int current_size = NUM_OF_PASSING_SEQUENCES * NUM_OF_PASSING_SEQUENCES;
    
    for (int i = 0; i < NUMBER_OF_MUTATIONS; i++) {
        new_sequences[i + current_size] = mutation(new_sequences[std::uniform_int_distribution<int>(0, i+current_size-1)(rng)], rng);
    }

    return new_sequences;
}
 
int main() {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::vector<std::vector<int>> init = generate_random_sequences(rng);

    for (int i=0;i<1000;i++) {
        auto tmp = genetic_algorithm_generation(init, rng);
        init = tmp;
    }

    return 0;
}