import subprocess
import argparse
import timeit


def run_test(app, data, **kwargs):
    test = subprocess.Popen(
        app,  stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
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
