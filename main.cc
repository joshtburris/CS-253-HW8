#include <iostream>
#include <fstream>
#include <vector>
#include <ctype.h>
#include "Alpha.h"
#include "PGM.h"
#include "Image.h"
#include <cassert>
#include <string>
#include <memory>

using namespace std;

void printingPixels(string dir) {
    (void)dir;
    try {
        
        string filename = "/s/bach/a/class/cs253/pub/input_images/hi.alpha ";
        
        auto a = Image::create(filename);
		Image::iterator it_begin = a->begin();
		Image::iterator it_end = a->end();
		cout <<"First"<<" pixel:: " << *it_begin << endl;
		cout <<"Last"<<" pixel:: " << *--it_end<< endl;
        
    
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
}

void inequalityOperator(string dir) {
    try {
        
        string filename = dir + "simple.alpha";
        
        auto a = Image::create(filename);
        auto b = Image::create(filename);

        Image::iterator it_a = a->begin();
        Image::iterator it_b = b->begin();
        Image::iterator it_c = a->end();
        if(*it_a == *it_b)
            cout<<"They are equal"<<endl;  
        if(*it_a != *--it_c)
            cout<<"They are inequal"<<endl;
        
    
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
}

void readTest(string dir) {
    (void)dir;
    try {
        
        string filename = "/s/bach/a/class/cs253/pub/input_images/hi.pgm";
        //   /s/bach/a/class/cs253/pub/input_images/hi.alpha
        
        Image *im = Image::create(filename);
		cin >> *im;
		im->write(cout);
        
    
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
}

int main () {
    cout << "\n";
    string directory = "test_files/";
    
    cout << "TESTING: Empty files\n";
    try {
        string filename = directory + "empty.alpha";
        Alpha a(filename);
        if (a.empty())
            cout << "\t\"" << filename << "\" is empty.\n";
        else {
            string s = "\"" + filename + "\" was not empty.\n";
            throw s;
        }
        cout << "\theight=" << a.height() << endl;
        cout << "\twidth=" << a.width() << endl;
        cout << "\trange=" << a.range() << endl;
        cout << "\tmirrored\n";
        cout << "\trotate\n";
        cout << "\tresized\n";
        cout << "\tWe're going to get an error now by trying to access a.get(0,0).\n";
        int i = a.get(0, 0);
        cout << "\tget=" << i << endl;
        cout << "\tThis should never appear.\n";
    } catch (string e) {
        cout << "\tERROR: "+e << endl;
    }
    try {
        string filename = directory + "empty.pgm";
        PGM p(filename);
        if (p.empty())
            cout << "\t\"" << filename << "\" is empty.\n";
        else {
            string s = "\"" + filename + "\" was not empty.\n";
            throw s;
        }
        for (auto v : p) {
            cout << v << ' ';
        } cout << '\n';
        cout << "\theight=" << p.height() << endl;
        cout << "\twidth=" << p.width() << endl;
        cout << "\trange=" << p.range() << endl;
        cout << "\tmirrored\n";
        cout << "\trotate\n";
        cout << "\tresized\n";
        cout << "\tWe're going to get an error now by trying to access p.get(0,0).\n";
        int i = p.get(0, 0);
        cout << "\tget=" << i << endl;
        cout << "\tThis should never appear.\n";
    } catch (string e) {
        cout << "\tERROR: "+e << endl;
    }
    cout << "\n";
    
    
    cout << "TESTING: Transform methods\n";
    try {
        
        Alpha p(directory + "simple.alpha");
        for (auto v : p) {
            cout << v << ' ';
        } cout << '\n';
        cout << "\tOriginal:\n" << p;
        p.mirror();
        cout << "\tMirror:\n" << p;
        p.rotate(90);
        cout << "\tRotate(90):\n" << p;
        p.resize(2.0);
        cout << "\tResize(2.0):\n" << p;
        p.resize(0.5);
        cout << "\tResize(0.5):\n" << p;
        
        cout << "\tNo errors!\n";
    } catch (string e) {
        cout << "\tERROR: "+e << endl;
    }
    cout << "\n";
    
    
    cout << "TESTING: const-ness\n";
    try {
        
        const string filename = directory + "simple.alpha";
        const Alpha a(filename);
        cout << "\t\"" << filename << "\" is " << a.width() << "×" << a.height() << '\n';
        const auto a1(a);
        
        const string pub = "test_files/feep.pgm";
        const PGM p;
        p.read(pub);
        cout << "\t\"" << pub << "\" is " << p.width() << "×" << p.height() << '\n';
        const int i = 1, r = 180;
        const double d = 2.0;
        cout << "\tp.get(1,1)=" << p.get(i, i) << endl;
        p.rotate(r);
        p.resize(d);
        const auto p1(p);
        p1.write(cout);
        
        
        cout << "\tNo errors!\n";
    } catch (string e) {
        cout << "\tERROR: "+e << endl;
    }
    cout << "\n";

    
    cout << "TESTING: Image::create\n";
    try {
        const string pub = "/s/bach/a/class/cs253/pub/";
        Image *im = Image::create(pub+"luna.pgm");
        cout << "\tLuna is " << im->width() << "×" << im->height() << '\n';
        im->resize(0.5);
        cout << "\tNow, Luna is " << im->width() << "×" << im->height() << '\n';
        assert(dynamic_cast<Alpha *>(im) == nullptr);
        assert(dynamic_cast<PGM *>(im) != nullptr);
        delete im;
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
    cout << "\n";
    
    
    cout << "TESTING: Image::min & Image::max\n";
    try {
        Image *i = Image::create(directory + "p3.alpha");
        Image *i2 = Image::create(directory + "p4.alpha");
        
        i->min(*i2);
        
        i->write(cout);
        
        delete i;
        delete i2;
        
        
        auto a = Image::create(directory + "p3.alpha");
        const Image *b = Image::create(directory + "p4.alpha");
        if (*a)
            a->write(cout);
        b->write(cout);
        cout << Image::SE << endl;
        a->min(Image::SE, *b);
        a->write(cout);
        delete a;
        delete b;
        
        
        const string pub = "/s/bach/a/class/cs253/pub/";
        unique_ptr<Image> luna(Image::create(pub+"luna.pgm"));
        unique_ptr<Image> checks(Image::create(pub+"checkerboard.alpha"));
        for (int i=0; i<6; i++)
            *checks *= 2.0;
        *luna += *checks;
        luna->write(directory + "lunaChecks.pgm");
        
        
        cout << "\tmin: alpha to alpha" << endl;
        Image* im1 = Image::create(directory + "simple.alpha");
		Image* im2 = Image::create(directory + "hi.alpha");
        
        *im1 -= *im2;
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::NE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::NW,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::SE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::SW,*im2);
		im1->write(cout);
        cout << '\n';
        
        cout << "\tmin: pgm to alpha\n";
        im1 = Image::create(directory + "simple.pgm");
		im2 = Image::create(directory + "hi.alpha");
        
        *im1 -= *im2;
		im1->write(cout);
        for (auto v : *im1) {
            cout << v << ' ';
        } cout << '\n';
        cout << '\n';
        
		im1 = Image::create(directory + "simple.pgm");
		im1->min(Image::NE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.pgm");
		im1->min(Image::NW,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.pgm");
		im1->min(Image::SE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.pgm");
		im1->min(Image::SW,*im2);
		im1->write(cout);
        cout << '\n';
        
        
        cout << "\tmax: alpha to alpha" << endl;
        im1 = Image::create(directory + "simple.alpha");
		im2 = Image::create(directory + "hi.alpha");
        
        *im1 += *im2;
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->max(Image::NE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->max(Image::NW,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->max(Image::SE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->max(Image::SW,*im2);
		im1->write(cout);
        cout << '\n';
        
        
        cout << "\tmin: alpha to pgm" << endl;
        im1 = Image::create(directory + "simple.alpha");
		im2 = Image::create(directory + "hi.pgm");
        
        *im1 -= *im2;
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::NE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::NW,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::SE,*im2);
		im1->write(cout);
        cout << '\n';
        
		im1 = Image::create(directory + "simple.alpha");
		im1->min(Image::SW,*im2);
		im1->write(cout);
        cout << '\n';
        
        
        cout << "\tNo errors!\n";
    } catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
    cout << "\n";
    
    
    cout << "TESTING: Iterator\n";
    try {
        
        auto a = Image::create("/s/bach/a/class/cs253/pub/luna.alpha");
        const Image::iterator it = a->begin();
        cout << "\tFirst luna pixel: " << *it << '\n';
        ifstream in("/s/bach/a/class/cs253/pub/tic-tac-toe.alpha");
        in.clear();
        if ((in >> *a).fail()) {
        	cerr << "Can’t read tic-tac-toe.alpha\n";
        	delete a;
        	return 1;
        }
        cout << *a;
        for (int pixel : *a)
            cout << pixel << ' ';
        cout << '\n';
        delete a;
        
        
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
    cout << "\n";
    
    
    cout << "TESTING: empty_bad.pgm\n";
    try {
        string filename = directory + "empty_bad.pgm";
        
        Image *im = Image::create(filename);
		if (*im)
			cout << filename.substr(39, filename.length()-1) << ":: " << "Not-Empty" << '\n';
		else
			cout << filename.substr(39, filename.length()-1) << ":: " << "Empty" << '\n';
        
    
        cout << "\tNo errors!\n";
    }
    catch (string err) {
        cerr << "\tERROR: " << err << '\n';
    }
    cout << "\n";
    
    
    cout << "TESTING: Printing pixels\n";
    printingPixels(directory);
    cout << "\n";
    
    
    cout << "TESTING: Inequality operator\n";
    inequalityOperator(directory);
    cout << "\n";
    
    
    cout << "TESTING: Read test\n";
    //readTest(directory);
    cout << "\n";
    
    
	return 0;
}





