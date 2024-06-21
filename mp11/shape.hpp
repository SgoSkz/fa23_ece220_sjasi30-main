/*
 * Author: Sean Jasin (sjasi3)
 * UIN: 672969187
 * Date: Some time in November
 * Description:
 *  The purpose of this code is to demonstrate knowledge in C++, a language
 *  which can be both functional and class based. This file contains multiple
 *  classes which create shapes and have methods to return their fields and
 *  calculate certain properties of the shape
 */
#ifndef SHAPE_H_
#define SHAPE_H_

#include <cstdio>
#include<iostream>
#include<cmath>
#include <math.h>
#include<string>
#include<algorithm>
#include<list>
#include<fstream>
#include <iomanip>    

using namespace std;

//Base class
//Please implement Shape's member functions
//constructor, getName()
class Shape{
public:
	//Base class' constructor should be called in derived classes'
	//constructor to initizlize Shape's private variable 
  	Shape(string name) {
        this->name_ = name;                         // Store name into name_ field
	}
	
  	string getName() {
        return this->name_;                         // Return the name_ field
	}
	
  	virtual double getArea() const = 0;
  	virtual double getVolume() const = 0;
	
private:
  string name_;


};

//Rectangle
//Please implement the member functions of Rectangle:
//constructor, getArea(), getVolume(), operator+, operator-

template <class T>
class Rectangle : public Shape{
public:
  	Rectangle<T>(T width = 0, T length = 0):Shape("Rectangle") {
        this->width_ = (width < 0) ? 0 : width;                 // If the width is less than zero,
                                                                // use 0, else use width
                                                                // store into width_ field
        this->length_ = (length < 0) ? 0 : length;              // If the length is less than zero,
                                                                // use 0, else use length
                                                                // store into length_ field
	}
	
  	double getArea() const {
        return (double)this->width_ * (double)this->length_;    // Calculate area and return
	}
	
  	double getVolume() const {
        return 0;                                               // Vol for 2D shape is 0
	}
	
	Rectangle<T> operator + (const Rectangle<T>& rec) {
        // Return a new rectangle with the added components
        return Rectangle<T>(this->width_ + rec.getWidth(), this->length_ + rec.getLength());
	}
	
	Rectangle<T> operator - (const Rectangle<T>& rec) {
        // Return a new rectangle with the subtracted components
        return Rectangle<T>(this->width_ - rec.getWidth(), this->length_ - rec.getLength());
	} 
	
	T getWidth() const { 
        return this->width_;                                    // Return width_ field
	}
	
	T getLength() const { 
        return this->length_;                                   // Return length_ field
	}
private:
	T width_;
    T length_;

};

//Circle
//Please implement the member functions of Circle:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Circle : public Shape{
public:
  	Circle(double radius):Shape("Circle") {
        this->radius_ = (radius < 0) ? 0 : radius;              // If radius < 0,
                                                                // use 0, else use radius
                                                                // store in radius_ field
	}
	
  	double getArea() const{
        return (double)(M_PI * pow(this->radius_, 2));          // Calculate area and return
	}
	
 	double getVolume() const{
        return (double)0;                                       // Vol of 2D shape is 0
	}
	
  	Circle operator + (const Circle& cir){
        return Circle(this->radius_ + cir.getRadius());         // Return a new Circle with added components
	}
	
	Circle operator - (const Circle& cir){
        return Circle(this->radius_ - cir.getRadius());         // Return a new Circle with subtracted components
	} 

	double getRadius() const { 
        return this->radius_;                                   // Return radius_ field
	}
	
private:
	double radius_;

};



//Sphere
//Please implement the member functions of Sphere:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here

class Sphere : public Shape{
public:
  	Sphere(double radius):Shape("Sphere") {
        this->radius_ = (radius < 0) ? 0 : radius;              // If radius < 0
                                                                // use 0, else use radius
                                                                // store in radius_ field
	}

  	double getVolume() const {
        return (double)((4.0/3.0) * M_PI * pow(this->radius_, 3));  // Calculate vol and return
	}	
	
  	double getArea() const {
        return (double)(4 * M_PI * pow(this->radius_, 2));          // Calculate area and return
	}

	Sphere operator + (const Sphere& sph) {
        return Sphere(this->radius_ + sph.getRadius());         // Return a new Sphere with added components
	}

	Sphere operator - (const Sphere& sph) {
        return Sphere(this->radius_ - sph.getRadius());         // Return a new Sphere with subtracted components
	} 
	
	double getRadius() const {
        return this->radius_;                                   // Return radius_ field
	}

private:
	double radius_;

};

//Rectprism
//Please implement the member functions of RectPrism:
//constructor, getArea(), getVolume(), operator+, operator-
//@@Insert your code here
class RectPrism : public Shape{
public:
  	RectPrism(double width, double length, double height):Shape("RectPrism") {
        this->width_ = (width< 0) ? 0 : width;                  // If width < 0
                                                                // use 0, else use width
                                                                // store in width_
        this->length_ = (length < 0) ? 0 : length;              // If length < 0
                                                                // use 0, else use length
                                                                // store in length_
        this->height_ = (height < 0) ? 0 : height;              // If height < 0
                                                                // use 0, else use height
                                                                // store in height_

	}
	
  	double getVolume() const {
        return this->width_ * this->length_ * this->height_;    // Calculate vol and return
	}
  	
	double getArea() const {
        return (double)2*(                                      // Calculate area and return
        this->width_ * this->length_ +
        this->length_ * this->height_ +
        this->width_ * this->height_);
	}
	
	RectPrism operator + (const RectPrism& rectp){
        return RectPrism(                                       // Return a new RectPrism with added components
            this->width_ + rectp.getWidth(),
            this->length_ + rectp.getLength(),
            this->height_ + rectp.getHeight());
	}
	
	RectPrism operator - (const RectPrism& rectp){
        return RectPrism(                                       // Return a new RectPrism with subtracted components
            this->width_ - rectp.getWidth(),
            this->length_ - rectp.getLength(),
            this->height_ - rectp.getHeight());
	}	
	
	double getWidth() const { 
        return this->width_;                                    // Return width_ field
	}
	
	double getLength() const { 
        return this->length_;                                   // Return length_ field
	}
	
	double getHeight() const { 
        return this->height_;                                   // Return height_ field
	}
  
private:
  	double length_;
  	double width_;
  	double height_;

};

// Read shapes from test.txt and initialize the objects
// Return a vector of pointers that points to the objects 
static list<Shape*> CreateShapes(char* file_name) {
	//@@Insert your code here
    list<Shape*> shapes;                                        // Init list of shapes
    string name;                                                // Create temp for name
    double x1, x2, x3;                                          // Create temp for vars, max 3
    ifstream ifs(file_name, std::ifstream::in);                 // Set up filestream
    for(;!ifs.eof(); ifs >> name){                              // Loop until EOF
        if(name == "Rectangle") {                               // Create Rectangle
            ifs >> x1 >> x2;                                    // Store width and length
            shapes.push_back(new Rectangle<double>(x1, x2));
        }
        if(name == "Circle") {                                  // Create Circle
            ifs >> x1;                                          // Store radius
            shapes.push_back(new Circle(x1));
        }
        if(name == "RectPrism") {                               // Create RectPrism
            ifs >> x1 >> x2 >> x3;                              // Store width, length, and height
            shapes.push_back(new RectPrism(x1, x2, x3));
        }
        if(name == "Sphere") {                                  // Create Sphere
            ifs >> x1;                                          // Store radius
            shapes.push_back(new Sphere(x1));
        }
    }
    ifs.close();                                                // Close filestream
	
	return shapes;
}

// call getArea() of each object 
// return the max area
static double MaxArea(list<Shape*> shapes){
	double max_area = 0;
	//@@Insert your code here
    
    // Iterate thru all the shapes
    for(list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) {
        if(max_area < (*it)->getArea()) {                       // If larger than prev largest
            max_area = (*it)->getArea();                        // Update max_area
        }
    }
	
	return max_area;                                            // Return max_area
}

// call getVolume() of each object 
// return the max volume
static double MaxVolume(list<Shape*> shapes){
	double max_volume = 0;
	//@@Insert your code here

    // Iterate thru all the shapes
    for(list<Shape*>::iterator it = shapes.begin(); it != shapes.end(); it++) {
        if(max_volume < (*it)->getVolume()) {                   // If larger than prev largest
            max_volume = (*it)->getVolume();                    // Update max_volume
        }
    }
	
	return max_volume;                                          // Return max_volume
}
#endif

