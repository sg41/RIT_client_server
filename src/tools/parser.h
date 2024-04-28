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
  Parser(const std::string& message);
  Parser(const std::string& message,
         const std::unordered_set<std::string>& valid_commands,
         const std::string& start = "<", const std::string& end = ">");
  //   template <typename T>
  //   Parser(const std::string& message, std::map<std::string, T>
  //   valid_commands);
  bool hasCommand() const;
  const std::string& getCommand() const;
  const std::string& getArgument() const;

 private:
  void parse();
  std::string extractTag(size_t& offset);

  const std::string& message_;
  bool has_command_ = false;
  std::string command_;
  std::string argument_;
  std::unordered_set<std::string> valid_commands_{};
};
#endif  // TOOLS_PARSER_H