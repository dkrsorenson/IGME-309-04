#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::m_uOctantCount;
uint MyOctant::m_uMaxLevel;
uint MyOctant::m_uIdealEntityCount;

// Gets this octant's size
float Simplex::MyOctant::GetSize(void) 
{ 
	return m_fSize; 
}

// Gets the center of the octant in global scape
vector3 Simplex::MyOctant::GetCenterGlobal(void) 
{ 
	return m_v3Center; 
}

// Gets the min corner of the octant in global space
vector3 Simplex::MyOctant::GetMinGlobal(void) 
{ 
	return m_v3Min; 
}

// Gets the max corner of the octant in global space
vector3 Simplex::MyOctant::GetMaxGlobal(void) 
{ 
	return m_v3Max; 
}

// Gets the total number of octants in the world
uint Simplex::MyOctant::GetOctantCount(void) 
{ 
	return m_uOctantCount; 
}

// Returns the child specified in the index
MyOctant* Simplex::MyOctant::GetChild(uint a_nChild) 
{ 
	return m_pChild[a_nChild]; 
}

// Returns the parent of the octant
MyOctant* Simplex::MyOctant::GetParent(void) 
{ 
	return m_pParent; 
}

// If the Octant does not contain any children (its a leaf)
bool Simplex::MyOctant::IsLeaf(void)
{
	return m_uChildren == 0;
}

// Checks if the Octant contains more than this many Bounding Objects
bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	return m_EntityList.size() > a_nEntities;
}

// Constructor
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init();
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	uint numEntities = m_pEntityMngr->GetEntityCount();

	// Set start min and max before calculating
	m_v3Max = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();
	m_v3Min = m_pEntityMngr->GetRigidBody()->GetCenterGlobal();

	// Get the min and max
	for (int i = 0; i < numEntities; i++)
	{
		vector3 gMax = m_pEntityMngr->GetRigidBody(i)->GetMaxGlobal();
		vector3 gMin = m_pEntityMngr->GetRigidBody(i)->GetMinGlobal();

		if (gMax.x > m_v3Max.x) m_v3Max.x = gMax.x;
		if (gMin.x < m_v3Min.x) m_v3Min.x = gMin.x;

		if (gMax.y > m_v3Max.y) m_v3Max.y = gMax.y;
		if (gMin.y < m_v3Min.y) m_v3Min.y = gMin.y;

		if (gMax.z > m_v3Max.z) m_v3Max.z = gMax.z;
		if (gMin.z < m_v3Min.z) m_v3Min.z = gMin.z;

		m_EntityList.push_back(i);
	}

	m_v3Center = (m_v3Min + m_v3Max) * 0.5f;
	m_fSize = abs(m_v3Max.x) + abs(m_v3Min.x);

	m_pRoot = this;

	// Construct tree
	ConstructTree(m_uMaxLevel);

	// Construct list
	ConstructList();
}

// Constructor
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - m_fSize * .5f;
	m_v3Max = m_v3Center + m_fSize * .5f;
}

// Copy Constructor
Simplex::MyOctant::MyOctant(MyOctant const& other)
{
	Init();
	m_uOctantCount = other.m_uOctantCount;
	m_uMaxLevel = other.m_uMaxLevel;
	m_uIdealEntityCount = other.m_uIdealEntityCount;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_uChildren = other.m_uChildren;
	m_fSize = other.m_fSize;
	m_pMeshMngr = other.m_pMeshMngr;
	m_pEntityMngr = other.m_pEntityMngr;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
	m_pParent = other.m_pParent;

	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i] = new MyOctant(*other.m_pChild[i]);
	}

	uint numEntities = other.m_EntityList.size();
	for (uint i = 0; i < numEntities; i++)
	{
		m_EntityList.push_back(other.m_EntityList[i]);
	}

	if (this == m_pRoot) {
		float numChild = other.m_lChild.size();
		for (uint i = 0; i < numChild; i++)
		{
			m_lChild.push_back(other.m_lChild[i]);
		}
	}
}

// Copy assignemnt operator
MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	if (this != &other) {
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}
	return *this;
}

// Destructor
Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

// Swaps for other object's contents
void Simplex::MyOctant::Swap(MyOctant& other)
{
	std::swap(m_uOctantCount, other.m_uOctantCount);
	std::swap(m_uMaxLevel, other.m_uMaxLevel);
	std::swap(m_uID, other.m_uID);
	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_uChildren, other.m_uChildren);
	std::swap(m_fSize, other.m_fSize);
	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
	std::swap(m_pParent, other.m_pParent);
	std::swap(m_pChild, other.m_pChild);
	std::swap(m_EntityList, other.m_EntityList);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);
	std::swap(m_pMeshMngr, other.m_pMeshMngr);
	std::swap(m_pEntityMngr, other.m_pEntityMngr);
}

// Checks if there is a collision with the Entity specified by index from the Bounding Object Manager
bool Simplex::MyOctant::IsColliding(uint a_uRBIndex)
{
	MyRigidBody* rb = m_pEntityMngr->GetRigidBody(a_uRBIndex);
	vector3 rbMin = rb->GetMinGlobal();
	vector3 rbMax = rb->GetMaxGlobal();

	if (m_v3Max.x < rbMin.x) //this to the right of other
		return false;
	if (m_v3Min.x > rbMax.x) //this to the left of other
		return false;

	if (m_v3Max.y < rbMin.y) //this below of other
		return false;
	if (m_v3Min.y > rbMax.y) //this above of other
		return false;

	if (m_v3Max.z < rbMin.z) //this behind of other
		return false;
	if (m_v3Min.z > rbMax.z) //this in front of other
		return false;

	return true;
}

// Displays the Octant volume specified by index including the objects underneath
void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (a_nIndex >= m_uOctantCount) {
		DisplayLeafs();
		return;
	}

	if (a_nIndex < m_lChild.size()) {
		m_lChild[a_nIndex]->Display(a_v3Color);
	}
}

// Displays the Octant volume in the color specified
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) * glm::scale(IDENTITY_M4, m_v3Max - m_v3Min), a_v3Color);
}

// Displays the non empty leafs in the octree
void Simplex::MyOctant::DisplayLeafs(vector3 a_v3Color)
{
	if (IsLeaf()) {
		Display(a_v3Color);
	}
	else {
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->DisplayLeafs();
		}
	}
}

// Clears the Entity list for each node
void Simplex::MyOctant::ClearEntityList(void)
{
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->ClearEntityList();
	}

	m_EntityList.clear();
}

// Allocates 8 smaller octants in the child pointers
void Simplex::MyOctant::Subdivide(void)
{
	float fourthSize = m_fSize * 0.25f;
	float halfSize = m_fSize * 0.5f;
	m_uChildren = 8;

	m_pChild[0] = new MyOctant(m_v3Center + vector3(-fourthSize, fourthSize, fourthSize), halfSize);
	m_pChild[1] = new MyOctant(m_v3Center + vector3(-fourthSize, -fourthSize, fourthSize), halfSize);
	m_pChild[2] = new MyOctant(m_v3Center + vector3(fourthSize, -fourthSize, fourthSize), halfSize);
	m_pChild[3] = new MyOctant(m_v3Center + vector3(fourthSize, fourthSize, fourthSize), halfSize);
	m_pChild[4] = new MyOctant(m_v3Center + vector3(fourthSize, fourthSize, -fourthSize), halfSize);
	m_pChild[5] = new MyOctant(m_v3Center + vector3(fourthSize, -fourthSize, -fourthSize), halfSize);
	m_pChild[6] = new MyOctant(m_v3Center + vector3(-fourthSize, fourthSize, -fourthSize), halfSize);
	m_pChild[7] = new MyOctant(m_v3Center + vector3(-fourthSize, -fourthSize, -fourthSize), halfSize);

	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->m_pRoot = m_pRoot;
		m_pChild[i]->m_pParent = this;
	}
}

// Deletes all children and the children of their children (almost sounds apocalyptic)
void Simplex::MyOctant::KillBranches(void)
{
	for (uint i = 0; i < m_uChildren; i++) {
		m_pChild[i]->KillBranches();
		SafeDelete(m_pChild[i]);
	}
}

// Creates a tree using subdivisions, the max number of objects and levels
void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	if (a_nMaxLevel <= m_uLevel || !ContainsMoreThan(m_uIdealEntityCount)) {
		m_pRoot->m_lChild.push_back(this);
		return;
	}

	if (!IsLeaf()) return;

	Subdivide();

	for (uint i = 0; i < m_uChildren; i++)
	{
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->m_pRoot = m_pRoot;

		uint numEntities = m_EntityList.size();
		for (uint j = 0; j < numEntities; j++) {
			if (m_pChild[i]->IsColliding(m_EntityList[j])) {
				m_pChild[i]->m_EntityList.push_back(m_EntityList[j]);
			}
		}

		m_pChild[i]->ConstructTree(a_nMaxLevel);
	}
}

// Traverse the tree up to the leafs and sets the objects in them to the index
void Simplex::MyOctant::AssignIDtoEntity(void)
{
	for (uint i = 0; i < m_EntityList.size(); i++) {
		m_pEntityMngr->GetEntity(m_EntityList[i])->AddDimension(m_uID);
	}
}

// Deallocates member fields
void Simplex::MyOctant::Release(void)
{
	if (this == m_pRoot) {
		KillBranches();
	}

	m_uOctantCount = 0;
}

// Allocates member fields
void Simplex::MyOctant::Init(void)
{
	m_pEntityMngr = m_pEntityMngr->GetInstance();
	m_pMeshMngr = m_pMeshMngr->GetInstance();
	m_uID = m_uOctantCount;
	m_uOctantCount++;
}

// Creates the list of all leafs that contains objects
void Simplex::MyOctant::ConstructList(void)
{
	if (IsLeaf()) {
		AssignIDtoEntity();
	}
	else {
		for (uint i = 0; i < m_uChildren; i++)
		{
			m_pChild[i]->ConstructList();
		}
	}
}
