import re

def parse_output(stdout, stderr):
    errors = []
    warnings = []

    # Simple pattern matching
    for line in stdout.splitlines() + stderr.splitlines():
        if re.search(r'error:', line, re.IGNORECASE):
            errors.append(line)
        elif re.search(r'warning:', line, re.IGNORECASE):
            warnings.append(line)

    print("\n📄 Build Summary:")
    print(f"  ❌ Errors: {len(errors)}")
    print(f"  ⚠️  Warnings: {len(warnings)}")

    if errors:
        print("\nTop Errors:")
        for line in errors[:5]:
            print("    " + line)

    if warnings:
        print("\nTop Warnings:")
        for line in warnings[:5]:
            print("    " + line)
