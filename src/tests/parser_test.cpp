#include "parser.h"

#include <gtest/gtest.h>

TEST(ParserTest, TestEmptyMessage) {
  Parser parser("");
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestEmptyCommand) {
  Parser parser("<><>");
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestEmptyCommandIsValid) {
  Parser parser("<><>", {""});
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestEmptyCommandIsValidNoDelimiter) {
  Parser parser("<><>", {""}, "", "");
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestEmptyEverything) {
  Parser parser("", {""}, "", "");
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestSimpleCommand) {
  Parser parser("<command> argument");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestMultipleCommands) {
  Parser parser("<command1> <command2> argument");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command1");
  EXPECT_EQ(parser.getArgument(), " <command2> argument");
  Parser parser2(parser.getArgument());
  EXPECT_TRUE(parser2.hasCommand());
  EXPECT_EQ(parser2.getCommand(), "command2");
  EXPECT_EQ(parser2.getArgument(), " argument");
}

TEST(ParserTest, TestInvalidCommand) {
  std::unordered_set<std::string> valid_commands = {"valid_command"};
  Parser parser("<invalid_command> argument", valid_commands);
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestCustomDelimiters) {
  Parser parser("[command] argument", {}, "[", "]");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestCustomLongDelimiters) {
  Parser parser("[!]command[!] argument", {}, "[!]", "[!]");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestCustomAsymmetricDelimiters) {
  Parser parser("[!]command[!!!!] argument", {}, "[!]", "[!!!!]");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestCustomAsymmetricDelimitersFirstLong) {
  Parser parser("[12345]command[!] argument", {}, "[12345]", "[!]");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestNestedDelimiters) {
  std::unordered_set<std::string> valid_commands = {"command1", "<<command1"};
  Parser parser("<<<command1>>> argument", valid_commands, "<", ">");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "<<command1");
  EXPECT_EQ(parser.getArgument(), ">> argument");
}

TEST(ParserTest, TestNoDelimiters) {
  std::unordered_set<std::string> valid_commands = {"command1", "command2"};
  Parser parser("command1 argument", valid_commands, "", "");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command1");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestNoFirstDelimiter) {
  std::unordered_set<std::string> valid_commands = {"command1", "command2"};
  Parser parser("command1 argument", valid_commands, "", "1");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command1");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestNoSecondDelimiter) {
  std::unordered_set<std::string> valid_commands = {"command1", "command2"};
  Parser parser("!command1 argument", valid_commands, "!", "");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command1");
  EXPECT_EQ(parser.getArgument(), " argument");
}

TEST(ParserTest, TestSecondDelimiterMissing) {
  std::unordered_set<std::string> valid_commands = {"command1", "command2"};
  Parser parser("<command1 argument", valid_commands, "<", ">");
  EXPECT_FALSE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "");
  EXPECT_EQ(parser.getArgument(), "");
}

TEST(ParserTest, TestTemplateConstructor) {
  std::map<std::string, std::string> valid_commands = {{"command1", "arg1"},
                                                       {"command", "arg2"}};
  Parser parser("!command11 argument", valid_commands, "!", "1");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command");
  EXPECT_EQ(parser.getArgument(), "1 argument");
}