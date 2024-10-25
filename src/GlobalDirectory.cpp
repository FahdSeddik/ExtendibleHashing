#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>

#include "GlobalDirectory.hpp"
#include "Bucket.hpp"

/**
 * @brief Initializes the GlobalDirectory with an initial file.
 * 
 * This function sets up the GlobalDirectory by creating two initial buckets
 * and setting the global depth to 1. It then rehashes the items from the 
 * provided initial file into the newly created buckets.
 * 
 * @tparam T The type of elements stored in the buckets.
 * @param initialFile A shared pointer to the initial bucket containing items to be rehashed.
 * @return true if the GlobalDirectory was successfully initialized, false if it was already initialized.
 */
template <typename T>
bool GlobalDirectory<T>::initialize(const std::shared_ptr<Bucket<T>>& initialFile) {
    if (!entry.empty()) return false; // already initialized

    globalDepth = 1;
    entry.resize(2);
    entry[0] = std::make_shared<Bucket<T>>(globalDepth);
    entry[1] = std::make_shared<Bucket<T>>(globalDepth);

    return reHashItems(initialFile);
}

template <typename T>
void GlobalDirectory<T>::display() const {
    if (entry.empty()) return; // Not initialized

    std::cout << "Global Directory\n";
    std::cout << "Global Depth: " << (uint32_t)globalDepth << "\n";
    
    // name each bucket with a letter
    // A, B, C, ..., Z, AA, AB, AC, ..., ZZ, AAA, ...
    std::unordered_map<std::shared_ptr<Bucket<T>>, std::string> bucketNames;
    uint32_t maxWidth = 0;
    for (const std::shared_ptr<Bucket<T>>& ptr : entry) {
        if (ptr && bucketNames.find(ptr) == bucketNames.end()) {
            std::string name;
            int temp = bucketNames.size();
            while (temp >= 0) {
                name = (char)('A' + (temp % 26)) + name;
                temp = temp / 26 - 1;
                if (temp < 0) break;
            }
            bucketNames[ptr] = name;
            maxWidth = std::max(maxWidth, (uint32_t)name.size());
        }
    }

    std::cout << "Number of buckets: " << bucketNames.size() << "/" << entry.size() << "\n";
    // Print header
    std::cout << std::setw(10) << std::left << "Index"
              << std::setw(maxWidth + 4) << "Bucket"
              << std::setw(12) << "Local Depth"
              << "Entries\n";

    for (size_t i = 0; i < entry.size(); ++i) {
        const std::shared_ptr<Bucket<T>>& ptr = entry[i];
        if (ptr) {
            std::cout << std::setw(10) << std::left << ("[" + std::to_string(i) + "] ->")
                      << std::setw(maxWidth + 4) << std::left << bucketNames[ptr]
                      << std::setw(12) << std::left << ("(" + std::to_string(ptr->getLocalDepth()) + ")")
                      << " ";
            ptr->display();
            std::cout << std::endl;
        }
    }
}

/**
 * @brief Computes the hash value for a given key.
 *
 * This function takes a key and computes its hash value based on the 
 * global depth and predefined constants. The hash value is derived by 
 * performing a bitwise AND operation with MAX_KEY_VALUE and then 
 * right-shifting the result by the difference between MAX_KEY_LENGTH 
 * and globalDepth.
 *
 * @tparam T The type parameter for the GlobalDirectory class.
 * @param key The key for which the hash value is to be computed.
 * @return The computed hash value as a 32-bit unsigned integer.
 */
template<typename T>
uint32_t GlobalDirectory<T>::hash(const uint32_t key) const {
    return (key & MAX_KEY_VALUE) >> (MAX_KEY_LENGTH - globalDepth);
}

/**
 * @brief Writes data to the global directory using the specified key.
 *
 * This function attempts to write the provided data to the global directory
 * at the position determined by the hash of the key. If the initial write
 * attempt fails, it will retry up to 5 times, extending the directory if necessary.
 *
 * @tparam T The type of data to be written.
 * @param key The key used to determine the position in the directory.
 * @param data The data to be written to the directory.
 * @return true if the data was successfully written, false otherwise.
 */
template <typename T>
bool GlobalDirectory<T>::write(const uint32_t key, const T& data) {
    if (entry.empty()) return false; // Not initialized

    // TODO 5
    uint32_t index = hash(key);
    if (entry[index]->write(key, data)) return true;
    const int RETRIES = 5;
    for (uint32_t i = 0; i < RETRIES; i++, index = hash(key)) {
        if (!extend(index)) continue;
        index = hash(key);
        if (entry[index]->write(key, data)) return true;
    }

    return false;
}

/**
 * @brief Erases an entry from the global directory based on the provided key.
 * 
 * This function attempts to remove an entry identified by the given key from the global directory.
 * If the entry is successfully removed, it may also attempt to merge and minimize the directory
 * structure to optimize storage.
 * 
 * @tparam T The type of the elements stored in the global directory.
 * @param key The key of the entry to be erased.
 * @return true if the entry was successfully erased, false otherwise.
 */
template <typename T>
bool GlobalDirectory<T>::erase(const uint32_t key) {
    if (entry.empty()) return false; // Not initialized

    // TODO 6
    uint32_t index = hash(key);
    // get target bucket
    std::shared_ptr<Bucket<T>> targetBucket = entry[index];
    if (targetBucket->erase(key)) {
        while(mergeOn(index) && minimize()) {
            index = hash(key);
        }
        return true;
    }

    return false;
}

/**
 * @brief Finds an entry in the GlobalDirectory using the provided key.
 * 
 * This function hashes the given key to determine the index in the directory
 * and then attempts to find the entry associated with that key.
 * 
 * @tparam T The type of the entry stored in the GlobalDirectory.
 * @param key The key used to locate the entry.
 * @return std::optional<T> An optional containing the entry if found, or std::nullopt if not found.
 */
template <typename T>
std::optional<T> GlobalDirectory<T>::find(const uint32_t key) const {
    // TODO 4
    uint32_t index = hash(key);
    return entry[index]->find(key);
}

/**
 * @brief Rehashes the items from the given old bucket into the global directory.
 *
 * This function iterates through all items in the provided old bucket and rehashes
 * them into the global directory. Only valid items are rehashed. If any item fails
 * to be written to the new location, the function returns false.
 *
 * @tparam T The type of the items stored in the bucket.
 * @param oldBucket A shared pointer to the bucket containing the items to be rehashed.
 * @return true if all valid items are successfully rehashed, false otherwise.
 */
template<typename T>
bool GlobalDirectory<T>::reHashItems(const std::shared_ptr<Bucket<T>>& oldBucket) {
    for (const auto& item : oldBucket->getItems()) {
        if (item.isValid()) {
            if(!write(item.getKey(), item.getData())) return false;
        }
    }
    return true;
}

/**
 * @brief Splits the bucket at the given hash value into two new buckets and redistributes the entries.
 *
 * This function is responsible for handling the splitting of a bucket when it becomes full. It identifies the 
 * appropriate bucket to split based on the provided hash value, creates two new buckets with an incremented 
 * local depth, and redistributes the entries between the old and new buckets.
 *
 * @tparam T The type of the elements stored in the buckets.
 * @param hashValue The hash value used to identify the bucket to split.
 * @return true if the rehashing of items was successful, false otherwise.
 *
 * The function performs the following steps:  
 * 1. Identifies the index of the bucket to split by decrementing the index until it finds a bucket with a 
 *    different local depth.
 * 2. Calculates the number of pointers (oldNumPtrs) pointing to the old bucket and the number of pointers
 *    (newNumPtrs) that will point to each of the new buckets.
 * 3. Creates two new buckets (newBucket1 and newBucket2) with an incremented local depth.
 * 4. Updates the directory entries to point to the new buckets.
 * 5. Calls the reHashItems function to redistribute the entries from the old bucket to the new buckets.
 */
template<typename T>
bool GlobalDirectory<T>::splitOn(const uint32_t hashValue) {
    // TODO 7
    uint32_t index = hashValue;
    while(index > 0 && entry[index] == entry[index - 1]) {
        index--;
    }
    uint32_t oldNumPtrs = 1 << (globalDepth - entry[index]->getLocalDepth());
    uint32_t newNumPtrs = oldNumPtrs / 2;
    // old bucket
    std::shared_ptr<Bucket<T>> oldBucket = entry[index];
    // new bucket
    std::shared_ptr<Bucket<T>> newBucket1 = std::make_shared<Bucket<T>>(oldBucket->getLocalDepth() + 1);
    std::shared_ptr<Bucket<T>> newBucket2 = std::make_shared<Bucket<T>>(oldBucket->getLocalDepth() + 1);
    for(size_t i = 0; i < newNumPtrs; i++) {
        entry[index + i] = newBucket1;
        entry[index + i + newNumPtrs] = newBucket2;
    }

    return reHashItems(oldBucket);
}

/**
 * @brief Extends the global directory by increasing its depth and redistributing entries.
 *
 * This function attempts to extend the global directory by increasing its depth and redistributing
 * the entries. If the local depth of the bucket at the given hash value is less than the global depth,
 * it splits the bucket. If the global depth has reached the maximum key length, the function returns false.
 * Otherwise, it doubles the size of the directory, updates the global depth, and redistributes the entries.
 *
 * @tparam T The type of elements stored in the buckets.
 * @param hashValue The hash value used to locate the bucket to be extended.
 * @return true if the directory was successfully extended and items were rehashed, false otherwise.
 */
template <typename T>
bool GlobalDirectory<T>::extend(const uint32_t hashValue) {
    std::shared_ptr<Bucket<T>> oldBucket = entry[hashValue];
    if (oldBucket->getLocalDepth() < globalDepth) {
        return splitOn(hashValue);
    }
    // Question to ask student:
    // Why is global depth maximum is MAX_KEY_LENGTH ?
    if (globalDepth >= MAX_KEY_LENGTH) return false;

    uint8_t oldGlobalDepth = globalDepth;
    size_t oldLength = entry.size();
    std::vector<std::shared_ptr<Bucket<T>>> newEntry(2 * oldLength);
    // TODO 9

    for (size_t oldIdx = 0, newIdx = 0; oldIdx < oldLength;) {
        int oldNumPtrs = 1 << (oldGlobalDepth - entry[oldIdx]->getLocalDepth());
        for (size_t i = 0; i< oldNumPtrs * 2; i++) {
            newEntry[newIdx++] = entry[oldIdx];
        }
        oldIdx += oldNumPtrs;
    }
    newEntry[hashValue * 2] = std::make_shared<Bucket<T>>(oldBucket->getLocalDepth() + 1);
    newEntry[hashValue * 2 + 1] = std::make_shared<Bucket<T>>(oldBucket->getLocalDepth() + 1);
    globalDepth = oldGlobalDepth + 1;

    // END TODO
    entry = std::move(newEntry);
    return reHashItems(oldBucket);
}

/**
 * @brief Merges two buckets in the global directory if possible.
 *
 * This function attempts to merge the bucket corresponding to the given hash value
 * with its buddy bucket. The merge is only performed if the following conditions are met:
 * - The global depth is greater than 1.
 * - The local depths of the two buckets are the same.
 * - The combined entry count of the two buckets does not exceed the bucket capacity.
 *
 * @tparam T The type of the elements stored in the buckets.
 * @param hashValue The hash value used to identify the bucket to be merged.
 * @return true if the merge was successful, false otherwise.
 */
template <typename T>
bool GlobalDirectory<T>::mergeOn(const uint32_t hashValue) {
    if (globalDepth == 1) return false;
    // TODO 8

    int deleteIndex = hashValue;
    while (deleteIndex > 0 && entry[deleteIndex] == entry[deleteIndex - 1]) {
        deleteIndex--;
    }
    auto deleteBucket = entry[deleteIndex];
    uint32_t numPtrs = 1 << (globalDepth - deleteBucket->getLocalDepth());
    uint32_t buddyIndex = deleteIndex ^ numPtrs;
    auto buddyBucket = entry[buddyIndex];
    if (deleteBucket->getLocalDepth() != buddyBucket->getLocalDepth() ||
    deleteBucket->getEntryCount() + buddyBucket->getEntryCount() > BUCKET_CAPACITY) return false;
    
    // merge
    uint32_t minIndex = deleteIndex < buddyIndex ? deleteIndex : buddyIndex;
    std::shared_ptr<Bucket<T>> mergedBucket = std::make_shared<Bucket<T>>(deleteBucket->getLocalDepth() - 1);
    for (size_t i = minIndex; i < minIndex + numPtrs * 2; i++) {
        entry[i] = mergedBucket;
    }

    return reHashItems(deleteBucket) && reHashItems(buddyBucket);
}

/**
 * @brief Minimizes the global directory by reducing the global depth.
 *
 * Attempts to minimize the global directory by reducing the global depth.
 * It iterates through the entries to check if any entry has a local depth equal to the global depth.
 * If such an entry is found, the function returns false, indicating that minimization is not possible.
 * If the global depth is already at its minimum value (1), the function also returns false.
 * Otherwise, the global depth is decremented, and the entries are halved and reassigned.
 *
 * @tparam T The type of elements stored in the buckets.
 * @return true if the global directory was successfully minimized, false otherwise.
 */
template <typename T>
bool GlobalDirectory<T>::minimize() {
    if (globalDepth == 1) return false;

    for(size_t i = 0; i < entry.size(); i++) {
        if(entry[i]->getLocalDepth() == globalDepth) {
            return false;
        }
    }

    globalDepth--;

    std::vector<std::shared_ptr<Bucket<T>>> newEntry(entry.size() / 2);
    for(size_t i = 0; i < newEntry.size(); i++) {
        newEntry[i] = entry[i * 2];
    }

    entry = std::move(newEntry);

    return true;
}

template class GlobalDirectory<int>;
