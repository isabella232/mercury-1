/**
 * \file DefaultTileLoader.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#ifndef INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_DEFAULTTILELOADER_H_
#define INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_DEFAULTTILELOADER_H_

#include <list>
#include <memory>
#include <string>

#include "kes_dashboard_live_tiles_client/ITile.h"
#include "kes_dashboard_live_tiles_client/ITileFactory.h"
#include "kes_dashboard_live_tiles_client/ITileLoader.h"
#include "kes_dashboard_live_tiles_client/TileFactory.h"

using std::list;
using std::make_shared;
using std::shared_ptr;
using std::string;


namespace KESDLTC {
namespace internal {

class DefaultTileLoader : public ITileLoader {
 public:  // Constructors & destructors.
    DefaultTileLoader(
        const shared_ptr<ITileFactory> tileFactory =
            make_shared<TileFactory>());
    ~DefaultTileLoader();

 public:  // ITileLoader Methods.
    list<shared_ptr<ITile>> getTiles() override;

 private:  // Members.
    const shared_ptr<ITileFactory> tileFactory;
};

}  // namespace internal
}  // namespace KESDLTC


#endif  // INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_DEFAULTTILELOADER_H_
