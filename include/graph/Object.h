//
// Created by Jens Klimke on 2019-05-12.
//

#ifndef SIMMAP_OBJECT_H
#define SIMMAP_OBJECT_H

namespace graph {

    class Edge;

    /**
     * An interface to define objects
     * @author Jens Klimke jens.klimke@rwth-aachen.de
     */
    struct Object {


        /**
         * Default constructor
         */
        Object() = default;


        /**
         * Default destructor
         */
        virtual ~Object() = default;


        /**
         * Returns the type of the object
         * @return Type of the object
         */
        virtual const std::string &getType() const = 0;


        /**
         * Returns the label of the object
         * @return Label of the object
         */
        virtual const std::string &getLabel() const = 0;


        /**
         * Returns the ID of the object
         * @return ID of the object
         */
        virtual const std::string &getID() const = 0;


        /**
         * Returns the value of the object
         * @return Value of the object
         */
        virtual double getValue() const = 0;


    };


} // namespace graph

#endif //SIMMAP_OBJECT_H
