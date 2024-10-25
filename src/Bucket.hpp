#pragma once
#include <optional>
#include <array>

#include "DataItem.hpp"
#include "Common.hpp"

template<typename T>
class Bucket {
public:
    Bucket() : localDepth(0), validEntryCount(0) {}
    Bucket(const uint32_t localDepth) : localDepth(localDepth), validEntryCount(0) {}

    uint8_t getLocalDepth() const { return localDepth; }
    uint32_t getEntryCount() const { return validEntryCount; }
    const std::array<DataItem<T>, BUCKET_CAPACITY>& getItems() const { return items; }

    bool write(const uint32_t key, const T& data);
    bool erase(const uint32_t key);

    void display() const;
    std::optional<T> find(const uint32_t key) const;

private:
    uint8_t localDepth{ 0 };       // Default initialization for localDepth
    uint32_t validEntryCount{ 0 };  // Default initialization for validEntryCount
    std::array<DataItem<T>, BUCKET_CAPACITY> items;
};
