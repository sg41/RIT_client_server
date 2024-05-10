/**
 * @file parser.h
 * @author Sergey Gorbatenko (sg41@mail.ru)
 * @brief Parser class declaration
 * @version 0.1
 * @date 2024-05-01
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef TOOLS_PARSER_H
#define TOOLS_PARSER_H
#include <map>
#include <string>
#include <unordered_set>

/**
 * The `Parser` class is responsible for parsing a given input string.
 * It extracts a command and an argument from the input string,
 * which is enclosed by a starting and ending tag.
 *
 * The `Parser` class can be instantiated with different types of input
 * messages. The starting and ending tags can be customized in the constructor.
 *
 * The `Parser` class provides the following functionality:
 * - Parse a given input string and extract the command and argument.
 * - Check if a command is present in the input string.
 * - Get the extracted command and argument.
 *
 * Example usage:
 */
class Parser {
 private:
  const std::string kTagStart = "<";
  const std::string kTagEnd = ">";
  const std::string kTagNotFound = "";
  const size_t kTagStartLength = kTagStart.length();
  const size_t kTagEndLength = kTagEnd.length();

 public:
  explicit Parser(const std::string& message);
  explicit Parser(const std::string& message,
                  const std::initializer_list<std::string>& valid_commands,
                  const std::string& start = "<", const std::string& end = ">");
  template <typename T>
  Parser(const std::string& message, const T& valid_commands,
         const std::string& start = "<", const std::string& end = ">");
  template <typename T>
  Parser(const std::string& message,
         const std::map<std::string, T>& valid_commands,
         const std::string& start = "<", const std::string& end = ">");
  bool hasCommand() const;
  const std::string& getCommand() const;
  const std::string& getArgument() const;

 private:
  void parse(const std::string& message);
  std::string extractTagByMatch(const std::string& message, size_t& offset);
  std::string extractTag(const std::string& message, size_t& offset);

 private:
  bool has_command_ = false;
  std::string command_{};
  std::string argument_{};
  std::unordered_set<std::string> valid_commands_{};
};

template <typename T>
Parser::Parser(const std::string& message,
               const std::map<std::string, T>& valid_commands,
               const std::string& start, const std::string& end)
    : kTagStart(start), kTagEnd(end) {
  for (auto it = valid_commands.begin(); it != valid_commands.end(); ++it) {
    valid_commands_.insert(it->first);
  }
  parse(message);
}

template <typename T>
Parser::Parser(const std::string& message, const T& valid_commands,
               const std::string& start, const std::string& end)
    : kTagStart(start),
      kTagEnd(end),
      valid_commands_(valid_commands.begin(), valid_commands.end()) {
  parse(message);
};
#endif  // TOOLS_PARSER_H