#include "client.h"

#include <gtest/gtest.h>

#include <chrono>
#include <thread>

#include "server.h"

const std::string kTestServerIP = "127.0.0.1";
const int kTestServerPort = 8080;

class ClientTest : public ::testing::Test {};

TEST_F(ClientTest, TestConnectSuccess) {
  Client client(kTestServerIP, kTestServerPort);
  // Assuming the server is running on the specified IP and port
  EXPECT_TRUE(client.connectToServer());
}

TEST_F(ClientTest, TestConnectFailure) {
  Client client("InvalidIP", 12345);
  EXPECT_FALSE(client.connectToServer());
}

TEST_F(ClientTest, TestSendMessage) {
  Client client(kTestServerIP, kTestServerPort);
  ASSERT_TRUE(client.connectToServer());

  std::string message = "Test message";
  EXPECT_TRUE(client.sendMessage(message));
}

TEST_F(ClientTest, TestReceiveMessage) {
  Client client(kTestServerIP, kTestServerPort);
  ASSERT_TRUE(client.connectToServer());

  std::string sent_message = "Test message";
  ASSERT_TRUE(client.sendMessage(sent_message));

  std::string received_message;
  EXPECT_TRUE(client.receiveMessage(received_message));
  EXPECT_EQ(
      received_message,
      "\nMessage |Test message\nT       |1\ne       |3\ns       |3\nt       "
      "|1\n        |1\nm       |1\na       |1\ng       |1\n");
}

TEST_F(ClientTest, TestReconnect) {
  Client client(kTestServerIP, kTestServerPort);
  ASSERT_TRUE(client.connectToServer());
  EXPECT_TRUE(client.reconnect());
}

TEST_F(ClientTest, TestCheckHaveMessage) {
  Client client(kTestServerIP, kTestServerPort);
  EXPECT_FALSE(client.checkHaveMessage());
  ASSERT_TRUE(client.connectToServer());
  EXPECT_FALSE(client.checkHaveMessage());
  ASSERT_TRUE(client.sendMessage("Test message"));
  std::this_thread::sleep_for(
      std::chrono::milliseconds(100));  // Wait for server to respond
  EXPECT_TRUE(client.checkHaveMessage());
}