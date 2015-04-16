#include <algorithm>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <sys/ioctl.h>

#include "logging.hpp"
// #include "server.h"

using namespace std;

LOG_MODULE("main");

bool setupLogging(const string &file, const string &level)
{
  string level_lc = level;
  transform(level_lc.begin(), level_lc.end(), level_lc.begin(), ::tolower);

  map<string, Logger::Level> levels = {
    {"debug", Logger::DEBUG},
    {"info", Logger::INFO},
    {"warning", Logger::WARNING},
    {"error", Logger::ERROR},
    {"fatal", Logger::FATAL},
  };

  if(!levels.count(level_lc)) {
    Logger::open();

    LOG_FATAL(boost::format("invalid log level: %s") % level);

    return false;
  }

  Logger::open(file, levels[level_lc]);

  return true;
}

int main(int argc, char *argv[])
{
  namespace po = boost::program_options;

  const string caption = "Chat on Web Sockets (COWS) v0.0.1";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  po::options_description desc(caption, w.ws_col);
  desc.add_options()
    ("listen,L", po::value<string>()
     ->value_name("HOST:PORT")->default_value("0.0.0.0:7169"),
     "listen for connections on the specified address and port")

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

  po::variables_map vm;

  try {
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
  }
  catch(po::error &err) {
    Logger::open();

    LOG_FATAL(err.what());

    return EXIT_FAILURE;
  }

  if (vm.count("help")) {
    cout << desc;
    return EXIT_SUCCESS;
  }

  if(vm.count("version")) {
    cout << caption << endl;
    return EXIT_SUCCESS;
  }

  const string listen_addr = vm["listen"].as<string>();
  const string log_file = vm["logfile"].as<string>();
  const string log_level = vm["loglevel"].as<string>();

  if(!setupLogging(log_file, log_level))
    return EXIT_FAILURE;

  // Server server;
  //
  // if(!server.open(parser.value(listenOption)))
  //   return EXIT_FAILURe;

  return EXIT_SUCCESS;
}
