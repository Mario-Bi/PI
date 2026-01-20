#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include <iomanip>

/**
 * @brief Program obliczajacy przyblizona wartosc liczby PI
 *
 * Program wykorzystuje metode calkowania numerycznego
 * (metoda prostokatow) oraz wielowatkowosc do przyspieszenia obliczen.
 *
 * Uzytkownik podaje liczbe podzialow oraz liczbe watkow.
 * Kazdy watek oblicza czesciowa sume calki, a wynik koncowy
 * jest suma wszystkich wynikow czastkowych.
 *
 * @return Zwraca 0 po poprawnym zakonczeniu programu
 */
int main() {
    /**
     * @brief Liczba podzialow przedzialu [0,1]
     */
    long long num_steps;

    /**
     * @brief Liczba watkow uzytych do obliczen
     */
    int num_threads;

    std::cout << "Podaj liczbe podzialow: ";
    std::cin >> num_steps;
    std::cout << "Podaj liczbe watkow: ";
    std::cin >> num_threads;

    /**
     * @brief Szerokosc pojedynczego kroku calkowania
     */
    double step = 1.0 / static_cast<double>(num_steps);

    /**
     * @brief Wektor przechowujacy wyniki czastkowe z poszczegolnych watkow
     */
    std::vector<double> partial_sums(num_threads, 0.0);

    /**
     * @brief Wektor obiektow std::thread
     */
    std::vector<std::thread> threads;

    /**
     * @brief Czas rozpoczecia obliczen
     */
    auto start_time = std::chrono::high_resolution_clock::now();

    /**
     * @brief Funkcja lambda obliczajaca czastkowa sume calki
     *
     * Kazdy watek przetwarza co num_threads-ty krok,
     * zaczynajac od swojego identyfikatora.
     *
     * @param thread_id Identyfikator watku
     * @param result Referencja do zmiennej przechowujacej wynik czastkowy
     */
    auto calculate_part = [step, num_steps, num_threads](int thread_id, double& result) {
        double sum = 0.0;
        for (long long i = thread_id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        result = sum * step;
        };

    /**
     * @brief Tworzenie i uruchamianie watkow
     */
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(calculate_part, i, std::ref(partial_sums[i]));
    }

    /**
     * @brief Oczekiwanie na zakonczenie wszystkich watkow
     */
    for (auto& t : threads) {
        t.join();
    }

    /**
     * @brief Obliczenie koncowej wartosci PI
     */
    double pi = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);

    /**
     * @brief Czas zakonczenia obliczen
     */
    auto end_time = std::chrono::high_resolution_clock::now();

    /**
     * @brief Czas trwania obliczen
     */
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Wynik PI: " << pi << std::endl;
    std::cout << "Czas obliczen: " << elapsed.count() << " s" << std::endl;

    return 0;
}