#include "parser.h"

#include <gtest/gtest.h>

TEST(ParserTest, TestEmptyMessage) {
  Parser parser("");
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

TEST(ParserTest, TestNoDelimiters) {
  std::unordered_set<std::string> valid_commands = {"command1", "command2"};
  Parser parser("command1 argument", valid_commands, "", "");
  EXPECT_TRUE(parser.hasCommand());
  EXPECT_EQ(parser.getCommand(), "command1");
  EXPECT_EQ(parser.getArgument(), " argument");
}