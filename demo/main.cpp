#include <boost/program_options.hpp>
#include <iostream>

#include "Builder.hpp"
#include "LogSettings.hpp"

int main(int argc, char** argv) {
  boost::program_options::options_description desc;

  desc.add_options()("help", "produce help message")(
      "config,c", boost::program_options::value<std::string>(),
      "set cmake config default = \"Debug\"")(
      "install,i", "Add install step in \"_install\" directory")(
      "package,p", "Add package step")("timeout,t",
                                       boost::program_options::value<size_t>(),
                                       "set waiting delay");

  boost::program_options::variables_map mp;
  boost::program_options::store(
      boost::program_options::parse_command_line(argc, argv, desc), mp);
  boost::program_options::notify(mp);

  if (mp.count("help")) {
    std::cout << desc << std::endl;
    return 0;
  }
  Init_logger("trace");
  Builder builder(mp);
  builder.Start();
}
