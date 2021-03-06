/**
 * \file TestTileManager.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#ifndef TEST_UNIT_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_
#define TEST_UNIT_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <memory>

#include "kes_dashboard_live_tiles_client/ITile.h"
#include "kes_dashboard_live_tiles_client/ITileManager.h"
#include "kes_dashboard_live_tiles_client/TileManager.h"

#include "mercury/utils/Time.h"

#include "test/helpers/TemplateTests.h"
#include "test/mocks/kes_dlt_cli/MockDefaultTileLoader.h"
#include "test/mocks/kes_dlt_cli/MockOnlineLoader.h"
#include "test/mocks/kes_dlt_cli/MockTile.h"
#include "test/mocks/kes_dlt_cli/MockTileCache.h"
#include "test/mocks/MockEnvironment.h"


namespace KESDLTC {
namespace test {


/**
 * Check that users of the class can use it through a pointer of the
 * ITileManager interface.
 */
TEST(TestTileManager, IsAnITileManager) {
    Mercury::test::testBaseIsAnInterface<
        KESDLTC::TileManager, KESDLTC::ITileManager>();
}


/**
 * Check that TileManager constructor calls Environment.get("HOME").
 */
TEST(TestTileManager, ConstructorCallsEnvironmentGetEnvWithHome) {
    auto mockEnvironment = std::make_shared<Mercury::Utils::test::MockEnvironment>();  // NOLINT

    ON_CALL(*mockEnvironment, get)
        .WillByDefault(::testing::Return(""));

    EXPECT_CALL(*mockEnvironment, get("HOME"))
        .Times(1);

    KESDLTC::TileManager tileManager(
        "", nullptr, nullptr, nullptr, mockEnvironment);
}

/**
 * Check that TileManager.getTiles(false) calls and returns
 * OnlineLoader.getTiles() when TileManager.isCooldown() is false.
 */
TEST(TestTileManager, GetTilesCacheFalseNoCooldownRetrievesWithOnlineLoader) {
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getTiles(false) does not call OnlineLoader.getTiles()
 * when TileManager.isCooldown() is true.
 */
TEST(TestTileManager, GetTilesCacheFalseWithCooldownDoesNotSpamOnlineLoader) {
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getLastUpdated)
        .WillByDefault(::testing::Return(getTimestamp()));

    ON_CALL(*mockOnlineLoader, getQueryCooldown)
        .WillByDefault(::testing::Return(getTimestamp()));

    EXPECT_CALL(*mockTileCache, getLastUpdated)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getQueryCooldown)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(0);

    tileManager.getTiles(false);
}

/**
 * Check that TileManager.getTiles(false) returns OnlineLoader.getTiles()
 * and calls TileCache.update(result) when TileManager.isCooldown() is false
 * and result is not empty.
 */
TEST(
    TestTileManager,
    GetTilesCacheFalseNoCooldownUpdatesTileCacheWhenThereAreResults) {
    // NOLINT
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockOnlineLoader, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, update(::testing::Eq(tiles)))
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getTiles(false) does not call TileCache.update()
 * when TileManager.isCooldown() is false and OnlineLoader.getTiles() result
 * is empty and TileCache.getTiles() is not empty.
 */
TEST(
    TestTileManager,
    GetTilesCacheFalseNoCooldownDoesNotUpdateCacheWhenThereAreNoResults) {
    // NOLINT
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, update)
        .Times(0);

    tileManager.getTiles(false);
}

/**
 * Check that TileManager.getTiles(false) returns TileCache.getTiles() when
 * TileManager.isCooldown() is false and OnlineLoader.getTiles() result
 * is empty.
 */
TEST(
    TestTileManager,
    GetTilesCacheFalseNoCooldownRetrievesFromTileCacheWhenThereAreNoResults) {
    // NOLINT
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getTiles(false) returns TileCache.getTiles() when
 * TileManager.isCooldown() is true.
 */
TEST(TestTileManager, GetTilesCacheFalseWithCooldonwRetrievesFromTileCache) {
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getLastUpdated)
        .WillByDefault(::testing::Return(getTimestamp()));

    ON_CALL(*mockOnlineLoader, getQueryCooldown)
        .WillByDefault(::testing::Return(getTimestamp()));

    EXPECT_CALL(*mockTileCache, getLastUpdated)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getQueryCooldown)
        .Times(1);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getTiles(true) returns TileCache.getTiles().
 */
TEST(TestTileManager, GetTilesCacheTrueRetrievesFromTileCache) {
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(true);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getTiles(true) returns DefaultTileLoader.getTiles()
 * when TileCache.getTiles() result is empty and calls TileCache.update().
 */
TEST(TestTileManager, GetTilesCacheTrueRetrievesDefaultTilesWhenCacheIsEmpty) {
    auto mockOnlineLoader = std::make_shared<MockOnlineLoader>();
    auto mockTileCache = std::make_shared<MockTileCache>();
    auto mockDefaultTileLoader = std::make_shared<MockDefaultTileLoader>();
    auto mockTile = std::make_shared<MockTile>();

    std::list<std::shared_ptr<KESDLTC::ITile>> tiles, result;
    tiles.push_back(mockTile);

    KESDLTC::TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockDefaultTileLoader, getTiles)
        .WillByDefault(::testing::Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    EXPECT_CALL(*mockDefaultTileLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, update(tiles))
        .Times(1);

    result = tileManager.getTiles(true);

    EXPECT_EQ(result, tiles);
}

}  // namespace test
}  // namespace KESDLTC

#endif  // TEST_UNIT_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_
