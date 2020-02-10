// Image must not have:                 
    // any public constructors.
    // a public assignment operator.
//Const-correctness, both arguments & methods, is your job. For example, it must be possible to call .width() on a const object, or to pass a const string to .read().

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

class image_iter; // Forward declaration

class Image {
    
protected:
    mutable std::vector<std::vector<int>> vec;
    mutable std::string filename;
    mutable int given_range=25;
    
    Image &operator=(const Image &);
    void max_min_helper(const bool, const int, const Image) const;
    bool is_alpha(std::string);
    bool is_pgm(std::string);
    
public:
    typedef image_iter iterator;
    static const int NW=0, NE=1, SW=2, SE=3;
    
    static Image *create(const std::string);
    Image();
    Image(const Image &);
    virtual ~Image();
    
    bool empty() const;
    int height() const;
    int width() const;
    int range() const;
    int get(const int, const int) const;
    
    void mirror() const;
    void rotate(const int) const;
    void resize(const double) const;
    
    void min(const Image) const;
    void min(const int, const Image) const;
    void max(const Image) const;
    void max(const int, const Image) const;
    
    virtual void read(const std::string) const;
    void read(std::istream &) const;
    virtual void write(const std::string) const;
    virtual void write(std::ostream&) const;
    
    Image &operator-=(const Image);
    Image &operator+=(const Image);
    Image &operator*=(const double);
    Image &operator/=(const double);    
    
    operator bool() const;
    
    iterator begin();
    iterator end();
        
};

std::ostream &operator<<(std::ostream &, const Image &);
std::istream &operator>>(std::istream &, const Image &);

class image_iter {
    typedef std::string string;
    
public:
    image_iter(Image *par=nullptr, int i=0) : parent(par), index(i) {}
    
    image_iter(const image_iter &it) : parent(it.parent), index(it.index) {}
    
    image_iter &operator=(const image_iter &it) {
        parent = it.parent;
        index = it.index;
        return *this;
    }
    
    image_iter &operator++() {
        if (!parent)
            throw string("Attempt to increment iterator without parent");
        if (index == (parent->height() * parent->width()))
            throw string("Attempt to increment end() iterator");
        ++index;
        return *this;
    }
    
    image_iter operator++(int) {
        const auto save = *this;
        ++*this;
        return save;
    }
    
    image_iter &operator--() {
        if (!parent)
            throw string("Attempt to increment iterator without parent");
        if ( index == ((parent->height() * parent->width()) + 1) )
            throw string("Attempt to increment end() iterator");
        --index;
        return *this;
    }
    
    image_iter operator--(int) {
        const auto save = *this;
        --*this;
        return save;
    }
    
    int operator*() const {
        if (!parent)
            throw string("Attempt to indirect uninitialized iterator");
        if (index == (parent->height() * parent->width()))
            throw string("Attempt to indirect end() iterator");
        return (*parent).get( index%parent->width(), floor(index/parent->width()) );
    }
    
    bool operator==(const image_iter &it) const {
        if (!parent || !it.parent)
            throw string("Attempt to compare uninitialized iterator");
        if (parent != it.parent)
            throw string("Attempt to compare iterators from different containers");
        return index == it.index;
    }
    
    bool operator!=(const image_iter &it) const {
        return !(*this == it);
    }
    
private:
    Image *parent;
    int index;
    
};

#endif /* IMAGE_H */
