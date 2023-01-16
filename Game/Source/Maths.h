#ifndef __Maths_H__
#define __Maths_H__

#include "Module.h"
#include "Point.h"
#include <Eigen/Dense>
#include <Eigen/Geometry>


using namespace Eigen;

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define TOLERANCE 10E-4


struct Point3D
{
	double x, y, z;
};
struct Point2D
{
	double x, y;
};

class Cube
{
public:

	Point3D points[8];
};
class Projection2D
{
public:
	Point2D points[8];
};


class Maths : public Module
{
public:

	Maths();
	~Maths();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Lab 3 functions
	MatrixXd QuaternionMultiplication(MatrixXd q, MatrixXd p);
	MatrixXd VectorRotationUsingQuaternions(MatrixXd v, MatrixXd q);
	MatrixXd CreateUxfromEulerAxis(double u1, double u2, double u3);
	MatrixXd EulerAxisAngleToRotationMat(double u1, double u2, double u3, double angle);
	void RotationMatToEulerAxisAngle(MatrixXd rotationMatrix, double* u1, double* u2, double* u3, double* angle);
	MatrixXd EulerAnglesToRotationMat(double z, double y, double x);
	MatrixXd RotationMatToEulerAngles(MatrixXd rotationMatrix);
	MatrixXd EulerAxisAngleToQuaternion(double u1, double u2, double u3, double angle);
	void QuaternionToEulerAxisAngle(MatrixXd q, double* u1, double* u2, double* u3, double* angle);
	MatrixXd EulerAxisAngleToRotationVec(double u1, double u2, double u3, double angle);
	void RotationVecToEulerAxisAngle(MatrixXd RotationVector, double* u1, double* u2, double* u3, double* angle);
	MatrixXd RotationChangeOfWritting(MatrixXd input, char from, char to);

	Projection2D FromCubeToProjection(Cube c);
	Cube RotatingCube(Cube c, MatrixXd r);

	MatrixXd rotationMatrix;
	Cube cube1;
	Point3D angles;
	Projection2D projection1;
	double focalLenght;
};


#endif // __MATHS_H__