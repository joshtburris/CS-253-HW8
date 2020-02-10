#include "Alpha.h"
#include "limits.h"
#include <sstream>

using namespace std;

Alpha::Alpha() {}

Alpha::Alpha(const string filename) {
    read(filename);
}

Alpha::Alpha(const Alpha &a) : super(a) {}

Alpha &Alpha::operator=(const Alpha &a) {
    vec = a.vec;
    filename = a.filename;
    return *this;
}

Alpha::~Alpha() {}

// *** Public methods ***

void Alpha::read(const string filename) const {
    Alpha::filename = filename;
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

void Alpha::read(istream &in) const {
    
    string header;
    in >> header;
    
    // Check file name is "Alpha"
    if (header != "Alpha") {
        string s = "Header for file \"" + filename + "\" was an unrecognized type.\n";
        throw header;
    }
    
    Alpha::filename = filename.empty() ? ".alpha" : filename;
    vec.clear();
    
    // Declare a variable to store the current character.
    char cur;
    // Declare a variable to keep track of the row you are on.
    int row = 0;
    bool asterisk_flag = false;
    // Add an empty vector to the 2D vector.
    vec.push_back(vector<int>());
    // A while loop will continually get the next character.
    while (in.get(cur)) {
        // If you hit a new line add a new vector to the vec unless the current row is empty.
        if (cur == '\n') {
            if (!vec[row].empty()) {
                vec.push_back(vector<int>());
                ++row;
                asterisk_flag = false;
            }
        }
        // Else if you hit a letter add the toLower char to the vec.
        else if (isalpha(cur)) {
            vec[row].push_back(tolower(cur));
            asterisk_flag = false;
        }
        // Else if you hit a '#' ignore then skip to the end of the line.
        else if (cur == '#') {
            while (cur != '\n' && !in.eof()) 
                in.get(cur);
            in.putback(cur);
            asterisk_flag = false;
        }
        // Else if an '*' occurs at the end of a line add it to the vec. The '*' must have something before it.
        else if (cur == '*') {
            if (vec[row].empty()) {
                string s = "File \"" + filename + "\" contained an asterisk with no characters before it.\n";
                throw s;
            } else if (isalpha(in.peek())) {
                string s = "Line contains a *, but not at the end.\n";
                throw s;
            } else if (asterisk_flag) {
                string s = "line contains more than one *\n";
                throw s;
            } else {
                vec[row].push_back(cur);
                asterisk_flag = true;
            }
        }
        // Else if cur is any other character (other than space or tab) then give an error. 
        else if (!isblank(cur)) {
            stringstream ss;
            ss << "bad character '" << cur << "'\n";
            throw ss.str();
        }
    }
    vec.pop_back();
    
    // Extend the lines that end with '*' with the previous char to the same length as all the other lines.
    extend_asterisk_lines();
    
    // Check all lines have the same number of pixels.
    check_consistant_pixel_count();
    
    in.clear();
    
}

void Alpha::write(const string filename) const {
    
    ofstream out(filename);
    
    if (!out) {
        string s = "File \"" + filename + "\" could not be opened.\n";
        throw s;
    }
    
    write(out);
    
    out.close();
    
}

void Alpha::write(ostream &out) const {
    out << *this;
}

// *** Private methods ***

void Alpha::extend_asterisk_lines() const {
    if (empty())
        return;
    
    unsigned int max = 0;
	for (auto const &val : vec) {
		if (val.size() > max)
			max = val.size();
	}
	
	unsigned int num_asterisks = 0;
    //#pragma omp parallel for
    for (unsigned int i = 0; i < vec.size(); ++i) {
		for (unsigned int j = 0; j < vec[i].size(); ++j) {
			if (vec[i][j] == '*') {
				num_asterisks++;
                if (j == 0) {
                    string s = "File \"" + filename + "\" contained an asterisk at the start of a line.\n";
                    throw s;
                }
                char prevChar = vec[i][j-1];
                vec[i].pop_back();
                int iters = max - vec[i].size();
                for (int k = 0; k < iters; ++k) {
                    vec[i].push_back(prevChar);
                }
			}
		}
	}
	
	if (num_asterisks == vec.size() && num_asterisks != 0) {
		string s = "File \"" + filename + "\" contained too many asterisks.\n";
		throw s;
	}
}

void Alpha::check_consistant_pixel_count() const {
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

ostream &operator<<(ostream &out, const Alpha &a) {
    stringstream s;
    s << "Alpha\n";
    for (int row = 0; row < a.height(); ++row) {
		for (int col = 0; col < a.width(); ++col) {
            char c = 'z' - a.get(col, row);
			s << c;
		}
		s << "\n";
	}
    return out << s.str();
}
