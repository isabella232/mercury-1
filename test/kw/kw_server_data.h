/**
 * \file kw_server_data.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief Tests for the Mercury::KW - Kano World - functions
 *
 */

#ifndef TEST_KW_SERVER_DATA
#define TEST_KW_SERVER_DATA

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <mercury/kw/kw.h>

using testing::Eq;


TEST(kw, login_get_correct_data)
{
    KanoWorld kw;

    EXPECT_EQ (kw.login("testing_user", "kano12345experience", true), true);
    EXPECT_GT (kw.get_token().length(), 0);
    EXPECT_GT (kw.get_expiration_date().length(), 0);
}

TEST(kw, login_get_no_data)
{
    KanoWorld kw;

    EXPECT_EQ (kw.login("nonexisting", "badpassword", true), false);
    EXPECT_EQ (kw.get_token().length(), 0);
    EXPECT_EQ (kw.get_expiration_date().length(), 0);
}

#endif  // TEST_KW_SERVER_DATA