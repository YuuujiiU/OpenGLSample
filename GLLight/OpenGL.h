// OpenGL.h: interface for the COpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGL_H__D2CFD1C5_EB19_4006_AAB2_9A624D43E313__INCLUDED_)
#define AFX_OPENGL_H__D2CFD1C5_EB19_4006_AAB2_9A624D43E313__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "gl\gl.h"
#include "gl\glu.h"
#include "gl\glaux.h"
#include "gl\glut.h"
#include "gl\glext.h"
#include "math.h"
#include <iostream>
#include <vector>
using namespace std;
//********************************************************
//ʹ�÷�����
/*
һ������
1 ��COpenGL �м̳�һ���࣬���磺CmyOpenGL
2 ��CXXXView������CmyOpenGL��һ��ʵ������m_gl
3 ��CXXXView�� OnCreate �м�����룺
		if(!m_gl.Init(this->GetSafeHwnd()))
			AfxMessageBox("error!");
4 ��CXXXView�� OnSize �м�����룺
		m_gl.OnSize(cx,cy);
5 ��CXXXView�� OnDraw �м�����룺
		m_gl.OnPaint();
6 ��CXXXView�� OnEraseBkgnd(CDC* pDC) �м�����룺�����ɲ�������
		��Ϊreturn TRUE;
�����ӵ����
7 ��CXXXView�� PreTranslateMessage �м�����룺
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP ||pMsg->message == WM_RBUTTONDOWN ||pMsg->message == WM_RBUTTONUP ||pMsg->message == WM_LBUTTONDBLCLK ||pMsg->message == WM_RBUTTONDBLCLK||pMsg->message==WM_MOUSEMOVE||pMsg->message==WM_MOUSEWHEEL)
		m_gl.OnMouse(pMsg->message,pMsg->wParam,pMsg->lParam);
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP || pMsg->message == WM_CHAR)
		m_gl.OnKey(pMsg->message,pMsg->wParam);
Ĭ�Ͽ���Ϊ
�����������ƶ� ����ƽ���ӵ㣬����Ctrl+˫������仯�ƶ���ʽ
����Ҽ������ƶ� ������ת�ӵ㣬����Ctrl+˫���Ҽ��仯��ת��ʽ
���̿���Ϊ
�ƶ���AaDd--X�᷽��  WwSs--Y�᷽�� QqEe--Z�᷽�� ZzCc--FN�᷽�� 
��ת��IiKk--X�᷽��  JjLl--Y�᷽�� UuOo--Z�᷽��
���У�����ת�仯�У����Shift������������ӵ�������ת�������ƹ̶�����ת(Ĭ��0,0,0��)
����״̬����ʾ�ӵ�
8 ��CMainFrame���趨��indicators[]��Ϊ
	static UINT indicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_SEPARATOR,
		ID_SEPARATOR,
		ID_SEPARATOR,
		ID_SEPARATOR
	};
9 ��CMainFrame�� OnCreate ��������룺
	g_MyGL.SetStatus(&m_wndStatusBar);
11 ��CMainFrame.cpp�а���#include "myOpenGL.h"

���ӣ�
virtual BOOL PostInit(void);
virtual BOOL InDraw(void);

BOOL CmyOpenGL::PostInit(void)
{
	glClearColor(0.6f,0.6f,0.9f,0.0f);
	SetInitView(35,25,50,0,0,0,TRUE);
	float a[4]={0.9,0.9,0.9,1},r[4]={1,1,1,1};
	SetLight(TRUE,a,r);
	SetControlRoam(FALSE);
	LoadModel();
	//SetMode(1);
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	return TRUE;
}
BOOL CmyOpenGL::InDraw(void)
{
	return TRUE;
}

*/
//********************************************************
//����ת�Ƕ�
#define RadToAng(x) ((x)/3.14159*180)
//�Ƕ�ת����
#define AngToRad(x) ((x)/180.0*3.14159)
#define  BYTE unsigned char
#define  WORD unsigned short
#define  UINT unsigned int
#define TGA_RGB		 2		// This tells us it's a normal RGB (really BGR) file
#define TGA_A		 3		// This tells us it's a ALPHA file
#define TGA_RLE		10		// This tells us that the targa is Run-Length Encoded (RLE)

#include "mmsystem.h"
#include <afxctl.h>
#include "assert.h"
#define g_MyGL COpenGL::GetSingleton()
template <typename T> class Singleton
{
	static T* ms_Singleton;
public:
	Singleton(void)
	{
		assert(!ms_Singleton);
		int offset=(int)(T*)1-(int)(Singleton <T>*)(T*)1;
		ms_Singleton=(T*)((int)this +offset);
	}
	~Singleton()
	{
		assert(ms_Singleton);
		ms_Singleton=0;
	}
	static T& GetSingleton(void)
	{
		assert(ms_Singleton);
		return *ms_Singleton;
	}
	static T* GetSingletonPtr(void)
	{
		return ms_Singleton;
	}
};
template <typename T> T* Singleton <T>::ms_Singleton=0;
class COpenGL:public Singleton <COpenGL>
{
//////////////////////////////////////////////////////////////////////////
//�̳����л�ʹ�õ��ĺ���
//////////////////////////////////////////////////////////////////////////
public:
	virtual BOOL OnViewChange(void);///�ӵ�仯ʱ���ã��̳У�������ʱû����
	virtual BOOL OnViewActualChange(void);///�ӵ�仯����ִ��OnView����ã��̳У�
	BOOL DrawCoord(float x,float y,float z,float len,BOOL bMark=TRUE,int *Color=0);//�������ᡣ(x,y,z)λ�ã�����len���Ƿ�д��bMark
	BOOL DrawArrow(GLfloat x,GLfloat y,GLfloat z,GLfloat xend,GLfloat yend,GLfloat zend,INT color);//����ͷ��(x,y,z)��ʼλ�ã�(xend,yend,zend)����λ�ã�color��ɫ
	VOID SetMode(int mode);//���û��Ʒ���(0��1��2��)Ҳ����ֱ��ʹ��opengl���ʵ�֡�
	VOID SetLight(BOOL bLight,float *ambient=NULL,float *diffuse=NULL,float *specular=NULL,float *position=NULL);
	//control����
	virtual BOOL OnKeyDown(UINT nChar){return TRUE;};
	BOOL SetRotateType(CString type);//������ת����
	BOOL SetMoveType(CString type);//�����ƶ�����
	VOID SetControlRoam(BOOL bRoam);//���ÿ���ģʽ��ΪTRUE ��ʾ����ʱ�������Σ�ΪFALSE��ʾ����ʱ���ù۲졣
	//font����
	BOOL Font3D(CString str,int *color=NULL);//д3D��
	BOOL Font2D(CString str,float x,float y,int size=3);//д2D��
	//view����
	BOOL SetStep(float mstep,float sstep);//�����ӵ��ƶ��ٶ�Ĭ��0.1,0.1
	BOOL GetStep(float &mstep,float &sstep);//�����ӵ��ƶ��ٶ�	
	BOOL SetView(GLfloat x,GLfloat y,GLfloat z,GLfloat h,GLfloat p,GLfloat r);//�����ӵ�λ�ò���Ϊ���Է�ʽ�۲�
	BOOL SetView(GLfloat m[], GLfloat r[]);
	BOOL SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt);//���ó�ʼ�ӵ�λ�÷���
	BOOL SetInitStep(GLfloat m,GLfloat s);//���ó�ʼ�ӵ㲽��	
	BOOL SetLookAt(float x, float y, float z, float tx, float ty, float tz);//�趨�ӵ�۲�ĳһ������Ϊ��Է�ʽ�۲�
	BOOL SetLookAt(float tx, float ty, float tz);//ͬ��
	BOOL SetLookAt(bool bLookAt);//�趨���Թ۲컹����Թ۲�
	BOOL RestoreView();						//�ָ���ʼ�ӵ�
	BOOL RestoreStep();//�ָ���ʼ����
	BOOL GetView(float pos[],float dir[]);//�õ��ӵ�λ�úͷ���
	BOOL GetLookCenter(float center[]);//�õ�����
	BOOL GetViewVector(float vec[]);	//�õ��ӷ�������
	float *GetViewMatrix();//�õ��ӵ�任����
	//frus����
	float *CalculateClip();
	//��������
	static BOOL Normalize(float normal[]);			//��׼��ʸ��
	static BOOL CalculateNormal(float v1[],float v2[],float v3[],float normal[]);//���������淽������		
	static BOOL Normalize(double normal[]);			//��׼��ʸ��
	static BOOL CalculateNormal(double v1[],double v2[],double v3[],double normal[]);//���������淽������		
	
	static double InverseMatrix(double *imat, double *mat);//��mat����������imat;��������ʽ��ֵ��
	static double InverseMatrix(float *imat, float *mat);//��mat����������imat;��������ʽ��ֵ��

	void GetModelMatrix(float mat[]);
protected:
	virtual BOOL PostInit(void);	//��ʼ������Ҫ����ִ�е�һЩ��������Ҫ�̳У�
	virtual BOOL InDraw(void);		//ֱ�ӻ��Ƴ�����������Ҫ�̳У�
	virtual BOOL ShowBeforeView();	//�ӵ�任֮ǰ�����
//////////////////////////////////////////////////////////////////////////
//����
//////////////////////////////////////////////////////////////////////////		
protected:
	class View//�����ӵ����
	{
	public:
		View();
		virtual ~View();
		BOOL ShowTriNum(int num);
		BOOL SetStatus(CStatusBar *pStatus);
		BOOL ShowFPS(float fps);
		BOOL ShowView();
		BOOL SetStep(float mstep,float sstep);//�����ӵ��ƶ��ٶ�
		BOOL GetStep(float &mstep, float &sstep);//ȡ���ӵ��ƶ��ٶ�
		BOOL MoveView(int sw,int len);//�ƶ��ӵ�sw(0x,1y,2z),len(�ƶ�����)
		BOOL MoveViewRel(int sw, int len);//�ƶ�����ӵ�sw(0lr,1ud,2fb),len(�ƶ�����)
		BOOL RotateView(int sw,int len);//��ת�ӵ�sw(0x,1y,2z),len(�ƶ�����)
		BOOL MoveViewDis(int len);//�ӵ�Զ���ƶ�
		BOOL SetView(GLfloat x,GLfloat y,GLfloat z,GLfloat h,GLfloat p,GLfloat r);//�����ӵ�λ��
		BOOL SetLookAt(float x, float y, float z, float tx, float ty, float tz);
		BOOL SetLookAt(float tx, float ty, float tz);
		BOOL SetLookAt(BOOL bLookAt);
		BOOL SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt);//���ó�ʼ�ӵ�λ�÷���
		BOOL SetInitStep(GLfloat m,GLfloat s);//���ó�ʼ�ӵ㲽��
		BOOL RestoreView();//�ָ���ʼ�ӵ�
		BOOL RestoreStep();//�ָ���ʼ����
		GLfloat m_moveView[3];	//�ӵ�λ��xyz
		GLfloat m_rotateView[3];//�ӵ㷽���hpr
		GLfloat m_rotateVector[3];//�ӵ㷽������
		GLfloat m_fLookTarget[3];
		GLfloat m_viewMatrix[16];//��ͼ����
		GLfloat m_RotateMatrix[16];//��ת����		
	protected:
		//�����ӵ��ƶ���
		GLfloat m_upVector[3];
		GLfloat m_moveStep;
		GLfloat m_rotateStep;
		BOOL m_bLookTarget;
		CStatusBar *m_pStatus;//״̬��ָ��
		BOOL SetView();
		BOOL m_bLookTargetHaveMove;
		FLOAT m_InitView[6];
		FLOAT m_InitStep[2];		
		BOOL m_InitViewType;
	}view;
	class Control //�����̿��Ʋ���
	{
	public:	
		BOOL OnMouse(UINT message,UINT wParam,UINT lParam);
		BOOL OnKey(UINT message,UINT nChar);
		BOOL AutoMove();
		BOOL SetRotateType(CString type);
		BOOL SetMoveType(CString type);
		BOOL SetParm(HWND hWnd,View *p);
		VOID SetControlRoam(BOOL bRoam);//���ÿ���ģʽ��ΪTRUE ��ʾ����ʱ�������Σ�ΪFALSE��ʾ����ʱ���ù۲졣		
		Control();
		virtual ~Control();
	protected:
		BOOL OnKeyUp(UINT nChar);
		BOOL OnKeyDown(UINT nChar);
		BOOL SwitchType(BOOL bMove);
		BOOL OnMoveFarNear(short len);//��Ӧ������
		BOOL OnMoveRotate(UINT xPos,UINT yPos,UINT fwKeys);//��Ӧ����ƶ�
		BOOL RotateEnd();//����Ҽ��ɿ�
		BOOL RotateBegin(UINT lParam);//����Ҽ�����
		BOOL MoveEnd();//�������ɿ�
		BOOL MoveBegin(UINT lParam);//����������
		View *m_pView;
		BOOL m_bMove;
		BOOL m_bRotate;
		enum{MV_X,MV_Y,MV_Z,MV_XY,MV_XZ,MV_FN}m_iMoveType;
		enum{MR_X,MR_Y,MR_Z,MR_XY,MR_XZ}m_iRotateType;
		HWND m_hWnd;
		BOOL m_keyMX,m_keyMY,m_keyMZ,m_keyRH,m_keyRP,m_keyRR,m_keyFN;
		BOOL m_keyChar;
		BOOL m_bShift;
		BOOL m_bControlRoam;
		INT  m_iMouseOldPosX,m_iMouseOldPosY;
	}control;
	class FontGL//�����������
	{
	public:
		BOOL Font3D(CString str);
		FontGL();
		virtual ~FontGL();
		BOOL Font2D(CString str,float x,float y,int size=3);
	private:
		BOOL Create3DFont();
	private:
		int m_iFormat ;
		float m_fDeviation ;
		float m_fExtrusion ;
		CFont* m_pFontSelected ;

		int m_iFormatDefault ;
		float m_fDeviationDefault ;
		float m_fExtrusionDefault ;

		LOGFONT m_logFontDefault ;
		CFont m_FontDefault ;	

		int m_iFirstChar ;
		int m_iLastChar ;
		int m_iNumberChars ;
		int m_iDisplayListStart ;
		int m_iDisplayListBase ;
		bool m_bHaveCreate3DFont;
	}font;
public:
	class CShape
	{
	public:
		static void DrawBall(float rad,int lon,int lat);//�뾶�����ȣ�γ��
	}shape;
	class CFrustum  
	{
	public:
		enum FrustumSide
		{
			RIGHT		= 0,		// The RIGHT side of the frustum
			LEFT		= 1,		// The LEFT	 side of the frustum
			BOTTOM		= 2,		// The BOTTOM side of the frustum
			TOP			= 3,		// The TOP side of the frustum
			BACK		= 4,		// The BACK	side of the frustum
			FRONT		= 5			// The FRONT side of the frustum
		};
		enum PlaneData
		{
			A = 0,				// The X value of the plane's normal
			B = 1,				// The Y value of the plane's normal
			C = 2,				// The Z value of the plane's normal
			D = 3				// The distance the plane is from the origin
		};
		CFrustum();
		virtual ~CFrustum();		
		float m_Frustum[6][4];		//�ü�ƽ��
		float m_Clip[16];			//�ü�����
		float CalculateDistance(float v[]);//����ռ����������׶��ľ���
		int CalculatePosition(float v[]);//����ռ������׶����Ǹ�λ��				
		int CalculateBoundingBox(float v[]);//�����Χ���Ƿ�����׶����v[6]���� x(min,max); y(min,max); z(min,max).����ֵ-1��ʾ������׶���У�0��ʾ��������׶���У�1��ʾ��ȫ����׶���С�
		int ConvertPosition(float v[]);//�ѿռ��ת�����ü�����ϵ��
		float* CalculateClip();//����ü�����
	private:
		void CalculateFrustum();
		void NormalParam(float param[]);

	}frus;
public:
	struct ImageData
	{
		int m_Width;
		int m_Height;
		int m_nColor;
		unsigned char *m_pData;
		ImageData()
		{
			m_nColor=0;
			m_Width=0;
			m_Height=0;
			m_pData=NULL;
		}
		~ImageData()
		{
			if(m_pData)
				delete m_pData;
			m_pData=NULL;
		}
	};
	class CPictureLoad: public CPictureHolder  
	{
	private:
		VOID Init();
		BOOL Load(CString sFilePathName);//���ļ���ȡͼ��
		BOOL Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType);//����Դ��ȡͼ��
		void Render(CDC* pDC, LPRECT pDrawRect/*Ŀ����Σ���λ���߼����굥λ*/
			,LPRECT pSrcRect=NULL/*��Դ���Σ���λ��0.01����,���Ϊ�գ�����������ͼ��Ŀ�����*/
			,LPCRECT prcWBounds=NULL/*ͼԪ�ļ�ר�ã��󶨾���*/
			);//�ڸ�����DC�ϻ�ͼ��
		BOOL LoadPictureData(BYTE* pBuffer, int nSize);//���ڴ��ȡͼ��
		BOOL SaveAsBitmap(CString sFilePathName);//д�뵽BMP�ļ�
		void UnloadPicture();//�ͷ�ͼ������ͬCPictureHolder::~CPictureHolder()
		LONG get_Height(); // ��0.01����Ϊ��λ��ͼ��߶�
		LONG get_Width(); // ��0.01����Ϊ��λ��ͼ����
		INT GetWidth();
		INT GetHeight();
		VOID CalculateWidthHeight();
		CBitmap m_memBMP;
		int m_Width,m_Height;
		int m_PixelToMillimeterY;
		int m_PixelToMillimeterX;
		CDC m_memDC;
		HDC m_hdc;
		bool m_bInit;
		void * GetTGAData(CString sFilePathName);
		BOOL m_bTransparence;//͸��ɫ����
		int m_iTransparence;//͸��ɫ
	public:
		void * GetData(CString sFilePathName,int *color=0);
		CPictureLoad();
		virtual ~CPictureLoad();	
	}picLoader;//��ͼƬ����
	class CScene
	{
	public:
		CScene();
		virtual ~CScene();
		//�ඨ��
	public:
		union INDEX{
			struct{
				int u,v,n;
			};
			int a[3];
		};
		union VECTOR{
			struct{
				float u,v,n;
			};
			float a[3];
		};
		union MAPCOORD{
			struct{
				float u,v;
			};
			float a[2];
		};
		struct FACEGROUP
		{
			UINT m_iMAT;//����
			UINT texture;//����
			INT m_color;//��ɫ
		};
		struct MAT
		{
			CString name;//��������
			float mat_ambient[4];//������
			float mat_diffuse[4];//ɢ���
			float mat_specular[4];//���淴���
			float mat_emission[4];//����⡪��δ��
			float mat_shininess[1];//�����ȡ���δ��
			UINT tex_index;//����ͼ������
			MAT()
			{
				for(int i=0;i<3;i++)
				{
					mat_ambient[i]=0;
					mat_diffuse[i]=0;
					mat_specular[i]=0;
					mat_emission[i]=0;
				}
				mat_ambient[3]=1;
				mat_diffuse[3]=1;
				mat_specular[3]=1;
				mat_emission[3]=1;
				mat_shininess[0]=75;
				tex_index=-1;
			}
		};
		struct MATMAP {
			CString mapname;//����ͼ����
			UINT texture;//�󶨵�����
			MATMAP()
			{
				texture=0;
			}
		};
		struct MAT_INDEX
		{
			MAT_INDEX()
			{
				mat_index = -1;
				tex_index=-1;
				color = -1;
			}
			~MAT_INDEX()
			{
				facets_index.clear();
			}
			int mat_index;
			int tex_index;//�������
			int color;//��ɫֵ,�������-1��ʾû����ɫֵ
			vector<int> facets_index;//���б�			
		};
		struct NORMAL
		{
			float a[3];
			BOOL bValid;
			NORMAL()
			{
				a[0]=a[1]=a[2]=0;
				bValid = TRUE;
			}
		};
		struct GROUP{
			GROUP()
			{
				isOn = TRUE;
			}
			~GROUP()
			{
				vertices.clear();
				facets_normal.clear();
				vertices_normal.clear();
				mapcoord.clear();
				facets.clear();
				material_index.clear();
			}
			void SetOn(bool bOn)
			{
				isOn = bOn;
			}
		   	CString name;//����
			vector<VECTOR> vertices;//���б�
			vector<NORMAL> facets_normal;//�淨������
			vector<NORMAL> vertices_normal;//�㷨�������б�
			vector<MAPCOORD> mapcoord;//��������
			vector<INDEX> facets;//���б�
			vector<MAT_INDEX> material_index;//�����б�
			bool isOn;
		};
		//��������
		public:
			UINT LoadTextureMap(MATMAP &map,CString path);//����������ͼ
			UINT LoadTextureMap(CString name,CString path);//����������ͼ
			UINT LoadTextureCubeMap(CString name[],CString path);//����������ͼ
			void Draw(int &numTriangle);
			void render();
			void Reset();
			void SetInitMatrix(float matrix[]);
			void Init(float* matrix=NULL,BOOL bList = TRUE);
			float *GetInitMatrix();
			void SetTriNum(int tri);
			int GetTriNum();
			void SetListNo(UINT List);
			UINT GetListNo();
			vector<GROUP> groups;//������
			vector<MAT> materials;//�����б�
			vector<MATMAP> maps;//��������ͼ�б�
			vector<VECTOR> vertices;//���б�����Ӧ����ͳһ�ĵ��б�
			vector<NORMAL> vertices_normal;//�㷨�������б�
			CString m_path;			
		protected:
			bool CreateNormal(BOOL bVertice=TRUE);//������ķ�������
			bool UseShowList();//������ʾ�б�
			void GetDirect(const char *str,CString &path,CString &pathPre);
			UINT SetTextureMap(ImageData *TextureImage);
		protected:
			int iTri;//��ǰ����Ƭ��
			float *m_InitMatrix;
			UINT m_uList;
			BOOL m_bList;
	};

public:
	//scene����
	CScene * GetScene(int n);	
	int CreateScene(int n=1);
	int GetSceneNum();
	BOOL DrawScene(int &numTri,int start=0,int end = -1);
	VOID SetStatus(CStatusBar *pStatus);	//����״̬����Ϣ
	virtual BOOL End();						//��OpenGL��ֹ���ͷŷ�����������
	virtual BOOL OnPaint();					//����
	virtual BOOL OnSize(int w,int h);		//����ͶӰ��ʽ��ģ����ͼ����ȱʡ����һ���Ӵ�
	virtual BOOL Init(HWND hWnd,BOOL bOnly = TRUE);//��ʼ��
	COpenGL();
	virtual ~COpenGL();	

	BOOL OnMouse(UINT message,UINT wParam,UINT lParam);
	BOOL OnKey(UINT message,UINT nChar);
protected:
	void ProcessSelect(int xPos,int yPos);
	virtual BOOL OnView();	//��ʾ�ӵ�
	virtual PIXELFORMATDESCRIPTOR SetPixelFormatDescriptor();//���õ��ʽ������
	virtual BOOL OnDraw();	//��װ����������ѹջ��glFlush();
	HWND m_hWnd;			//��ʾ���ھ��
	HDC m_hDC;				//��ʾ�豸DC 
	HGLRC m_hglrc;			//������
	BOOL m_bOnly;			//��ǰֻ����OpenGL
	BOOL m_bEnd;			//OpenGL��Ⱦ����
	CRect m_rect;			//���ڿ��
	int m_iTriangle;		//�����͸���
	INT m_timeBegin,m_cnFrame;//��ʼʱ���ˢ��֡��
	FLOAT m_fps;			//֡��
	FLOAT m_RenderTime;		//����һ֡�õ�ʱ��
	FLOAT m_aveRenderTime;	//ƽ������һ֡�õ�ʱ�䣬40msһƽ��
	FLOAT m_avefps;			//ƽ������֡�ʣ�40msһƽ��
	BOOL m_bViewChanged;	//�Ƿ����ӵ�仯
	BOOL m_bLight;			//�Ƿ��������
	BOOL m_bContinue;		//�Ƿ���������
	GLfloat LightAmbient[4];//����
	GLfloat LightDiffuse[4];//ɢ��� 
	GLfloat LightSpecular[4];//�����
	GLfloat LightPosition[4];//��Դ��λ��
	GLdouble m_PerspectiveParam[4];//ͶӰ����[0]��ֱ�ӽ�[1]���ݱ�[2]zNear[3]zFar
	vector<CScene *> scene;//�����б�
	float m_pixLength;//��ʾһ����Ļ���ص���Դ����OpenGL����
	float m_pixLength2;//m_pixLength��ƽ��
};
#endif // !defined(AFX_OPENGL_H__D2CFD1C5_EB19_4006_AAB2_9A624D43E313__INCLUDED_)

/*
//////////////////////////////////////////////////////////////////////////
�汾����v1.29 06-04-04
1�����˸���Font2D����Ҫ����font.Font2D�������ջ���
//////////////////////////////////////////////////////////////////////////
�汾����v1.28 06-03-14
1������֡�ʷ�����������ǰʹ��GetTickTime,�����޷����㵥֡���Ƶ�ʱ�䣬��������:
	GetTickCount()��GetCurrentTime()��ֻ��ȷ��55ms(1��tick����55ms)�����Ҫ��ȷ�����룬Ӧ��ʹ��timeGetTime������QueryPerformanceCounter������
	��ȻtimeGetTime����ֵ�ĵ�λ��1ms����ʵ�������ľ���ֻ��10ms���ҡ�
	�������߾��ȣ�����ʹ��QueryPerformanceCounter��QueryPerformanceFrequency������������������ÿ��ϵͳ�ж�֧�֡�����֧�����ǵ�ϵͳ�У����Ի�õ���1ms�ľ��ȡ�Windows �ڲ���һ�����ȷǳ��ߵĶ�ʱ��, ������΢�뼶, ����ͬ��ϵͳ�����ʱ����Ƶ�ʲ�ͬ, ���Ƶ����Ӳ���Ͳ���ϵͳ�������йء����� API ���� QueryPerformanceFrequency ���Եõ������ʱ����Ƶ�ʡ����� API ���� QueryPerformanceCounter ���Եõ���ʱ���ĵ�ǰֵ������Ҫ��ʱ��ʱ��Ͷ�ʱ����Ƶ��, �������Ҫ��ʱ��ʱ�䶨ʱ������������������ѭ������ QueryPerformanceCounter ��ͣ�Ķ�����ʱ��ֵ, һֱ��������ָ���������ٽ���ѭ��, �ʹﵽ�˸߾�����ʱ��Ŀ�ġ����磺 
	Private Declare Function QueryPerformanceCounter Lib "kernel32" (lpPerformanceCount As Currency) As Long 
	Private Declare Function QueryPerformanceFrequency Lib "kernel32" (lpFrequency As Currency) As Long 
	
	//DelayNumΪ��ʱ�ĺ����� 
	Private Sub DelayTime(ByVal DelayNum As Long) 
	Dim Ctr1, Ctr2, Freq As Currency 
	Dim Count As Double 
	  
	If QueryPerformanceFrequency(Freq) Then 
		QueryPerformanceCounter Ctr1 
		Do 
			QueryPerformanceCounter Ctr2 
		Loop While (Ctr2 - Ctr1) / Freq * 1000 < DelayNum 
	Else 
		MsgBox "��֧�ָ߾��ȼ�����!" 
	End If 
	End Sub 
		
	������Windows����ʵʱ����ϵͳ���������̫�࣬�����������ж����󣬶����ܵ��³�������ʱ���ӳٲ���ȷ��һ���Windows����Ҳ���Խ��ܡ�������ʱ�侫��Ҫ��ܸߣ�һ��ֻ��ʹ��Windows��ʵʱ��չRTX������ʹ������ʵʱ����ϵͳ����VXWorks�ȡ�
	----------------------------------------------
	�Ҿ�������timeGetTime,��Ϊ10ms�ľ����Ѿ���๻�ˡ��ܱ�֤100FPS�ļ���������
	������MSDN��˵��timeGetTime���Դﵽ5ms�����ҿ���ͨ���趨�ı侫�ȡ�5ms�Ѿ��㹻�ˡ�
	���⣬�Ѽ���֡���ʵĺ����ŵ�OnPain()���ˣ�������OnDraw()�
//////////////////////////////////////////////////////////////////////////
�汾����v1.27 06-03-09
1������void OnKeyDown(UINT nChar)�麯���ڰ��¼�ʱ�Զ�����.��Control�е�OnKeyDown()�е���
2������һ������m_pixLength;��ʾһ����Ļ���ص���Դ����OpenGL����
//05-12-13
1����Բ�ķ�����ȷ����ת�����ˡ�����ʹ��DrawArray�ӿ��ٶȡ�sin,cos�ļ������ʹ�ò������
//////////////////////////////////////////////////////////////////////////
�汾����v1.26 05-12-14
1���޸��ڼ����ƶ������У��������������Ȼ���ɿ�����˶�ֹͣ�����⡣m_bContinue
   ���෴�����ƶ��������������⣬û�иġ�
2���޸��ڼ����ƶ�����Ӧ������һ���ӹ̶��ƶ�50����λ�����ˢ���ʸ��ƶ����ᣬ�����ƶ����������ٶȲ���
//////////////////////////////////////////////////////////////////////////
�汾����v1.25 05-10-31
1����render��û��������ж�����m==0&&l>0��Ϊm==0;
//////////////////////////////////////////////////////////////////////////
�汾����v1.24 05-10-26
1��ͳ��֡����ʾ��������Ϊʱ��ȡ������������ʵ����ʾ�ѹ�,ʧ�ܣ�
2���ĳ��̻߳��ƣ�ʧ�ܣ���Ӧ����HGLRC��ʼ��������
3������ѡ���ܡ�10-27
	1) ��OnDraw��glInitNames();
	2) ��DrawScene�м�	glPushName(i);
	3) ����ProcessSelect(int xPos, int yPos)��������ѡ��
	4) �������ڲ���Ҫ������ܣ���û�н��в��ԡ�����ѡ��״̬Ҳû��д�����Ҷ���֧��GPU��װ��CSceneû��ʵ�֣�����ֻ��ͨ��CreateScene�����CScene���Խ���ѡ��
//////////////////////////////////////////////////////////////////////////
�汾����v1.23 05-10-18
1��ʹ��Singleton��������ģʽ�޸��࣬glָ��ı仯...
//////////////////////////////////////////////////////////////////////////
�汾����v1.22 05-10-16
1������ģ�Ͷ�ȡ��Scene���Render�й����Ե����ã���ǰֻ��GL_FRONT��Ӧ��ǰ���涼�У���Ȼ����˸��
2��Ϊ�����˸�����ѵ�λ�á���������double ��ʾ���������ⲻ������Ļ�����float��ʾ
3����ֱ������Щ���ֵ���ͼ�����ˣ�����������˺�����Ͳ���...�������glPushAttr�����⣬����...
	glPushAttr()���������⣬����ͻȻ���ˣ���ôҲû���ˣ��������ǰ����е�glPushAttrɾ������...
//////////////////////////////////////////////////////////////////////////
�汾����v1.21 05-10-13
1����������16λɫģʽ�µ���ɫ����ʾ��
2���������պ���,���ǵ�ʲôʱ���û�����ʲôʱ�����������

//////////////////////////////////////////////////////////////////////////
�汾����v1.20 05-10-12
1����CFrustum�����Ӻ���
	CalculateDistance�������
	CalculatePosition������λ��
	ConvertPosition�任�㵽�ü��ռ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.19 05-10-10~
1����������CreateScene����ֵ������������ɹ�ʱ����������
2�����Ӻ���float *GetViewMatrix();//�õ��ӵ�任����
3����������͸��ɫ���ã���CPictureLoad�����ˣ��ڸ���ģ�Ͷ�ȡ����δ�ģ�ֱֻ����LoadTextureMap��͸��ɫ��Ϊ��ɫ��
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.18 05-09-28
1���޸ĵƹ����ã��ѻ������ʼ��Ϊ0.����SetLight�п������õƹ����ԡ�
2�����Ļ����������ɫ�ĺ������ѻ�������ʱ�趨Ϊ�׹⡣������Ϊ����ֱ��ֻ�л������ܺܺõ�������
3��glColorMaterial����û�����á���ʵ���п���ʹ����������ѹ��պ���ɫ��������
4�������ù��յĵط�������˫����գ�glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE)
//////////////////////////////////////////////////////////////////////////
�汾����v1.17 05-??-??
1����16λɫ�»������ͼ����û�иģ�����������
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.16 05-08-31
1���޸���תƽ�Ʒ�������⡣v
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.15 05-08-26
1�����ӿ�����ʾ��v
2�����б���Ա����ڳ�����Ҳ���Ա��������С�����ڳ����У���û���������ꡣv
3����䶯��
	��ȡ�����еģ������Ķ���Ū�ɶ�̬���ӿ⡣��ÿ����ģ�͵Ĳ��ֵ����������ɶ�̬���ӿ⡣
	�ڶ�ģ�Ͳ��ֲ����ã�����첿�ֵĺ��������޸Ľṹ
	(1)CreateNormal(TRUE)��Scene�е��á�v
	(2)LoadTextureMap�ڳ�����ʼ������ɡ�v
	(3)���ӳ�����ʼ��������Init();	v
4���Ѷ�ȡtexflt���͵�ģ�ͱ�׼����v
5�����Ӳ����ظ���Ĳ���������
6��ȡ��pGLȫ�ֱ��������ø���ָ��ķ�ʽ��v
7������������COpenGL�С�����������ָ�롣v
8��������Ԫ��ʾVolumn XX ���ˣ������ֽṹ�²��ʺϱ���ÿ�����ж�����ɫ�������
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.14 05-08-24
1��������������⡣����LOD������غ�
2��flt��ͼ�����⡣u,v��ַ����double ,Ӧ����float
3������һ���������ģ���ļ���
4�����б�Ӧ����һ��һ���ɣ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.14 05-08-24
1����ʹ����ʾ�б������ΪCScane�Ĳ���������Loader�Ĳ�����
2�������ļ�����·����Ϊ: ��ǰĿ¼>ͬ��Textures\Ŀ¼>�¼�TexturesĿ¼
3��������֯�м��ַ�ʽ��
	��3DSһ�����Բ��ʰ������������ں�������Ϣ���Բ���Ϊ����������Ƭ��
	FLT��������Ƭ�а����˲��ʡ�������ɫ����Ϣ������ƬΪ�������ơ����߰Ѳ��ʺ�����ֿ���ͳһ�����档����ͬ������ͬ����Ĺ�Ϊһ�顣
	�����Ȳ����������ڱ�����ʱ�������
	ʹ��ͳһ��ķ�������������ʡ��������ɫ��ֻҪ��һ����ͬ�Ͳ���һ�顣���ڰ�����Ƭ������
4����Scene��Mat_INDEX �м���������ʺ���ɫ��
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.13 05-08-23
1 ȱʡ��ɫ���������⣬��3ds��ȡ����������˵Ļ�������дһ�����¹�񻯺�����
1������ߵķ��������ڹ���м�������ڸն�����ʱ����㡣UserMeshFacet
2�����ӵ㷨�������Ĺ���Ҳ���ں����񻯽��У������ڶ����ʱ����С�UserMeshVertex
3���������� VerticesNormal()--->CreateNormal(bVertices) ���Լ���߷������͵㷨������
4���Զ������ɡ������������������д���
5���ѳ�ʼ����ת�Ȳ�����render()����������ŵ���CreateNormal�У�ֱ�Ӹı��λ�á�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.12 05-07-17
1 Font3D ������ɫ��û�б������ɫ
//////////////////////////////////////////////////////////////////////////
�汾����v1.12 05-07-5
1 �޸Ļ������ᣬ���Ӳ���color����������ɫ��
2 ����DrawArrow��������������λ�û���ͷ��
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.11 05-07-3
1 ��Scene��������SetInitMatrix�������ڳ�ʼ��Scene��Loadģ��ǰ���ã����Ե�����ʼλ�ò���ģ�ͱ任���뵽��ʾ�б��С�����Ҫ��
2 ��3dsLoader �е�Load����һ��������bList������ֱ������ʹ����ʾ�б�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.10 05-06-24
1 ��MoveViewDis�ڵ�if(m_bLookTarget)Ȼ���������ĵ�ȥ���ˣ�ֱ�ӿ��á�
2 ����ƣ������ֻ�ƶ�û�а���������Ҽ�ʱ��ֱ�ӷ��أ���ˢ����Ļ��
3 ע����׵����ӵ�ƽ������ã�̫С���ܻ��ڼ�����׶��Warpʱ���ִ���
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.09 05-06-11
1 ��OnInit��������OnPaintȥ���ˣ�������ǰ��������ʲô�ã����ǻ�Ӱ���һ�ε���׶��ļ��㣬����ȥ���ˡ�û�з��ֳ�ʲô���⡣
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.08 05-06-10
1 VOID SetControlRoam(BOOL bRoam);//���ÿ���ģʽ��ΪTRUE ��ʾ����ʱ�������Σ�ΪFALSE��ʾ����ʱ���ù۲졣
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.07 05-06-01����02
1 �ص���������ģ����̬���������ڵĴ��������ĵ�Ȳ����������Ѿ�����
2 ���߲��ô��任����ķ����������Ѿ�����
3 �ڹ�����ʾ�б�֮ǰ������̬֮������������׼����������ȥ�Զ���׼�����̡�����Ҫʱ�ٸ�
4 ����ģ����ƽṹ������Ҫʱ�ٸ�
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.06 05-06-01����02
1 ���Ӷ�ȡFltת�ı���ģ�͡�
2 glFrontFace(gl_CCW)�̶���������ǰ���߼��㷴�ˣ�Ӧ����123��˳����ǰ��д3D�ֵ�ʱ��FrontFace���Զ��޸ģ����ڱ���ָ���
3 ��ʼ��ʱglDepthFunc(GL_LESS); ��ǰ��С�ڵ��ڣ��ĳ�С���ˡ�z����32λ��16λ�о�û��ʲô�����ٿ��ĳ�24�б仯���Ѿ�����2005-7-5
4 ȡ��DC������ʹ��wglGetCurrentDC()������ˡ�
5 ���Ӷ�ȡģ�ͺ���Ե���ģ�ͷ�λ������SetModelCenter()...����ȥ���ˣ����ַ�ʽ���ã�05-7-5
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.05 05-05-29 ����05-05-31
1 ���ε�����⡣�������ǳ���
2 ����һ����������һ��case����û�м�break�Ĵ��󣬵���һЩģ�Ͳ��ܶ�ȡ��
3 ���ӹ��ܣ���'+'���������ٶȣ�'-'���������ٶ�'
4 ����GetStep��float ,float)������ȡ�������ٶ�
5 ����������Щ����������'-','+' if(nChar>0XA5&&nChar<0XF6)	m_keyChar = TRUE;
6 ����������ǰ������ȷ��������ԭ����glBegin��λ��д��̫�磬��glBegin�ڲ��ܰﶨ������������дglEnable(texture??).
7 ���Ľṹ��ʹÿ��ͼֻ����һ��������֤�ڴ�������١�
8 �����������
9 ����͹��ߺϳ�
10 ģ������Ŀ¼�Զ�����
11 ���ĸ�λ��۲�㲻��λ����
12 ������Release�в���ִ�����⣺
	unsigned int i,j;
	i=ReadWord();
	j=ReadWord();
	return i+(j<<16);
	//	return ReadWord()+(ReadWord()<<16);
	��һ�ַ�����Debug�п���ִ�У���Release�в����ԡ�
13 ������ȫ��glEnable(GL_TEXTURE_2D);��glDisable(GL_TEXTURE_2D);
14 ��OnRestoreView�м�����ʾˢ��
15 �޸����е�m_pGL Ϊȫ��ָ��pGL
16 ���ӻָ���������
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.04 05-05-27
1 ����	GLdouble m_PerspectiveParam[4];//ͶӰ����[0]��ֱ�ӽ�[1]���ݱ�[2]zNear[3]zFar
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.03 05-05-23
1 ���Ӱ�Shift�ƶ���ת���ĵĹ���
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
�汾����v1.02 05-05-22
1 ���Ӷ�3ds����
2 ����Scene������
//////////////////////////////////////////////////////////////////////////
�汾����v1.01 05-05-17
1 ���Ӽ�����׶����࣬������ü�ƽ��m_clip[16].
2 ����OnView�е��ú���OnViewActualChange()//���ӵ�ı䲢��ִ����OnView���������
3 ȥ��m_bNoTimer һ���Ϊû���á�֡����Ϣ�ǲ���ͳ�Ƶġ�
4 �޸��ӵ�仯ʱ����m_pGL->OnViewChange()��λ��
5������InverseMatrix��������4ά������档
*/
/*
//////////////////////////////////////////////////////////////////////////
���⼰Ҫ�Ľ���v1.01 05-10-12
1��glBlendFunc����˳�������
3�����Ӱ��������Ĺ���
4��������ͼ
5�����̳߳����⣿��
6��COORD1~7������ã��� glActiveTexture�� glMultiTexCoord2fvʵ�ֶ���ͼ

*/
/*
//////////////////////////////////////////////////////////////////////////
ʹ��ע�⣺
1������˳����glBegin��glEnd�м䲻Ҫʹ�ò��ʸı亯��������Щ�����ϲ������á�
2����һ�λ��ƺ���ɫ���������Ȼ������ڵ�ֵ������Ļ��ƻ��ܶ࣬С�ڵ�����ȵĶ����ƣ�Ȼ��Ƭ����ɫ�����Ч��
*/