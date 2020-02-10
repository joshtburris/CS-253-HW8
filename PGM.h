#ifndef PGM_H
#define PGM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Image.h"

class PGM : public Image {
    typedef Image super;
    
public:
    // Default ctor:
    PGM();
    // Filename ctor:
    PGM(const std::string);
    // Copy ctor:
    PGM(const PGM &);
    // Assignment operator:
    PGM &operator=(const PGM &);
    // Destructor:
    ~PGM();
    
    virtual void read(const std::string) const;
    void read(std::istream &) const;
    virtual void write(const std::string) const;
    virtual void write(std::ostream&) const;
    
private:
    void check_consistant_pixel_count() const;
        
};

std::ostream &operator<<(std::ostream &, const PGM &);

#endif
