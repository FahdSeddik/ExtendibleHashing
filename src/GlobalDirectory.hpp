#pragma once
#include <optional>
#include <vector>
#include <memory>

#include "Common.hpp"

template<typename T>
class Bucket;

template<typename T>
class GlobalDirectory {
public:
    // Singleton pattern
    static GlobalDirectory& getInstance() {
        static GlobalDirectory instance;
        return instance;
    }

    // Called when required to create directory
    bool initialize(const std::shared_ptr<Bucket<T>>& initialFile);

    [[nodiscard]] bool write(const uint32_t key, const T& data);
    [[nodiscard]] bool erase(const uint32_t key);

    void display() const;
    [[nodiscard]] std::optional<T> find(const uint32_t key) const;

    uint8_t getGlobalDepth() const { return globalDepth; }

    // Deleted copy constructor and assignment operator
    GlobalDirectory(const GlobalDirectory&) = delete;
    GlobalDirectory& operator=(const GlobalDirectory&) = delete;

private:
    // Private constructor
    GlobalDirectory() = default;

    // Utility functions
    [[nodiscard]] bool extend(const uint32_t hashValue);
    [[nodiscard]] bool minimize();

    [[nodiscard]] bool reHashItems(const std::shared_ptr<Bucket<T>>& oldBucket);

    [[nodiscard]] bool mergeOn(const uint32_t hashValue);
    [[nodiscard]] bool splitOn(const uint32_t hashValue);

    uint32_t hash(const uint32_t key) const;

    uint8_t globalDepth{ 0 };
    std::vector<std::shared_ptr<Bucket<T>>> entry;
};
