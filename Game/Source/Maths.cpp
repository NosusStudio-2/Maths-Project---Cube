#include <math.h>
#define _USE_MATH_DEFINES
#include <iostream>
#include "Eigen/Dense"
#include "App.h"
#include "Render.h"

#include "Maths.h"

using namespace std;
using namespace Eigen;


Maths::Maths()
{

}

Maths::~Maths()
{

}

bool Maths::Start()
{
	bool ret = true;
	MatrixXd newrot(3, 3);

	newrot <<	1, 0, 0,
				0, 1, 0,
				0, 0, 1;
	rotationMatrix = newrot;


	return ret;
}

bool Maths::PreUpdate()
{
	bool ret = true;

	//applies current rotation
	Cube cubeRotated = RotatingCube(cube1, rotationMatrix);
	//from 3d points to 2d projection
	projection1 = FromCubeToProjection(cubeRotated);



	return ret;
}

bool Maths::Update(float dt)
{
	bool ret = true;

	return ret;
}

bool Maths::PostUpdate()
{
	bool ret = true;
	//print cube's projection

	app->render->DrawLine(projection1.points[0].x, projection1.points[0].y, projection1.points[1].x, projection1.points[1].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[2].x, projection1.points[2].y, projection1.points[1].x, projection1.points[1].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[2].x, projection1.points[2].y, projection1.points[3].x, projection1.points[3].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[0].x, projection1.points[0].y, projection1.points[3].x, projection1.points[3].y, 255, 255, 255);

	app->render->DrawLine(projection1.points[0+4].x, projection1.points[0+4].y, projection1.points[1+4].x, projection1.points[1+4].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[2+4].x, projection1.points[2+4].y, projection1.points[1+4].x, projection1.points[1+4].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[2+4].x, projection1.points[2+4].y, projection1.points[3+4].x, projection1.points[3+4].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[0+4].x, projection1.points[0+4].y, projection1.points[3+4].x, projection1.points[3+4].y, 255, 255, 255);

	app->render->DrawLine(projection1.points[0].x, projection1.points[0].y, projection1.points[4].x, projection1.points[1].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[1].x, projection1.points[1].y, projection1.points[5].x, projection1.points[1].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[2].x, projection1.points[2].y, projection1.points[6].x, projection1.points[3].y, 255, 255, 255);
	app->render->DrawLine(projection1.points[3].x, projection1.points[3].y, projection1.points[7].x, projection1.points[3].y, 255, 255, 255);
	return ret;
}

bool Maths::CleanUp()
{
	bool ret = true;

	return ret;
}

MatrixXd Maths::QuaternionMultiplication(MatrixXd q, MatrixXd p)
{
	//qp = Q(q) * p
	MatrixXd qp(4, 1);
	MatrixXd Qq(4, 4);
	Qq << q(1 - 1, 0), -q(2 - 1, 0), -q(3 - 1, 0), -q(4 - 1, 0),
		q(2 - 1, 0), q(1 - 1, 0), -q(4 - 1, 0), q(3 - 1, 0),
		q(3 - 1, 0), q(4 - 1, 0), q(1 - 1, 0), -q(2 - 1, 0),
		q(4 - 1, 0), -q(3 - 1, 0), q(2 - 1, 0), q(1 - 1, 0);

	qp = Qq * p;

	return qp;
}

MatrixXd Maths::VectorRotationUsingQuaternions(MatrixXd v, MatrixXd q)
{
	q = q / q.norm();//transforms q quaternion into a unit quaternion

	MatrixXd vquat(4, 1), qñ(4, 1), w(4, 1), wresult(3, 1);
	vquat << 0,
		v(1 - 1, 0),
		v(2 - 1, 0),
		v(3 - 1, 0);

	qñ << q(1 - 1, 0),
		-q(2 - 1, 0),
		-q(3 - 1, 0),
		-q(4 - 1, 0);

	w = QuaternionMultiplication(q, vquat);
	w = QuaternionMultiplication(w, qñ);

	wresult << w(2 - 1, 0),
		w(3 - 1, 0),
		w(4 - 1, 0);

	return wresult;
}

MatrixXd Maths::CreateUxfromEulerAxis(double u1, double u2, double u3)
{
	MatrixXd Ux(3, 3);
	Ux << 0, -u3, u2,
		u3, 0, -u1,
		-u2, u1, 0;
	return Ux;
}

MatrixXd Maths::EulerAxisAngleToRotationMat(double u1, double u2, double u3, double angle)
{
	MatrixXd RotationMatrix(3, 3);
	MatrixXd I3(3, 3);
	I3 << 1, 0, 0,
		0, 1, 0,
		0, 0, 1;
	RotationMatrix = CreateUxfromEulerAxis(u1, u2, u3);//now Rotation matrix is [U]x
	RotationMatrix = I3 + sin(angle * DEGTORAD) * RotationMatrix + (1 - cos(DEGTORAD * angle)) * (RotationMatrix * RotationMatrix); // apply rodrigues formula
	return RotationMatrix;
}

void Maths::RotationMatToEulerAxisAngle(MatrixXd rotationMatrix, double* u1, double* u2, double* u3, double* angle)
{
	//first, find the angle (in radiants)
	double newAngle = acos((rotationMatrix.trace() - 1) / 2);
	MatrixXd angleOperationsMat(3, 3);
	angleOperationsMat << 1 / (2 * sin(newAngle)), 0, 0,
		0, 1 / (2 * sin(newAngle)), 0,
		0, 0, 1 / (2 * sin(newAngle));

	rotationMatrix = (rotationMatrix - rotationMatrix.transpose()) * angleOperationsMat;
	newAngle *= RADTODEG;

	*angle = newAngle;
	*u1 = rotationMatrix(3 - 1, 2 - 1);
	*u2 = rotationMatrix(1 - 1, 3 - 1);
	*u3 = rotationMatrix(2 - 1, 1 - 1);
}

MatrixXd Maths::EulerAnglesToRotationMat(double z, double y, double x) //USES ZYX SYSTEM, ANGLES IN DEGREES
{
	z *= DEGTORAD;
	y *= DEGTORAD;
	x *= DEGTORAD;
	MatrixXd rotationMatrix(3, 3);
	rotationMatrix << cos(y) * cos(z), cos(z)* sin(y)* sin(x) - cos(x) * sin(z), cos(x)* cos(z)* sin(y) + sin(z) * sin(x),
		cos(y)* sin(z), sin(z)* sin(y)* sin(x) + cos(x) * cos(z), sin(y)* sin(z)* cos(x) - cos(z) * sin(x),
		-1 * sin(y), cos(y)* sin(x), cos(y)* cos(x);

	return rotationMatrix;
}

MatrixXd Maths::RotationMatToEulerAngles(MatrixXd rotationMatrix)//IT RETURNS ANGLES IN DEGREES
{
	double y = asin(-1 * rotationMatrix(3 - 1, 1 - 1));

	MatrixXd eulerAngles(3, 1);

	if (abs(cos(y)) < TOLERANCE)
	{
		cout << "\n\n CANNOT DIVIDE BY 0\n\n";
		return eulerAngles;
	}


	eulerAngles << atan2(rotationMatrix(2 - 1, 1 - 1) / cos(y), rotationMatrix(1 - 1, 1 - 1) / cos(y)) * RADTODEG,
		y* RADTODEG,
		atan2(rotationMatrix(3 - 1, 2 - 1) / cos(y), rotationMatrix(3 - 1, 3 - 1) / cos(y))* RADTODEG;



	return eulerAngles;
}

MatrixXd Maths::EulerAxisAngleToQuaternion(double u1, double u2, double u3, double angle)
{
	angle *= DEGTORAD;
	MatrixXd q(4, 1);
	q << u1 * sin(angle / 2),
		u2* sin(angle / 2),
		u3* sin(angle / 2),
		cos(angle / 2);
	return q;
}

void Maths::QuaternionToEulerAxisAngle(MatrixXd q, double* u1, double* u2, double* u3, double* angle)
{
	double newAngle = 2 * acos(q(4 - 1, 0));

	newAngle *= RADTODEG;
	*angle = newAngle;
	*u1 = q(1 - 1, 0) / sqrt(1 - pow(q(4 - 1, 0), 2));
	*u2 = q(2 - 1, 0) / sqrt(1 - pow(q(4 - 1, 0), 2));
	*u3 = q(3 - 1, 0) / sqrt(1 - pow(q(4 - 1, 0), 2));
}

MatrixXd Maths::EulerAxisAngleToRotationVec(double u1, double u2, double u3, double angle)
{
	MatrixXd RotationVector(3, 1);
	RotationVector << u1 * angle * DEGTORAD,
		u2* angle* DEGTORAD,
		u3* angle* DEGTORAD;

	return RotationVector;
}

void Maths::RotationVecToEulerAxisAngle(MatrixXd RotationVector, double* u1, double* u2, double* u3, double* angle)
{
	double newAngle = RotationVector.norm();

	*u1 = RotationVector(1 - 1, 0) / newAngle;
	*u2 = RotationVector(2 - 1, 0) / newAngle;
	*u3 = RotationVector(3 - 1, 0) / newAngle;

	newAngle *= RADTODEG;
	*angle = newAngle;
}

MatrixXd Maths::RotationChangeOfWritting(MatrixXd input, char from, char to)
{
	if (from == to)
	{
		return input;
	}
	else if (from == 'r' && to == 'e')
	{
		MatrixXd output(3, 1);
		output = RotationMatToEulerAngles(input);
		return output;
	}
	else if (from == 'r' && to == 'p')
	{
		MatrixXd output(4, 1);
		RotationMatToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		return output;
	}
	else if (from == 'r' && to == 'q')
	{
		MatrixXd output(4, 1);
		RotationMatToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		output = EulerAxisAngleToQuaternion(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return output;
	}
	else if (from == 'r' && to == 'v')
	{
		MatrixXd output(4, 1);
		RotationMatToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 1);
		newOutput = EulerAxisAngleToRotationVec(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'e' && to == 'r')
	{
		MatrixXd output(3, 3);
		output = EulerAnglesToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0));
		return output;
	}
	else if (from == 'e' && to == 'p')
	{
		MatrixXd output(3, 3);
		output = EulerAnglesToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0));
		MatrixXd newOutput(4, 1);
		RotationMatToEulerAxisAngle(output, &newOutput(1 - 1, 0), &newOutput(2 - 1, 0), &newOutput(3 - 1, 0), &newOutput(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'e' && to == 'q')
	{
		MatrixXd output(3, 3);
		output = EulerAnglesToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0));
		MatrixXd newOutput(4, 1);
		RotationMatToEulerAxisAngle(output, &newOutput(1 - 1, 0), &newOutput(2 - 1, 0), &newOutput(3 - 1, 0), &newOutput(4 - 1, 0));
		newOutput = EulerAxisAngleToQuaternion(newOutput(1 - 1, 0), newOutput(2 - 1, 0), newOutput(3 - 1, 0), newOutput(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'e' && to == 'v')
	{
		MatrixXd output(3, 3);
		output = EulerAnglesToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0));
		MatrixXd newOutput(4, 1);
		RotationMatToEulerAxisAngle(output, &newOutput(1 - 1, 0), &newOutput(2 - 1, 0), &newOutput(3 - 1, 0), &newOutput(4 - 1, 0));
		MatrixXd newnewOutput(3, 1);
		newnewOutput = EulerAxisAngleToRotationVec(newOutput(1 - 1, 0), newOutput(2 - 1, 0), newOutput(3 - 1, 0), newOutput(4 - 1, 0));
		return newnewOutput;
	}
	else if (from == 'p' && to == 'r')
	{
		MatrixXd output(3, 3);
		output = EulerAxisAngleToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0), input(4 - 1, 0));
		return output;
	}
	else if (from == 'p' && to == 'e')
	{
		MatrixXd output(3, 3);
		output = EulerAxisAngleToRotationMat(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0), input(4 - 1, 0));
		MatrixXd newOutput(3, 1);
		newOutput = RotationMatToEulerAngles(output);
		return newOutput;
	}
	else if (from == 'p' && to == 'q')
	{
		MatrixXd output(4, 1);
		output = EulerAxisAngleToQuaternion(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0), input(4 - 1, 0));
		return output;
	}
	else if (from == 'p' && to == 'v')
	{
		MatrixXd output(3, 1);
		output = EulerAxisAngleToRotationVec(input(1 - 1, 0), input(2 - 1, 0), input(3 - 1, 0), input(4 - 1, 0));
		return output;
	}
	else if (from == 'q' && to == 'r')
	{
		MatrixXd output(4, 1);
		QuaternionToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 3);
		newOutput = EulerAxisAngleToRotationMat(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'q' && to == 'e')
	{
		MatrixXd output(4, 1);
		QuaternionToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 3);
		newOutput = EulerAxisAngleToRotationMat(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		MatrixXd newnewOutput(3, 1);
		newnewOutput = RotationMatToEulerAngles(newOutput);
		return newnewOutput;
	}
	else if (from == 'q' && to == 'p')
	{
		MatrixXd output(4, 1);
		QuaternionToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		return output;
	}
	else if (from == 'q' && to == 'v')
	{
		MatrixXd output(4, 1);
		QuaternionToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 1);
		newOutput = EulerAxisAngleToRotationVec(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'v' && to == 'r')
	{
		MatrixXd output(4, 1);
		RotationVecToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 3);
		newOutput = EulerAxisAngleToRotationMat(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return newOutput;
	}
	else if (from == 'v' && to == 'e')
	{
		MatrixXd output(4, 1);
		RotationVecToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		MatrixXd newOutput(3, 3);
		newOutput = EulerAxisAngleToRotationMat(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		MatrixXd newnewOutput(3, 1);
		newnewOutput = RotationMatToEulerAngles(newOutput);
		return newnewOutput;
	}
	else if (from == 'v' && to == 'p')
	{
		MatrixXd output(4, 1);
		RotationVecToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		return output;
	}
	else if (from == 'v' && to == 'q')
	{
		MatrixXd output(4, 1);
		RotationVecToEulerAxisAngle(input, &output(1 - 1, 0), &output(2 - 1, 0), &output(3 - 1, 0), &output(4 - 1, 0));
		output = EulerAxisAngleToQuaternion(output(1 - 1, 0), output(2 - 1, 0), output(3 - 1, 0), output(4 - 1, 0));
		return output;
	}
	cout << "\n\none of the chars or both are wrong\n\n";
	return input;
}

Projection2D Maths::FromCubeToProjection(Cube c)
{
	Projection2D p;
	for (size_t i = 0; i < 8; i++)
	{
		p.points[i].x = c.points[i].x / c.points[i].z * focalLenght;
		p.points[i].y = c.points[i].y / c.points[i].y * focalLenght;
	}
	return p;
}

Cube Maths::RotatingCube(Cube c, MatrixXd r)
{
	Cube newc;
	MatrixXd cubePoint(3,1);
	for (size_t i = 0; i < 8; i++)
	{
		cubePoint <<	newc.points[i].x,
						newc.points[i].y,
						newc.points[i].z;

		r * cubePoint;

		newc.points[i].x = cubePoint(0, 0);
		newc.points[i].y = cubePoint(1, 0);
		newc.points[i].z = cubePoint(2, 0);
	}

	return newc;
}