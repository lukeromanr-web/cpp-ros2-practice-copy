#!/usr/bin/env python3
"""固定输入、输出和退出码；不负责替用户编译。仅使用 Python 标准库。"""
import argparse
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]


def check(bin_dir, target, label, expected, stdin="", args=(), code=0):
    try:
        result = subprocess.run(
            [str(bin_dir / target), *args], input=stdin,
            text=True, capture_output=True, timeout=8, check=False,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        print(f"FAIL {label}: {error}")
        return False
    ok = result.returncode == code and result.stdout == expected
    print(f"{'PASS' if ok else 'FAIL'} {label}")
    if not ok:
        print(f"  exit: expected={code}, actual={result.returncode}")
        print(f"  stdout expected: {expected!r}")
        print(f"  stdout actual:   {result.stdout!r}")
        print(f"  stderr: {result.stderr!r}")
    return ok


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--bin", type=Path, required=True)
    parser.add_argument("--mode", choices=["all", "teaching", "reference", "practice"],
                        default="all")
    options = parser.parse_args()
    bin_dir = options.bin.resolve()
    outcomes = []
    if options.mode in ("all", "teaching"):
        expected = "".join(f"[topic] Hello, world! {i}\n" for i in range(3))
        for target in ("stage01", "stage02", "stage03", "stage04"):
            outcomes.append(check(bin_dir, target, target, expected))

    if options.mode in ("all", "reference", "practice"):
        practice = options.mode == "practice"
        publisher = "vector_publisher_todo" if practice else "vector_publisher_solution"
        monitor = "monitor_todo" if practice else "monitor_solution"
        outcomes.append(check(bin_dir, publisher, "vector-strings",
                              "[message] Apple\n[message] Banana\n[message] Cherry\n"))
        fixtures = ROOT / "lessons/vector/cases"
        for name, args in (("base", ()), ("v2", ("--v2",))):
            outcomes.append(check(
                bin_dir, monitor, name,
                (fixtures / f"{name}.out").read_text(encoding="utf-8"),
                (fixtures / f"{name}.in").read_text(encoding="utf-8"), args,
            ))
        outcomes.append(check(
            bin_dir, monitor, "v2-window-eviction", "WAIT\n"
            "process=4 previous=0 diff=4 warnings=1 state=WARNING system=STABLE\n"
            "process=8 previous=4 diff=4 warnings=2 state=WARNING system=UNSTABLE\n"
            "process=8 previous=8 diff=0 warnings=2 state=NORMAL system=UNSTABLE\n"
            "process=8 previous=8 diff=0 warnings=1 state=NORMAL system=STABLE\n",
            "4 8 8 8 8\n", ("--v2",),
        ))

        edges = [
            ("empty", "", ""),
            ("single-kept-in-queue", "2\n", "WAIT\n"),
            ("equal-speeds", "2 2 2\n", "WAIT\n"
             "process=2 previous=0 diff=2 warnings=0 state=NORMAL system=STABLE\n"
             "process=2 previous=2 diff=0 warnings=0 state=NORMAL system=STABLE\n"),
            ("zero-and-max", "0 100 0\n", "WAIT\n"
             "process=0 previous=0 diff=0 warnings=0 state=NORMAL system=STABLE\n"
             "process=100 previous=0 diff=100 warnings=1 state=WARNING system=STABLE\n"),
            ("window-evicts-old-warnings", "3 6 9 12 15 15 15 15 15 15\n", "WAIT\n"
             "process=3 previous=0 diff=3 warnings=1 state=WARNING system=STABLE\n"
             "process=6 previous=3 diff=3 warnings=2 state=WARNING system=UNSTABLE\n"
             "process=9 previous=6 diff=3 warnings=3 state=WARNING system=UNSTABLE\n"
             "process=12 previous=9 diff=3 warnings=4 state=WARNING system=UNSTABLE\n"
             "process=15 previous=12 diff=3 warnings=4 state=WARNING system=UNSTABLE\n"
             "process=15 previous=15 diff=0 warnings=3 state=NORMAL system=UNSTABLE\n"
             "process=15 previous=15 diff=0 warnings=2 state=NORMAL system=UNSTABLE\n"
             "process=15 previous=15 diff=0 warnings=1 state=NORMAL system=STABLE\n"
             "process=15 previous=15 diff=0 warnings=0 state=NORMAL system=STABLE\n"),
        ]
        for label, stdin, expected in edges:
            outcomes.append(check(bin_dir, monitor, label, expected, stdin))
        for label, stdin in (("reject-negative", "-1\n"),
                             ("reject-too-large", "101\n"),
                             ("reject-text", "abc\n")):
            outcomes.append(check(bin_dir, monitor, label, "", stdin, code=1))
        outcomes.append(check(bin_dir, monitor, "reject-unknown-option", "",
                              args=("--unknown",), code=1))

    print(f"{sum(outcomes)}/{len(outcomes)} passed ({options.mode})")
    return 0 if all(outcomes) else 1


if __name__ == "__main__":
    raise SystemExit(main())
