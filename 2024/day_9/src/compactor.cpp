#include "compactor.h"

#include <algorithm>
#include <numeric>

std::vector<int> expandInput(const std::vector<int>& input) {
    const auto length = static_cast<size_t>(std::accumulate(input.begin(), input.end(), 0));
    auto index = 0;
    auto is_space = false;
    auto data = std::vector<int>{};
    data.reserve(length);
    for (const auto& c : input) {
        for (int i = 0; i < c; ++i) {
            if (is_space) {
                data.emplace_back(-1);
            } else {
                data.emplace_back(index);
            }
        }

        if (! is_space) {
            index++;
        }
        is_space = ! is_space;
    }
    return data;
}

std::string makeIntoString(const std::vector<int>& input) {
    auto out = std::string{};
    for (const auto i : input) {
        if (i == -1) {
            out += ".";
        } else {
            out += std::to_string(i);
        }
    }
    return out;
}

Compactor::Compactor(const std::vector<int>& input)
    : data_(expandInput(input))
    , start_(0)
    , end_(data_.size() - 1) {
}

std::string Compactor::toString() const {
    return makeIntoString(data_);
}

void Compactor::next() {
    if (fullyCompact()) {
        return;
    }
    for (auto i = start_; i < end_; ++i) {
        if (data_[i] == -1) {
            start_ = i;
            break;
        }
    }
    for (auto i = end_; i > start_; --i) {
        if (data_[i] != -1) {
            end_ = i;
            break;
        }
    }
    std::swap(data_[start_], data_[end_]);
    start_++;
    end_--;
}

bool Compactor::fullyCompact() const {
    return start_ >= end_;
}

size_t Compactor::checksum() const {
    auto sum = size_t{ 0 };
    for (auto i = size_t{ 0 }; i < data_.size(); ++i) {
        if (data_[i] == -1)
            break;
        sum += i * data_[i];
    }
    return sum;
}

Compactor2::Compactor2(const std::vector<int>& input)
    : data_(expandInput(input))
    , start_space_(0)
    , end_space_(0)
    , start_word_(data_.size() - 1)
    , end_word_(data_.size() - 1)
    , max_index_(std::ranges::max(data_)) {
}

std::string Compactor2::toString() const {
    return makeIntoString(data_);
}

void Compactor2::next() {
    if (fullyCompact()) {
        return;
    }
    word();
    const auto word_len = end_word_ - start_word_;
    firstSpace();
    while (end_space_ <= start_word_) {
        if (end_space_ - start_space_ >= word_len) {
            const auto ss = data_.begin() + start_space_;
            const auto ws = data_.begin() + start_word_;
            std::ranges::swap_ranges(ss, ss + word_len, ws, ws + word_len);
            break;
        }
        nextSpace();
    }
}

bool Compactor2::fullyCompact() const {
    return max_index_==0;
}

size_t Compactor2::checksum() const {
    auto sum = size_t{ 0 };
    for (auto i = size_t{ 0 }; i < data_.size(); ++i) {
        if (data_[i] == -1)
            continue;
        sum += i * data_[i];
    }
    return sum;
}

std::pair<size_t, size_t> Compactor2::firstSpace() {
    start_space_ = 0;
    end_space_ = 0;
    return nextSpace();
}

std::pair<size_t, size_t> Compactor2::nextSpace() {
    start_space_ = end_space_;
    while (data_[start_space_] != -1 && start_space_ < data_.size()) {
        start_space_++;
    }
    end_space_ = start_space_ + 1;
    while (data_[end_space_] == -1 && end_space_ <= data_.size()) {
        end_space_++;
    }
    return { start_space_, end_space_ };
}

std::pair<size_t, size_t> Compactor2::word() {
    end_word_ = start_word_;
    while (data_[end_word_] != max_index_ && end_word_ > 0) {
        end_word_--;
    }
    end_word_++;

    start_word_ = end_word_ - 1;
    while (data_[start_word_] == max_index_ && start_word_ > 0) {
        start_word_--;
    }
    start_word_++;
    max_index_--;
    return { start_word_, end_word_ };
}
