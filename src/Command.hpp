#pragma once

#include "Common.hpp"
#include "MemoryManager.hpp"
#include "Bucket.hpp"
#include <assert.h>

enum class CommandType {
    WRITE,
    ERASE,
    SEARCH,
    DISPLAY
};


// Command virtual class
template <typename T>
class Command {
public:
    CommandType operation;

    Command(CommandType operation) : operation(operation) {}

    virtual void execute(MemoryManager<T>& manager) const = 0;

    CommandType getOperation() const { return operation; }
};

// Display Command
template <typename T>
class DisplayCommand : public Command<T> {
public:
    DisplayCommand() : Command<T>(CommandType::DISPLAY) {}

    void execute(MemoryManager<T>& manager) const override {
        manager.display();
    }
};

// virtual class for assertive commands like write and erase
template <typename T>
class AssertiveCommand : public Command<T> {
public:
    bool expected;

    AssertiveCommand(CommandType operation, bool expected) 
        : Command<T>(operation), expected(expected) {}
};

// Write Command
template <typename T>
class WriteCommand : public AssertiveCommand<T> {
public:
    const uint32_t key;
    const T data;

    WriteCommand(uint32_t key, T data, bool expected) 
        : AssertiveCommand<T>(CommandType::WRITE, expected), key(key), data(data) {}

    void execute(MemoryManager<T>& manager) const override {
        bool result = manager.write(key, data);
        assert(result == this->expected);
    }
};


// Erase Command
template <typename T>
class EraseCommand : public AssertiveCommand<T> {
public:
    const uint32_t key;

    EraseCommand(uint32_t key, bool expected) 
        : AssertiveCommand<T>(CommandType::ERASE, expected), key(key) {}

    void execute(MemoryManager<T>& manager) const override {
        bool result = manager.erase(key);
        assert(result == this->expected);
    }
};

// Search Command
template <typename T>
class SearchCommand : public AssertiveCommand<T> {
public:
    const uint32_t key;

    SearchCommand(uint32_t key, bool expected) 
        : AssertiveCommand<T>(CommandType::SEARCH, expected), key(key) {}

    void execute(MemoryManager<T>& manager) const override {
        bool result = manager.searchAndPrint(key);
        assert(result == this->expected);
    }
};