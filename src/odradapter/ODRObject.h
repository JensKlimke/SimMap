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
// Created by Jens Klimke on 2019-05-12.
//

#ifndef SIMMAP_ODROBJECT_H
#define SIMMAP_ODROBJECT_H

#include <graph/Object.h>

struct ODRObject : public graph::Object {};


class ODROrigObject : public ODRObject {

public:

    std::string _id;
    std::string _type;
    std::string _label;
    double _value = 0.0;

    ODROrigObject() = default;
    ~ODROrigObject() override = default;

    const std::string &getType() const override {

        return _type;

    }


    const std::string &getLabel() const override {

        return _label;

    }


    const std::string &getID() const override {

        return _id;

    }


    double getValue() const override {

        return _value;

    }


};


struct ODRRefObject : public ODRObject {

    const ODRObject *_ref = nullptr;


    ODRRefObject() = default;
    ~ODRRefObject() override = default;

    const std::string &getType() const override {

        return _ref->getType();

    }


    const std::string &getLabel() const override {

        return _ref->getLabel();

    }


    const std::string &getID() const override {

        return _ref->getID();

    }


    double getValue() const override {

        return _ref->getValue();

    }

};


#endif //SIMMAP_ODROBJECT_H
