import os
import sys
import yaml
import glob
import shlex
import subprocess
from pathlib import Path

def load_config(config_path):
    with open(config_path, 'r') as f:
        return yaml.safe_load(f)

def expand_source_patterns(patterns):
    files = []
    for pattern in patterns:
        files.extend(glob.glob(pattern))
    return files

def get_cmake_flags(config_path):
    with open(config_path, 'r') as f:
        config = yaml.safe_load(f)

    cmake_args = []

    config = load_config(config_path)

    # CXX_STANDARD, OPT_FLAGS, SAN_FLAGS, DEBUG_FLAGS
    for key in ['CXX_STANDARD', 'OPT_FLAGS', 'SAN_FLAGS', 'DEBUG_FLAGS', 'TARGET_NAME']:
        if key in config:
            cmake_args.append(f"-D{key}={config[key]}")

    # EXTRA_OPTIONS as a space-joined string
    if 'EXTRA_OPTIONS' in config:
        extra_opts = " ".join(config['EXTRA_OPTIONS'])
        cmake_args.append(f"-DEXTRA_OPTIONS={shlex.quote(extra_opts)}")

    # Expand source files and pass as SOURCE_LIST
    if 'SOURCE_PATTERNS' in config:
        source_files = expand_source_patterns(config['SOURCE_PATTERNS'])
        if not source_files:
            print("⚠️ No source files matched. Check your patterns.")
            sys.exit(1)
        source_list = " ".join(shlex.quote(file) for file in source_files)
        cmake_args.append(f"-DSOURCE_LIST={source_list}")

    return cmake_args

# Allow CLI use too
if __name__ == "__main__":
    import sys
    if len(sys.argv) < 2:
        print("Usage: python3 generate_flags.py <config.yaml>")
        exit(1)
    flags = get_cmake_flags(sys.argv[1])
    print(" ".join(flags))