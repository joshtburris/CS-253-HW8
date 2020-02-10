#include "PGM.h"
#include "limits.h"
#include <sstream>

using namespace std;

PGM::PGM() {}

PGM::PGM(const string filename) {
    read(filename);
}

PGM::PGM(const PGM &p) : super(p) {}

PGM &PGM::operator=(const PGM &p) {
    vec = p.vec;
    filename = p.filename;
    return *this;
}

PGM::~PGM() {}

// *** Public methods ***

void PGM::read(const string filename) const {
    PGM::filename = filename;
    vec.clear();
    
    filebuf fb;
    if (fb.open(filename, ios::in)) {
        istream in(&fb);
        read(in);
        fb.close();
    } else {
        string s = "File \"" + filename + "\" could not be opened.\n";
        throw s;
    }
}

void PGM::read(istream &in) const {
    
    string header;
    in >> header;
    
    if (header != "P2") {
        string s = "Header for file \"" + filename + "\" was an unrecognized type.\n";
        throw s;
    }
    
    PGM::filename = filename.empty() ? ".pgm" : filename;
    vec.clear();
    
    char cur;
    int row = 0;
    int width, height, range, calculated_range=0;
    vec.push_back(vector<int>());
    bool width_flag=true, height_flag=true, range_flag=true;
    while (in.get(cur)) {
        if (cur == '\n') {
            // Purposefully do nothing.
        }
        else if (cur == '#') {
            while (cur != '\n' && !in.eof()) 
                in.get(cur);
            in.putback(cur);
        }
        else if (isdigit(cur)) {
            in.putback(cur);
            if (width_flag) {
                in >> width;
                width_flag = false;
            }
            else if (height_flag) {
                in >> height;
                height_flag = false;
            }
            else if (range_flag) {
                in >> range;
                range_flag = false;
            }
            else {
                int tmp;
                in >> tmp;
                vec[row].push_back(tmp);
                if (tmp > calculated_range)
                    calculated_range = tmp;
            }
        }
        else if (!isblank(cur)) {
            string s = "File \"" + filename + "\" contained an invalid character \"" + cur + "\"\n";
            throw s;
        }
        if (width != 0 && vec[row].size() == static_cast<unsigned int>(width)) {
            vec.push_back(vector<int>());
            ++row;
        }
    }
    vec.pop_back();
    
    // Check all lines have the same number of pixels.
    check_consistant_pixel_count();
    
    if (PGM::height() != height || PGM::width() != width || range < calculated_range) {
        string s = "Too many pixels available in " + filename;
        throw s;
    }
    
    PGM::given_range = range;
    
    in.clear();
    
}

void PGM::write(const string filename) const {
    
    ofstream out(filename);
    
    if (!out) {
        string s = "File \"" + filename + "\" could not be opened.\n";
        throw s;
    }
    
    write(out);
    
    out.close();
    
}

void PGM::write(ostream &out) const {
    out << *this;
}

// *** Private methods ***

void PGM::check_consistant_pixel_count() const {
    if (empty())
        return;
    
    unsigned int pixels = vec[0].size();
    for (auto const &val : vec) {
        if (val.size() != pixels) {
            string s = "File \"" + filename + "\" had an inconsistent pixel count for 1 or more lines.\n";
            throw s;
        }
    }
}

// *** Non-member functions ***

ostream &operator<<(ostream &out, const PGM &p) {
    stringstream ss;
    ss << "P2\n" << p.width() << ' ' << p.height() << '\n' << (p.range()-1) << '\n';
    for (int row = 0; row < p.height(); ++row) {
		for (int col = 0; col < p.width(); ++col) {
			ss << to_string(p.get(col, row)) << ' ';
		}
		ss << '\n';
	}
    return out << ss.str();
}



