from conan import ConanFile
from conan.tools.build import check_min_cppstd
from conan.tools.cmake import CMakeToolchain, CMakeDeps, CMake, cmake_layout
from pathlib import Path
from conan.tools.files import copy, mkdir
from jinja2 import Template

required_conan_version = ">=1.58"


class aoc(ConanFile):
    name = "aoc-10"
    author = "Matthew McMillan"
    url = "https://gitlab.bigrep.com/matthew.mcmillan/aoc2024"
    description = "A project to learn with"
    topics = "c++"
    settings = "os", "compiler", "build_type", "arch"
    version = "0.1.0"

    def _should_test(self) -> bool:
        # add `-c tools.build:skip_test=False` to conan install
        test_configured = self.conf.get("tools.build:skip_test", False, check_type=bool)
        is_debug = self.settings.build_type == "Debug"
        return test_configured or is_debug

    def layout(self):
        # https://docs.conan.io/en/1.64/reference/conanfile/tools/cmake/cmake_layout.html#cmake-layout
        # source dir is '.' (because that is where cmakelists.txt is)
        # include dir is 'include'
        # build dir is 'build/release' or 'build/debug'
        cmake_layout(self)
        self.folders.generators = f"{self.folders.build}/generators"  # keeps things neat

        if self._should_test():
            # since there are text files to test in test dir
            self.cpp.package.resdirs.append("tests")

    def validate(self):
        """Validation section"""
        if self.settings.compiler.get_safe("cppstd"):
            check_min_cppstd(self, 20)

    def build_requirements(self):
        """Any project that this project needs only to build or only to test
        use self.build_requires and self.test_requires respectively"""
        self.test_requires("gtest/1.14.0")

    def requirements(self):
        """Any projects that this project needs to run use self.requires"""
        pass

    def _visual_studio(self):
        file_in = Path(self.source_folder, "scripts", "visual_studio.jinja")
        file_out = Path(self.source_folder, ".vs", "launch.vs.json")
        file_out.parent.mkdir(exist_ok=True)
        t = Template(file_in.read_text())
        file_out.write_text(t.render(
            example_text=Path(self.source_folder, "example.txt").as_posix(),
            main_text=Path(self.source_folder, "real.txt").as_posix(),
        ))

    def generate(self):
        self._visual_studio()
        # this is to add packages to cmake and create cmake files
        # https://docs.conan.io/en/1.64/reference/conanfile/tools/cmake/cmakedeps.html#cmakedeps
        deps = CMakeDeps(self)
        deps.generate()

        # this is used to make a custom cmake lists file with all paths etc
        # https://docs.conan.io/en/1.64/reference/conanfile/tools/cmake/cmaketoolchain.html#cmaketoolchain
        # values can be set here based on profile or in profile file
        # https://docs.conan.io/2/reference/config_files/global_conf.html#reference-config-files-global-conf
        tc = CMakeToolchain(self)
        tc.cache_variables["RUN_TESTS"] = True # just a simple example
        tc.generate()

    def build(self):
        """Runs from the build folder"""
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

        if self._should_test():
            self.run("ctest --output-junit engine_test.xml")
    
