#ifndef SERVER_PARSER_H
#define SERVER_PARSER_H
#include <map>
#include <set>
#include <string>
#include <vector>

const std::string kTagStart = "<";
const std::string kTagEnd = ">";
const std::string kTagNotFound = "";
const size_t kTagStartLength = kTagStart.length();
const size_t kTagEndLength = kTagEnd.length();

class Parser {
 private:
  std::string message_;
  bool has_command_ = false;
  int num_arguments_ = 0;
  std::string command_;
  std::vector<std::string> arguments_;
  std::map<std::string, int> valid_commands_{
      {"shutdown", 0}, {"show", 1}, {"communicate", 2}};

 public:
  Parser(std::string message) : message_(message) { parse(); }

  std::string extractTag(size_t& offset) {
    size_t start_pos = message_.find(kTagStart, offset);

    if (start_pos == std::string::npos) {
      return kTagNotFound;
    }

    size_t end_pos =
        message_.find(kTagEnd, offset + start_pos + kTagStartLength);

    if (end_pos == std::string::npos) {
      return kTagNotFound;
    }

    offset = end_pos + kTagEndLength;
    return message_.substr(start_pos + kTagStartLength,
                           end_pos - start_pos - kTagStartLength);
  }

  void parse() {
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

  bool hasCommand() const { return has_command_; };
  int numberArguments() const { return num_arguments_; };
  const std::string& getCommand() const { return command_; };
  const std::vector<std::string>& getArguments() const { return arguments_; };
};
#endif