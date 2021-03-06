/**
 * \file ITileLoader.h
 *
 * \copyright Copyright (C) 2019 Kano Computing Ltd.
 *            License: http://www.gnu.org/licenses/gpl-2.0.txt GNU GPL v2
 *
 * \brief     TODO
 */


#ifndef INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_INTERNAL_ITILELOADER_H_
#define INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_INTERNAL_ITILELOADER_H_


#include <list>
#include <memory>

#include "kes_dashboard_live_tiles_client/ITile.h"


namespace KESDLTC {
namespace internal {

class ITileLoader {
 public:  // Constructors & destructors.
    virtual ~ITileLoader() {}

 public:  // Methods.
    virtual std::list<std::shared_ptr<ITile>> getTiles() = 0;
};

}  // namespace internal
}  // namespace KESDLTC


#endif  // INCLUDE_KES_DASHBOARD_LIVE_TILES_CLIENT_INTERNAL_ITILELOADER_H_
