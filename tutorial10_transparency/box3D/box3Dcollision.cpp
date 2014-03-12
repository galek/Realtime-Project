#include "box3Dcollision.h"
#include "box3DglobalRule.h"
#include "box3DcalculateForce.cpp"

#define gridSize 10

class GridCell
{
public:
	vector<Cube> cube;
	vector<Cylinder> cylinder;
	vector<Sphere> sphere;
	vector<Plane> plane;
	GridCell(){
	
	}
	GridCell(vector<Cube> cu,vector<Cylinder> cy,vector<Sphere> sp, vector<Plane>pl){
		cube = cu;
		cylinder = cy;
		sphere = sp;
		plane = pl;
	}

};

class Grid
{
public:
	//GridCell gridcell [gridSize][gridSize];
	Grid(int n){

	}
};


float inline minn(float x, float y){
	return (x < y ?  x : y) ;
}
//completed with intersection
void inline checkCollision_SphereCube(Sphere sph,Cube cube){
	if(dot(cube.velocity - sph.velocity,cube.position - sph.position) >= 0) return;
	vec4 dist = sph.position-cube.position;
	vec4 surfaceSp1 = dist*( (dist.length()-sph.radius) / dist.length() );
	vec4 point = cube.getInverseRatationMatrix()*surfaceSp1;
	vec3 cubeSkin = cube.getSkin();
	if(abs(point.x)<=cubeSkin.x && abs(point.y)<=cubeSkin.y && abs(point.z)<=cubeSkin.z) {

		//onCollision
		colSphere_Cube(sph,cube);
	}

}
//completed
void inline checkCollision_SphereCylinder(Sphere sph,Cylinder cyl){
	vec4 spherePos = cyl.getInverseRatationMatrix()*(sph.position-cyl.position);
	vec4 cylNormal = vec4(0,1,0,0);
	float projectDist = dot(spherePos,cylNormal);
	vec4 minDist = projectDist*cylNormal-spherePos;
	if(minDist.length() >= cyl.radius + sph.radius) return;
	if(minDist.length() < cyl.radius){
		if(projectDist <= cyl.length + sph.radius) return;
		else colSphere_Cylinder(sph,cyl);
	} else {
		if((projectDist*cylNormal + vec4(cyl.radius,0,0,0) - spherePos).length() >= sph.radius) return;
		else colSphere_Cylinder(sph,cyl);
	}
}
//completed
void inline checkCollision_SpherePlane(Sphere sp1,Plane pl1){
	vec4 spPos = sp1.position;
	float radius = sp1.radius;
	vec4 centerVec = spPos-pl1.position;
	float distance = dot(centerVec,pl1.getNormal());
	if(distance<=radius) {
		//onCollision
		colSphere_Plane(sp1,pl1);
	}

}
//completed
void inline checkCollision_SphereSphere(Sphere sp1, Sphere sp2){
	vec4 spPos = sp1.position;
	float radius = sp1.radius;
	vec4 d = spPos - sp2.position;
	float distance = d.length();
	float sumR = radius + sp2.radius;
	if(distance<=sumR) {
		//onCollision
		colSphere_Sphere(sp1,sp2);
	}
}
void inline checkCollision_PlaneCube(Plane pl1,Cube cu1){
	vec4 plPos = pl1.position;
	vec4 plNormal = pl1.getNormal();
	vec4 temp = cu1.position-pl1.position;
	float distance = dot(temp,plNormal);
}
//conpleted
void inline checkCollision_PlaneCylinder(Plane pl,Cylinder cylinder){
	vec4 dist = cylinder.position-pl.position;
	vec4 cylinderPosition_PlaneModel = pl.getInverseRatationMatrix()*dist;
	vec4 cylinderNormal_PlaneModel = pl.getInverseRatationMatrix()*cylinder.getRotationMatrix()*vec4(0,1,0,0);
	float height = dot(cylinderNormal_PlaneModel,vec4(0,1,0,0)) + (cross(vec3(0,1,0),vec3(cylinderNormal_PlaneModel))*cylinder.radius).length();
	if(height<=cylinder.position.y){
		//Collision
		colPlane_Cylinder(pl,cylinder);
	}
}

void inline checkCollision_CubeCube(Cube cu1,Cube cu2){
}
void inline checkCollision_CubeCylinder(Cube cu1,Cylinder cylinder){
}
void inline checkCollision_CylinderCylinder(Cylinder cylinder1,Cylinder cylinder2){
	vec4 cylinder1Normal = cylinder1.getNormal();
	vec4 cylinder1Center = cylinder1.position;
	//vec4 ep1 = cylinder2.getEndPoint1();
	//vec4 ep2 = cylinder2.getEndPoint2();
	//vec4 centerVec1 = ep1 - cylinder1Center;
	//vec4 centerVec2 = ep2 - cylinder1Center;
	//float distance = minn(dot(centerVec1,cylinder1Normal),dot(centerVec2,cylinder1Normal));
	//if(distance<=cylinder1.radius+cylinder2.radius){
	//	//oncollision
	//}

}

bool inline isMoveout(Rigidbody* obj1,Rigidbody* obj2){
	if(dot(obj2->velocity - obj1->velocity,obj2->position - obj1->position) >= 0) return true;
	else return false;
}

void inline checkCollision(vector<Cube> cu, vector<Cylinder> cy, vector<Plane> pl, vector<Sphere> sp){
	for(int i=0;i<sp.size();i++){
			Sphere sp1 = sp.at(i);
			for(int j=0;j<cu.size();j++) checkCollision_SphereCube(sp1,cu.at(j));
			for(int j=0;j<cy.size();j++) checkCollision_SphereCylinder(sp1,cy.at(j));
			for(int j=0;j<pl.size();j++) checkCollision_SpherePlane(sp1,pl.at(j));
			for(int j=i+1;j<sp.size();j++) checkCollision_SphereSphere(sp1,sp.at(j));
		}
		for(int i=0;i<pl.size();i++){
			Plane pl1 = pl.at(i);
			for(int j=0;j<cu.size();j++) checkCollision_PlaneCube(pl1,cu.at(j));
			for(int j=0;j<cy.size();j++) checkCollision_PlaneCylinder(pl1,cy.at(j));
		}
		for(int i=0;i<cu.size();i++){
			Cube cu1 = cu.at(i);
			for(int j=i+1;j<cu.size();j++) checkCollision_CubeCube(cu1,cu.at(j));
			for(int j=0;j<cy.size();j++) checkCollision_CubeCylinder(cu1,cy.at(j));
		}
		for(int i=0;i<cy.size()-1;i++){
			Cylinder cy1 = cy.at(i);
			for(int j=i+1;j<cy.size();j++) checkCollision_CylinderCylinder(cy1,cy.at(j));

		}
}
