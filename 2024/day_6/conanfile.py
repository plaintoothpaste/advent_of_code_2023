from pathlib import Path
from conan import ConanFile, errors


required_conan_version = ">=1.58"


class aoc(ConanFile):
    name = "aoc-6"
    author = "Matthew McMillan"
    url = "https://gitlab.bigrep.com/matthew.mcmillan/aoc2024"
    description = "A project to learn with"
    topics = "conan"
    settings = "os", "compiler", "build_type", "arch"

    # exports = "LICENSE*" for items such as source conan files

    def layout(self):
        self.folders.source = "."  # default values
        self.folders.build = "build"  # only used for venv
        self.folders.generators = "generators"  # keeps things neat

    def requirements(self):
        self.requires("python-for-blade/3.10.11")

    def generate(self):
        self._generate_venv()

    def build(self):
        """Runs from the build folder"""
        if self.in_local_cache:
            raise errors.ConanInvalidConfiguration("Cannot be built in cache")

        # self.output.info("running tests")
        # self._run_in_venv("pytest")

        self.output.info("running formatting")
        self._run_in_venv("ruff format")
        self._run_in_venv("ruff check")

        self.output.info("Compiling with mypyc and then running")
        for file in ["day_6", "day_6_2"]:
            self._run_in_venv(f"mypy {file}.py")
            self._run_in_venv(f"mypyc {file}.py")
        self._run_in_venv("python run.py")

    def package(self):
        p = Path(self.build_folder)
        for d in p.glob("lib*cpython*"):
            self.copy("*.pyd", src=d, keep_path=False)

    # some useful helper functions
    def _generate_venv(self):
        """Create a venv in self.build_folder"""
        loc = {
            "package_interpreter": Path(self.deps_user_info["python-for-blade"].python),
            "venv": Path(self.build_folder),
        }
        self.output.warn(f"Creating a virtual environment in {loc['venv']}")

        if not loc["package_interpreter"].exists():
            raise errors.ConanException("You can only make a venv in projects that have python for blade")
        self.run(f"{loc['package_interpreter']} -m venv {loc['venv']}")

        if not self._venv().exists():
            raise errors.ConanException(f"Virtual environment Python interpreter not found at: {self._venv()}")

        # Install the base_packages
        self._run_in_venv("python -m pip install -r requirements.txt")

    def _venv(self):
        return Path(self.build_folder, "Scripts", "python.exe")

    def _run_in_venv(self, cmd: str):
        """Run a single command in a venv from the root of the project"""
        working_folder = self.source_folder
        activate = str(Path(self._venv().parent, "activate.bat"))
        python_path = f"set PYTHONPATH=%PYTHONPATH%;{str(working_folder)}"
        cmd = f"{activate} && {python_path} && {cmd}"
        self.run(cwd=working_folder, command=cmd, ignore_errors=False)
