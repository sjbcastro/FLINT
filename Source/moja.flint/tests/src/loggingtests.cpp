#include "moja/logging.h"

#include <boost/test/unit_test.hpp>
#include <fstream>
#include <filesystem>

const std::string defaultConfigFile = std::filesystem::current_path().string() + "\\logging.conf";
const std::string customConfigFile = std::filesystem::current_path().string() + "\\custom.conf";

struct LoggingTestsFixture {
   LoggingTestsFixture() {}
   ~LoggingTestsFixture() {
      std::filesystem::remove(defaultConfigFile);
      std::filesystem::remove(customConfigFile);
   }
};

BOOST_AUTO_TEST_SUITE(LoggingTests)

BOOST_FIXTURE_TEST_CASE(moja_Logging_MainTest, LoggingTestsFixture) {

   moja::Logging::init();
   BOOST_CHECK_EQUAL(moja::Logging::configurationFilename(), "internal default");

   moja::Logging::setConfigurationText("[Sinks.console]\n");
   moja::Logging::init();
   BOOST_CHECK_EQUAL(moja::Logging::configurationFilename(), "internal text");

   std::ofstream custom_config(customConfigFile);
   custom_config << "[Sinks.console]\n";
   custom_config.close();

   moja::Logging::setConfigurationFile(customConfigFile);
   moja::Logging::init();
   BOOST_CHECK_EQUAL(moja::Logging::configurationFilename(), customConfigFile);
   
   moja::Logging::setConfigurationFile("nonexistant.cfg");
   moja::Logging::init();
   BOOST_CHECK_EQUAL(moja::Logging::configurationFilename(), "internal default");

   std::ofstream default_config(defaultConfigFile);
   default_config << "[Sinks.console]\n";
   default_config.close();

   moja::Logging::setConfigurationFile("nonexistant.cfg");
   moja::Logging::init();
   BOOST_CHECK_EQUAL(moja::Logging::configurationFilename(), defaultConfigFile);
}

BOOST_AUTO_TEST_SUITE_END();