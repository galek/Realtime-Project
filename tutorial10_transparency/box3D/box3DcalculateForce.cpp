#include "Cube.cpp"
#include "Sphere.cpp"
#include "Cylinder.cpp"
#include "Plane.cpp"
#include "Cone.cpp"

#define PARALLEL_SIZE 0.01f
#define SMALL_NUM 0.00000001f
//return project vector = project vec to base
vec4 inline projectVec(vec4 vec,vec4 base){
	return normalize(base)*dot(vec,base)/length(base);
}
vec3 inline projectVec(vec3 vec,vec3 base){
	return normalize(base)*dot(vec,base)/length(base);
}
//return size of project vector = project vec to base
float inline projectSize(vec4 vec,vec4 base){
	return dot(vec,base)/length(base);
}
//return project opos project vec
vec3 inline projectVecCross_vec3(vec3 vec,vec3 base){
	vec3 temp = cross(vec,normalize(base));
	return cross(normalize(base),temp);
}
//parallel test
bool inline isParallel(vec4 normal1,vec4 normal2){
	if(dot(normal1,normal2)-length(normal1)*length(normal2) <= PARALLEL_SIZE){

	}
}
//
float inline dist3D_Line_to_Line( vec4 l1p1, vec4 l1p0, vec4 l2p1, vec4 l2p0)
{
	vec4   u = l1p1 - l1p0;
	vec4   v = l2p1 - l2p0;
	vec4   w = l1p0 - l2p0;
	float    a = dot(u,u);         // always >= 0
	float    b = dot(u,v);
	float    c = dot(v,v);         // always >= 0
	float    d = dot(u,w);
	float    e = dot(v,w);
	float    D = a*c - b*b;        // always >= 0
	float    sc, tc;

	// compute the line parameters of the two closest points
	if (D < SMALL_NUM) {          // the lines are almost parallel
		sc = 0.0;
		tc = (b>c ? d/b : e/c);    // use the largest denominator
	}
	else {
		sc = (b*e - c*d) / D;
		tc = (a*e - b*d) / D;
	}

	// get the difference of the two closest points
	vec4   dP = w + (sc * u) - (tc * v);  // =  L1(sc) - L2(tc)

	return length(dP);   // return the closest distance
}
//===================================================================
// dist3D_Segment_to_Segment(): get the 3D minimum distance between 2 segments
//    Input:  two 3D line segments S1 and S2
//    Return: the shortest distance between S1 and S2
vec4 inline dist3D_Segment_to_Segment( vec4 s1p1, vec4 s1p0, vec4 s2p1, vec4 s2p0)
{
	vec4   u = s1p1 - s1p0;
	vec4   v = s2p1 - s2p0;
	vec4   w = s1p0 - s2p0;
	float    a = dot(u,u);         // always >= 0
	float    b = dot(u,v);
	float    c = dot(v,v);         // always >= 0
	float    d = dot(u,w);
	float    e = dot(v,w);
	float    D = a*c - b*b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

	// compute the line parameters of the two closest points
	if (D < SMALL_NUM) { // the lines are almost parallel
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b*e - c*d);
		tN = (a*e - b*d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d +  b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
	tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

	// get the difference of the two closest points
	vec4   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return dP;   // return the closest distance
}
//minimum distance point to line
vec4 inline dist3D_Line_to_point(vec4 line_start, vec4 line_end, vec4 point)
{
	vec4 lineVec = line_end - line_start;
	vec4 pointPos = point - line_start;
	vec4 proj = projectVec(pointPos,lineVec);
	return -pointPos + proj;
}
//minimum distance segment to line
vec4 inline dist3D_Segment_to_point(vec4 line_start, vec4 line_end, vec4 point)
{
	vec4 lineVec = line_end - line_start;
	vec4 pointPos = point - line_start;
	vec4 proj = projectVec(pointPos,lineVec);
	if(dot(proj,lineVec)<=0)return -pointPos;
	if(length(proj)>=length(lineVec)) return -pointPos + lineVec;
	return -pointPos + proj;
}

//completed
void inline colSphere_Sphere(Sphere* sph1, Sphere* sph2){
	vec4 dist = sph2->position-sph1->position;
	vec4 velo1 = sph1->velocity;
	vec4 velo2 = sph2->velocity;
	vec4 relatevelo = velo2-velo1;//ref from sph1
	float lineMomentum = projectSize(relatevelo,normalize(dist));
	sph1->addMomentum(normalize(dist)*lineMomentum);
	sph2->addMomentum(normalize(dist)*-lineMomentum);
	vec3 angularMomentum = cross(vec3(relatevelo),normalize(vec3(dist)) );
	sph1->addAngularMomentum_vec4( sph1->getInverseRatationMatrix()*vec4(angularMomentum,0) );
	sph2->addAngularMomentum_vec4( sph2->getInverseRatationMatrix()*vec4(-angularMomentum,0) );
}
//completed
void inline colSphere_Plane(Sphere* sph1, Plane* plane2,vec4 height){
	vec4 planeNormal = plane2->getNormal();
	vec4 newVelo = projectVec(-sph1->velocity,plane2->getNormal());
	vec4 velo = sph1->velocity;
	velo = sph1->velocity + newVelo*2;
	sph1->addMomentum(newVelo*2);
	sph1->updatePosition(height*(length(sph1->radius) - length(height) ) );
}
//completed
void inline colSphere_Cube(Sphere* sph1, Cube* cube2,vec4 colPoint_ModelSphere){
	//cout<<"col";
	vec4 relatevelo = cube2->velocity - sph1->velocity;//ref from sph1
	vec4 moment1 = -projectVec(relatevelo,normalize(colPoint_ModelSphere));
	//printVec4("cube velo",cube2->velocity);
	sph1->addMomentum(-moment1);
	sph1->addAngularMomentum_vec4(-relatevelo - moment1);
	vec4 colPoint_ModelCube = sph1->position - cube2->position + colPoint_ModelSphere; 
	vec4 moment2 = projectVec(relatevelo,-colPoint_ModelCube);
	cube2->addMomentum(moment1);
	cube2->addAngularMomentum_vec4(relatevelo - moment2);
	
	//printVec4("cube mmt",-moment1);
	//sph1->velocity=vec4(0,0,0,0);
	//cube2->velocity=vec4(0,0,0,0);
	//sph1->angularVelocity=vec3(0,0,0);
	//cube2->angularVelocity=vec3(0,0,0);
}
//not test
void inline colSphere_Cylinder(Sphere* sph1, Cylinder* cy2, vec4 colPoint_ModelSphere){
	//cout<<"col sphere cylinder\n";
	vec4 dist = cy2->position-sph1->position;
	vec4 velo1 = sph1->velocity;
	vec4 velo2 = cy2->velocity;
	vec4 relatevelo = velo2-velo1;//ref from sph1
	float exchangeMomentum = projectSize(relatevelo,normalize(dist));
	sph1->addMomentum(normalize(dist)*exchangeMomentum);
	cy2->addMomentum(normalize(dist)*-exchangeMomentum);
	
	float newLineEngergy1 = pow(length(sph1->velocity),2);
	float newLineEngergy2 = pow(length(cy2->velocity),2);
	vec3 angularMomentum = cross(vec3(relatevelo),normalize(vec3(dist)));
	sph1->addAngularMomentum(angularMomentum);
	cy2->addAngularMomentum(-angularMomentum);
}
//dummy
void inline colCube_Plane(Cube* cube1, Plane* plane2, vec4 colPoint_ModelCube){
	//cout<<"col cube plane";
	/*vec4 moment1 = projectVec(-cube1->velocity , plane2->getNormal());
	cube1->addMomentum(moment1);
	cube1->addAngularMomentum_vec4(cube1->velocity - moment1);
*/

	vec4 relatevelo = cube1->velocity;//ref from sph1
	vec4 moment1 = -projectVec(relatevelo,normalize(plane2->getNormal()));
	//printVec4("cube velo",cube2->velocity);
	cube1->addMomentum(moment1*2);
	//cube1->addAngularMomentum_vec4(-relatevelo - moment1);
}
void inline colCube_Cube(Cube* cube1, Cube* cube2,vec4 colPoint_ModelCube1){
	vec4 relatevelo = cube2->velocity - cube1->velocity;//ref from cu1
	vec4 moment1 = -projectVec(relatevelo,normalize(colPoint_ModelCube1));
	cube1->addMomentum(-moment1);
	cube1->addAngularMomentum_vec4(-relatevelo - moment1);
	vec4 colPoint_ModelCube2 = cube1->position - cube2->position + colPoint_ModelCube1; 
	vec4 moment2 = projectVec(relatevelo,-colPoint_ModelCube2);
	cube2->addMomentum(moment1);
	cube2->addAngularMomentum_vec4((relatevelo - moment2)*0.01f);
	//printVec4("spmmt",moment1);
	
}
void inline colCube_Cylinder(Cube* cube1, Cylinder* cy2,vec4 colPoint_ModelCube){
	vec4 relatevelo = cy2->velocity - cube1->velocity;//ref from cu1
	vec4 moment1 = -projectVec(relatevelo,normalize(colPoint_ModelCube));
	cube1->addMomentum(-moment1);
	cube1->addAngularMomentum_vec4(-relatevelo - moment1);
	vec4 colPoint_ModelCyl = cube1->position - cy2->position + colPoint_ModelCube; 
	vec4 moment2 = projectVec(relatevelo,-colPoint_ModelCyl);
	cy2->addMomentum(moment1);
	cy2->addAngularMomentum_vec4(relatevelo - moment2);
}

//dummy code
void inline colPlane_Cylinder(Cylinder* cyl1,Plane* plane1, vec4 colPointCyl){
	//vec4 moment1 = projectVec(-cylinder2->velocity , plane1->getNormal());
	//cylinder2->addMomentum(moment1*2);

	vec4 relatevelo = -cyl1->velocity;//ref from sph1
	vec4 moment1 = -projectVec(relatevelo,normalize(plane1->getNormal()));
	//printVec4("cube velo",cube2->velocity);
	cyl1->addMomentum(-moment1);
	cyl1->addAngularMomentum_vec4((-relatevelo - moment1)*0.1f);
}

//Cylinder
void inline colCylinder_Cylinder(Cylinder* cy1, Cylinder* cy2, vec4 colPoint_ModelCy1){
	vec4 relatevelo = cy2->velocity - cy1->velocity;//ref from cu1
	vec4 moment1 = -projectVec(relatevelo,normalize(colPoint_ModelCy1));
	cy1->addMomentum(-moment1);
	cy1->addAngularMomentum_vec4(-relatevelo - moment1);
	vec4 colPoint_ModelCube2 = cy1->position - cy2->position + colPoint_ModelCy1; 
	vec4 moment2 = projectVec(relatevelo,-colPoint_ModelCube2);
	cy2->addMomentum(moment1);
	cy2->addAngularMomentum_vec4(relatevelo - moment2);
	//printVec4("spmmt",moment1);
	
}
