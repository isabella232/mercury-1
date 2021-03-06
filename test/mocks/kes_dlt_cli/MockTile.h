/**
 * \file MockTile.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief TODO
 */


#ifndef TEST_MOCKS_KES_DLT_CLI_MOCKTILE_H_
#define TEST_MOCKS_KES_DLT_CLI_MOCKTILE_H_


#include <gmock/gmock.h>
#include <parson.h>

#include <string>

#include "kes_dashboard_live_tiles_client/ITile.h"


namespace KESDLTC {
namespace test {

class MockTile : public KESDLTC::ITile {
 public:  // ISerialisable Methods.
    MOCK_METHOD1(initialise, bool(JSON_Value* serialisedData));
    MOCK_CONST_METHOD0(serialise, JSON_Value*());

 public:  // IDownloadable Methods.
    MOCK_METHOD1(download, bool(const std::string& baseDir));

 public:  // ITile Methods.
    MOCK_CONST_METHOD0(getId, std::string());
    MOCK_CONST_METHOD0(getCover, std::string());
    MOCK_CONST_METHOD0(getTitle, std::string());
    MOCK_CONST_METHOD0(getDescription, std::string());
    MOCK_CONST_METHOD0(getUsername, std::string());
    MOCK_CONST_METHOD0(getApp, std::string());
    MOCK_CONST_METHOD0(getOpenUrl, std::string());
    MOCK_CONST_METHOD0(getFallbackUrl, std::string());
    MOCK_CONST_METHOD0(getCoverPath, std::string());
};

}  // namespace test
}  // namespace KESDLTC

#endif  // TEST_MOCKS_KES_DLT_CLI_MOCKTILE_H_
