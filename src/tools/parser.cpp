#include "parser.h"

void Parser::parse() {
  size_t offset = 0;
  command_ = extractTag(offset);

  if (command_ == kTagNotFound) {
    return;
  }

  // Check if the extracted command is valid if valid_commands_ is not empty
  if (!valid_commands_.empty()) {
    auto it = valid_commands_.find(command_);
    if (it == valid_commands_.end()) {
      return;  // Invalid command
    }
  }

  has_command_ = true;

  argument_ = message_.substr(offset);
}

std::string Parser::extractTag(size_t& offset) {
  if (kTagStartLength == 0 || kTagEndLength == 0) {
    auto best_match = kTagNotFound;
    auto best_it = std::string::npos;
    for (auto command : valid_commands_) {
      auto it = message_.find(command);
      if (it != std::string::npos) {
        if (it < best_it) {
          best_it = it;
          best_match = command;
        }
      }
    }
    return best_match;
  }

  size_t start_pos = message_.find(kTagStart, offset);

  if (start_pos == std::string::npos) {
    return kTagNotFound;
  }

  size_t end_pos = message_.find(kTagEnd, start_pos + kTagStartLength);

  if (end_pos == std::string::npos) {
    return kTagNotFound;
  }

  offset = end_pos + kTagEndLength;
  return message_.substr(start_pos + kTagStartLength,
                         end_pos - start_pos - kTagEndLength);
}

Parser::Parser(const std::string& message) : message_(message) { parse(); }

Parser::Parser(const std::string& message,
               std::unordered_set<std::string> valid_commands,
               std::string start, std::string end)
    : kTagStart(start),
      kTagEnd(end),
      message_(message),
      valid_commands_(valid_commands) {
  parse();
};
// template <typename T>
// Parser::Parser(const std::string& message,
//                std::map<std::string, T> valid_commands)
//     : message_(message) {
//   for (auto it = valid_commands.begin(); it != valid_commands.end(); ++it) {
//     valid_commands_.insert(it->first);
//   }
//   parse();
// }
bool Parser::hasCommand() const { return has_command_; };
const std::string& Parser::getCommand() const { return command_; };
const std::string& Parser::getArgument() const { return argument_; };
