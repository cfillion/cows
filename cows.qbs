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
    "src/logger.*",
    "src/main.cpp",
    "src/message.*",
    "src/module.h",
    "src/peer.*",
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
