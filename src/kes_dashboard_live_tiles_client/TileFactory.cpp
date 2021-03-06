/**
 * \file TileFactory.cpp
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#include <memory>
#include <string>

#include "kes_dashboard_live_tiles_client/ITile.h"
#include "kes_dashboard_live_tiles_client/Tile.h"
#include "kes_dashboard_live_tiles_client/TileFactory.h"
#include "mercury/http/http_client.h"
#include "mercury/http/http_client_interface.h"

using std::make_shared;
using std::shared_ptr;
using std::string;

using KESDLTC::ITile;
using KESDLTC::Tile;
using KESDLTC::TileFactory;

using Mercury::HTTP::IHTTPClient;
using Mercury::HTTP::HTTPClient;


shared_ptr<ITile> TileFactory::create() const {
    return make_shared<Tile>();
}


shared_ptr<ITile> TileFactory::create(
        const string& id, const string& cover, const string& title,
        const string& description, const string& username, const string& app,
        const string& openUrl, const string& fallbackUrl,
        const string& coverPath,
        const shared_ptr<IHTTPClient>& httpClient) const {
    return make_shared<Tile>(
        id, cover, title, description, username, app, openUrl, fallbackUrl,
        coverPath, httpClient);
}
