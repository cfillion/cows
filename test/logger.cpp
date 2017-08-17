#include <catch.hpp>

#include <logger.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <fstream>
#include <sstream>

using namespace boost::algorithm;
using namespace std;

using Catch::Matchers::Contains;
using Catch::Matchers::EndsWith;

const char *M = "[logging]";

TEST_CASE("log higher level", M)
{
  ostringstream stream;
  Logger logger(Logger::DEBUG, &stream);

  logger.log(Logger::DEBUG, "logger_test", "hello world!");
  REQUIRE_THAT(stream.str(), EndsWith("] (DD) logger_test: hello world!\n"));

  logger.log(Logger::INFO, "logger_test", "hello world!");
  REQUIRE_THAT(stream.str(), EndsWith("] (II) logger_test: hello world!\n"));
}

TEST_CASE("don't log lower levels", M)
{
  ostringstream stream;
  Logger logger(Logger::INFO, &stream);

  logger.log(Logger::DEBUG, "logger_test", "hello world!");
  REQUIRE_THAT(stream.str(), !EndsWith("] (DD) logger_test: hello world!\n"));

  logger.log(Logger::WARNING, "logger_test", "hello world!");
  REQUIRE_THAT(stream.str(), EndsWith("] (WW) logger_test: hello world!\n"));
}

TEST_CASE("successfully set log file", M)
{
  ostringstream original_stream;
  Logger logger(Logger::INFO, &original_stream);

  ofstream file_stream;
  CHECK(logger.stream() == &original_stream);
  REQUIRE(logger.open_file("/dev/null", &file_stream));
  REQUIRE(logger.stream() == &file_stream);
  REQUIRE(file_stream.is_open());

  logger.log(Logger::INFO, "logger_test", "hello world!");
  REQUIRE(original_stream.str().empty());
}

TEST_CASE("failure to open log file", M)
{
  ostringstream stream;
  Logger logger(Logger::INFO, &stream);

  ofstream file;
  CHECK(!file.fail());
  CHECK(logger.stream() == &stream);
  REQUIRE(!logger.open_file(".", &file));
  REQUIRE(logger.stream() == &stream);
  REQUIRE(file.fail());

  REQUIRE_THAT(stream.str(), Contains("(EE) logger: cannot open '.' for writing"));
}