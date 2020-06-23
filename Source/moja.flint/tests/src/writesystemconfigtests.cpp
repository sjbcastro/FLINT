#include "moja/flint/writesystemconfig.h"
#include <moja/flint/LandUnitController.h>
#include <moja/exception.h>

#include <boost/test/unit_test.hpp>

#include <fstream>
#include <filesystem>

BOOST_AUTO_TEST_SUITE(asdf)

BOOST_AUTO_TEST_CASE(flint_WriteSystemConfig_ExceptionIsThrownWhenFileNotFound) {
   std::filesystem::path outputPath = "outputPath";
   std::filesystem::path testName = "testName";
   std::filesystem::path expectedFilename = outputPath / testName / "00000_000_000000_PreTimingSequence_0_0.json";

   std::filesystem::remove_all(outputPath);

   BOOST_REQUIRE(!std::filesystem::exists(outputPath));

   Poco::Mutex mutex;
   moja::flint::WriteSystemConfig writeSysConfig(mutex);

   moja::DynamicObject config(
       {{"output_path", outputPath.string()}, {"name", testName.string()}, {"on_notification", "PreTimingSequence"}});
   writeSysConfig.configure(config);

   moja::flint::ModuleMetaData metaData;
   writeSysConfig.StartupModule(metaData);

   moja::flint::LandUnitController landUnitController;
   writeSysConfig.setLandUnitController(landUnitController);

   BOOST_CHECK_NO_THROW(writeSysConfig.onSystemInit());

   std::filesystem::path test_dir = outputPath / testName;
   BOOST_CHECK(std::filesystem::exists(test_dir));

   writeSysConfig.onPreTimingSequence();

   BOOST_REQUIRE(std::filesystem::exists(expectedFilename));
   BOOST_REQUIRE(std::filesystem::file_size(expectedFilename) == 0);

   std::ofstream out(expectedFilename);
   out << "{ \"key\" : 2}";
   out.close();

   BOOST_REQUIRE(std::filesystem::file_size(expectedFilename) > 0);

   writeSysConfig.onPreTimingSequence();

   BOOST_REQUIRE(std::filesystem::exists(expectedFilename));
   BOOST_CHECK(std::filesystem::file_size(expectedFilename) == 0);

   std::filesystem::remove_all(outputPath);
}

BOOST_AUTO_TEST_SUITE_END();
