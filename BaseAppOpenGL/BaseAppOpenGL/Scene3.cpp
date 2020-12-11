#include "Scene3.h"


CScene3::CScene3()
{

	pCamera = NULL;
	pTexto = NULL;
	pTextures = NULL;

	pModel3DS_3 = NULL;

	bIsWireframe = false;
	bIsCameraFPS = true;
	fog = false;

	iFPS = 0;
	iFrames = 0;
	ulLastFPS = 0;
	szTitle[256] = 0;

	// Cria gerenciador de impress�o de texto na tela
	pTexto = new CTexto();

	// Cria camera
	pCamera = new CCamera(10.0f, 20.0f, 20.0f);

	// Cria o Timer
	pTimer = new CTimer();
	pTimer->Init();

	fTimerPosY = 0.0f;
	fRenderPosY = 0.0f;

	// Carrega todas as texturas
	pTextures = new CTexture();
	pTextures->CreateTextureClamp(0, "../Scene1/back.bmp");
	pTextures->CreateTextureClamp(1, "../Scene1/front.bmp");
	pTextures->CreateTextureClamp(2, "../Scene1/down.bmp");
	pTextures->CreateTextureClamp(3, "../Scene1/up.bmp");
	pTextures->CreateTextureClamp(4, "../Scene1/left.bmp");
	pTextures->CreateTextureClamp(5, "../Scene1/right.bmp");

	pTextures->CreateTextureMipMap(6, "../Scene1/crate.bmp");
	pTextures->CreateTextureMipMap(7, "../Scene1/grass.bmp");
	pTextures->CreateTextureLinear(8, "../Scene1/pl_earth_full.jpg");
	pTextures->CreateTextureTGA(9, "../Scene3/link.tga");
	pTextures->CreateTextureTGA(10, "../Scene3/arvore2.tga");



	fPosX = 0.0f;
	fPosY = 10.0f;
	fPosZ = 0.0f;
	fMovementFactor = 0.1f;


	// Carrega Objetos da  Cena (gramado)
	pModel3DS_3 = new CModel_3DS();
	pModel3DS_3->Load("../Scene3/montanhaPlano1.3ds");
	// Carrega Objetos da  Cena (Pedra maior)
	pModel3DS_1 = new CModel_3DS();
	pModel3DS_1->Load("../Scene3/pedraMaior.3ds");
	// Carrega Objetos da  Cena (Pedra menor)
	pModel3DS_2 = new CModel_3DS();
	pModel3DS_2->Load("../Scene3/pedraMenor.3ds");
	
	pModel3DS_4 = new CModel_3DS();
	pModel3DS_4->Load("../Scene3/arvore.3ds");


	


	fFogColor[0] = 0.5f;
	fFogColor[1] = 0.5f;
	fFogColor[2] = 0.5f;
	fFogColor[3] = 0.5f;
}


CScene3::~CScene3(void)
{
	if (pTexto)
	{
		delete pTexto;
		pTexto = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}


	if (pModel3DS_3)
	{
		delete pModel3DS_3;
		pModel3DS_3 = NULL;
	}
}




int CScene3::DrawGLScene(void)	// Fun��o que desenha a cena
{
	// Get FPS
	if (GetTickCount() - ulLastFPS >= 1000)	// When A Second Has Passed...
	{
		ulLastFPS = GetTickCount();				// Update Our Time Variable
		iFPS = iFrames;							// Save The FPS
		iFrames = 0;							// Reset The FPS Counter
	}
	iFrames++;									// FPS counter

	pTimer->Update();							// Atualiza o timer

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Limpa a tela e o Depth Buffer
	glLoadIdentity();									// Inicializa a Modelview Matrix Atual


	// Seta as posi��es da c�mera
	pCamera->setView();

	// Desenha grid 
	//Draw3DSGrid(20.0f, 20.0f);

	// Modo FILL ou WIREFRAME (pressione barra de espa�o)	
	if (bIsWireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (IN�CIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	glColor3f(1.0f, 1.0f, 1.0f);

	// Habilita mapeamento de texturas 2D
	glEnable(GL_TEXTURE_2D);
	if (fog) {

		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, fFogColor);
		glFogf(GL_FOG_START, 30.0f);
		glFogf(GL_FOG_END, 120.0f);
		glFogi(GL_FOG_MODE, GL_LINEAR);
	}
	else {
		glEnable(GL_FOG);
		glFogfv(GL_FOG_COLOR, fFogColor);
		glFogf(GL_FOG_START, 1.0f);
		glFogf(GL_FOG_END, 0.0f);
		glFogi(GL_FOG_MODE, GL_LINEAR);
	}


	// Desenha o SkyBox
	CreateSkyBox(0.0f, 100.0f, 0.0f,
		1000.0f, 1000.0f, 1000.0f,
		pTextures);

	// Desenha chao e montanha
	glPushMatrix();
	pModel3DS_3->Draw();
	glPopMatrix();

	// Desenha pedras
	DrawBiggerStone(25.0f, 10.0f, -15.0f);
	DrawSmallerStone(-10.0f, 10.0f, 16.0f);
	DrawSmallerStone(-20.0f, 10.0f, 16.0f);
	DrawSmallerStone(-30.0f, 10.0f, 16.0f);


	// Desenha objetos aplicando Blending.
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.95f);
	// desenhas arvores
	DrawTree(-50.0f, 10.0f, -20.0f);
	DrawTree(-30.0f, 10.0f, 16.0f);
	DrawTree(-50.0f, 10.0f, 16.0f);
	DrawTree(-50.0f, 10.0f, 30.0f);
	DrawTree(15.0f, 10.0f, 20.0f);
	DrawTree(15.0f, 10.0f, -30.0f);
	DrawTree(-20.0f, 10.0f, 28.0f);
	DrawTree(-20.0f, 10.0f, -25.0f);
	DrawTree(-35.0f, 10.0f, -25.0f);
	DrawTree(-20.0f, 10.0f, -40.0f);
	DrawTree(-60.0f, 10.0f, 50.0f);
	DrawTree(-55.0f, 10.0f, -40.0f);
	DrawTree(-30.0f, 10.0f, -50.0f);
	DrawTree(-60.0f, 10.0f, -60.0f);
	// desenha link
	DrawLink(15.0f, 15.0f, 40.0f);
	
	glPushMatrix();
	glTranslatef(-10.0f, -35.0f, 15.0f);
	pModel3DS_4->Draw();
	glPopMatrix();
	

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glDisable(GL_FOG);

	glDisable(GL_TEXTURE_2D);


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	fTimerPosY = pTimer->GetTime() / 1000.0f;
	fRenderPosY += 0.2f;

	// Impress�o de texto na tela...
	// Muda para modo de proje��o ortogonal 2D
	// OBS: Desabilite Texturas e Ilumina��o antes de entrar nesse modo de proje��o
	OrthoMode(0, 0, WIDTH, HEIGHT);


	glPushMatrix();
	glTranslatef(0.0f, HEIGHT - 100, 0.0f);	// Move 1 unidade para dentro da tela (eixo Z)

	// Cor da fonte
	glColor3f(1.0f, 1.0f, 0.0f);


	glRasterPos2f(10.0f, 0.0f);	// Posicionando o texto na tela
	if (!bIsWireframe) {
		pTexto->glPrint("[TAB]  Modo LINE"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[TAB]  Modo FILL");
	}
	if (!fog) {
		pTexto->glPrint("[N]  Sem neblina"); // Imprime texto na tela
	}
	else {
		pTexto->glPrint("[N]  Com  neblina");
	}


	//// Camera LookAt
	glRasterPos2f(10.0f, 40.0f);
	pTexto->glPrint("Player LookAt  : %f, %f, %f", pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);

	//// Posi��o do Player
	glRasterPos2f(10.0f, 60.0f);
	pTexto->glPrint("Player Position: %f, %f, %f", pCamera->Position[0], pCamera->Position[1], pCamera->Position[2]);

	//// Imprime o FPS da aplica��o e o Timer
	glRasterPos2f(10.0f, 80.0f);
	pTexto->glPrint("Frames-per-Second: %d ---- Timer: %.1f segundos", iFPS, (pTimer->GetTime() / 1000));


	glPopMatrix();

	// Muda para modo de proje��o perspectiva 3D
	PerspectiveMode();

	return true;
}

void CScene3::DrawBiggerStone(float pX, float pY, float pZ) {
	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	pModel3DS_1->Draw();
	glPopMatrix();
}
void CScene3::DrawSmallerStone(float pX, float pY, float pZ) {
	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	pModel3DS_2->Draw();
	glPopMatrix();
}

void CScene3::DrawTree(float pX, float pY, float pZ) {
	pTextures->ApplyTexture(10);
	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-10.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(10.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(10.0, 20.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-10.0, 20.0, 0.0);

	glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0, 0.0, 10.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(0.0, 0.0, -10.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(0.0, 20.0, -10.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0, 20.0, 10.0);
	glEnd();
	glPopMatrix();
}
void CScene3::DrawLink(float pX, float pY, float pZ) {
	pTextures->ApplyTexture(9);
	glPushMatrix();
	glTranslatef(pX, pY, pZ);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-3.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(3.0, 0.0, 0.0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(3.0, 10.0, 0.0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-3.0, 10.0, 0.0);
	glEnd();
	glPopMatrix();
}

void CScene3::MouseMove(void) // Tratamento de movimento do mouse
{
	// Realiza os c�lculos de rota��o da vis�o do Player (atrav�s das coordenadas
	// X do mouse.
	POINT mousePos;
	int middleX = WIDTH >> 1;
	int middleY = HEIGHT >> 1;

	GetCursorPos(&mousePos);

	if ((mousePos.x == middleX) && (mousePos.y == middleY)) return;

	SetCursorPos(middleX, middleY);

	fDeltaX = (float)((middleX - mousePos.x)) / 10;
	fDeltaY = (float)((middleY - mousePos.y)) / 10;

	// Rotaciona apenas a c�mera
	pCamera->rotateGlob(-fDeltaX, 0.0f, 1.0f, 0.0f);
	pCamera->rotateLoc(-fDeltaY, 1.0f, 0.0f, 0.0f);
}

void CScene3::KeyPressed(void) // Tratamento de teclas pressionadas
{

	// Verifica se a tecla 'W' foi pressionada e move o Player para frente
	if (GetKeyState('W') & 0x80)
	{
		pCamera->moveGlob(pCamera->Forward[0], pCamera->Forward[1], pCamera->Forward[2]);
	}
	// Verifica se a tecla 'S' foi pressionada e move o Player para tras
	else if (GetKeyState('S') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Forward[0], -pCamera->Forward[1], -pCamera->Forward[2]);
	}
	// Verifica se a tecla 'A' foi pressionada e move o Player para esquerda
	else if (GetKeyState('A') & 0x80)
	{
		pCamera->moveGlob(-pCamera->Right[0], -pCamera->Right[1], -pCamera->Right[2]);
	}
	// Verifica se a tecla 'D' foi pressionada e move o Player para direira
	else if (GetKeyState('D') & 0x80)
	{
		pCamera->moveGlob(pCamera->Right[0], pCamera->Right[1], pCamera->Right[2]);
	}
	// Sen�o, interrompe movimento do Player
	else
	{
	}


	if (GetKeyState(VK_UP) & 0x80)
	{
		fPosZ -= fMovementFactor;
	}
	if (GetKeyState(VK_DOWN) & 0x80)
	{
		fPosZ += fMovementFactor;
	}
	if (GetKeyState(VK_LEFT) & 0x80)
	{
		fPosX -= fMovementFactor;
	}
	if (GetKeyState(VK_RIGHT) & 0x80)
	{
		fPosX += fMovementFactor;
	}
	if (GetKeyState(VK_PRIOR) & 0x80)
	{
		fPosY += fMovementFactor;
	}
	if (GetKeyState(VK_NEXT) & 0x80)
	{
		fPosY -= fMovementFactor;
	}
	
}

void CScene3::KeyDownPressed(WPARAM	wParam) // Tratamento de teclas pressionadas
{
	switch (wParam)
	{
		case VK_TAB:
			bIsWireframe = !bIsWireframe;
			break;

		case VK_SPACE:
			pTimer->Init();

		case 0x4E:
			fog = !fog;
			break;
	}
}

//	Cria um grid horizontal ao longo dos eixos X e Z
void CScene3::Draw3DSGrid(float width, float length)
{

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0f, 0.3f, 0.0f);
	glPushMatrix();
	for (float i = -width; i <= length; i += 1)
	{
		for (float j = -width; j <= length; j += 1)
		{
			// inicia o desenho das linhas
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(i, 0.0f, j + 1);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(i + 1, 0.0f, j + 1);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(i + 1, 0.0f, j);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(i, 0.0f, j);
			glEnd();
		}
	}
	glPopMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


void CScene3::CreateSkyBox(float x, float y, float z,
	float width, float height, float length,
	CTexture* pTextures)
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();

	// Centraliza o Skybox em torno da posi��o especificada(x, y, z)
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Aplica a textura que representa a parte da frente do skybox (BACK map)
	pTextures->ApplyTexture(0);

	// Desenha face BACK do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (FRONT map)
	pTextures->ApplyTexture(1);

	// Desenha face FRONT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (DOWN map)
	pTextures->ApplyTexture(2);

	// Desenha face BOTTOM do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (UP map)
	pTextures->ApplyTexture(3);

	// Desenha face TOP do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (LEFT map)
	pTextures->ApplyTexture(4);

	// Desenha face LEFT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glEnd();


	// Aplica a textura que representa a parte da frente do skybox (RIGHT map)
	pTextures->ApplyTexture(5);

	// Desenha face RIGHT do cubo do skybox
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	glPopMatrix();
}