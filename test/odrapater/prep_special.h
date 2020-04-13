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
// Created by Jens Klimke on 2019/02/19
//

#ifndef SIMMAP_TESTS_PREP_SPECIAL_H
#define SIMMAP_TESTS_PREP_SPECIAL_H

#include <odradapter/LaneSectionSequence.h>

void addLanes(LaneSection::LanesVector *lns, const std::vector<double> &widths) {

    // add lanes
    for (auto &w : widths)
        lns->emplace_back(new ODREdge);

}

void generateComplexLaneSections(LaneSectionSequence *lanes) {

    // ONLY USE FOR TESTING ODR 1.4

    // create first lane section (both sides)
    auto ls = lanes->create(0.0, base::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // create lane section (both sides)
    ls = lanes->create(10.0, base::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // create lane sections (right)
    ls = lanes->create(25.0, base::Side::RIGHT);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});

    // create lane section (left)
    ls = lanes->create(25.0, base::Side::LEFT);
    addLanes(&ls->left(), {3.75});

    ls = lanes->create(30.0, base::Side::LEFT);
    addLanes(&ls->left(), {3.75, 3.75});

    ls = lanes->create(60.0, base::Side::LEFT);
    addLanes(&ls->left(), {3.75, 3.75});

    // create lane sections (right)
    ls = lanes->create(75.0, base::Side::RIGHT);
    addLanes(&ls->right(), {3.75});

    // create lane section (left)
    ls = lanes->create(75.0, base::Side::LEFT);
    ls->center(new ODREdge);
    addLanes(&ls->left(), {3.75, 3.75});

    // create lane section (both sides)
    ls = lanes->create(90.0, base::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // index
    lanes->length(100.0);
    lanes->index(true);

}

#endif // SIMMAP_TESTS_PREP_SPECIAL_H
