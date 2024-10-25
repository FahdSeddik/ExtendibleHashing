#include "Bucket.hpp"
#include <iostream>

template <typename T>
void Bucket<T>::display() const {
    std::cout << "[";
    for (size_t i = 0; i < items.size() - 1; i++) {
        items[i].display();
        std::cout << ", ";
    }
    items.back().display();
    std::cout << "]";
}

/**
 * @brief Finds the data associated with the given key in the bucket.
 * 
 * This function searches through the items in the bucket to find an item
 * with the specified key. If such an item is found and is valid, the function
 * returns the associated data. If no such item is found, the function returns
 * std::nullopt to indicate that the key was not found.
 * 
 * @param key The key to search for in the bucket.
 * @return std::optional<T> The data associated with the key if found, or std::nullopt if not found.
 */
template <typename T>
std::optional<T> Bucket<T>::find(const uint32_t key) const {
    // TODO 3
    for (const auto& item : items) {
        if (item.isValid() && item.getKey() == key) {
            return item.getData();
        }
    }
    // END TODO

    // return std::nullopt to indicate not found
    return std::nullopt;
}


/**
 * @brief Writes a data item into the bucket.
 *
 * This function attempts to write a data item into the bucket using the provided key and data.
 * If the bucket is already full (i.e., the number of valid entries equals the bucket capacity),
 * the function returns false. Otherwise, it searches for an invalid entry slot in the bucket's
 * items and writes the new data item there, marking it as valid and incrementing the valid entry count.
 *
 * @tparam T The type of the data item to be written.
 * @param key The key associated with the data item.
 * @param data The data item to be written into the bucket.
 * @return true if the data item was successfully written into the bucket, false if the bucket is full.
 */
template <typename T>
bool Bucket<T>::write(const uint32_t key, const T& data) {
    // TODO 1

    if (validEntryCount == BUCKET_CAPACITY) return false;

    for (auto& item : items) {
        if (!item.isValid()) {
            item = DataItem<T>(key, data);
            validEntryCount++;
            return true;
        }
    }

    return false;
}

/**
 * @brief Erases an item from the bucket based on the provided key.
 * 
 * This method searches for an item with the specified key in the bucket.
 * If the item is found and is valid, it marks the item as invalid and 
 * decrements the count of valid entries. If the item is not found or 
 * there are no valid entries, the method returns false.
 * 
 * @tparam T The type of items stored in the bucket.
 * @param key The key of the item to be erased.
 * @return true if the item was found and erased, false otherwise.
 */
template <typename T>
bool Bucket<T>::erase(const uint32_t key) {
    // TODO 2

    if (validEntryCount == 0) return false;

    for (auto& item : items) {
        if (item.isValid() && item.getKey() == key) {
            item.markInvalid();
            validEntryCount--;
            return true;
        }
    }
    
    return false;
}

template class Bucket<int>;