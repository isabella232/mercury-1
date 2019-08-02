/**
 * \file TestTileManager.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#ifndef TEST_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_
#define TEST_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_


#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>
#include <memory>
#include <string>

#include "kes_dashboard_live_tiles_client/ITile.h"
#include "kes_dashboard_live_tiles_client/TileManager.h"

#include "mercury/utils/Time.h"

#include "test/mocks/kes_dlt_cli/MockDefaultTileLoader.h"
#include "test/mocks/kes_dlt_cli/MockOnlineLoader.h"
#include "test/mocks/kes_dlt_cli/MockTile.h"
#include "test/mocks/kes_dlt_cli/MockTileCache.h"

using std::list;
using std::make_shared;
using std::shared_ptr;
using std::string;

using ::testing::Eq;
using ::testing::Return;

using KESDLTC::ITile;
using KESDLTC::TileManager;

/**
 * Check that TileManager.getCache(false) calls and returns
 * OnlineLoader.getTiles() when TileManager.isCooldown() is false.
 */
TEST(TestTileManager, GetTilesCacheFalseNoCooldownRetrievesWithOnlineLoader) {
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getCache(false) does not call OnlineLoader.getTiles()
 * when TileManager.isCooldown() is true.
 */
TEST(TestTileManager, GetTilesCacheFalseWithCooldownDoesNotSpamOnlineLoader) {
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getLastUpdated)
        .WillByDefault(Return(getTimestamp()));

    ON_CALL(*mockOnlineLoader, getQueryCooldown)
        .WillByDefault(Return(getTimestamp()));

    EXPECT_CALL(*mockTileCache, getLastUpdated)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getQueryCooldown)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(0);

    tileManager.getTiles(false);
}

/**
 * Check that TileManager.getCache(false) returns OnlineLoader.getTiles()
 * and calls TileCache.update(result) when TileManager.isCooldown() is false.
 */
TEST(TestTileManager,
     GetTilesCacheFalseNoCooldownUpdatesTileCacheWhenThereAreResults) {
    // NOLINT
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockOnlineLoader, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, update(Eq(tiles)))
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getCache(false) does not call TileCache.update()
 * when TileManager.isCooldown() is false and OnlineLoader.getTiles() result
 * is empty.
 */
TEST(TestTileManager,
     GetTilesCacheFalseNoCooldownDoesNotUpdateCacheWhenThereAreNoResults) {
    // NOLINT
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, update)
        .Times(0);

    tileManager.getTiles(false);
}

/**
 * Check that TileManager.getCache(false) returns TileCache.getTiles() when
 * TileManager.isCooldown() is false and OnlineLoader.getTiles() result
 * is empty.
 */
TEST(TestTileManager,
     GetTilesCacheFalseNoCooldownRetrievesFromTileCacheWhenThereAreNoResults) {
    // NOLINT
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockOnlineLoader, getTiles)
        .Times(1);

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getCache(false) returns TileCache.getTiles() when
 * TileManager.isCooldown() is true.
 */
TEST(TestTileManager, GetTilesCacheFalseWithCooldonwRetrievesFromTileCache) {
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getLastUpdated)
        .WillByDefault(Return(getTimestamp()));

    ON_CALL(*mockOnlineLoader, getQueryCooldown)
        .WillByDefault(Return(getTimestamp()));

    EXPECT_CALL(*mockTileCache, getLastUpdated)
        .Times(1);

    EXPECT_CALL(*mockOnlineLoader, getQueryCooldown)
        .Times(1);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(false);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getCache(true) returns TileCache.getTiles().
 */
TEST(TestTileManager, GetTilesCacheTrueRetrievesFromTileCache) {
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockTileCache, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    result = tileManager.getTiles(true);

    EXPECT_EQ(result, tiles);
}

/**
 * Check that TileManager.getCache(true) returns DefaultTileLoader.getTiles()
 * when TileCache.getTiles() result is empty.
 */
TEST(TestTileManager, GetTilesCacheTrueRetrievesDefaultTilesWhenCacheIsEmpty) {
    auto mockOnlineLoader = make_shared<MockOnlineLoader>();
    auto mockTileCache = make_shared<MockTileCache>();
    auto mockDefaultTileLoader = make_shared<MockDefaultTileLoader>();
    auto mockTile = make_shared<MockTile>();

    list<shared_ptr<ITile>> tiles, result;
    tiles.push_back(mockTile);

    TileManager tileManager(
        "", mockOnlineLoader, mockTileCache, mockDefaultTileLoader);

    ON_CALL(*mockDefaultTileLoader, getTiles)
        .WillByDefault(Return(tiles));

    EXPECT_CALL(*mockTileCache, getTiles)
        .Times(1);

    EXPECT_CALL(*mockDefaultTileLoader, getTiles)
        .Times(1);

    result = tileManager.getTiles(true);

    EXPECT_EQ(result, tiles);
}

#endif  // TEST_KES_DASHBOARD_LIVE_TILES_CLIENT_TESTTILEMANAGER_H_