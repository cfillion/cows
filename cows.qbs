import qbs 1.0

CppApplication {
  name: "cows"
  consoleApplication: true

  cpp.cxxFlags: [
    "-fcolor-diagnostics",
    "-std=c++11",
  ]

  files: [
    "src/chat.*",
    "src/command.*",
    "src/logger.*",
    "src/main.cpp",
    "src/module.h",
    "src/peer.*",
    "src/room.*",
    "src/server.*",
  ]

  Depends { name: "Qt"; submodules: [
    "core",
    "websockets",
  ]}

  Group {
    fileTagsFilter: "application"
    qbs.install: true
    qbs.installDir: "bin"
  }
}
