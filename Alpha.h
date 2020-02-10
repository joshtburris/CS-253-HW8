#ifndef ALPHA_H
#define ALPHA_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Image.h"

class Alpha : public Image {
    typedef Image super;
    
public:
    // Default ctor:
    Alpha();
    // Filename ctor:
    Alpha(const std::string);
    // Copy ctor:
    Alpha(const Alpha &);
    // Assignment operator:
    Alpha &operator=(const Alpha &);
    // Destructor:
    ~Alpha();
    
    virtual void read(const std::string) const;
    void read(std::istream &) const;
    virtual void write(const std::string) const;
    virtual void write(std::ostream&) const;
        
private:
    void extend_asterisk_lines() const;
    void check_consistant_pixel_count() const;
            
};

std::ostream &operator<<(std::ostream &, const Alpha &);

#endif
