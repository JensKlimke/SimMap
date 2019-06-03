//
// Created by klimke on 19.02.2019.
//

#ifndef SIMMAP_TESTS_PREP_SPECIAL_H
#define SIMMAP_TESTS_PREP_SPECIAL_H

#include <LaneSectionSequence.h>

void addLanes(LaneSection::LanesVector *lns, const std::vector<double> &widths) {

    // add lanes
    for (auto &w : widths)
        lns->emplace_back(new ODREdge);

}

void generateComplexLaneSections(LaneSectionSequence *lanes) {

    // ONLY USE FOR TESTING ODR 1.4

    // create first lane section (both sides)
    auto ls = lanes->create(0.0, def::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // create lane section (both sides)
    ls = lanes->create(10.0, def::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // create lane sections (right)
    ls = lanes->create(25.0, def::Side::RIGHT);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});

    // create lane section (left)
    ls = lanes->create(25.0, def::Side::LEFT);
    addLanes(&ls->left(), {3.75});

    ls = lanes->create(30.0, def::Side::LEFT);
    addLanes(&ls->left(), {3.75, 3.75});

    ls = lanes->create(60.0, def::Side::LEFT);
    addLanes(&ls->left(), {3.75, 3.75});

    // create lane sections (right)
    ls = lanes->create(75.0, def::Side::RIGHT);
    addLanes(&ls->right(), {3.75});

    // create lane section (left)
    ls = lanes->create(75.0, def::Side::LEFT);
    ls->center(new ODREdge);
    addLanes(&ls->left(), {3.75, 3.75});

    // create lane section (both sides)
    ls = lanes->create(90.0, def::Side::BOTH);
    ls->center(new ODREdge);
    addLanes(&ls->right(), {3.75});
    addLanes(&ls->left(), {3.75});

    // index
    lanes->length(100.0);
    lanes->index(true);

}

#endif // SIMMAP_TESTS_PREP_SPECIAL_H
