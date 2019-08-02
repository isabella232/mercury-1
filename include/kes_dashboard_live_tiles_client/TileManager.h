/**
 * \file TileManager.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#ifndef INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_TILEMANAGER_H_
#define INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_TILEMANAGER_H_


#include <stdlib.h>

#include <list>
#include <memory>
#include <string>

#include "kes_dashboard_live_tiles_client/IOnlineLoader.h"
#include "kes_dashboard_live_tiles_client/ITile.h"
#include "kes_dashboard_live_tiles_client/ITileCache.h"
#include "kes_dashboard_live_tiles_client/ITileLoader.h"
#include "kes_dashboard_live_tiles_client/ITileManager.h"

using std::list;
using std::string;
using std::shared_ptr;


namespace KESDLTC {

class TileManager : public ITileManager {
 public:  // Constructors & destructors.
    TileManager(
        const string& cacheDir = "",
        const shared_ptr<internal::IOnlineLoader> onlineLoader = nullptr,
        const shared_ptr<internal::ITileCache> tileCache = nullptr,
        const shared_ptr<internal::ITileLoader> defaultTileLoader = nullptr);
    ~TileManager();

 public:  // ITileManager Methods.
    list<shared_ptr<ITile>> getTiles(bool cache = true) const override;

 private:  // Methods.
    bool isCooldown() const;

 private:  // Members.
    string cacheDir;
    shared_ptr<internal::IOnlineLoader> onlineLoader;
    shared_ptr<internal::ITileCache> tileCache;
    shared_ptr<internal::ITileLoader> defaultTileLoader;

 private:  // Constants.
    const string CACHE_DIRNAME = ".kes-dlt-cli";
};

}  // namespace KESDLTC


#endif  // INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_TILEMANAGER_H_