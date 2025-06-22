import subprocess
import sys
import os
from pathlib import Path
import shlex
import scripts.generate_flags as gen
import scripts.log_parser as parser

def main():
    config_path = "config/dev.yaml"
    build_dir = "build"
    os.makedirs(build_dir, exist_ok=True)

    # Generate flags
    flags = gen.get_cmake_flags(config_path)

    # Run cmake
    cmake_cmd = ['cmake', '-S', '.', '-B', build_dir] + flags
    print("🔧 Running:", " ".join(cmake_cmd))
    cmake_proc = subprocess.run(cmake_cmd, capture_output=True, text=True)

    if cmake_proc.returncode != 0:
        print("❌ CMake failed")
        parser.parse_output(cmake_proc.stdout, cmake_proc.stderr)
        sys.exit(cmake_proc.returncode)

    # Build
    build_cmd = ['cmake', '--build', build_dir]
    print("🛠️ Building project...")
    build_proc = subprocess.run(build_cmd, capture_output=True, text=True)

    # Log parsing
    parser.parse_output(build_proc.stdout, build_proc.stderr)

    if build_proc.returncode != 0:
        print("❌ Build failed.")
        sys.exit(build_proc.returncode)

    print("✅ Build completed successfully.")

if __name__ == "__main__":
    main()
