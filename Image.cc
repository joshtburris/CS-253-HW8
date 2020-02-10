#include "Image.h"
#include "Alpha.h"
#include "PGM.h"
#include "limits.h"
#include <fstream>
#include <sstream>

using namespace std;

Image *Image::create(const string filename) {
    // If filename contains a PGM image, then return a pointer to a dynamically-allocaed PGM object containing the image.
    // If filename contains an Alpha image, then return a pointer to a dynamically-allocaed Alpha object containing the image.
    // Otherwise, that’s a problem. Throws an explanatory std::string upon problems.
    
    string header = filename.substr(filename.find_last_of("."));
    
    if (header == ".alpha") {
        Image *a = new Alpha(filename);
        return a;
    } else if (header == ".pgm") {
        Image *p = new PGM(filename);
        return p;
    } else {
        string s = "File \"" + filename + "\" was an unrecognized file type.\n";
        throw s;
    }
}

Image::Image() {}

Image::Image(const Image &im) : vec(im.vec), filename(im.filename), given_range(im.given_range) {}

Image &Image::operator=(const Image &im) {
    vec = im.vec;
    filename = im.filename;
    given_range = im.given_range;
    return *this;
}

Image::~Image() {}

bool Image::empty() const {
    return vec.empty() && vec[0].empty();
}

int Image::height() const {
    return vec.size();
}

int Image::width() const {
    if (empty())
        return 0;
    return (!vec[0].empty()) ? vec[0].size() : 0;
}

int Image::range() const {
    return given_range + 1;
}

int Image::get(const int column, const int row) const {
    if (column < 0 || column >= width()
        || row < 0 || row >= height()) {
        stringstream ss;
        ss << "get(" << column << "," << row << ") attempted on a " << width() << "x" << height() << " image\n";
        throw ss.str();
    }
    
    string header = filename.substr(filename.find_last_of("."));
    
    if (header == ".alpha") {
        return 'z' - vec[row][column];
    } else if (header == ".pgm") {
        return vec[row][column];
    }
    
    return vec[row][column];
}

void Image::mirror() const {
    if (empty())
        return;
    
    vector<vector<int>> newVec;
    for (unsigned int i = 0; i < vec.size(); ++i) {
        newVec.push_back(vector<int>());
        for (unsigned int j = vec[i].size(); j > 0; --j) {
            newVec[i].push_back(vec[i][j-1]);
        }
    }
    vec.swap(newVec);
}

void Image::rotate(const int degrees) const {
    
    int deg = degrees;
    
    if (deg % 90 != 0) {
        string s = "\"rotate(const int)\" was given an invalid number of degrees ["+to_string(deg)+"]. Degrees must be a positive multiple of 90.\n";
        throw s;
    }
    
    if (empty())
        return;
    
    if (deg < 0) {
        deg = -deg;
        deg *= 3;
    }
    
    while (deg % 360 == 0 && deg != 0) {
        deg -= 360;
    }
    
    if (deg == 0)
        return;
    
    for (int i = 0; i < deg; i += 90) {
        // The actual rotating:
        vector<vector<int>> newVec;
        for (unsigned int col = 0; col < vec[0].size(); ++col) {
            newVec.push_back(vector<int>());
            for (unsigned int row = vec.size(); row > 0; --row) {
                newVec[col].push_back(vec[row-1][col]);
            }
        }
        vec.swap(newVec);
    }
}

void Image::resize(const double factor) const {
    if (factor != 0.5 && factor != 2.0) {
        string s = "\"resize(const double)\" was given an invalid value ["+to_string(factor)+"].\n";
        throw s;
    }
    if (empty())
        return;
    if (factor == 0.5) {
        vector<vector<int>> newVec;
        for (unsigned int row = 0; row < vec.size()-1; row+=2) {
            newVec.push_back(vector<int>());
            for (unsigned int col = 0; col < vec[row].size()-1; col+=2) {
                int one=vec[row][col], two=vec[row][col+1], three=vec[row+1][col], four=vec[row+1][col+1];
                int entry = (one + two + three + four) / 4;
                newVec[row/2].push_back(entry);
            }
        }
        vec.swap(newVec);
    } else if (factor == 2.0) {
        vector<vector<int>> newVec;
        for (unsigned int row = 0; row < vec.size(); ++row) {
            newVec.push_back(vector<int>());
            for (unsigned int col = 0; col < vec[row].size(); ++col) {
                newVec[row*2].push_back(vec[row][col]);
                newVec[row*2].push_back(vec[row][col]);
            }
            newVec.push_back(newVec[row*2]);
        }
        vec.swap(newVec);
    }
}

void Image::min(const Image im2) const {
    min(NW, im2);
}

void Image::min(const int direction, const Image im2) const {
    max_min_helper(true, direction, im2);
}

void Image::max(const Image im2) const {
    max(NW, im2);
}

void Image::max(const int direction, const Image im2) const {
    max_min_helper(false, direction, im2);
}

void Image::max_min_helper(const bool min, const int direction, const Image im2) const {
    
    if (direction != NE && direction != NW && direction != SE && direction != SW) {
        string s = "An unknown direction was given: " + to_string(direction);
        throw s;
    }
    
    // These start at the NW position.
    unsigned int start_col = 0, start_col2 = 0;
    unsigned int end_col = 0, end_col2 = 0;
    unsigned int start_row = 0, start_row2 = 0;
    unsigned int end_row = 0, end_row2 = 0;
        
    if (direction == NW || direction == NE) {
        if (height() <= im2.height()) {
            end_row = height() - 1;
            end_row2 = height() - 1;
        } else {
            end_row = im2.height() - 1;
            end_row2 = im2.height() - 1;
        }
    }
    
    if (direction == NE || direction == SE) {
        if (width() <= im2.width()) {
            start_col = 0;
            start_col2 = im2.width() - width();
        } else {
            start_col = width() - im2.width();
            start_col2 = 0;
        }
        end_col = width() - 1;
        end_col2 = im2.width() - 1;
    }
    
    if (direction == SW || direction == SE) {
        if (height() <= im2.height()) {
            start_row = 0;
            start_row2 = im2.height() - height();
        } else {
            start_row = height() - im2.height();
            start_row2 = 0;
        }
        
        end_row = height() - 1;
        end_row2 = im2.height() - 1;
    }
    
    if (direction == NW || direction == SW) {
        if (width() <= im2.width()) {
            end_col = width() - 1;
            end_col2 = width() - 1;
        } else {
            end_col = im2.width() - 1;
            end_col2 = im2.width() - 1;
        }
    }
    
    /*cout << "im1:\n";
    cout << "height=" << height() << endl;
    cout << "width=" << width() << endl;
    cout << "col=" << start_col << "-" << end_col << endl;
    cout << "row=" << start_row << "-" << end_row << '\n' << endl;
    cout << "im2:\n";
    cout << "height=" << im2.height() << endl;
    cout << "width=" << im2.width() << endl;
    cout << "col=" << start_col2 << "-" << end_col2 << endl;
    cout << "row=" << start_row2 << "-" << end_row2 << '\n' << endl;*/
    
    for (unsigned int row = start_row, row2 = start_row2; row <= end_row && row2 <= end_row2; ++row, ++row2) {
        for (unsigned int col = start_col, col2 = start_col2; col <= end_col && col2 <= end_col2; ++col, ++col2) {
            
            // a/255 is light
            // min didn't work for PGM to Alpha
            // max didn't work for Alpha to Alpha or PGM to Alpha
            // test operator didn't work
            
            // Must scale.
            double brightness1 = 0.0, brightness2 = 0.0;
            if (range() != 1)
                brightness1 = (double)get(col, row) / ((double)range() - 1.0);
            if (im2.range() != 1)
                brightness2 = (double)im2.get(col2, row2) / ((double)im2.range() - 1.0);
            
            /*if (col == 0 && row == 0) {
                cout << "brightness1=" << brightness1*100 << "%" << endl;
                cout << "brightness2=" << brightness2*100 << "%" << endl;
                cout << "(double)im2.get(col2, row2) = " << (double)im2.get(col2, row2) << endl;
                cout << "((double)im2.range() - 1.0) = " << ((double)im2.range() - 1.0) << endl;
            }*/
            
            if ( (min && brightness2 < brightness1) || (!min && brightness2 > brightness1) ) {
                
                int newval = floor(((double)range() - 1.0) * brightness2);
                int aval = (25 - newval) + 97;
                int pval = newval;
                
                string header = filename.substr(filename.find_last_of("."));
                
                if (header == ".alpha") {
                    vec[row][col] = aval;
                } else if (header == ".pgm") {
                    vec[row][col] = pval;
                }
                
                //cout << "Replacing [" << row << "," << col << "] " << vec[row][col] << " with [" << row2 << "," << col2 << "] " << newval << '\n';
            }
            
        }
    }
    
}

bool Image::is_alpha(string filename) {
    return filename.substr(filename.find_last_of(".")) == ".alpha";
}

bool Image::is_pgm(string filename) {
    return filename.substr(filename.find_last_of(".")) == ".pgm";
}

void Image::read(const string filename) const {
    const Alpha *a; const PGM *p;
    if ( (a = dynamic_cast<const Alpha *>(this)) != nullptr ) {
        a->read(filename);
    } else if ( (p = dynamic_cast<const PGM *>(this)) != nullptr ) {
        p->read(filename);
    } else {
        string s = "This image object was an unknown file type.\n";
        throw s;
    }
}

void Image::read(istream &in) const {
    const Alpha *a; const PGM *p;
    if ( (a = dynamic_cast<const Alpha *>(this)) != nullptr ) {
        a->read(in);
    } else if ( (p = dynamic_cast<const PGM *>(this)) != nullptr ) {
        p->read(in);
    } else {
        string s = "This image object was an unknown file type.\n";
        throw s;
    }
}

void Image::write(const string filename) const {
    
    ofstream out(filename);
    
    if (!out) {
        string s = "File \"" + filename + "\" could not be opened.\n";
        throw s;
    }
    
    write(out);
    
    out.close();
    
}

void Image::write(ostream &out) const {
    const Alpha *a; const PGM *p;
    if ( (a = dynamic_cast<const Alpha *>(this)) != nullptr )
        out << *a;
    else if ( (p = dynamic_cast<const PGM *>(this)) != nullptr )
        out << *p;
    else {
        string s = "\"write(ostream &) const\" has failed. This should never happen unless the image was neither an Alpha nor a PGM file.\n";
        throw s;
    }
}

// ***** Operator functions *****

Image &Image::operator-=(const Image im) {
    this->min(im);
    return *this;
}

Image &Image::operator+=(const Image im) {
    this->max(im);
    return *this;
}

Image &Image::operator*=(const double d) {
    this->resize(d);
    return *this;
}

Image &Image::operator/=(const double d) {
    this->resize(1/d);
    return *this;
}

Image::operator bool() const { return !empty(); }

Image::iterator Image::begin() { return iterator(this, 0); }

Image::iterator Image::end() { return iterator(this, height()*width()); }

// ***** Non-member functions *****

ostream &operator<<(ostream &out, const Image &im) {
    const Alpha *a; const PGM *p;
    if ( (a = dynamic_cast<const Alpha *>(&im)) != nullptr )
        a->write(out);
    else if ( (p = dynamic_cast<const PGM *>(&im)) != nullptr )
        p->write(out);
    else {
        string s = "\"write(ostream &) const\" has failed. This should never happen unless the image was neither an Alpha nor a PGM file.\n";
        throw s;
    }
    return out;
}

istream &operator>>(istream &in, const Image &im) {
    try {
        im.read(in);
    } catch (string s) {
        in.setstate(ios::failbit);
    } catch (char const *c) {
        in.setstate(ios::failbit);
    }
    return in;
}

