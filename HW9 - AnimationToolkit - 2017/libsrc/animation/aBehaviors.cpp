#include "aBehaviors.h"

#include <math.h>
#include "GL/glew.h"
#include "GL/glut.h"

// Base Behavior
///////////////////////////////////////////////////////////////////////////////
Behavior::Behavior()
{
}

Behavior::Behavior( char* name) 
{
	m_name = name;
	m_pTarget = NULL;
}

Behavior::Behavior( Behavior& orig) 
{
	m_name = orig.m_name;
	m_pTarget = NULL;
}

string& Behavior::GetName() 
{
    return m_name;
}

// Behaviors derived from Behavior
//----------------------------------------------------------------------------//
// Seek behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Seek returns a maximum velocity towards the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position


Seek::Seek( AJoint* target) 
{
	m_name = "seek";
	m_pTarget = target;

}

Seek::Seek( Seek& orig) 
{
	m_name = "seek";
	m_pTarget = orig.m_pTarget;
}


Seek::~Seek()
{
}

vec3 Seek::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 error = targetPos - actorPos;
	double vd = actor->gMaxSpeed;
	double thetad = atan2(actor->getVelocity()[2], actor->getVelocity()[0]) + atan2(error[1], error[0]);
	//Vdesired = vec3(vd* cos(thetad), 0, vd * sin(thetad));

	Vdesired = targetPos - actorPos;
	Vdesired = Vdesired.Normalize() * actor->gMaxSpeed;

	return Vdesired;
}


// Flee behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Flee calculates a a maximum velocity away from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position

Flee::Flee( AJoint* target) 
{
	m_name = "flee";
	m_pTarget = target;
}

Flee::Flee( Flee& orig) 
{
	m_name = "flee";
	m_pTarget = orig.m_pTarget;
}

Flee::~Flee()
{
}

vec3 Flee::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	Vdesired =  actorPos - targetPos;
	Vdesired = Vdesired.Normalize() * BehaviorController::gMaxSpeed;



	return Vdesired;

}

// Arrival behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// the actors distance from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Arrival strength is in BehavioralController::KArrival


Arrival::Arrival( AJoint* target) 
{
	m_name = "arrival";
	m_pTarget = target;
}

Arrival::Arrival( Arrival& orig) 
{
	m_name = "arrival";
	m_pTarget = orig.m_pTarget;
}

Arrival::~Arrival()
{
}

vec3 Arrival::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired

	vec3 error = targetPos - actorPos;
	Vdesired = actor->KArrival * error;
	return Vdesired;
}


// Departure behavior
///////////////////////////////////////////////////////////////////////////////
// Given the actor, return a desired velocity in world coordinates
// Arrival returns a desired velocity vector whose speed is proportional to
// 1/(actor distance) from the target
// m_pTarget contains target world position
// actor.getPosition() returns Agent's world position
//  Departure strength is in BehavioralController::KDeparture

Departure::Departure(AJoint* target) 
{
	m_name = "departure";
	m_pTarget = target;
}

Departure::Departure( Departure& orig) 
{
	m_name = "departure";
	m_pTarget = orig.m_pTarget;
}

Departure::~Departure()
{
}

vec3 Departure::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();

	// TODO: add your code here to compute Vdesired
	vec3 error = actorPos - targetPos;
	Vdesired = actor->KDeparture * error / (error.Length() * error.Length());
	return Vdesired;





	return Vdesired;
}


// Avoid behavior
///////////////////////////////////////////////////////////////////////////////
//  For the given the actor, return a desired velocity in world coordinates
//  If an actor is near an obstacle, avoid adds a normal response velocity to the 
//  the desired velocity vector computed using arrival
//  Agent bounding sphere radius is in BehavioralController::radius
//  Avoidance parameters are  BehavioralController::TAvoid and BehavioralController::KAvoid

Avoid::Avoid(AJoint* target, vector<Obstacle>* obstacles) 
{
	m_name = "avoid";
	m_pTarget = target;
	mObstacles = obstacles;
}

Avoid::Avoid( Avoid& orig) 
{
	m_name = "avoid";
	m_pTarget = orig.m_pTarget;
	mObstacles = orig.mObstacles;
}

Avoid::~Avoid()
{
}

vec3 Avoid::calcDesiredVel( BehaviorController* actor)
{

	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	m_actorPos = actor->getPosition();
	m_actorVel = actor->getVelocity();

	//TODO: add your code here
	vec3 Varrival(0, 0, 0);
	// Step 1. compute initial value for Vdesired = Varrival so agent moves toward target
	Varrival = Arrival(m_pTarget).calcDesiredVel(actor);


	vec3 Vavoid(0, 0, 0);
	//TODO: add your code here to compute Vavoid 




	// Step 2. compute Lb
	//TODO: add your code here
	float lb = BehaviorController::TAvoid * actor->getVelocity().Length();

	// Step 3. find closest obstacle 
	//TODO: add your code here
	int closest = 999;
	vec3 d = vec3(8000.0, 8000.0, 8000.0);
	for (int i = 0; i < (int)mObstacles->size(); i++) {
		vec3 dtmp = mObstacles->at(i).m_Center.getLocalTranslation() - m_actorPos;
		if (dtmp.Length() < d.Length()) {
			closest = i;
			d = dtmp;
		}
	}



	// Step 4. determine whether agent will collide with closest obstacle (only consider obstacles in front of agent)
	//TODO: add your code here
	if (closest < (int)mObstacles->size() && d[0] > 0) { // check if obstacle index is valid, and the closest obstacle is in front of agent
		if (fabs(d[0]) <= lb) { // |dx| < lb + rb + ro
			if (fabs(d[1]) <= mObstacles->at(closest).m_Radius + BehaviorController::gAgentRadius) {
				vec3 dy = vec3(0.0, 0.0, d[2]);
				vec3 nAvoid = -1.0 * dy.Normalize();
				double vAvoidm = BehaviorController::KAvoid *
						(mObstacles->at(closest).m_Radius + BehaviorController::gAgentRadius - dy.Length()) /
							mObstacles->at(closest).m_Radius + BehaviorController::gAgentRadius;
				Vavoid = vAvoidm * nAvoid;
			}
		}
	}

	// Step 5.  if potential collision detected, compute Vavoid and set Vdesired = Varrival + Vavoid
	//TODO: add your code here


	Vdesired = Varrival + Vavoid;

	return Vdesired;
	
}

void Avoid::display( BehaviorController* actor)
{
	//  Draw Debug info
	vec3 angle = actor->getOrientation();
	vec3 vel = actor->getVelocity();
	vec3 dir = vec3(cos(angle[1]), 0, sin(angle[1]));
	vec3 probe = dir * (vel.Length()/BehaviorController::gMaxSpeed)*BehaviorController::TAvoid;
	
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_obstaclePos[0], m_obstaclePos[1], m_obstaclePos[2]);
	glColor3f(0, 1, 1);
	glVertex3f(m_actorPos[0], m_actorPos[1], m_actorPos[2]);
	glVertex3f(m_actorPos[0] + probe[0], m_actorPos[1] + probe[1], m_actorPos[2] + probe[2]);
	glEnd();
}


// Wander Behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity in world coordinates
// Wander returns a desired velocity vector whose direction changes at randomly from frame to frame
// Wander strength is in BehavioralController::KWander

Wander::Wander() 
{
	m_name = "wander";
	m_Wander = vec3(1.0, 0.0, 0.0);
}

Wander::Wander( Wander& orig) 
{
	m_name = "wander";
	m_Wander = orig.m_Wander;
}

Wander::~Wander()
{
}

vec3 Wander::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();

	// Step. 1 find a random direction
	//TODO: add your code here
	float randAngle = float(rand() % 360) / 180.0 * M_PI;
	vec3 rand = vec3(cos(randAngle), 0.0, sin(randAngle));

	// Step2. scale it with a noise factor
	//TODO: add your code here
	vec3 noiseRand = rand * actor->KNoise;

	// Step3. change the current Vwander  to point to a random direction
	//TODO: add your code here
	vec3 wanderNoise = noiseRand + m_Wander;
	m_Wander = actor->KWander * wanderNoise / wanderNoise.Length();



	// Step4. scale the new wander velocity vector and add it to the nominal velocity
	//TODO: add your code here
	vec3 nominal = vec3(1.0, 0.0, 0.0);
	
	Vdesired = nominal + m_Wander;


	return Vdesired;
}


// Alignment behavior
///////////////////////////////////////////////////////////////////////////////
// For the given the actor, return a desired velocity vector in world coordinates
// Alignment returns the average velocity of all active agents in the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Alignment parameters are in BehavioralController::RNeighborhood and BehavioralController::KAlign


Alignment::Alignment(AJoint* target, vector<AActor>* agents) 
{
	m_name = "alignment";
	m_pAgentList = agents;
	m_pTarget = target;
}



Alignment::Alignment( Alignment& orig) 
{
	m_name = orig.m_name;
	m_pAgentList = orig.m_pAgentList;
	m_pTarget = orig.m_pTarget;

}

Alignment::~Alignment()
{
}

vec3 Alignment::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_pAgentList;
	

	// compute Vdesired 
	
	// Step 1. compute value of Vdesired for fist agent (i.e. m_AgentList[0]) using an arrival behavior so it moves towards the target
	 
	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	//TODO: add your code here


	double count = 0.0;
	vec3 vArrive = Arrival(m_pTarget).calcDesiredVel(actor);
	vec3 velSum = vec3(0.0, 0.0, 0.0);;
	vec3 vAlign = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < (int)agentList.size(); i++) {
		double dx = actorPos[0] - agentList[i].getBehaviorController()->getPosition()[0];
		double dy = actorPos[2] - agentList[i].getBehaviorController()->getPosition()[2];
		double dist = sqrt(dx*dx + dy*dy);
		if (dist < BehaviorController::gKNeighborhood) {
			velSum += agentList[i].getBehaviorController()->getVelocity();
			count++;
		}
	}
	if (count > 0) {
		vAlign = BehaviorController::KAlignment * velSum / count;
	}
	// Check whether the actor is the leader
	if (actorPos[0] == leader->getPosition()[0] &&
		actorPos[2] == leader->getPosition()[2]) {
		Vdesired = Arrival(m_pTarget).calcDesiredVel(actor); // the leader should be able to arrive at the target
	}

	// Step 2. if not first agent compute Valign as usual
	//TODO: add your code here
	else {
		Vdesired = vAlign;
	}
	
	return Vdesired;
}

// Separation behavior
///////////////////////////////////////////////////////////////////////////////
// For the given te actor, return a desired velocity vector in world coordinates
// Separation tries to maintain a constant distance between all agents
// within the neighborhood
// agents[i] gives the pointer to the ith agent in the environment
// Separation settings are in BehavioralController::RNeighborhood and BehavioralController::KSeperate

 

Separation::Separation( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "separation";
	m_AgentList = agents;
	m_pTarget = target;
}

Separation::~Separation()
{
}

Separation::Separation( Separation& orig) 
{
	m_name = "separation";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

vec3 Separation::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 targetPos = m_pTarget->getLocalTranslation();
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vseparate
	// TODO: add your code here to compute Vdesired 
	vec3 separateSum = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < (int)agentList.size(); i++) {
		double dx = actorPos[0] - agentList[i].getBehaviorController()->getPosition()[0];
		double dy = actorPos[2] - agentList[i].getBehaviorController()->getPosition()[2];
		if (dx == 0 && dy == 0) {
			continue;
		}
		double dist = sqrt(dx*dx + dy*dy);
		if (dist < BehaviorController::gKNeighborhood) {
			separateSum[0] += dx / (dist*dist);
			separateSum[2] += dy / (dist*dist);
		}
	}
	Vdesired = actor->KSeparation * separateSum;


	if (Vdesired.Length() < 5.0)
		Vdesired = 0.0;
	
	return Vdesired;
}


// Cohesion behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// Cohesion moves actors towards the center of the group of agents in the neighborhood
//  agents[i] gives the pointer to the ith agent in the environment
//  Cohesion parameters are in BehavioralController::RNeighborhood and BehavioralController::KCohesion


Cohesion::Cohesion( vector<AActor>* agents) 
{
	m_name = "cohesion";
	m_AgentList = agents;
}

Cohesion::Cohesion( Cohesion& orig) 
{
	m_name = "cohesion";
	m_AgentList = orig.m_AgentList;
}

Cohesion::~Cohesion()
{
}

vec3 Cohesion::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;
	
	// compute Vdesired = Vcohesion
	// TODO: add your code here 
	double count = 0.0;
	vec3 posSum = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < (int)agentList.size(); i++) {
		double dx = actorPos[0] - agentList[i].getBehaviorController()->getPosition()[0];
		double dy = actorPos[2] - agentList[i].getBehaviorController()->getPosition()[2];
		if (dx == 0 && dy == 0) {
			continue;
		}
		double dist = sqrt(dx*dx + dy*dy);
		if (dist < BehaviorController::gKNeighborhood) {
			posSum[0] += agentList[i].getBehaviorController()->getPosition()[0];
			posSum[2] += agentList[i].getBehaviorController()->getPosition()[2];
			count++;
		}
	}
	if (count > 0) {
		vec3 Xcm = posSum / count;
		Vdesired = BehaviorController::KCohesion*(Xcm - actorPos);
	}






	return Vdesired;
}

// Flocking behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector  in world coordinates
// Flocking combines separation, cohesion, and alignment behaviors
//  Utilize the Separation, Cohesion and Alignment behaviors to determine the desired velocity vector


Flocking::Flocking( AJoint* target,  vector<AActor>* agents) 
{
	m_name = "flocking";
	m_AgentList = agents;
	m_pTarget = target;
}

Flocking::Flocking( Flocking& orig) 
{
	m_name = "flocking";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Flocking::~Flocking()
{
}

vec3 Flocking::calcDesiredVel( BehaviorController* actor)
{
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// compute Vdesired = Vflocking
	// TODO: add your code here 
	double cSeparate = 1.0;
	double cCohesion = 1.0;
	double cAlignment = 1.0;
	
	Vdesired = cSeparate * Separation(m_pTarget, &agentList).calcDesiredVel(actor) + cCohesion * Cohesion(&agentList).calcDesiredVel(actor)
				+ cAlignment * Alignment(m_pTarget, &agentList).calcDesiredVel(actor);
	




	return Vdesired;
}

//	Leader behavior
///////////////////////////////////////////////////////////////////////////////
// For the given actor, return a desired velocity vector in world coordinates
// If the agent is the leader, move towards the target; otherwise, 
// follow the leader at a set distance behind the leader without getting to close together
//  Utilize Separation and Arrival behaviors to determine the desired velocity vector
//  You need to find the leader, who is always agents[0]

Leader::Leader( AJoint* target, vector<AActor>* agents) 
{
	m_name = "leader";
	m_AgentList = agents;
	m_pTarget = target;
}

Leader::Leader( Leader& orig) 
{
	m_name = "leader";
	m_AgentList = orig.m_AgentList;
	m_pTarget = orig.m_pTarget;
}

Leader::~Leader()
{
}

vec3 Leader::calcDesiredVel( BehaviorController* actor)
{
	
	vec3 Vdesired = vec3(0.0, 0.0, 0.0);
	vec3 actorPos = actor->getPosition();
	vector<AActor>& agentList = *m_AgentList;

	// TODO: compute Vdesired  = Vleader
	// followers should stay directly behind leader at a distance of -200 along the local z-axis

	float CSeparation = 4.0;  float CArrival = 2.0;

	BehaviorController* leader = agentList[0].getBehaviorController(); // first agent is the leader
	mat3 Rmat = leader->getGuide().getLocalRotation();  // is rotattion matrix of lead agent

	vec3 separateSum = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < (int)agentList.size(); i++) {
		double dx = actorPos[0] - agentList[i].getBehaviorController()->getPosition()[0];
		double dy = actorPos[2] - agentList[i].getBehaviorController()->getPosition()[2];
		if (dx == 0 && dy == 0) {
			continue;
		}
		double dist = sqrt(dx*dx + dy*dy);
		if (dist < BehaviorController::gKNeighborhood) {
			separateSum[0] += dx / (dist*dist);
			separateSum[2] += dy / (dist*dist);
		}
	}
	vec3 vSeparation = actor->KSeparation * separateSum;

	if (actorPos[0] == agentList[0].getBehaviorController()->getPosition()[0] &&
		actorPos[2] == agentList[0].getBehaviorController()->getPosition()[2]) {
		vec3 targetPos = m_pTarget->getLocalTranslation();
		vec3 error = targetPos - actorPos;
		vec3 vArrival = actor->KArrival * error;
		Vdesired = vArrival; // the leader should be able to arrive at the target
	}
	else {
		vec3 targetPos = leader->getPosition() - vec3(0.0, 0.0, 200.0);
		vec3 error = targetPos - actorPos;
		vec3 vArrival = actor->KArrival * error;
		Vdesired = CSeparation * vSeparation + CArrival * vArrival;
	}




	return Vdesired;
}

///////////////////////////////////////////////////////////////////////////////

