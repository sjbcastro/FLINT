#include "moja/flint/writevariablegrid.h"
#include <moja/flint/LandUnitController.h>
#include <moja/flint/flintdatavariable.h>
#include <moja/flint/spatiallocationinfo.h>

#include <boost/test/unit_test.hpp>

#include <fstream>
#include <filesystem>

BOOST_AUTO_TEST_SUITE(WriteVariableGridTests)

BOOST_AUTO_TEST_CASE(flint_WriteVariableGrid_1) {
   std::filesystem::path outputPath = "outputPath";
   std::filesystem::path testName = "testName";
   std::filesystem::path itemTestName = "itemTestName";
   std::filesystem::path tileFolder = "000_000";
   std::filesystem::path gridFilename = "itemTestName_000_000_00_0.grd";
   std::filesystem::path gridHeaderFilename = "itemTestName_000_000_00_0.hdr";

   std::filesystem::path gridFilepath = outputPath / itemTestName / tileFolder / gridFilename;
   std::filesystem::path gridHeaderFilepath = outputPath / itemTestName / tileFolder / gridHeaderFilename;

   std::filesystem::remove_all(outputPath);
   BOOST_REQUIRE(!std::filesystem::exists(outputPath));

   Poco::Mutex mutex;
   moja::flint::WriteVariableGrid writeVariableGrid(mutex);

   moja::flint::ModuleMetaData metaData;
   writeVariableGrid.StartupModule(metaData);

   moja::flint::LandUnitController landUnitController;

   std::shared_ptr<moja::flint::IFlintData> locationInfo = std::make_shared<moja::flint::SpatialLocationInfo>();
   std::shared_ptr<moja::flint::IVariable> dataVariable =
   std::make_shared<moja::flint::FlintDataVariable>(locationInfo, "testLibraryName", "spatialLocationInfo", moja::flint::VariableInfo{});
   landUnitController.addVariable("spatialLocationInfo", dataVariable);

   writeVariableGrid.setLandUnitController(landUnitController);

   moja::DynamicObject configItem1({{"enabled", true}, {"variable_data_type", "UInt8"}, {"data_name", itemTestName.string()}, {"variable_name", "spatialLocationInfo"}});

   moja::DynamicVector configItems;
   configItems.push_back(configItem1);

   moja::DynamicObject config({{"output_path", outputPath.string()},
                               {"name", testName.string()},
                               {"items", configItems}});

   writeVariableGrid.configure(config);

   writeVariableGrid.onSystemInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / itemTestName));

   writeVariableGrid.onLocalDomainInit();
   writeVariableGrid.onLocalDomainProcessingUnitInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / itemTestName / tileFolder));

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   // Write some random data to test it is cleared in onLocalDomainProcessingUnitShutdown
   std::ofstream out(gridFilepath);
   out << "aaaaaaaaaaa";
   out.close();

   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) > 0);

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));
   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) == 0);
}

BOOST_AUTO_TEST_CASE(flint_WriteVariableGrid_2) {
   std::filesystem::path outputPath = "outputPath";
   std::filesystem::path testName = "testName";
   std::filesystem::path itemTestName = "itemTestName";
   std::filesystem::path tileFolder = "000_000";
   std::filesystem::path gridFilename = "itemTestName_000_000_00_0.grd";
   std::filesystem::path gridHeaderFilename = "itemTestName_000_000_00_0.hdr";

   std::filesystem::path gridFilepath = outputPath / tileFolder / gridFilename;
   std::filesystem::path gridHeaderFilepath = outputPath / tileFolder / gridHeaderFilename;

   std::filesystem::remove_all(outputPath);
   BOOST_REQUIRE(!std::filesystem::exists(outputPath));

   Poco::Mutex mutex;
   moja::flint::WriteVariableGrid writeVariableGrid(mutex);

   moja::flint::ModuleMetaData metaData;
   writeVariableGrid.StartupModule(metaData);

   moja::flint::LandUnitController landUnitController;

   std::shared_ptr<moja::flint::IFlintData> locationInfo = std::make_shared<moja::flint::SpatialLocationInfo>();
   std::shared_ptr<moja::flint::IVariable> dataVariable = std::make_shared<moja::flint::FlintDataVariable>(
       locationInfo, "testLibraryName", "spatialLocationInfo", moja::flint::VariableInfo{});
   landUnitController.addVariable("spatialLocationInfo", dataVariable);

   writeVariableGrid.setLandUnitController(landUnitController);

   moja::DynamicObject configItem1({{"enabled", true},
                                    {"variable_data_type", "UInt8"},
                                    {"data_name", itemTestName.string()},
                                    {"variable_name", "spatialLocationInfo"}});

   moja::DynamicVector configItems;
   configItems.push_back(configItem1);

   moja::DynamicObject config({{"output_path", outputPath.string()},
                               {"name", testName.string()},
                               {"items", configItems},
                               {"force_variable_folder_name", false}});

   writeVariableGrid.configure(config);

   writeVariableGrid.onSystemInit();

   BOOST_CHECK(std::filesystem::exists(outputPath));
   BOOST_CHECK(!std::filesystem::exists(outputPath / itemTestName));

   writeVariableGrid.onLocalDomainInit();
   writeVariableGrid.onLocalDomainProcessingUnitInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / tileFolder));

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   // Write some random data to test it is cleared in onLocalDomainProcessingUnitShutdown
   std::ofstream out(gridFilepath);
   out << "aaaaaaaaaaa";
   out.close();

   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) > 0);

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));
   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) == 0);
}

BOOST_AUTO_TEST_CASE(flint_WriteVariableGrid_3) {
   std::filesystem::path outputPath = "outputPath";
   std::filesystem::path testName = "testName";
   std::filesystem::path itemTestName = "itemTestName";
   std::filesystem::path tileFolder = "000000";
   std::filesystem::path gridFilename = "itemTestName_000000_00_0.grd";
   std::filesystem::path gridHeaderFilename = "itemTestName_000000_00_0.hdr";

   std::filesystem::path gridFilepath = outputPath / itemTestName / tileFolder / gridFilename;
   std::filesystem::path gridHeaderFilepath = outputPath / itemTestName / tileFolder / gridHeaderFilename;

   std::filesystem::remove_all(outputPath);
   BOOST_REQUIRE(!std::filesystem::exists(outputPath));

   Poco::Mutex mutex;
   moja::flint::WriteVariableGrid writeVariableGrid(mutex);

   moja::flint::ModuleMetaData metaData;
   writeVariableGrid.StartupModule(metaData);

   moja::flint::LandUnitController landUnitController;

   std::shared_ptr<moja::flint::IFlintData> locationInfo = std::make_shared<moja::flint::SpatialLocationInfo>();
   std::shared_ptr<moja::flint::IVariable> dataVariable = std::make_shared<moja::flint::FlintDataVariable>(
       locationInfo, "testLibraryName", "spatialLocationInfo", moja::flint::VariableInfo{});
   landUnitController.addVariable("spatialLocationInfo", dataVariable);

   writeVariableGrid.setLandUnitController(landUnitController);

   moja::DynamicObject configItem1({{"enabled", true},
                                    {"variable_data_type", "UInt8"},
                                    {"data_name", itemTestName.string()},
                                    {"variable_name", "spatialLocationInfo"}});

   moja::DynamicVector configItems;
   configItems.push_back(configItem1);

   moja::DynamicObject config({{"output_path", outputPath.string()},
                               {"name", testName.string()},
                               {"items", configItems},
                               {"use_indexes_for_folder_name", true}});

   writeVariableGrid.configure(config);

   writeVariableGrid.onSystemInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / itemTestName));

   writeVariableGrid.onLocalDomainInit();
   writeVariableGrid.onLocalDomainProcessingUnitInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / itemTestName / tileFolder));

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   // Write some random data to test it is cleared in onLocalDomainProcessingUnitShutdown
   std::ofstream out(gridFilepath);
   out << "aaaaaaaaaaa";
   out.close();

   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) > 0);

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));
   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) == 0);
}

BOOST_AUTO_TEST_CASE(flint_WriteVariableGrid_4) {
   std::filesystem::path outputPath = "outputPath";
   std::filesystem::path testName = "testName";
   std::filesystem::path itemTestName = "itemTestName";
   std::filesystem::path tileFolder = "000000";
   std::filesystem::path gridFilename = "itemTestName_000000_00_0.grd";
   std::filesystem::path gridHeaderFilename = "itemTestName_000000_00_0.hdr";

   std::filesystem::path gridFilepath = outputPath / tileFolder / gridFilename;
   std::filesystem::path gridHeaderFilepath = outputPath / tileFolder / gridHeaderFilename;

   std::filesystem::remove_all(outputPath);
   BOOST_REQUIRE(!std::filesystem::exists(outputPath));

   Poco::Mutex mutex;
   moja::flint::WriteVariableGrid writeVariableGrid(mutex);

   moja::flint::ModuleMetaData metaData;
   writeVariableGrid.StartupModule(metaData);

   moja::flint::LandUnitController landUnitController;

   std::shared_ptr<moja::flint::IFlintData> locationInfo = std::make_shared<moja::flint::SpatialLocationInfo>();
   std::shared_ptr<moja::flint::IVariable> dataVariable = std::make_shared<moja::flint::FlintDataVariable>(
       locationInfo, "testLibraryName", "spatialLocationInfo", moja::flint::VariableInfo{});
   landUnitController.addVariable("spatialLocationInfo", dataVariable);

   writeVariableGrid.setLandUnitController(landUnitController);

   moja::DynamicObject configItem1({{"enabled", true},
                                    {"variable_data_type", "UInt8"},
                                    {"data_name", itemTestName.string()},
                                    {"variable_name", "spatialLocationInfo"}});

   moja::DynamicVector configItems;
   configItems.push_back(configItem1);

   moja::DynamicObject config({{"output_path", outputPath.string()},
                               {"name", testName.string()},
                               {"items", configItems},
                               {"force_variable_folder_name", false},
                               {"use_indexes_for_folder_name", true}});

   writeVariableGrid.configure(config);

   writeVariableGrid.onSystemInit();

   BOOST_CHECK(std::filesystem::exists(outputPath));
   BOOST_CHECK(!std::filesystem::exists(outputPath / itemTestName));

   writeVariableGrid.onLocalDomainInit();
   writeVariableGrid.onLocalDomainProcessingUnitInit();

   BOOST_CHECK(std::filesystem::exists(outputPath / tileFolder));

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   // Write some random data to test it is cleared in onLocalDomainProcessingUnitShutdown
   std::ofstream out(gridFilepath);
   out << "aaaaaaaaaaa";
   out.close();

   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) > 0);

   writeVariableGrid.onPreTimingSequence();
   writeVariableGrid.onLocalDomainProcessingUnitShutdown();

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));

   BOOST_CHECK(std::filesystem::exists(gridFilepath));
   BOOST_CHECK(std::filesystem::exists(gridHeaderFilepath));
   BOOST_REQUIRE(std::filesystem::file_size(gridFilepath) == 0);
}

BOOST_AUTO_TEST_SUITE_END();
