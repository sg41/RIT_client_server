import subprocess
import re
import threading
import time


def run_test(app, data, **kwargs):
    test = subprocess.Popen(
        app,  stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    if kwargs.get("timeout"):
        time.sleep(kwargs.get("timeout"))
    (out, err) = test.communicate(input=data)
    if test.returncode != 0:
        print(data)
        print(err)
        print(out)
        print("RE")
    res = list(out.split("\n"))
    return res


def test_simpleMessage():
    res = run_test(["build/client", "127.0.0.1", "8080"], "12345\n")
    expected = ["Client: Connected to server!",
                "> Server: ",
                "Message |12345",
                "1       |1",
                "2       |1",
                "3       |1",
                "4       |1",
                "5       |1",
                "",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res)):
        assert res[i] == expected[i]


def test_showList():

    t = threading.Thread(target=run_test, args=([
        "build/client", "127.0.0.1", "8080"], f"help\n"), kwargs={"timeout": 2})
    t.start()
    time.sleep(1)
    res = run_test(["build/client", "127.0.0.1", "8080"], "show list\n")
    expected = ["Client: Connected to server!",
                "> Server: ",
                "Client ID:",
                "client_[0-9]+",
                "client_[0-9]+",
                "",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res)):
        assert re.match(expected[i], res[i])
    t.join()


def test_showNumber():
    threads = []
    time.sleep(1)  # Wait for previous test to finish
    for i in range(5):
        threads.append(threading.Thread(target=run_test, args=([
            "build/client", "127.0.0.1", "8080"], f"show number\n"), kwargs={"timeout": 1}))
        threads[i].start()
    res = run_test(["build/client", "127.0.0.1", "8080"], "show number\n")
    expected = ["Client: Connected to server!",
                "^> Server: [1-6]$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res)):
        assert re.search(expected[i], res[i])
    for t in threads:
        t.join()


def test_sendMessageOk():
    threads = []
    for i in range(4):
        threads.append(threading.Thread(target=run_test, args=([
            "build/client", "127.0.0.1", "8080"], f"exit\n\n"), kwargs={"timeout": 1}))
        threads[i].start()
    res = run_test(["build/client", "127.0.0.1", "8080"], "show list\n")
    res2 = run_test(["build/client", "127.0.0.1", "8080"],
                    "send<"+res[3]+">message\n")
    expected = ["Client: Connected to server!",
                "^> Server: Message sent$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res2)):
        assert re.search(expected[i], res2[i])
    for t in threads:
        t.join()


def test_sendMessageError():
    res2 = run_test(["build/client", "127.0.0.1", "8080"],
                    "send<client_0>message\n")
    expected = ["Client: Connected to server!",
                "^> Server: Error$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res2)):
        assert re.search(expected[i], res2[i])


def test_SendMessageSelf():
    res2 = run_test(["build/client", "127.0.0.1", "8080"],
                    "send<self>message\n")
    expected = ["Client: Connected to server!",
                "^> Server: client_[0-9]+: message$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res2)):
        assert re.search(expected[i], res2[i])
