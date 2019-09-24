#include "AppClass.h"
void Application::InitVariables(void)
{
	// Create the meshes
	for (int i = 0; i < numberOfCubes; i++) {
		MyMesh* m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		meshList.push_back(m_pMesh);
	}

	// Full middle row
	locations.push_back(vector3(0.0f, 0.0f, 0.0f));
	locations.push_back(vector3(1.0f, 0.0f, 0.0f));
	locations.push_back(vector3(2.0f, 0.0f, 0.0f));
	locations.push_back(vector3(3.0f, 0.0f, 0.0f));
	locations.push_back(vector3(4.0f, 0.0f, 0.0f));
	locations.push_back(vector3(5.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-2.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-3.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-4.0f, 0.0f, 0.0f));
	locations.push_back(vector3(-5.0f, 0.0f, 0.0f));

	// Row with eyes
	locations.push_back(vector3(0.0f, 1.0f, 0.0f));
	locations.push_back(vector3(1.0f, 1.0f, 0.0f));
	locations.push_back(vector3(3.0f, 1.0f, 0.0f));
	locations.push_back(vector3(4.0f, 1.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 1.0f, 0.0f));
	locations.push_back(vector3(-3.0f, 1.0f, 0.0f));
	locations.push_back(vector3(-4.0f, 1.0f, 0.0f));

	// Row above eyes
	locations.push_back(vector3(0.0f, 2.0f, 0.0f));
	locations.push_back(vector3(1.0f, 2.0f, 0.0f));
	locations.push_back(vector3(2.0f, 2.0f, 0.0f));
	locations.push_back(vector3(3.0f, 2.0f, 0.0f));
	locations.push_back(vector3(-1.0f, 2.0f, 0.0f));
	locations.push_back(vector3(-2.0f, 2.0f, 0.0f));
	locations.push_back(vector3(-3.0f, 2.0f, 0.0f));

	// Little ear things
	locations.push_back(vector3(2.0f, 3.0f, 0.0f));
	locations.push_back(vector3(-2.0f, 3.0f, 0.0f));
	locations.push_back(vector3(3.0f, 4.0f, 0.0f));
	locations.push_back(vector3(-3.0f, 4.0f, 0.0f));

	// Row below full middle row 
	locations.push_back(vector3(0.0f, -1.0f, 0.0f));
	locations.push_back(vector3(1.0f, -1.0f, 0.0f));
	locations.push_back(vector3(2.0f, -1.0f, 0.0f));
	locations.push_back(vector3(3.0f, -1.0f, 0.0f));
	locations.push_back(vector3(5.0f, -1.0f, 0.0f));
	locations.push_back(vector3(-1.0f, -1.0f, 0.0f));
	locations.push_back(vector3(-2.0f, -1.0f, 0.0f));
	locations.push_back(vector3(-3.0f, -1.0f, 0.0f));
	locations.push_back(vector3(-5.0f, -1.0f, 0.0f));

	// Mouth row
	locations.push_back(vector3(3.0f, -2.0f, 0.0f));
	locations.push_back(vector3(5.0f, -2.0f, 0.0f));
	locations.push_back(vector3(-3.0f, -2.0f, 0.0f));
	locations.push_back(vector3(-5.0f, -2.0f, 0.0f));

	// Bottom row
	locations.push_back(vector3(1.0f, -3.0f, 0.0f));
	locations.push_back(vector3(2.0f, -3.0f, 0.0f));
	locations.push_back(vector3(-1.0f, -3.0f, 0.0f));
	locations.push_back(vector3(-2.0f, -3.0f, 0.0f));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	static float value = 0.0f;
	static int currentDirection = 1;
	static bool switchedDirection = false;
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(5.0, 5.0f, 5.0f));

	// Draw the meshes
	for (int i = 0; i < numberOfCubes; i++) {
		// Translate the model
		matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(locations[i].x + value, locations[i].y, -15.0f));
		matrix4 m4Model = m4Scale * m4Translate;

		// Render the mesh
		meshList[i]->Render(m4Projection, m4View, m4Model);
	}

	// Switch the direction once it reaches this value on the screen
	if ((value > 10.0 || value < -10.0) && !switchedDirection) {
		currentDirection *= -1;
		switchedDirection = true;
	}
	else if (value < 5.0 && value > -5.0) {
		switchedDirection = false;
	}

	// Increment the movement value
	value += 0.05f * currentDirection;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//if (m_pMesh != nullptr)
	//{
	//	delete m_pMesh;
	//	m_pMesh = nullptr;
	//}
	//SafeDelete(m_pMesh1);

	for each (MyMesh* mesh in meshList)
	{
		if (mesh != nullptr) {
			delete mesh;
			mesh = nullptr;
		}
		SafeDelete(mesh);
	}

	//release GUI
	ShutdownGUI();
}