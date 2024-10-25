#include <iostream>
#include <bitset>

#include "MemoryManager.hpp"
#include "Bucket.hpp"

/**
 * @brief Displays the current state of the MemoryManager.
 * 
 * This function prints the details of the MemoryManager to the standard output.
 * It starts by printing a header, then checks the global depth of the global directory.
 * If the global depth is zero, it prints information about the initial file, including its local depth,
 * and calls the display function of the initial file. Otherwise, it calls the display function of the global directory.
 * Finally, it prints a footer to indicate the end of the display.
 */
template <typename T>
void MemoryManager<T>::display() const {
    std::cout << "########## Start of MemoryManager Display ##########\n";
    if(globalDirectory.getGlobalDepth() == 0) {
        std::cout << "Initial File\n";
        std::cout << "Local Depth: " << initialFile->getLocalDepth() << std::endl;
        initialFile->display();
        std::cout << std::endl;
    } else {
        globalDirectory.display();
    }
    std::cout << "########## End of MemoryManager Display ##########\n";
}

/**
 * @brief Searches for a given key in the memory manager and prints the result.
 *
 * This function searches for the specified key in the memory manager. If the global directory's
 * depth is zero, it searches in the initial file; otherwise, it searches in the global directory.
 * The function prints the key in binary format and the corresponding value if found, or "Not found"
 * if the key does not exist.
 *
 * @tparam T The type of the value associated with the key.
 * @param key The key to search for.
 * @return true if the key is found, false otherwise.
 */
template<typename T>
bool MemoryManager<T>::searchAndPrint(const uint32_t key) const {
    const auto& result = (globalDirectory.getGlobalDepth() == 0) 
                         ? initialFile->find(key) 
                         : globalDirectory.find(key);

    std::cout << "Search for Key: " << std::bitset<MAX_KEY_LENGTH>(key) << " Value: ";
    if (result.has_value()) {
        std::cout << result.value() << std::endl;
    } else {
        std::cout << "Not found" << std::endl;
    }

    return result.has_value();
}


/**
 * @brief Writes data to the memory manager using a specified key.
 * 
 * This function attempts to write the provided data to the memory manager.
 * If the global directory's depth is zero, it first tries to write to the
 * initial file. If successful, it returns true. If the write to the initial
 * file fails, it initializes the global directory with the initial file.
 * Finally, it attempts to write the data to the global directory.
 * 
 * @tparam T The type of data to be written.
 * @param key The key associated with the data to be written.
 * @param data The data to be written.
 * @return true if the data was successfully written, false otherwise.
 */
template <typename T>
bool MemoryManager<T>::write(const uint32_t key, const T& data) {
    if (globalDirectory.getGlobalDepth() == 0) {
        if (initialFile->write(key, data)) {
            return true; // Success
        }
        globalDirectory.initialize(initialFile);
    }

    return globalDirectory.write(key, data);;
}

/**
 * @brief Erases an entry with the specified key from the memory manager.
 * 
 * This function attempts to erase an entry identified by the given key.
 * If the global directory's depth is 0, it erases the entry from the initial file.
 * Otherwise, it erases the entry from the global directory.
 * 
 * @tparam T The type of the elements managed by the memory manager.
 * @param key The key of the entry to be erased.
 * @return true if the entry was successfully erased, false otherwise.
 */
template <typename T>
bool MemoryManager<T>::erase(const uint32_t key) {
    if (globalDirectory.getGlobalDepth() == 0) {
        return initialFile->erase(key);
    }
    // Note: Global directory remains initialized and does not revert to depth 0
    return globalDirectory.erase(key);
}

template class MemoryManager<int>;
