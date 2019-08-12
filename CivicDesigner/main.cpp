//
//  main.cpp
//  CivicDesigner
//
//  Created by Breyden Monyemoratho on 2019/08/05.
//  Copyright © 2019 Breyden Monyemoratho. All rights reserved.
//

#include <iostream>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include <sstream>  
#include <locale.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
// #include <boost/algorithm/string.hpp> 

using namespace std;

// 3D coordiante
struct coordinate
{
    float x,y,z;
  // this will used to initialize the variables
 // of the structure
 coordinate(float x,float y, float z)
      :x(x),y(y),z(z)
 {
 }
};
// sight line , a straight line between A and B
struct Ray
{
   coordinate orig;
   coordinate dir;
   Ray(coordinate orig,coordinate dir)
      :orig(orig),dir(dir)
 {
 }
};
// Triangle
struct  Triangle
{
   coordinate side1,side2,side3;
  // this will used to initialize the variables
 // of the structure
 Triangle(coordinate side1,coordinate side2, coordinate side3)
      :side1(side1),side2(side2),side3(side3)
 {
 }
};

vector<string> split(string str, char delimiter);
float distance(float x1, float y1,float z1, float x2, float y2, float z2);
std::vector<coordinate> read_record_Polyline(string inputFileName);
std::vector< Triangle> read_record_Road_Surface(string inputFileName);
coordinate sub( coordinate a,  coordinate b);
coordinate cross( coordinate a,  coordinate b);
float rayTriangleIntersect(Ray *r,  coordinate *v0,  coordinate *v1,  coordinate *v2);
coordinate normalize( coordinate v);
float len( coordinate v);
float dot( coordinate a,  coordinate b);



int main(int argc, const char * argv[]) {
    // insert code here...
	std::string filename = "polyline.csv";
	std::vector<coordinate> C=read_record_Polyline(filename); //The polyline described in 2, contained in “polyline.csv”
	
	filename = "TIN Model Road Surface.csv";
  // T Any triangle on the road surface as described in 1, contained in TIN Model Road
	std::vector< Triangle>  T=read_record_Road_Surface(filename);
	


	// Let A follow the polyline, so iterate through the vertices of C, starting at S
	//  the driver can see <=350m 
	float Max_A_Dirver_Can_See=350;
	for (int i=0;i<C.size();i++){
		coordinate A=C[i]; // A The position of the driver’s eyes.
		int j=i; // B starts at A
		for (int j=i ;j <C.size();j++)
		{
			coordinate B=C[j];  // B Be an object on the road which the driver must be able to see.
			float AB=distance(A.x,A.y,A.z,B.x,B.y,B.z);   // AB The distance from A to B, along the polyline C
			if (AB>Max_A_Dirver_Can_See)  // if B exceeds 350m, check the intersection 
			{	int numHit = 0;
				// define a Ray or sight line between A and B here
				Ray r= Ray(A,normalize((sub(B, r.orig))));
				 
				for (int y=0;y=T.size();y++)  // iterating through the triangles
				{ // check to see if the sight line intersects with any of the triangles
					float IP=rayTriangleIntersect(&r,&T[y].side1,  &T[y].side2,  &T[y].side3);
					if (IP>0){
						numHit++; // record the intersection
					}

				}
			
				
				if (numHit>0){
					cout<<"reporting AB :"<<AB<<std::endl; // report AB
				}
				break;

			}

		}
	}
	


	
  
	
    return 0;
}
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;
 
  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
	
  }
 
  return internal;
}

std::vector<coordinate> read_record_Polyline(string inputFileName)
{ 
	std::vector<coordinate> coordinates;
	// File pointer 
	fstream fin; 

	// Open an existing file 
	fin.open(inputFileName, ios::in); 

    std::string str;
	vector<string> result; 
  
    while (std::getline(fin, str)) {
        // output the line
        // std::cout << str << std::endl;
		vector<string> temp=split(str, ',');

		coordinates.push_back(coordinate( std::stof(temp[0].c_str()),std::stof(temp[1].c_str()),std::stof(temp[2].c_str())));

		// boost::split(result, str, boost::is_any_of(","));
        // now we loop back and get the next line in 'str'
    }
	return coordinates;


} 

std::vector<Triangle> read_record_Road_Surface(string inputFileName)
{	std::vector<Triangle> triangles;
	// File pointer 
	fstream fin; 

	// Open an existing file 
	fin.open(inputFileName, ios::in); 

    std::string str;
	
	bool skip=true; // used for skipping  X,Y,Z,X,Y,Z,X,Y,Z
    while (std::getline(fin, str)) {
        // output the line
        // std::cout << str << std::endl;
		if (skip){
			skip=false;
		}
		else
		{			
		vector<string> temp=split(str,',');
		triangles.push_back(Triangle(coordinate( std::stof(temp[0].c_str()),std::stof(temp[1].c_str()),std::stof(temp[2].c_str())),coordinate( std::stof(temp[3].c_str()),std::stof(temp[4].c_str()),std::stof(temp[5].c_str())),coordinate( std::stof(temp[6].c_str()),std::stof(temp[7].c_str()),std::stof(temp[8].c_str()))));


		}

    }
	return triangles;

}
float distance(float x1, float y1,float z1, float x2, float y2, float z2)
 {
     

   float square_difference_x = (x2 - x1) * (x2 - x1);
   float square_difference_y = (y2 - y1) * (y2 - y1);
  float square_difference_z = (z2 - z1) * (z2 - z1);
   float sum = square_difference_x + square_difference_y+square_difference_z;

	float value = sqrt(sum);
	return value;
}
 coordinate sub( coordinate a,  coordinate b)
{
  return coordinate(a.x - b.x, a.y - b.y, a.z - b.z);
}

float dot( coordinate a,  coordinate b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float len( coordinate v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

 coordinate normalize( coordinate v)
{
  float l = len(v);
  return coordinate(v.x / l, v.y / l, v.z / l );
}

 coordinate cross( coordinate a,  coordinate b)
{
  return coordinate(
    a.y * b.z - a.z * b.y,
    a.z * b.x - a.x * b.z,
    a.x * b.y - a.y * b.x
  );
}


/* Ray-triangle intersection routine */

float rayTriangleIntersect(Ray *r,  coordinate *v0,  coordinate *v1,  coordinate *v2)
{
   coordinate v0v1 = sub(*v1, *v0);
   coordinate v0v2 = sub(*v2, *v0);

   coordinate pvec = cross(r->dir, v0v2);

  float det = dot(v0v1, pvec);

  if (det < 0.000001)
    return -INFINITY;

  float invDet = 1.0 / det;

   coordinate tvec = sub(r->orig, *v0);

  float u = dot(tvec, pvec) * invDet;

  if (u < 0 || u > 1)
    return -INFINITY;

   coordinate qvec = cross(tvec, v0v1);

  float v = dot(r->dir, qvec) * invDet;

  if (v < 0 || u + v > 1)
    return -INFINITY;

  return dot(v0v2, qvec) * invDet;
}
long ellapsedMs(struct timeval t0, struct timeval t1)
{
  return 1000 * (t1.tv_sec  - t0.tv_sec) +
                (t1.tv_usec - t0.tv_usec) / 1000;
}