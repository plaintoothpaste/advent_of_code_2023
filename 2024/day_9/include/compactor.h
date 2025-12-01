#pragma once
#include <string>
#include <vector>

class Compactor {
private:
    std::vector<int> data_;
    size_t start_;
    size_t end_;

public:
    explicit Compactor(const std::vector<int>& input);

    [[nodiscard]] std::string toString() const;

    void next();

    [[nodiscard]] bool fullyCompact() const;
    [[nodiscard]] size_t checksum() const;
};


class Compactor2 {
private:
    std::vector<int> data_;
    size_t start_space_;
    size_t end_space_;

    size_t start_word_;
    size_t end_word_;
    int max_index_;
public:
    explicit Compactor2(const std::vector<int>& input);

    [[nodiscard]] std::string toString() const;

    void next();

    [[nodiscard]] bool fullyCompact() const;
    [[nodiscard]] size_t checksum() const;

    /// return the first space index and set class values
    std::pair<size_t, size_t> firstSpace();

    /// return the next space based on the classes current space and also set class
    std::pair<size_t, size_t> nextSpace();

    /// starting from the highest count return the start and end of a word, also set class
    std::pair<size_t, size_t> word();
};