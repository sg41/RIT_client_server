#ifndef SERVER_PARSER_H
#define SERVER_PARSER_H
#include <map>
#include <string>
#include <vector>

const std::string kTagStart = "<";
const std::string kTagEnd = ">";
const std::string kTagNotFound = "";
const size_t kTagStartLength = kTagStart.length();
const size_t kTagEndLength = kTagEnd.length();

class Parser {
 public:
  Parser(std::string message);
  Parser(std::string message, std::map<std::string, int> valid_commands);
  bool hasCommand() const;
  int numberArguments() const;
  const std::string& getCommand() const;
  const std::vector<std::string>& getArguments() const;

 private:
  void parse();
  std::string extractTag(size_t& offset);

  std::string message_;
  bool has_command_ = false;
  int num_arguments_ = 0;
  std::string command_;
  std::vector<std::string> arguments_;
  std::map<std::string, int> valid_commands_{
      {"shutdown", 0}, {"show", 1}, {"communicate", 2}};  // Default commands
};
#endif