#pragma once

#include <boost/container/small_vector.hpp>

class PixelContainer {
protected:
    typedef boost::container::small_vector<unsigned,16777216ul> Container_t;
    Container_t container;
    unsigned w = 0, h = 0;
public:
    unsigned width (void) const noexcept { return w; }
    unsigned height(void) const noexcept { return h; }

    void ClearAndResize(unsigned const argw, unsigned const argh)
    {
        w = argw;
        h = argh;
        this->container.resize(argw * argh);
    }

    class SquareBracketClass {
        unsigned *const p;
    public:
        SquareBracketClass(unsigned *const argp) noexcept : p(argp) {}
        unsigned &operator[](int const y) noexcept
        {
            return this->p[y];
        }
    };

    SquareBracketClass operator[](unsigned const x) noexcept
    {
        assert( x < this->w );
        assert( (x * this->h) < this->container.size() );
        return SquareBracketClass( &this->container.front() + x * this->h );
    }
};
