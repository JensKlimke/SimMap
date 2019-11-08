//
// Created by Jens Klimke on 2019-05-01.
//

#ifndef SIMMAP_ODR_ODRADAPTER_H
#define SIMMAP_ODR_ODRADAPTER_H

#include <string>
#include <map>
#include <memory>

#include <odr/lib.h>
#include <server/Map.h>


namespace simmap {
namespace odra {

    class ODRAdapter : public simmap::server::Map {

    public:


        ODRAdapter() = default;
        ~ODRAdapter() override = default;


        /**
         * Loads the xodr file
         * @param filename Name of the file
         */
        void loadFile(const std::string &filename);


        /**
         * Stream the content into the out-stream
         * @param os Out-stream
         * @return Out-stream
         */
        std::ostream &streamTo(std::ostream &os) const;

    };


}}


inline std::ostream &operator<< (std::ostream &os, const simmap::odra::ODRAdapter &ad) {

    return ad.streamTo(os);

}

#endif //SIMMAP_ODR_ODRADAPTER_H
