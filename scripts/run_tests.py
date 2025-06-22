import subprocess
from pathlib import Path

TEST_DIR = Path("build")
LOG_DIR = Path("test_logs")
LOG_DIR.mkdir(exist_ok=True)

def find_test_binaries():
    return [f for f in TEST_DIR.glob("test_*") if f.is_file() and f.stat().st_mode & 0o111]

def run_all_tests():
    test_binaries = find_test_binaries()
    if not test_binaries:
        print("❌ No test binaries found.")
        exit(1)

    failed = 0

    for binary in test_binaries:
        name = binary.stem
        xml_path = LOG_DIR / f"{name}.xml"
        log_path = LOG_DIR / f"{name}.log"

        print(f"🚀 Running {name}...")

        result = subprocess.run(
            [str(binary), f"--gtest_output=xml:{xml_path}"],
            capture_output=True,
            text=True
        )

        with open(log_path, "w") as f:
            f.write(result.stdout)
            f.write(result.stderr)

        if result.returncode != 0:
            print(f"❌ {name} failed. See {log_path}")
            failed += 1
        else:
            print(f"✅ {name} passed.")

    exit(failed)

if __name__ == "__main__":
    run_all_tests()
