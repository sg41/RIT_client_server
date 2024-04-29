#ifndef TOOLS_PARSER_H
#define TOOLS_PARSER_H
#include <map>
#include <string>
#include <unordered_set>

class Parser {
 private:
  const std::string kTagStart = "<";
  const std::string kTagEnd = ">";
  const std::string kTagNotFound = "";
  const size_t kTagStartLength = kTagStart.length();
  const size_t kTagEndLength = kTagEnd.length();

 public:
  explicit Parser(const std::string& message);
  Parser(const std::string& message,
         const std::unordered_set<std::string>& valid_commands,
         const std::string& start = "<", const std::string& end = ">");
  template <typename T>
  Parser(const std::string& message, std::map<std::string, T> valid_commands,
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
               std::map<std::string, T> valid_commands,
               const std::string& start, const std::string& end)
    : kTagStart(start), kTagEnd(end) {
  for (auto it = valid_commands.begin(); it != valid_commands.end(); ++it) {
    valid_commands_.insert(it->first);
  }
  parse(message);
}
#endif  // TOOLS_PARSER_H