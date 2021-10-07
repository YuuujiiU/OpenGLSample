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
//使用方法：
/*
一、基本
1 从COpenGL 中继承一个类，例如：CmyOpenGL
2 在CXXXView中生成CmyOpenGL的一个实例，如m_gl
3 在CXXXView的 OnCreate 中加入代码：
		if(!m_gl.Init(this->GetSafeHwnd()))
			AfxMessageBox("error!");
4 在CXXXView的 OnSize 中加入代码：
		m_gl.OnSize(cx,cy);
5 在CXXXView的 OnDraw 中加入代码：
		m_gl.OnPaint();
6 在CXXXView的 OnEraseBkgnd(CDC* pDC) 中加入代码：——可不做——
		改为return TRUE;
二、视点控制
7 在CXXXView的 PreTranslateMessage 中加入代码：
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP ||pMsg->message == WM_RBUTTONDOWN ||pMsg->message == WM_RBUTTONUP ||pMsg->message == WM_LBUTTONDBLCLK ||pMsg->message == WM_RBUTTONDBLCLK||pMsg->message==WM_MOUSEMOVE||pMsg->message==WM_MOUSEWHEEL)
		m_gl.OnMouse(pMsg->message,pMsg->wParam,pMsg->lParam);
	if(pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP || pMsg->message == WM_CHAR)
		m_gl.OnKey(pMsg->message,pMsg->wParam);
默认控制为
鼠标左键按下移动 进行平移视点，按下Ctrl+双击左键变化移动方式
鼠标右键按下移动 进行旋转视点，按下Ctrl+双击右键变化旋转方式
键盘控制为
移动：AaDd--X轴方向  WwSs--Y轴方向 QqEe--Z轴方向 ZzCc--FN轴方向 
旋转：IiKk--X轴方向  JjLl--Y轴方向 UuOo--Z轴方向
其中：在旋转变化中，如果Shift键按下则进行视点自身旋转，否则绕固定点旋转(默认0,0,0点)
三、状态栏显示视点
8 在CMainFrame中设定把indicators[]改为
	static UINT indicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_SEPARATOR,
		ID_SEPARATOR,
		ID_SEPARATOR,
		ID_SEPARATOR
	};
9 在CMainFrame的 OnCreate 最后加入代码：
	g_MyGL.SetStatus(&m_wndStatusBar);
11 在CMainFrame.cpp中包含#include "myOpenGL.h"

例子：
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
//弧度转角度
#define RadToAng(x) ((x)/3.14159*180)
//角度转弧度
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
//继承类中会使用到的函数
//////////////////////////////////////////////////////////////////////////
public:
	virtual BOOL OnViewChange(void);///视点变化时调用（继承）——暂时没有用
	virtual BOOL OnViewActualChange(void);///视点变化并且执行OnView后调用（继承）
	BOOL DrawCoord(float x,float y,float z,float len,BOOL bMark=TRUE,int *Color=0);//画坐标轴。(x,y,z)位置，长度len，是否写字bMark
	BOOL DrawArrow(GLfloat x,GLfloat y,GLfloat z,GLfloat xend,GLfloat yend,GLfloat zend,INT color);//画箭头。(x,y,z)开始位置，(xend,yend,zend)结束位置，color颜色
	VOID SetMode(int mode);//设置绘制方法(0面1线2点)也可以直接使用opengl语句实现。
	VOID SetLight(BOOL bLight,float *ambient=NULL,float *diffuse=NULL,float *specular=NULL,float *position=NULL);
	//control操作
	virtual BOOL OnKeyDown(UINT nChar){return TRUE;};
	BOOL SetRotateType(CString type);//设置旋转类型
	BOOL SetMoveType(CString type);//设置移动类型
	VOID SetControlRoam(BOOL bRoam);//设置控制模式，为TRUE 表示正常时采用漫游，为FALSE表示正常时采用观察。
	//font操作
	BOOL Font3D(CString str,int *color=NULL);//写3D字
	BOOL Font2D(CString str,float x,float y,int size=3);//写2D字
	//view操作
	BOOL SetStep(float mstep,float sstep);//设置视点移动速度默认0.1,0.1
	BOOL GetStep(float &mstep,float &sstep);//设置视点移动速度	
	BOOL SetView(GLfloat x,GLfloat y,GLfloat z,GLfloat h,GLfloat p,GLfloat r);//设置视点位置并设为绝对方式观察
	BOOL SetView(GLfloat m[], GLfloat r[]);
	BOOL SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt);//设置初始视点位置方向
	BOOL SetInitStep(GLfloat m,GLfloat s);//设置初始视点步长	
	BOOL SetLookAt(float x, float y, float z, float tx, float ty, float tz);//设定视点观察某一对象并设为相对方式观察
	BOOL SetLookAt(float tx, float ty, float tz);//同上
	BOOL SetLookAt(bool bLookAt);//设定绝对观察还是相对观察
	BOOL RestoreView();						//恢复初始视点
	BOOL RestoreStep();//恢复初始步长
	BOOL GetView(float pos[],float dir[]);//得到视点位置和方向
	BOOL GetLookCenter(float center[]);//得到中心
	BOOL GetViewVector(float vec[]);	//得到视方向向量
	float *GetViewMatrix();//得到视点变换矩阵
	//frus操作
	float *CalculateClip();
	//其他操作
	static BOOL Normalize(float normal[]);			//标准化矢量
	static BOOL CalculateNormal(float v1[],float v2[],float v3[],float normal[]);//计算三角面方向向量		
	static BOOL Normalize(double normal[]);			//标准化矢量
	static BOOL CalculateNormal(double v1[],double v2[],double v3[],double normal[]);//计算三角面方向向量		
	
	static double InverseMatrix(double *imat, double *mat);//求mat矩阵的逆矩阵imat;返回行列式的值。
	static double InverseMatrix(float *imat, float *mat);//求mat矩阵的逆矩阵imat;返回行列式的值。

	void GetModelMatrix(float mat[]);
protected:
	virtual BOOL PostInit(void);	//初始化后需要继续执行的一些操作（主要继承）
	virtual BOOL InDraw(void);		//直接绘制场景及对象（主要继承）
	virtual BOOL ShowBeforeView();	//视点变换之前会调用
//////////////////////////////////////////////////////////////////////////
//结束
//////////////////////////////////////////////////////////////////////////		
protected:
	class View//关于视点操作
	{
	public:
		View();
		virtual ~View();
		BOOL ShowTriNum(int num);
		BOOL SetStatus(CStatusBar *pStatus);
		BOOL ShowFPS(float fps);
		BOOL ShowView();
		BOOL SetStep(float mstep,float sstep);//设置视点移动速度
		BOOL GetStep(float &mstep, float &sstep);//取得视点移动速度
		BOOL MoveView(int sw,int len);//移动视点sw(0x,1y,2z),len(移动距离)
		BOOL MoveViewRel(int sw, int len);//移动相对视点sw(0lr,1ud,2fb),len(移动距离)
		BOOL RotateView(int sw,int len);//旋转视点sw(0x,1y,2z),len(移动距离)
		BOOL MoveViewDis(int len);//视点远近移动
		BOOL SetView(GLfloat x,GLfloat y,GLfloat z,GLfloat h,GLfloat p,GLfloat r);//设置视点位置
		BOOL SetLookAt(float x, float y, float z, float tx, float ty, float tz);
		BOOL SetLookAt(float tx, float ty, float tz);
		BOOL SetLookAt(BOOL bLookAt);
		BOOL SetInitView(GLfloat x,GLfloat y,GLfloat z,GLfloat x1,GLfloat y1,GLfloat z1,BOOL bLookAt);//设置初始视点位置方向
		BOOL SetInitStep(GLfloat m,GLfloat s);//设置初始视点步长
		BOOL RestoreView();//恢复初始视点
		BOOL RestoreStep();//恢复初始步长
		GLfloat m_moveView[3];	//视点位置xyz
		GLfloat m_rotateView[3];//视点方向角hpr
		GLfloat m_rotateVector[3];//视点方向向量
		GLfloat m_fLookTarget[3];
		GLfloat m_viewMatrix[16];//视图矩阵。
		GLfloat m_RotateMatrix[16];//旋转矩阵。		
	protected:
		//关于视点移动。
		GLfloat m_upVector[3];
		GLfloat m_moveStep;
		GLfloat m_rotateStep;
		BOOL m_bLookTarget;
		CStatusBar *m_pStatus;//状态栏指针
		BOOL SetView();
		BOOL m_bLookTargetHaveMove;
		FLOAT m_InitView[6];
		FLOAT m_InitStep[2];		
		BOOL m_InitViewType;
	}view;
	class Control //鼠标键盘控制操作
	{
	public:	
		BOOL OnMouse(UINT message,UINT wParam,UINT lParam);
		BOOL OnKey(UINT message,UINT nChar);
		BOOL AutoMove();
		BOOL SetRotateType(CString type);
		BOOL SetMoveType(CString type);
		BOOL SetParm(HWND hWnd,View *p);
		VOID SetControlRoam(BOOL bRoam);//设置控制模式，为TRUE 表示正常时采用漫游，为FALSE表示正常时采用观察。		
		Control();
		virtual ~Control();
	protected:
		BOOL OnKeyUp(UINT nChar);
		BOOL OnKeyDown(UINT nChar);
		BOOL SwitchType(BOOL bMove);
		BOOL OnMoveFarNear(short len);//对应鼠标滚轮
		BOOL OnMoveRotate(UINT xPos,UINT yPos,UINT fwKeys);//对应鼠标移动
		BOOL RotateEnd();//鼠标右键松开
		BOOL RotateBegin(UINT lParam);//鼠标右键按下
		BOOL MoveEnd();//鼠标左键松开
		BOOL MoveBegin(UINT lParam);//鼠标左键按下
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
	class FontGL//关于字体操作
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
		static void DrawBall(float rad,int lon,int lat);//半径，经度，纬度
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
		float m_Frustum[6][4];		//裁剪平面
		float m_Clip[16];			//裁剪矩阵
		float CalculateDistance(float v[]);//计算空间点距离最近视锥体的距离
		int CalculatePosition(float v[]);//计算空间点在视锥体的那个位置				
		int CalculateBoundingBox(float v[]);//计算包围盒是否在视锥体中v[6]代表 x(min,max); y(min,max); z(min,max).返回值-1表示不在视锥体中，0表示部分在视锥体中，1表示完全在视锥体中。
		int ConvertPosition(float v[]);//把空间点转换到裁减坐标系中
		float* CalculateClip();//计算裁减矩阵
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
		BOOL Load(CString sFilePathName);//从文件读取图像
		BOOL Load(HINSTANCE hInstance,LPCTSTR lpszResourceName, LPCSTR ResourceType);//从资源读取图像
		void Render(CDC* pDC, LPRECT pDrawRect/*目标矩形，单位是逻辑坐标单位*/
			,LPRECT pSrcRect=NULL/*来源矩形，单位是0.01毫米,如果为空，则拉伸整个图像到目标矩形*/
			,LPCRECT prcWBounds=NULL/*图元文件专用，绑定矩形*/
			);//在给定的DC上画图，
		BOOL LoadPictureData(BYTE* pBuffer, int nSize);//从内存读取图像
		BOOL SaveAsBitmap(CString sFilePathName);//写入到BMP文件
		void UnloadPicture();//释放图像，作用同CPictureHolder::~CPictureHolder()
		LONG get_Height(); // 以0.01毫米为单位的图像高度
		LONG get_Width(); // 以0.01毫米为单位的图像宽度
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
		BOOL m_bTransparence;//透明色允许
		int m_iTransparence;//透明色
	public:
		void * GetData(CString sFilePathName,int *color=0);
		CPictureLoad();
		virtual ~CPictureLoad();	
	}picLoader;//读图片的类
	class CScene
	{
	public:
		CScene();
		virtual ~CScene();
		//类定义
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
			UINT m_iMAT;//材质
			UINT texture;//纹理
			INT m_color;//颜色
		};
		struct MAT
		{
			CString name;//材质名称
			float mat_ambient[4];//环境光
			float mat_diffuse[4];//散射光
			float mat_specular[4];//镜面反射光
			float mat_emission[4];//辐射光——未用
			float mat_shininess[1];//光亮度——未用
			UINT tex_index;//纹理图的索引
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
			CString mapname;//纹理图名称
			UINT texture;//绑定的纹理
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
			int tex_index;//邦定的纹理
			int color;//颜色值,如果等于-1表示没有颜色值
			vector<int> facets_index;//面列表			
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
		   	CString name;//名称
			vector<VECTOR> vertices;//点列表
			vector<NORMAL> facets_normal;//面法向向量
			vector<NORMAL> vertices_normal;//点法向向量列表
			vector<MAPCOORD> mapcoord;//纹理坐标
			vector<INDEX> facets;//面列表
			vector<MAT_INDEX> material_index;//材质列表
			bool isOn;
		};
		//变量定义
		public:
			UINT LoadTextureMap(MATMAP &map,CString path);//读入纹理帖图
			UINT LoadTextureMap(CString name,CString path);//读入纹理帖图
			UINT LoadTextureCubeMap(CString name[],CString path);//读入纹理帖图
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
			vector<GROUP> groups;//对象组
			vector<MAT> materials;//材质列表
			vector<MATMAP> maps;//材质纹理图列表
			vector<VECTOR> vertices;//点列表——对应可能统一的点列表
			vector<NORMAL> vertices_normal;//点法向向量列表
			CString m_path;			
		protected:
			bool CreateNormal(BOOL bVertice=TRUE);//产生点的法向量。
			bool UseShowList();//产生显示列表。
			void GetDirect(const char *str,CString &path,CString &pathPre);
			UINT SetTextureMap(ImageData *TextureImage);
		protected:
			int iTri;//当前三角片数
			float *m_InitMatrix;
			UINT m_uList;
			BOOL m_bList;
	};

public:
	//scene操作
	CScene * GetScene(int n);	
	int CreateScene(int n=1);
	int GetSceneNum();
	BOOL DrawScene(int &numTri,int start=0,int end = -1);
	VOID SetStatus(CStatusBar *pStatus);	//设置状态栏信息
	virtual BOOL End();						//令OpenGL终止，释放翻译描述符表
	virtual BOOL OnPaint();					//绘制
	virtual BOOL OnSize(int w,int h);		//设置投影方式和模型视图，并缺省建立一个视窗
	virtual BOOL Init(HWND hWnd,BOOL bOnly = TRUE);//初始化
	COpenGL();
	virtual ~COpenGL();	

	BOOL OnMouse(UINT message,UINT wParam,UINT lParam);
	BOOL OnKey(UINT message,UINT nChar);
protected:
	void ProcessSelect(int xPos,int yPos);
	virtual BOOL OnView();	//显示视点
	virtual PIXELFORMATDESCRIPTOR SetPixelFormatDescriptor();//设置点格式描述符
	virtual BOOL OnDraw();	//包装了清屏、出压栈和glFlush();
	HWND m_hWnd;			//显示窗口句柄
	HDC m_hDC;				//显示设备DC 
	HGLRC m_hglrc;			//描述符
	BOOL m_bOnly;			//当前只绘制OpenGL
	BOOL m_bEnd;			//OpenGL渲染结束
	CRect m_rect;			//窗口宽高
	int m_iTriangle;		//三角型个数
	INT m_timeBegin,m_cnFrame;//开始时间和刷新帧数
	FLOAT m_fps;			//帧率
	FLOAT m_RenderTime;		//绘制一帧用的时间
	FLOAT m_aveRenderTime;	//平均绘制一帧用的时间，40ms一平均
	FLOAT m_avefps;			//平均绘制帧率，40ms一平均
	BOOL m_bViewChanged;	//是否有视点变化
	BOOL m_bLight;			//是否允许光照
	BOOL m_bContinue;		//是否连续绘制
	GLfloat LightAmbient[4];//泛光
	GLfloat LightDiffuse[4];//散射光 
	GLfloat LightSpecular[4];//反射光
	GLfloat LightPosition[4];//光源的位置
	GLdouble m_PerspectiveParam[4];//投影参数[0]垂直视角[1]横纵比[2]zNear[3]zFar
	vector<CScene *> scene;//场景列表
	float m_pixLength;//表示一个屏幕像素点可以代表的OpenGL长度
	float m_pixLength2;//m_pixLength的平方
};
#endif // !defined(AFX_OPENGL_H__D2CFD1C5_EB19_4006_AAB2_9A624D43E313__INCLUDED_)

/*
//////////////////////////////////////////////////////////////////////////
版本修正v1.29 06-04-04
1、改了个错，Font2D里面要返回font.Font2D，否则堆栈溢出
//////////////////////////////////////////////////////////////////////////
版本修正v1.28 06-03-14
1、计算帧率方法改正。以前使用GetTickTime,发现无法计算单帧绘制的时间，现在明白:
	GetTickCount()和GetCurrentTime()都只精确到55ms(1个tick就是55ms)。如果要精确到毫秒，应该使用timeGetTime函数或QueryPerformanceCounter函数。
	虽然timeGetTime返回值的单位是1ms，但实际上它的精度只有10ms左右。
	如果想提高精度，可以使用QueryPerformanceCounter和QueryPerformanceFrequency。这两个函数不是在每个系统中都支持。对于支持它们的系统中，可以获得低于1ms的精度。Windows 内部有一个精度非常高的定时器, 精度在微秒级, 但不同的系统这个定时器的频率不同, 这个频率与硬件和操作系统都可能有关。利用 API 函数 QueryPerformanceFrequency 可以得到这个定时器的频率。利用 API 函数 QueryPerformanceCounter 可以得到定时器的当前值。根据要延时的时间和定时器的频率, 可以算出要延时的时间定时器经过的周期数。在循环里用 QueryPerformanceCounter 不停的读出定时器值, 一直到经过了指定周期数再结束循环, 就达到了高精度延时的目的。例如： 
	Private Declare Function QueryPerformanceCounter Lib "kernel32" (lpPerformanceCount As Currency) As Long 
	Private Declare Function QueryPerformanceFrequency Lib "kernel32" (lpFrequency As Currency) As Long 
	
	//DelayNum为延时的毫秒数 
	Private Sub DelayTime(ByVal DelayNum As Long) 
	Dim Ctr1, Ctr2, Freq As Currency 
	Dim Count As Double 
	  
	If QueryPerformanceFrequency(Freq) Then 
		QueryPerformanceCounter Ctr1 
		Do 
			QueryPerformanceCounter Ctr2 
		Loop While (Ctr2 - Ctr1) / Freq * 1000 < DelayNum 
	Else 
		MsgBox "不支持高精度计数器!" 
	End If 
	End Sub 
		
	不过，Windows不是实时操作系统，如果任务太多，或者有其他中断请求，都可能导致程序运行时的延迟不精确，一般的Windows程序也可以接受。如果你对时间精度要求很高，一般只有使用Windows的实时扩展RTX，或者使用其他实时操作系统，如VXWorks等。
	----------------------------------------------
	我决定采用timeGetTime,因为10ms的精度已经差不多够了。能保证100FPS的计算数据了
	另外在MSDN中说，timeGetTime可以达到5ms，而且可以通过设定改变精度。5ms已经足够了。
	另外，把计算帧速率的函数放到OnPain()中了，而不在OnDraw()里。
//////////////////////////////////////////////////////////////////////////
版本修正v1.27 06-03-09
1、增加void OnKeyDown(UINT nChar)虚函数在按下键时自动调用.在Control中的OnKeyDown()中调用
2、增加一个变量m_pixLength;表示一个屏幕像素点可以代表的OpenGL长度
//05-12-13
1、画圆的方向不正确，旋转方向反了。可以使用DrawArray加快速度。sin,cos的计算可以使用查表方法。
//////////////////////////////////////////////////////////////////////////
版本修正v1.26 05-12-14
1、修改在键盘移动过程中，如果按下其他键然后松开造成运动停止的问题。m_bContinue
   向相反方向移动还会出现这个问题，没有改。
2、修改在键盘移动的响应次数，一秒钟固定移动50个单位，如果刷新率高移动连贯，否则移动跳动，但速度不变
//////////////////////////////////////////////////////////////////////////
版本修正v1.25 05-10-31
1、在render中没有纹理把判断条件m==0&&l>0改为m==0;
//////////////////////////////////////////////////////////////////////////
版本修正v1.24 05-10-26
1、统计帧率显示方法，改为时间取倒数方法，与实际显示脱钩,失败！
2、改成线程绘制，失败！！应该是HGLRC初始化的问题
3、增加选择功能。10-27
	1) 在OnDraw加glInitNames();
	2) 在DrawScene中加	glPushName(i);
	3) 增加ProcessSelect(int xPos, int yPos)函数处理选择。
	4) 由于现在不需要这个功能，并没有进行测试。处理选中状态也没有写。而且对于支持GPU封装入CScene没有实现，现在只有通过CreateScene加入的CScene可以进行选择。
//////////////////////////////////////////////////////////////////////////
版本修正v1.23 05-10-18
1、使用Singleton（单件）模式修改类，gl指针的变化...
//////////////////////////////////////////////////////////////////////////
版本修正v1.22 05-10-16
1、修正模型读取中Scene类的Render中光属性的设置，以前只有GL_FRONT，应该前后面都有，不然会闪烁。
2、为解决闪烁曾经把点位置、法向量用double 表示，发现问题不在这里，改回来用float表示
3、画直升机有些部分的贴图不见了，发现如果画了海网格就不见...最后发现是glPushAttr的问题，不解...
	glPushAttr()好像有问题，后来突然好了，怎么也没事了？？？还是把所有的glPushAttr删除掉了...
//////////////////////////////////////////////////////////////////////////
版本修正v1.21 05-10-13
1、修正了在16位色模式下的颜色的显示。
2、修正光照函数,考虑到什么时候用环境光什么时候用漫反射光

//////////////////////////////////////////////////////////////////////////
版本修正v1.20 05-10-12
1、在CFrustum中增加函数
	CalculateDistance计算距离
	CalculatePosition计算点的位置
	ConvertPosition变换点到裁剪空间
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.19 05-10-10~
1、产生场景CreateScene返回值变成整数，当成功时返回索引。
2、增加函数float *GetViewMatrix();//得到视点变换矩阵。
3、增加纹理透明色设置，在CPictureLoad中设了，在各个模型读取库中未改，只直接在LoadTextureMap中透明色设为白色。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.18 05-09-28
1、修改灯光设置，把环境光初始化为0.并在SetLight中可以设置灯光属性。
2、更改画坐标轴的颜色的函数。把环境光暂时设定为白光。这是因为对于直线只有环境光能很好的照亮。
3、glColorMaterial函数没有启用。在实际中可以使用它来方便把光照和颜色绑定起来。
4、在设置光照的地方增加了双面光照：glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE)
//////////////////////////////////////////////////////////////////////////
版本修正v1.17 05-??-??
1、在16位色下会出现贴图错误。没有改！！！！！！
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.16 05-08-31
1、修改旋转平移方向的问题。v
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.15 05-08-26
1、增加开关显示组v
2、点列表可以保存在场景中也可以保存在组中。如果在场景中，则没有纹理坐标。v
3、大变动：
	争取把所有的ｏｐｅｎｇｌ的东西弄成动态链接库。把每个读模型的部分单独出来做成动态链接库。
	在读模型不分不调用ｏｐｅｎｇｌ部分的函数，则修改结构
	(1)CreateNormal(TRUE)在Scene中调用。v
	(2)LoadTextureMap在场景初始化中完成。v
	(3)增加场景初始化函数。Init();	v
4、把读取texflt类型的模型标准化。v
5、增加查找重复面的操作？？？
6、取消pGL全局变量，采用父亲指针的方式。v
7、场景集中在COpenGL中。不单独申请指针。v
8、增加体元表示Volumn XX 不了，在这种结构下不适合表现每个面有独自颜色的情况。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.14 05-08-24
1、解决闪动的问题。是有LOD多个面重合
2、flt贴图的问题。u,v地址用了double ,应该是float
3、增加一个函数输出模型文件。
4、点列表应该是一个一个吧？
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.14 05-08-24
1、把使用显示列表参数作为CScane的参数而不是Loader的参数。
2、纹理文件搜索路径改为: 当前目录>同级Textures\目录>下级Textures目录
3、场景组织有几种方式：
	象3DS一样，以材质包含纹理，材质内含有面信息，以材质为基础绘制面片。
	FLT，则在面片中包含了材质、纹理、颜色等信息。以面片为基础绘制。或者把材质和纹理分开，统一到上面。即相同材质相同纹理的归为一组。
	纹理先产生，材质在遍历的时候产生。
	使用统一组的方法。组包含材质、纹理和颜色，只要有一个不同就不是一组。组内包含面片索引。
4、在Scene中Mat_INDEX 中加上纹理材质和颜色。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.13 05-08-23
1 缺省颜色好像有问题，在3ds读取后，如果缩放了的话。打算写一个重新规格化函数。
1、计算边的法向量放在规格化中计算而不在刚读到的时候计算。UserMeshFacet
2、增加点法向向量的工作也放在后面规格化进行，不放在读点的时候进行。UserMeshVertex
3、函数改名 VerticesNormal()--->CreateNormal(bVertices) 可以计算边法向量和点法向量。
4、自动规格化完成。——————但是有错误
5、把初始化旋转等操作从render()中提出来，放到了CreateNormal中，直接改变点位置。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.12 05-07-17
1 Font3D 增加颜色，没有保存旧颜色
//////////////////////////////////////////////////////////////////////////
版本修正v1.12 05-07-5
1 修改画坐标轴，增加参数color设置坐标颜色。
2 增加DrawArrow函数，可以任意位置画箭头。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.11 05-07-3
1 在Scene中增加了SetInitMatrix函数，在初始化Scene后Load模型前调用，可以调整初始位置并把模型变换加入到显示列表中。（重要）
2 在3dsLoader 中的Load加入一个参数：bList，可以直接设置使用显示列表。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.10 05-06-24
1 把MoveViewDis内的if(m_bLookTarget)然后启用中心点去掉了，直接可用。
2 简化设计，当鼠标只移动没有按左键或者右键时，直接返回，不刷新屏幕。
3 注意视椎体近视点平面的设置，太小可能会在计算视锥体Warp时出现错误。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.09 05-06-11
1 把OnInit中最后调用OnPaint去掉了，忘了以前加上它有什么用，但是会影响第一次的视锥体的计算，所以去掉了。没有发现出什么问题。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.08 05-06-10
1 VOID SetControlRoam(BOOL bRoam);//设置控制模式，为TRUE 表示正常时采用漫游，为FALSE表示正常时采用观察。
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.07 05-06-01——02
1 回调函数调整模型姿态（不用现在的传几个中心点等参数）——已经改了
2 或者采用传变换矩阵的方法。——已经改了
3 在构造显示列表之前调整姿态之后做法向量标准化，可以免去自动标准化过程——需要时再改
4 采用模块设计结构——需要时再改
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.06 05-06-01——02
1 增加读取Flt转文本的模型。
2 glFrontFace(gl_CCW)固定下来，以前法线计算反了，应该用123的顺序，以前在写3D字的时候FrontFace被自动修改，现在保存恢复了
3 初始化时glDepthFunc(GL_LESS); 以前是小于等于，改成小于了。z缓存32位和16位感觉没有什么区别。再看改成24有变化，已经改了2005-7-5
4 取得DC，现在使用wglGetCurrentDC()来获得了。
5 增加读取模型后可以调整模型方位函数。SetModelCenter()...——去掉了，这种方式不好，05-7-5
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.05 05-05-29 ——05-05-31
1 屏蔽掉反射光。发现总是出错
2 改正一个错误，修正一个case后面没有加break的错误，导致一些模型不能读取。
3 增加功能，按'+'增加漫游速度，'-'减少漫游速度'
4 增加GetStep（float ,float)函数来取得漫游速度
5 修正错误，有些按键不灵如'-','+' if(nChar>0XA5&&nChar<0XF6)	m_keyChar = TRUE;
6 修正错误，以前不能正确读入纹理，原因是glBegin的位置写的太早，在glBegin内不能帮定纹理。还有望了写glEnable(texture??).
7 更改结构，使每幅图只生成一个纹理，保证内存的消耗少。
8 读入各种纹理
9 纹理和光线合成
10 模型纹理目录自动搜索
11 更改复位后观察点不复位现象
12 改正在Release中不能执行问题：
	unsigned int i,j;
	i=ReadWord();
	j=ReadWord();
	return i+(j<<16);
	//	return ReadWord()+(ReadWord()<<16);
	后一种方法在Debug中可以执行，在Release中不可以。
13 设置完全的glEnable(GL_TEXTURE_2D);和glDisable(GL_TEXTURE_2D);
14 在OnRestoreView中加入显示刷新
15 修改所有的m_pGL 为全局指针pGL
16 增加恢复步长函数
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.04 05-05-27
1 增加	GLdouble m_PerspectiveParam[4];//投影参数[0]垂直视角[1]横纵比[2]zNear[3]zFar
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.03 05-05-23
1 增加按Shift移动旋转中心的功能
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
版本修正v1.02 05-05-22
1 增加读3ds的类
2 增加Scene场景类
//////////////////////////////////////////////////////////////////////////
版本修正v1.01 05-05-17
1 增加计算视锥体的类，计算出裁减平面m_clip[16].
2 增加OnView中调用函数OnViewActualChange()//在视点改变并且执行了OnView函数后调用
3 去掉m_bNoTimer 一项，因为没有用。帧率信息是不断统计的。
4 修改视点变化时调用m_pGL->OnViewChange()的位置
5、增加InverseMatrix函数，求4维矩阵的逆。
*/
/*
//////////////////////////////////////////////////////////////////////////
问题及要改进的v1.01 05-10-12
1、glBlendFunc绘制顺序的问题
3、增加按键调整的功能
4、环境贴图
5、用线程出问题？？
6、COORD1~7如何设置？用 glActiveTexture和 glMultiTexCoord2fv实现多贴图

*/
/*
//////////////////////////////////////////////////////////////////////////
使用注意：
1、绘制顺序：在glBegin和glEnd中间不要使用材质改变函数，在有些机器上不起作用。
2、第一次绘制后不着色，不清除深度缓冲区内的值，后面的绘制会快很多，小于等于深度的都绘制，然后片段着色会更有效。
*/