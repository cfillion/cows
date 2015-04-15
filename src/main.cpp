#include <algorithm>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <sys/ioctl.h>

#include "logging.hpp"
// #include "server.h"

LOG_MODULE("main");

bool setupLogging(const std::string &file, const std::string &level)
{
  std::string level_lc = level;
  std::transform(level_lc.begin(), level_lc.end(), level_lc.begin(), ::tolower);

  std::map<std::string, Logger::Level> levels = {
    {"debug", Logger::DEBUG},
    {"info", Logger::INFO},
    {"warning", Logger::WARNING},
    {"error", Logger::ERROR},
    {"fatal", Logger::FATAL},
  };

  if(!levels.count(level_lc)) {
    Logger::open();

    LOG_FATAL(str(boost::format("invalid log level: %s") % level));

    return false;
  }

  Logger::open(file, levels[level_lc]);

  return true;
}

int main(int argc, char *argv[])
{
  namespace po = boost::program_options;

  const std::string caption = "Chat on Web Sockets (COWS) v0.0.1";

  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  po::options_description desc(caption, w.ws_col);
  desc.add_options()
    ("listen,L", po::value<std::string>()
     ->value_name("ADDRESS:PORT")->default_value("0.0.0.0:7169"),
     "listen for connections on the specified address and port")

    ("logfile,o", po::value<std::string>()
     ->value_name("FILE")->default_value("-"),
     "write log messages to a file instead of outputting to stderr")
    ("loglevel,l", po::value<std::string>()
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
    std::cout << desc;
    return EXIT_SUCCESS;
  }

  if(vm.count("version")) {
    std::cout << caption << std::endl;
    return EXIT_SUCCESS;
  }

  const std::string listen_addr = vm["listen"].as<std::string>();
  const std::string log_file = vm["logfile"].as<std::string>();
  const std::string log_level = vm["loglevel"].as<std::string>();

  if(!setupLogging(log_file, log_level))
    return EXIT_FAILURE;

  // Server server;
  //
  // if(!server.open(parser.value(listenOption)))
  //   return EXIT_FAILURe;

  return EXIT_SUCCESS;
}
