#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include <iomanip>

int main() {
    long long num_steps;
    int num_threads;

    std::cout << "Podaj liczbe podzialow: ";
    std::cin >> num_steps;
    std::cout << "Podaj liczbe watkow: ";
    std::cin >> num_threads;

    double step = 1.0 / static_cast<double>(num_steps);
    std::vector<double> partial_sums(num_threads, 0.0);
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    // Wyrażenie lambda dla obliczeń cząstkowych
    auto calculate_part = [step, num_steps, num_threads](int thread_id, double& result) {
        double sum = 0.0;
        for (long long i = thread_id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        result = sum * step;
        };

    // Tworzenie wątków (Standard POSIX/C++ thread)
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(calculate_part, i, std::ref(partial_sums[i]));
    }

    for (auto& t : threads) {
        t.join();
    }

    double pi = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Wynik PI: " << pi << std::endl;
    std::cout << "Czas obliczen: " << elapsed.count() << " s" << std::endl;

    return 0;
}