/**
 * @file parser.cpp
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Parser class implementation
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "parser.h"

void Parser::parse(const std::string& message) {
  size_t offset = 0;
  command_ = extractTag(message, offset);

  if (command_ == kTagNotFound) {
    return;
  }

  // Check if the extracted command is valid if valid_commands_ is not empty
  // if valid_commands_ is empty - all commands are valid
  if (!valid_commands_.empty()) {
    auto it = valid_commands_.find(command_);
    if (it == valid_commands_.end()) {
      command_ = kTagNotFound;
      return;  // Invalid command
    }
  }

  has_command_ = true;

  argument_ = message.substr(offset);
}

std::string Parser::extractTagByMatch(const std::string& message,
                                      size_t& offset) {
  auto best_match = kTagNotFound;
  auto best_it = std::string::npos;
  for (auto command : valid_commands_) {
    auto it = message.find(command);
    if (it != std::string::npos) {
      if (it < best_it) {
        best_it = it;
        best_match = command;
      }
    }
  }
  offset = best_it + best_match.length();
  return best_match;
}

std::string Parser::extractTag(const std::string& message, size_t& offset) {
  // Empty delimiters section
  if (kTagStartLength == 0 || kTagEndLength == 0) {
    return extractTagByMatch(message, offset);
  }

  // Extract tag surrounded by kTagStart and kTagEnd
  size_t start_pos = message.find(kTagStart, offset);

  if (start_pos == std::string::npos) {
    return kTagNotFound;
  }
  size_t end_pos = message.find(kTagEnd, start_pos + kTagStartLength);

  if (end_pos == std::string::npos) {
    return kTagNotFound;
  }

  offset = end_pos + kTagEndLength;
  return message.substr(start_pos + kTagStartLength,
                        end_pos - (start_pos + kTagStartLength));
}

Parser::Parser(const std::string& message) { parse(message); }

Parser::Parser(const std::string& message,
               const std::unordered_set<std::string>& valid_commands,
               const std::string& start, const std::string& end)
    : kTagStart(start), kTagEnd(end), valid_commands_(valid_commands) {
  parse(message);
};

bool Parser::hasCommand() const { return has_command_; };
const std::string& Parser::getCommand() const { return command_; };
const std::string& Parser::getArgument() const { return argument_; };
