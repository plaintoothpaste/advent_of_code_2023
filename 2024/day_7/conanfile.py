from pathlib import Path
from conan import ConanFile, errors
from conan.tools.env import Environment


required_conan_version = ">=1.58"


class aoc(ConanFile):
    name = "aoc-7"
    version = "1.0.0"
    author = "Matthew McMillan"
    url = "https://gitlab.bigrep.com/matthew.mcmillan/aoc2024"
    description = "A project to learn with"
    topics = "conan"
    settings = "os", "compiler", "build_type", "arch"

    options = {"testing": [True, False]}
    default_options = {"testing": True}
    # exports = "LICENSE*" for items such as source conan files

    def layout(self):
        self.folders.source = "."  # default values
        self.folders.build = "build"  # only used for venv
        self.folders.generators = "generators"  # keeps things neat
        if self.options.testing:
            self.cpp.package.resdirs.append("reference_files")

    def requirements(self):
        self.requires("python-for-blade/3.10.11")

    def _default_run(self, cmd) -> None:
        self.run(cmd, env="python_venv", cwd=self.source_folder)

    def generate(self):
        self.output.highlight(f"Generating python venv in: {self.build_folder}")
        package_python = Path(self.dependencies["python-for-blade"].cpp_info.bindirs[0], "python.exe")
        venv = Path(self.build_folder)
        self.run(f"{package_python} -m venv {venv}")
        self._venv_path = venv / "Scripts/python.exe"

        env1 = Environment()
        env1.prepend_path("PATH", self._venv_path.parent.as_posix())
        envvars = env1.vars(self, scope="build")
        envvars.save_script("python_venv")

        self._default_run("python -m pip install -r requirements.txt")

    def build(self):
        """Runs from the build folder"""
        if self.in_local_cache:
            raise errors.ConanInvalidConfiguration("Cannot be built in cache")

        self.output.info("running formatting")
        self._default_run("ruff format")
        self._default_run("ruff check")

        self.output.info("running unit tests")
        self._default_run("pytest")


        self.output.info("Compiling with mypyc")
        self._default_run("mypy src/day_7.py")
        self._default_run("mypyc src/day_7.py")