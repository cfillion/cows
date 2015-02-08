import qbs 1.0

CppApplication {
  name: "cows"
  consoleApplication: true

  cpp.cxxFlags: [
    "-fcolor-diagnostics",
    "-std=c++11",
  ]

  files: [
    "src/client.*",
    "src/logger.*",
    "src/main.cpp",
    "src/message.*",
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
