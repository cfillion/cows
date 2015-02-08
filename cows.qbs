import qbs 1.0

CppApplication {
  name: "cows"

  cpp.cxxFlags: [
    "-fcolor-diagnostics",
    "-std=c++11",
  ]

  files: [
    "src/main.cpp",
    "src/logger.*",
  ]

  Depends { name: "Qt"; submodules: [
    "core",
  ]}

  Group {
    fileTagsFilter: "application"
    qbs.install: true
    qbs.installDir: "bin"
  }
}
