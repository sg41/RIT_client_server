#include "parser.h"

#include <map>
#include <set>
#include <string>
#include <vector>

void Parser::parse() {
  size_t offset = 0;
  command_ = extractTag(offset);

  // Check if the extracted command is valid
  auto it = valid_commands_.find(command_);
  if (it == valid_commands_.end()) {
    return;  // Invalid command
  }

  has_command_ = true;
  num_arguments_ = it->second;

  // Extract arguments based on the required number
  for (int i = 0; i < num_arguments_; ++i) {
    arguments_.push_back(extractTag(offset));
  }
}

std::string Parser::extractTag(size_t& offset) {
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

Parser::Parser(std::string message) : message_(message) { parse(); }
Parser::Parser(std::string message, std::map<std::string, int> valid_commands)
    : message_(message), valid_commands_(valid_commands) {
  parse();
}
bool Parser::hasCommand() const { return has_command_; };
int Parser::numberArguments() const { return num_arguments_; };
const std::string& Parser::getCommand() const { return command_; };
const std::vector<std::string>& Parser::getArguments() const {
  return arguments_;
};
