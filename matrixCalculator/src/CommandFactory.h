#ifndef COMMANDFACTORY_H
#define COMMANDFACTORY_H

#include <map>
#include <string>
#include "ECommand.h"

/**
 * @brief Factory for creating ECommand responses
 */
class CommandFactory {
private:
    std::map<std::string, ECommand> commandDatabase;
public:
    /**
     * @brief Construct a new Command Factory object
     * 
     */
    CommandFactory();
    /**
     * @brief Get the Command object
     * 
     * @param command key to database
     * @return ECommand:: response ECommand::INVALID id key is not present
     */
    ECommand getCommand(const std::string &command) const;

};

#endif //COMMANDFACTORY_H
