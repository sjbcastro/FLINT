#include "moja/datarepository/datarepositoryexceptions.h"
#include "moja/datarepository/rasterreader.h"
#include "moja/datarepository/tileblockcellindexer.h"

#include <boost/test/unit_test.hpp>
#include <Poco/JSON/JSONException.h>

#include <filesystem>
#include <fstream>

const std::string emptyJsonFile = "temp\\test.json";
const std::string emptyBlkFile = "temp\\test_4.5_3.4.blk";

struct RasterReaderTestFixture {
   RasterReaderTestFixture() { std::filesystem::create_directory("temp"); }
   ~RasterReaderTestFixture() {
      std::filesystem::remove(emptyJsonFile);
      std::filesystem::remove(emptyBlkFile);
      std::filesystem::remove("temp");
   }
};

BOOST_FIXTURE_TEST_SUITE(RasterReaderTests, RasterReaderTestFixture);

BOOST_AUTO_TEST_CASE(datarepository_FlintMetaDataRasterReader_RasterReaderThrowsFileNotFoundException) {
   moja::datarepository::FlintMetaDataRasterReader rasterReader("temp", "test", {});
   BOOST_CHECK_THROW(rasterReader.readMetaData(), moja::datarepository::FileNotFoundException);
}

BOOST_AUTO_TEST_CASE(datarepository_FlintMetaDataRasterReader_RasterReaderThrowsJSONExceptionOnEmptyFile) {
   std::ofstream out(emptyJsonFile);
   out.close();

   moja::datarepository::FlintMetaDataRasterReader rasterReader("temp", "test", {});
   BOOST_CHECK_THROW(rasterReader.readMetaData(), Poco::JSON::JSONException);
}

BOOST_AUTO_TEST_CASE(datarepository_FlintTileRasterReader_RasterReaderThrowsFileNotFoundException) {
   moja::datarepository::TileBlockCellIndexer indexer{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
   moja::Point point;
   point.lat = 3.4;
   point.lon = 4.5;
   moja::datarepository::FlintTileRasterReader rasterReader("temp", point, "test", indexer, {});
   moja::datarepository::BlockIdx blockIndex{{}, {}, indexer};
   std::vector<double> output;
   BOOST_CHECK_THROW(rasterReader.readBlockData(blockIndex, &output), moja::datarepository::FileReadException);
}

BOOST_AUTO_TEST_CASE(datarepository_FlintTileRasterReader_RasterReaderThrowsJSONExceptionOnEmptyFile) {
   std::ofstream out(emptyBlkFile);
   out.close();

   moja::datarepository::TileBlockCellIndexer indexer{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
   moja::Point point;
   point.lat = 3.4;
   point.lon = 4.5;
   moja::datarepository::FlintTileRasterReader rasterReader("temp", point, "test", indexer, {});
   moja::datarepository::BlockIdx blockIndex{{}, {}, indexer};
   std::vector<double> output;
   BOOST_CHECK_NO_THROW(rasterReader.readBlockData(blockIndex, &output));
}

BOOST_AUTO_TEST_CASE(datarepository_FlintStackRasterReader_RasterReaderThrowsFileNotFoundException) {
   moja::datarepository::TileBlockCellIndexer indexer{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
   moja::Point point;
   point.lat = 3.4;
   point.lon = 4.5;
   moja::datarepository::FlintStackRasterReader rasterReader("temp", point, "test", indexer, {});
   moja::datarepository::BlockIdx blockIndex{{}, {}, indexer};
   std::vector<double> output;
   BOOST_CHECK_THROW(rasterReader.readBlockData(blockIndex, 0, &output), moja::datarepository::FileReadException);
}

BOOST_AUTO_TEST_CASE(datarepository_FlintStackRasterReader_RasterReaderThrowsJSONExceptionOnEmptyFile) {
   std::ofstream out(emptyBlkFile);
   out.close();

   moja::datarepository::TileBlockCellIndexer indexer{0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
   moja::Point point;
   point.lat = 3.4;
   point.lon = 4.5;
   moja::datarepository::FlintStackRasterReader rasterReader("temp", point, "test", indexer, {});
   moja::datarepository::BlockIdx blockIndex{{}, {}, indexer};
   std::vector<double> output;
   BOOST_CHECK_NO_THROW(rasterReader.readBlockData(blockIndex, 0, &output));
}

BOOST_AUTO_TEST_SUITE_END();
