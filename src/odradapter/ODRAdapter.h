//
// Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
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
