#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <chrono>
#include <future>


const size_t TOPK = 10;

using Counter = std::map<std::string, std::size_t>;

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](const unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

Counter count_words(const std::string& filename) {
    Counter counter{};
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        std::cerr << "Failed to open file " << filename << '\n';
        return {};
    }
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) { ++counter[tolower(s)]; });
    return counter;
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
        std::begin(words), std::begin(words) + k, std::end(words),
        [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + k,
        [&stream](const Counter::const_iterator &pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                      << '\n';
        });
}

int main(const int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    const auto start = std::chrono::high_resolution_clock::now();

    Counter freq_dict;

    std::vector<std::future<Counter>> vector_counters;

    for (int i = 1; i < argc; ++i) {
        vector_counters.emplace_back(std::async(std::launch::async, count_words, std::string(argv[i])));
    }

    for(auto& counter_element : vector_counters) {
        for(Counter result = counter_element.get(); const auto& [word, count] :result) {
            freq_dict[word] += count;
        }
    }
    print_topk(std::cout, freq_dict, TOPK);
    const auto end = std::chrono::high_resolution_clock::now();
    const auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

// C:\Users\User\CLionProjects\untitled2\1.txt
// C:\Users\User\CLionProjects\untitled2\2.txt
// C:\Users\User\CLionProjects\untitled2\3.txt
// C:\Users\User\CLionProjects\untitled2\4.txt
// C:\Users\User\CLionProjects\untitled2\5.txt
// C:\Users\User\CLionProjects\untitled2\6.txt
// C:\Users\User\CLionProjects\untitled2\7.txt
// C:\Users\User\CLionProjects\untitled2\8.txt
// C:\Users\User\CLionProjects\untitled2\9.txt
// C:\Users\User\CLionProjects\untitled2\10.txt
// C:\Users\User\CLionProjects\untitled2\11.txt
// C:\Users\User\CLionProjects\untitled2\12.txt
// C:\Users\User\CLionProjects\untitled2\13.txt
// C:\Users\User\CLionProjects\untitled2\14.txt
// C:\Users\User\CLionProjects\untitled2\15.txt
// C:\Users\User\CLionProjects\untitled2\16.txt
// C:\Users\User\CLionProjects\untitled2\17.txt
// C:\Users\User\CLionProjects\untitled2\18.txt
// C:\Users\User\CLionProjects\untitled2\19.txt
// C:\Users\User\CLionProjects\untitled2\20.txt