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
