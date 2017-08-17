#include <boost/algorithm/string/case_conv.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <sys/ioctl.h>

#include "logging.hpp"
#include "server.hpp"

using namespace std;

LOG_MODULE("main");

static const map<string, Logger::Level> LOG_LEVELS = {
  {"debug", Logger::DEBUG},
  {"info", Logger::INFO},
  {"warning", Logger::WARNING},
  {"error", Logger::ERROR},
  {"fatal", Logger::FATAL},
};

static const string CAPTION = "Chat on Web Sockets (COWS) v0.0.1";

int main(int argc, char *argv[])
{
  namespace po = boost::program_options;

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  po::options_description desc(CAPTION, w.ws_col);
  desc.add_options()
    ("bind,b", po::value<string>()
     ->value_name("ADDRESS")->default_value("0.0.0.0"),
     "listen for connections on the specified hostname or IP address")
    ("port,p", po::value<string>()
     ->value_name("PORT")->default_value("7169"),
     "listen for connections on the specified port")

    ("logfile,o", po::value<string>()
     ->value_name("FILE")->default_value("-"),
     "write log messages to a file instead of outputting to stderr")
    ("loglevel,l", po::value<string>()
     ->value_name("LEVEL")->default_value("INFO"),
     "change the log verbosity level. Possible values are: "
     "FATAL, ERROR, WARNING, INFO and DEBUG")

    ("help,h",
     "display this help and exit")
    ("version,v",
     "output version information and exit")
  ;

  po::variables_map opts;

  try {
    po::store(po::parse_command_line(argc, argv, desc), opts);
    po::notify(opts);
  }
  catch(po::error &err) {
    Logger temp_logger;

    LOG_FATAL(err.what());

    return EXIT_FAILURE;
  }

  if(opts.count("help")) {
    cout << desc;
    return EXIT_SUCCESS;
  }

  if(opts.count("version")) {
    cout << CAPTION << endl;
    return EXIT_SUCCESS;
  }

  const string &log_file = opts["logfile"].as<string>();
  string log_level = opts["loglevel"].as<string>();
  boost::to_lower(log_level);

  if(!LOG_LEVELS.count(log_level)) {
    Logger temp_logger;

    LOG_FATAL(boost::format("invalid log level: %s")
      % opts["loglevel"].as<string>());

    return EXIT_FAILURE;
  }

  Logger logger(log_file, LOG_LEVELS.at(log_level));

  const string &host = opts["bind"].as<string>();
  const string &port = opts["port"].as<string>();

  Server server;
  return server.run(host, port) ? EXIT_SUCCESS : EXIT_FAILURE;
}
