#pragma once
#include <array>
#include <utility>
#include <iostream>

#include "Common.hpp"

template<typename T>
class Bucket;

/**
 * @class DataItem
 * @brief A template class representing a data item with a key and validity status.
 * 
 * This class provides functionality to manage a data item, including move semantics,
 * validity marking, and data display.
 * 
 * @tparam T The type of the data stored in the DataItem.
 */
template<typename T>
class DataItem {
public:
    // Move Constructor
    DataItem(DataItem&& other) noexcept
        : valid(std::move(other.valid)),
        data(std::move(other.data)),
        key(std::move(other.key)) {
        other.valid = false; // Invalidate the moved-from object
    }

    // Move Assignment Operator
    DataItem& operator=(DataItem&& other) noexcept {
        if (this != &other) {
            valid = std::move(other.valid);
            data = std::move(other.data);
            key = std::move(other.key);
            other.valid = false; // Invalidate the moved-from object
        }
        return *this;
    }

    void markInvalid() { valid = false; }
    void markValid() { valid = true; }

    void display() const { 
        if (valid) {
            std::cout << data;
        } else {
            std::cout << "null";
        }
    }

    uint32_t getKey() const { return key; }
    T getData() const { return data; }
    [[nodiscard]] bool isValid() const { return valid; }


private:
    // private constructors as only Bucket & std::array can invoke constructor
    constexpr DataItem() : valid(false), data(T()), key(0) {}
    constexpr DataItem(const uint32_t key, const T& data) : valid(true), data(data), key(key) {}

    bool valid{ false };    // Initialized as invalid by default
    T data;                 // Default initialization for data
    uint32_t key{ 0 };      // Default initialization for key

    friend class Bucket<T>;
    friend class std::array<DataItem, BUCKET_CAPACITY>;
};
