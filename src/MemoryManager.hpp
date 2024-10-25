#pragma once

#include "GlobalDirectory.hpp"
#include "Common.hpp"

template <typename T>
class MemoryManager {
public:
    static MemoryManager& getInstance() {
        static MemoryManager instance;
        return instance;
    }

    [[nodiscard]] bool write(const uint32_t key, const T& data);
    [[nodiscard]] bool erase(const uint32_t key);

    [[nodiscard]] bool searchAndPrint(const uint32_t key) const;

    void display() const;

    // Deleted copy constructor and assignment operator
    MemoryManager(const MemoryManager&) = delete;
    MemoryManager& operator=(const MemoryManager&) = delete;

private:
    // Private constructor
    MemoryManager() = default;

    GlobalDirectory<T>& globalDirectory = GlobalDirectory<T>::getInstance();
    std::shared_ptr<Bucket<T>> initialFile = std::make_shared<Bucket<T>>();
};
