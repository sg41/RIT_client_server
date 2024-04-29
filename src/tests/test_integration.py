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


def test_1():
    res = run_test(["build/client", "127.0.0.1", "8080"], "12345\n")
    expected = ["Connected to server: success",
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


def test_2():

    t = threading.Thread(target=run_test, args=([
        "build/client", "127.0.0.1", "8080"], f"show list\n"), kwargs={"timeout": 2})
    t.start()
    time.sleep(1)
    res = run_test(["build/client", "127.0.0.1", "8080"], "show list\n")
    expected = ["Connected to server: success",
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


def test_3():
    threads = []
    time.sleep(1)  # Wait for previous test to finish
    for i in range(5):
        threads.append(threading.Thread(target=run_test, args=([
            "build/client", "127.0.0.1", "8080"], f"show number\n"), kwargs={"timeout": 1}))
        threads[i].start()
    res = run_test(["build/client", "127.0.0.1", "8080"], "show number\n")
    expected = ["Connected to server: success",
                "^> Server: [1-6]$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res)):
        assert re.search(expected[i], res[i])
    for t in threads:
        t.join()


def test_4():
    threads = []
    for i in range(2):
        threads.append(threading.Thread(target=run_test, args=([
            "build/client", "127.0.0.1", "8080"], f"show number\n"), kwargs={"timeout": 1}))
        threads[i].start()
    res = run_test(["build/client", "127.0.0.1", "8080"], "show list\n")
    res2 = run_test(["build/client", "127.0.0.1", "8080"],
                    "send<"+res[4]+">message\n")
    expected = ["Connected to server: success",
                "^> Server: Message sent$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res2)):
        assert re.search(expected[i], res2[i])
    for t in threads:
        t.join()


def test_4():
    res2 = run_test(["build/client", "127.0.0.1", "8080"],
                    "send<client_0>message\n")
    expected = ["Connected to server: success",
                "^> Server: Error$",
                "> ",
                "Bye!",
                ""]
    for i in range(len(res2)):
        assert re.search(expected[i], res2[i])
