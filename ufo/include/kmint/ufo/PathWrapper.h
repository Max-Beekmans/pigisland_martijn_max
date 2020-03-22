//
// Created by Max on 3/14/20.
//

#ifndef UFO_MARTIJN_MAX_PATHWRAPPER_H
#define UFO_MARTIJN_MAX_PATHWRAPPER_H

#include "NodeWrapper.h"

namespace kmint::ufo {
    //! Holds RAII over the path for actor
    class PathWrapper {
    public:
        explicit PathWrapper(std::vector<NodeWrapper*> path): _path(std::move(path)), _index(_path.size()) {}
        PathWrapper(): _path(), _index(0) {}
        //! call deletePath!
        ~PathWrapper() {}
        //! You want the next node in the path so you pop on of the front
        //! However under the hood it's inverted since it's better for performance.
        //! The _path is filled from goal to begin-1
        NodeWrapper* popFront() {
            NodeWrapper* tmp = _path.back();
            _path.pop_back();
            _index--;
            if(_index < 1) {
                reachedEnd_ = true;
            }
            return tmp;
        }
        //! If the index smaller than 1 we've moved to the last node
        bool reachedEnd() {
            return reachedEnd_;
        }
        //! If vector is empty
        bool isEmpty() {
            return _path.empty();
        }
        //! Delete ptrs of path
        void deletePath() {
            for(auto ptr : _path) {
                delete ptr;
            }
        }
    private:
        std::vector<NodeWrapper*> _path;
        std::size_t _index;
        bool reachedEnd_ = false;
    };
}
#endif //UFO_MARTIJN_MAX_PATHWRAPPER_H
