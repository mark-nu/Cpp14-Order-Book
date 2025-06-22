from invoke import task

@task
def build(ctx):
    """
    Build the program by compiling all .cpp files in src and utils directories.
    """
    print("Building the program...")
    ctx.run('g++ -o build/program src/*.cpp utils/*.cpp', pty=True)

@task
def clean(ctx):
    """
    Clean the build directory by removing the compiled program.
    """
    print("Cleaning the build directory...")
    ctx.run('rm -f build/program', pty=True)

@task
def run(ctx):
    """
    Run the compiled program.
    """
    print("Running the program...")
    ctx.run('./build/program', pty=True)

@task(pre=[build])
def all(ctx):
    """
    Build and run the program.
    """
    print("Building and running the program...")
    ctx.run('./build/program', pty=True)

@task
def test(ctx):
    """
    Run tests for the program.
    """
    print("Running tests...")
    ctx.run('g++ -o build/test src/test.cpp utils/test_utils.cpp', pty=True)
    ctx.run('./build/test', pty=True)

@task
def lint(ctx):
    """
    Lint the source code using cpplint.
    """
    print("Linting the source code...")
    ctx.run('cpplint src/*.cpp utils/*.cpp', pty=True)

@task
def format(ctx):
    """
    Format the source code using clang-format.
    """
    print("Formatting the source code...")
    ctx.run('clang-format -i src/*.cpp utils/*.cpp', pty=True)