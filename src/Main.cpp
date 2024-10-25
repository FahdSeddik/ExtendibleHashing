#include "Command.hpp"

#define DATA_TYPE int

int main() {
	MemoryManager<DATA_TYPE>& manager = MemoryManager<DATA_TYPE>::getInstance();
    std::vector<std::unique_ptr<Command<DATA_TYPE>>> commands;

    auto addCommand = [&commands](Command<DATA_TYPE>* cmd) {
        commands.emplace_back(cmd);
    };

    // Empty directory
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    // test insertion without making extra directory
    addCommand(new WriteCommand<DATA_TYPE>(13, 33, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    addCommand(new SearchCommand<DATA_TYPE>(13, true));
    addCommand(new EraseCommand<DATA_TYPE>(13, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(1, 20, true));
    addCommand(new WriteCommand<DATA_TYPE>(2, 70, true));
    addCommand(new WriteCommand<DATA_TYPE>(42, 11, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(112, 54, true));
    addCommand(new WriteCommand<DATA_TYPE>(240, 16, true));
    addCommand(new WriteCommand<DATA_TYPE>(241, 99, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(159, 30, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(14, 45, true));
    addCommand(new WriteCommand<DATA_TYPE>(13, 88, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(37, 1, true));
    addCommand(new SearchCommand<DATA_TYPE>(13, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new EraseCommand<DATA_TYPE>(14, true));
    addCommand(new EraseCommand<DATA_TYPE>(13, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    //================================================
    addCommand(new WriteCommand<DATA_TYPE>(158, 5, true));
    addCommand(new DisplayCommand<DATA_TYPE>());
    addCommand(new WriteCommand<DATA_TYPE>(3, 9, true));
    addCommand(new DisplayCommand<DATA_TYPE>());

    // Execute each command in the commands vector
    for (const auto& command : commands) {
        command->execute(manager);
    }

	return 0;
}