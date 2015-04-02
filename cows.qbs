import qbs 1.0

CppApplication {
  name: "cows"
  type: "application"
  consoleApplication: true

  cpp.cxxFlags: [
    "-fcolor-diagnostics",
    "-std=c++11",
  ]

  Properties {
    condition: qbs.targetOS.contains("osx")
    cpp.cxxFlags: outer.concat("-stdlib=libc++")
  }

  cpp.minimumOsxVersion: "10.7"

  files: [
    "src/command.*",
    "src/logger.*",
    "src/main.cpp",
    "src/names.*",
    "src/peer.*",
    "src/room.*",
    "src/server.*",
    "src/user.*",
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
