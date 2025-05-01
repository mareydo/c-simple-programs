#include "CommandFactory.h"

/**
 * Initialize map of string commands with enum responses
 */
CommandFactory::CommandFactory() {
    commandDatabase["EXIT"] = ECommand::EXIT;
    commandDatabase["HELP"] = ECommand::HELP;
    commandDatabase["INVALID"] = ECommand::INVALID;
    commandDatabase["LOAD"] = ECommand::LOAD;
    commandDatabase["ENTER"] = ECommand::ENTER;
    commandDatabase["REMOVE"] = ECommand::REMOVE;
    commandDatabase["RENAME"] = ECommand::RENAME;
    commandDatabase["SAVE"] = ECommand::SAVE;
    commandDatabase["SUM"] = ECommand::SUM;
    commandDatabase["SUB"] = ECommand::SUB;
    commandDatabase["MULT"] = ECommand::MULT;
    commandDatabase["POW"] = ECommand::POW;
    commandDatabase["SPLIT"] = ECommand::SPLIT;
    commandDatabase["MERGE"] = ECommand::MERGE;
    commandDatabase["GEM"] = ECommand::GEM;
    commandDatabase["INV"] = ECommand::INV;
    commandDatabase["TRANS"] = ECommand::TRANS;
    commandDatabase["DET"] = ECommand::DET;
    commandDatabase["RANK"] = ECommand::RANK;
    commandDatabase["DIM"] = ECommand::DIM;
    commandDatabase["SQ"] = ECommand::SQ;
    commandDatabase["DS"] = ECommand::DS;
    commandDatabase["ELEM"] = ECommand::ELEM;
    commandDatabase["PRINT"] = ECommand::PRINT;
    commandDatabase["PRINTALL"] = ECommand::PRINTALL;
}

ECommand CommandFactory::getCommand(const std::string &command) const {
    if (commandDatabase.count(command) != 1) {
        return ECommand::INVALID;
    } else {
        return commandDatabase.at(command);
    }
}
