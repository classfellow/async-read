#pragma once
/********************************************************************
	created:	2006/03/10
	filename: 	BitmapButtons.h
	file path:	
	file base:	BitmapButtons
	file ext:	h
	author:		×ÚÐ¡±ó
	
	purpose:	
*********************************************************************/
//#include "StringEx.h"
#ifndef __cplusplus
#error ATL requires C++ compilation (use a .cpp suffix)
#endif

#ifndef __ATLAPP_H__
#error atlctrlx.h requires atlapp.h to be included first
#endif

#ifndef __ATLCTRLS_H__
#error atlctrlx.h requires atlctrls.h to be included first
#endif

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE                0x0128
#endif // !WM_UPDATEUISTATE

// bitmap button extended styles
#define BMPBTNS_HOVER		0x00000001
#define BMPBTNS_HORIZON  	0x00000002
#define BMPBTNS_VERTICAL    0x00000004
#define BMPBTNS_AUTOSIZE	0x00000008
#define BMPBTNS_TAB     	0x00000010

#include <atlframe.h>
class Button
{
public:
	Button():m_lpstrButtonText(NULL),m_lpstrToolTipText(NULL),
		m_nSpaceWidth(-1),m_dwData(NULL),m_bEnabled(TRUE)
	{
		m_nImage[_nImageNormal] = -1;
		m_nImage[_nImagePushed] = -1;
		m_nImage[_nImageFocusOrHover] = -1;
		m_nImage[_nImageDisabled] = -1;
	}

	~Button()
	{		
		delete [] m_lpstrToolTipText;
		delete [] m_lpstrButtonText;
	}

	Button& SetText(LPCTSTR lpstrText)
	{
		if(m_lpstrButtonText != NULL)
		{
			delete [] m_lpstrButtonText;
			m_lpstrButtonText = NULL;
		}

		if(lpstrText == NULL)
		{
			return (*this);
		}

		int cchLen = lstrlen(lpstrText) + 1;
		ATLTRY(m_lpstrButtonText = new TCHAR[cchLen]);
		if(m_lpstrButtonText == NULL)
			return (*this);
#if _SECURE_ATL
		ATL::Checked::tcscpy_s(m_lpstrButtonText, cchLen, lpstrText);
		bool bRet = true;
#else
		bool bRet = (lstrcpy(m_lpstrButtonText, lpstrText) != NULL);
#endif
		return (*this);
	}
	Button& SetToolTipText(LPCTSTR lpstrText)
	{
		if(m_lpstrToolTipText != NULL)
		{
			delete [] m_lpstrToolTipText;
			m_lpstrToolTipText = NULL;
		}

		if(lpstrText == NULL)
		{
			return (*this);
		}

		int cchLen = lstrlen(lpstrText) + 1;
		ATLTRY(m_lpstrToolTipText = new TCHAR[cchLen]);
		if(m_lpstrToolTipText == NULL)
			return (*this);
#if _SECURE_ATL
		ATL::Checked::tcscpy_s(m_lpstrToolTipText, cchLen, lpstrText);
		bool bRet = true;
#else
		bool bRet = (lstrcpy(m_lpstrToolTipText, lpstrText) != NULL);
#endif
		return (*this);
	}
	Button& SetImageList(HIMAGELIST hImageList)
	{
		m_ImageList = hImageList;
		return (*this);
	}
	// Operations
	Button& SetImages(int nNormal, int nPushed = -1, int nFocusOrHover = -1, int nDisabled = -1)
	{
		if(nNormal != -1)
			m_nImage[_nImageNormal] = nNormal;
		if(nPushed != -1)
			m_nImage[_nImagePushed] = nPushed;
		if(nFocusOrHover != -1)
			m_nImage[_nImageFocusOrHover] = nFocusOrHover;
		if(nDisabled != -1)
			m_nImage[_nImageDisabled] = nDisabled;
		return (*this);
	}
	Button& SetItemData(DWORD_PTR dwData )
	{
		m_dwData = dwData;
		return (*this);
	}

	enum
	{
		_nImageNormal = 0,
		_nImagePushed,
		_nImageFocusOrHover,
		_nImageDisabled,

		_nImageCount = 4,
	};


	LPTSTR      m_lpstrToolTipText;
	LPTSTR      m_lpstrButtonText;
	CImageList  m_ImageList;
	int         m_nImage[_nImageCount];
	CRect       m_rc;
	int         m_nSpaceWidth;
	BOOL        m_bEnabled;
	DWORD_PTR   m_dwData;

};

template <class T, class TBase = CButton, class TWinTraits = ATL::CControlWinTraits>
class ATL_NO_VTABLE CBitmapButtonsImpl : public ATL::CWindowImpl< T, TBase, TWinTraits>,
	                                     public CDoubleBufferImpl< CBitmapButtonsImpl< T,TBase,TWinTraits > >
{
public:
	DECLARE_WND_SUPERCLASS(NULL, TBase::GetWndClassName())
	// Constructor/Destructor
	CBitmapButtonsImpl(DWORD dwExtendedStyle = BMPBTNS_AUTOSIZE, HIMAGELIST hImageList = NULL) : 
	m_ImageList(hImageList), m_dwExtendedStyle(dwExtendedStyle), 
		m_fMouseOver(0), m_fFocus(0), m_fPressed(0),
		m_nSpaceWidth(0),m_pHoverBtn(NULL),m_pClickedTabBtn(NULL),
		m_rgbBackground(RGB(255,255,255)),m_rgbText(RGB(255,255,255))
	{
		m_nImage[_nImageNormal] = -1;
		m_nImage[_nImagePushed] = -1;
		m_nImage[_nImageFocusOrHover] = -1;
		m_nImage[_nImageDisabled] = -1;
	}

	~CBitmapButtonsImpl()
	{
	}

	enum
	{
		_nImageNormal = 0,
		_nImagePushed,
		_nImageFocusOrHover,
		_nImageDisabled,

		_nImageCount = 4,
	};


	CSimpleArray<Button * > m_apButtons;
	Button  * m_pHoverBtn;
	Button  * m_pClickedTabBtn;


	// Bitmap button specific extended styles
	DWORD m_dwExtendedStyle;

	CImageList m_ImageList;
	int m_nImage[_nImageCount];

	CToolTipCtrl m_tip;
	COLORREF     m_rgbBackground;
	COLORREF     m_rgbText;	
	int   m_Itemcx;
	int   m_Itemcy;
	int   m_nSpaceWidth;

	// Internal states
	unsigned m_fMouseOver:1;
	unsigned m_fFocus:1;
	unsigned m_fPressed:1;


	

	// overridden to provide proper initialization
	BOOL SubclassWindow(HWND hWnd)
	{
#if (_MSC_VER >= 1300)
		BOOL bRet = ATL::CWindowImpl< T, TBase, TWinTraits>::SubclassWindow(hWnd);
#else // !(_MSC_VER >= 1300)
		typedef ATL::CWindowImpl< T, TBase, TWinTraits>   _baseClass;
		BOOL bRet = _baseClass::SubclassWindow(hWnd);
#endif // !(_MSC_VER >= 1300)
		if(bRet)
			Init();
		return bRet;
	}

	int GetSize()
	{
		return m_apButtons.GetSize();
	}
	void SetBkColor(COLORREF color)
	{
		m_rgbBackground = color;
	}
	void SetTextColor(COLORREF color)
	{
		m_rgbText = color;
	}
	void AddButton(Button &btn)
	{
		Button * pbtn = new Button();
		pbtn->SetToolTipText(btn.m_lpstrToolTipText);
		pbtn->SetText(btn.m_lpstrButtonText);
		pbtn->SetItemData(btn.m_dwData);
		pbtn->m_bEnabled = btn.m_bEnabled;

		if(btn.m_nImage[_nImageNormal] == -1)
			pbtn->m_nImage[_nImageNormal] = m_nImage[_nImageNormal];
		else
			pbtn->m_nImage[_nImageNormal] = btn.m_nImage[_nImageNormal];
			
		if(btn.m_nImage[_nImagePushed] == -1)
			pbtn->m_nImage[_nImagePushed] = m_nImage[_nImagePushed];
		else
			pbtn->m_nImage[_nImagePushed] = btn.m_nImage[_nImagePushed];

		if(btn.m_nImage[_nImageFocusOrHover] == -1)
			pbtn->m_nImage[_nImageFocusOrHover] = m_nImage[_nImageFocusOrHover];
		else
	        pbtn->m_nImage[_nImageFocusOrHover] = btn.m_nImage[_nImageFocusOrHover];

		if(btn.m_nImage[_nImageDisabled] == -1)
			pbtn->m_nImage[_nImageDisabled] = m_nImage[_nImageDisabled] ;	
		else
	        pbtn->m_nImage[_nImageDisabled] = btn.m_nImage[_nImageDisabled];

		if(btn.m_ImageList.IsNull())
			pbtn->m_ImageList = m_ImageList;
		else
			pbtn->m_ImageList = btn.m_ImageList;

		if(btn.m_nSpaceWidth == -1 )
			pbtn->m_nSpaceWidth = m_nSpaceWidth;
		else
			pbtn->m_nSpaceWidth = btn.m_nSpaceWidth;

		if( btn.m_rc.IsRectEmpty())
		{
			int cx = 0;
			int cy = 0;
			if((m_dwExtendedStyle & BMPBTNS_AUTOSIZE) != 0 )
			{
				pbtn->m_ImageList.GetIconSize(cx,cy);
			}
			else
			{
				cx = m_Itemcx;
				cy = m_Itemcy;
			}				
			pbtn->m_rc = CRect(0,0,cx,cy);

		}
		else
		{
			pbtn->m_rc = btn.m_rc;
		}
		m_apButtons.Add(pbtn);
		CalButtonPos();
		
	}
	void Delete(int index)
	{
		delete m_apButtons[index];
		m_apButtons.RemoveAt(index);
		CalButtonPos();
	}
	Button * GetButtonFrom(CPoint pt)
	{
		for( int i = 0;i<m_apButtons.GetSize();i++)
		{
			if( m_apButtons[i]->m_rc.PtInRect(pt))
			{
				if( m_apButtons[i]->m_bEnabled )
				  return m_apButtons[i];
				return NULL;
			}
		}
		return NULL;
	}
	
	void EnableItem(int index,BOOL bEnable)
	{
		Button* pbtn = GetButtonFrom(index);
		if(pbtn)
        {
            pbtn->m_bEnabled = bEnable;
            InvalidateItem(index, TRUE);
        }
	}
	Button * GetButtonFrom(int index)
	{
		if( IsValidateIndex(index))
			return m_apButtons[index];
		return NULL;
	}
	void CalButtonPos()
	{
		int cx = 0;
		int cy = 0;
		Button * pBtn = NULL;
		for( int i = 0;i<m_apButtons.GetSize();i++ )
		{
			pBtn = m_apButtons[i];
			if(IsHorizon())
			{
				pBtn->m_rc.MoveToX(cx);
				cx += pBtn->m_rc.Width() + pBtn->m_nSpaceWidth;
			}
			else
			{
				pBtn->m_rc.MoveToY(cy);
				cy += pBtn->m_rc.Height() + pBtn->m_nSpaceWidth;
			}
		}

	}
	// Attributes
	DWORD GetBitmapButtonExtendedStyle() const
	{
		return m_dwExtendedStyle;
	}

	DWORD SetBitmapButtonExtendedStyle(DWORD dwExtendedStyle, DWORD dwMask = 0)
	{
		DWORD dwPrevStyle = m_dwExtendedStyle;
		if(dwMask == 0)
			m_dwExtendedStyle = dwExtendedStyle;
		else
			m_dwExtendedStyle = (m_dwExtendedStyle & ~dwMask) | (dwExtendedStyle & dwMask);
		return dwPrevStyle;
	}

	HIMAGELIST GetImageList() const
	{
		return m_ImageList;
	}

	void  SetButtonspaceWidth(int nWidth)
	{
		m_nSpaceWidth = nWidth;
	}
	HIMAGELIST SetImageList(HIMAGELIST hImageList)
	{
		HIMAGELIST hImageListPrev = m_ImageList;
		m_ImageList = hImageList;
		return hImageListPrev;
	}
	void SetButtonSize(int cx,int cy)
	{
		m_Itemcx = cx;
		m_Itemcy = cy;
	}


	// Operations
	void SetItemImages(int nIndex,int nNormal, int nPushed = -1, int nFocusOrHover = -1, int nDisabled = -1)
	{
		if( nIndex < m_apButtons.GetSize() )
		{
			m_apButtons[nIndex]->SetImages(nNormal,nPushed,nFocusOrHover,nDisabled);
		}
		
	}
	// Operations
	void SetItemText(int nIndex,LPCTSTR lpstrText)
	{
		if( nIndex < m_apButtons.GetSize() )
		{
			m_apButtons[nIndex]->SetText(lpstrText);
		}		
	}	
	// Operations
	void SetImages(int nNormal, int nPushed = -1, int nFocusOrHover = -1, int nDisabled = -1)
	{
		if(nNormal != -1)
			m_nImage[_nImageNormal] = nNormal;
		if(nPushed != -1)
			m_nImage[_nImagePushed] = nPushed;
		if(nFocusOrHover != -1)
			m_nImage[_nImageFocusOrHover] = nFocusOrHover;
		if(nDisabled != -1)
			m_nImage[_nImageDisabled] = nDisabled;
	}

	BOOL SizeToImage()
	{
		CalButtonPos();
		CPoint pt = m_apButtons[m_apButtons.GetSize()-1]->m_rc.BottomRight();	

		return ResizeClient(pt.x,pt.y);
	}

	void DrawBkgnd(CDCHandle dcPaint)
	{
		CRect rc,rcClip;
		GetClientRect(&rc);
		dcPaint.GetClipBox(&rcClip);
		rc.IntersectRect(rcClip,rc);
		dcPaint.FillSolidRect(&rc,m_rgbBackground);		
	}
	void DrawButtons(CDCHandle dcPaint)
	{
		CRect rcClip;
		if ( dcPaint.GetClipBox( rcClip ) == ERROR )
			return;

		dcPaint.SaveDC();
		dcPaint.SetBkMode(TRANSPARENT);
		dcPaint.SetTextColor(m_rgbText);//RGB(255,255,255));
		dcPaint.SelectFont(AtlGetDefaultGuiFont());

		CFont fontbold;
		for( int i = 0;i< m_apButtons.GetSize();i++)
		{
			Button *pBtn = m_apButtons[i];

			if ( pBtn->m_rc.left > rcClip.right ||
				 pBtn->m_rc.right < rcClip.left ||
				 pBtn->m_rc.top > rcClip.bottom ||
				 pBtn->m_rc.bottom < rcClip.top) 
				continue;			
			// set bitmap according to the current button state
			int nImage = -1;
			bool bHover = IsHoverMode();
			if(!IsWindowEnabled()||pBtn->m_bEnabled == FALSE )
				nImage = pBtn->m_nImage[_nImageDisabled];
			else if(((m_fPressed == 1)&& (m_pHoverBtn == pBtn))|| ((m_pClickedTabBtn == pBtn)&&IsTabButtons()))
				nImage = pBtn->m_nImage[_nImagePushed];
			else if(bHover && m_pHoverBtn == pBtn)//((!bHover && m_fFocus == 1) || (bHover && m_pHoverBtn == pBtn))
				nImage = pBtn->m_nImage[_nImageFocusOrHover];
			if(nImage == -1)   // not there, use default one
				nImage = pBtn->m_nImage[_nImageNormal];

			if(IsTabButtons())
			{
				if((m_pClickedTabBtn == pBtn))
				{
					fontbold.Attach(AtlCreateBoldFont(AtlGetDefaultGuiFont()));
					dcPaint.SelectFont(fontbold);
				}
				else
				{
					dcPaint.SelectFont(AtlGetDefaultGuiFont());
				}
			}

		    int cx,cy;
			CRect rcItem = pBtn->m_rc;
			pBtn->m_ImageList.GetIconSize(cx,cy);
			if( (cx == rcItem.Width())&&(cy == rcItem.Height()) )
			{
				pBtn->m_ImageList.Draw(dcPaint,nImage,rcItem.TopLeft(),ILD_NORMAL );
				rcItem.DeflateRect(5,0);
				dcPaint.DrawText(pBtn->m_lpstrButtonText,-1,rcItem,
					DT_CENTER|DT_SINGLELINE|DT_VCENTER);
			}
			else
			{
				CDC  memdc;
				memdc.CreateCompatibleDC(dcPaint);
				CBitmap bmp;
				memdc.SaveDC();
				bmp.CreateCompatibleBitmap(dcPaint,rcItem.Width(),rcItem.Height());
				memdc.SelectBitmap(bmp);
				pBtn->m_ImageList.Draw(memdc,nImage,0,0,ILD_NORMAL);
				
				if( IsHorizon())
				{
	
					int scx = cx/3;
					dcPaint.StretchBlt(rcItem.left,rcItem.top,scx,rcItem.Height(),
						                memdc,0,0,scx,cy,SRCCOPY);
					dcPaint.StretchBlt(rcItem.right-scx,rcItem.top,
						               scx,rcItem.Height(),memdc,cx - scx,0,scx,cy,
									   SRCCOPY);
					dcPaint.StretchBlt(rcItem.left+scx,rcItem.top,
						                rcItem.Width()-scx-scx,rcItem.Height(),memdc,
										cx/2,0,1,cy,SRCCOPY);

					rcItem.DeflateRect(5,0);
					dcPaint.DrawText(pBtn->m_lpstrButtonText,-1,rcItem,
						             DT_CENTER|DT_SINGLELINE|DT_VCENTER);
					
				}
// 				else
// 				{
// 					int scy = cy/3;
// 					dcPaint.StretchBlt(rcItem.left,rcItem.top,rcItem.Width(),scy,
// 						               memdc,0,0,cx,scy,SRCCOPY);
// 					dcPaint.StretchBlt(rcItem.left,rcItem.bottom-scy,rcItem.Width(),scy,
// 						               memdc,0,cy-scy,cx,scy,SRCCOPY);
// 					dcPaint.StretchBlt(rcItem.left,rcItem.top+scy,rcItem.Width(),
// 						               rcItem.Height()-scy-scy,memdc,
// 									   0,cy/2,cx,1,SRCCOPY);
// 					//rcItem.DeflateRect(0,0);
// 					CRect rc = rcItem;
// 
// 					CStringEx str;
// 					str.SetString(pBtn->m_lpstrButtonText);
// 					dcPaint.DrawText(str.GetVerString(),-1,
// 						&rc,DT_TOP|DT_CALCRECT);
// 					rc.MoveToXY( rc.left+ ((rcItem.right - rc.right)/2),
// 						        rc.top + (rcItem.bottom - rc.bottom)/2);
// 					dcPaint.DrawText(str.GetVerString(),-1,
// 						&rc,DT_CENTER);


//				}
				memdc.RestoreDC(-1);
			}

		}
		dcPaint.RestoreDC(-1);
	}
	// Overrideables
	void DoPaint(CDCHandle dc)
	{		
		DrawBkgnd( dc );
		DrawButtons( dc );
		return;
	}

	// Message map and handlers
	BEGIN_MSG_MAP(CBitmapButtonsImpl)
		CHAIN_MSG_MAP(CDoubleBufferImpl< CBitmapButtonsImpl >)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseMessage)
		//MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
		//MESSAGE_HANDLER(WM_PAINT, OnPaint)
		//MESSAGE_HANDLER(WM_PRINTCLIENT, OnPaint)
		MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_KILLFOCUS, OnFocus)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_CAPTURECHANGED, OnCaptureChanged)
		MESSAGE_HANDLER(WM_ENABLE, OnEnable)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		MESSAGE_HANDLER(WM_UPDATEUISTATE, OnUpdateUiState)
	END_MSG_MAP()

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Init();
		bHandled = FALSE;
		return 1;
	}
	bool IsValidateIndex( int nIndex)
	{
		if( nIndex < 0 || nIndex > (m_apButtons.GetSize()-1))
			return false;
		return true;
	}
	bool GetClickedRect(CRect&rc)
	{
		if( m_pClickedTabBtn )
		{
			CRect rcBtns;
			GetWindowRect(&rcBtns);
			rc = m_pClickedTabBtn->m_rc;
			rc.MoveToXY(rc.left+rcBtns.left,rc.top + rcBtns.top);
			return true;
		}
		return false;
	}
	int  GetClickedButtonIndex()
	{
		return m_apButtons.Find(m_pClickedTabBtn);
	}
	void ClearClickedBtn()
	{
		m_pClickedTabBtn = NULL;
	}
	int  GetSelectedTabIndex()
	{
		if( IsTabButtons())
		return GetClickedButtonIndex();
		return -1;
	}
	DWORD_PTR GetItemData(int nIndex)
	{
       if(IsValidateIndex(nIndex))
		return	m_apButtons[nIndex]->m_dwData;
	   return 0;
	}
	void SetItemData(int nIndex,DWORD_PTR dwdata)
	{
		if(IsValidateIndex(nIndex))
		m_apButtons[nIndex]->m_dwData = dwdata;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(m_tip.IsWindow())
		{
			m_tip.DestroyWindow();
			m_tip.m_hWnd = NULL;
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnMouseMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		MSG msg = { m_hWnd, uMsg, wParam, lParam };
		if(m_tip.IsWindow())
			m_tip.RelayEvent(&msg);
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnFocus(UINT uMsg, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		m_fFocus = (uMsg == WM_SETFOCUS) ? 1 : 0;
		Invalidate();
		UpdateWindow();
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = 0;
		if(IsHoverMode())
			SetCapture();
		else
			lRet = DefWindowProc(uMsg, wParam, lParam);
		if(::GetCapture() == m_hWnd)
		{
			m_fPressed = 1;
			Invalidate();
			UpdateWindow();
		}
		
		return lRet;
	}

	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = 0;
		if(!IsHoverMode())
			lRet = DefWindowProc(uMsg, wParam, lParam);
		if(::GetCapture() != m_hWnd)
			SetCapture();
		if(m_fPressed == 0)
		{
			m_fPressed = 1;
			Invalidate();
			UpdateWindow();
		}
		return lRet;
	}

	LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
	{
		LRESULT lRet = 0;
		bool bHover = IsHoverMode();

		if( m_pHoverBtn )
		{
			m_pClickedTabBtn = m_pHoverBtn;
		}
		if(!bHover)
			lRet = DefWindowProc(uMsg, wParam, lParam);
		if(::GetCapture() == m_hWnd)
		{
			if(m_fPressed == 1&&m_pHoverBtn)//bHover && 
				::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
			::ReleaseCapture();
		}
		return lRet;
	}

	BOOL  ClickItem(int nIndex)
	{
		if(IsValidateIndex(nIndex))
		{
			m_pClickedTabBtn = m_apButtons[nIndex];
			::SendMessage(GetParent(), WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)m_hWnd);
			Invalidate();
			return TRUE;
		}
		return FALSE;
	}
	LRESULT OnCaptureChanged(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(m_fPressed == 1)
		{
			m_fPressed = 0;
			Invalidate();
			UpdateWindow();
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnEnable(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		Invalidate();
		UpdateWindow();
		bHandled = FALSE;
		return 1;
	}
	void UpdateTip()
	{
		if( m_pHoverBtn )
		{
			m_tip.Activate(TRUE);
			m_tip.AddTool(m_hWnd,m_pHoverBtn->m_lpstrToolTipText,m_pHoverBtn->m_rc,1000);
		}
		else
		{
			m_tip.Activate(FALSE);
			m_tip.DelTool(m_hWnd,1000);
		}
	}

	void InvalidateItem(int nIndex,BOOL bErase = FALSE)
	{
		if(IsValidateIndex(nIndex))
		{
		   InvalidateRect(m_apButtons[nIndex]->m_rc,bErase);
		}
	}
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& bHandled)
	{
		CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));

		Button *pBtn = GetButtonFrom(pt);
	
		
		if( m_pHoverBtn != pBtn)
		{
			CRect rc;
			if( m_pHoverBtn )
			{
				rc = m_pHoverBtn->m_rc;
				m_pHoverBtn = NULL;
				InvalidateRect( rc ,FALSE );
			}
			m_pHoverBtn = pBtn;
			if( m_pHoverBtn )
			rc = m_pHoverBtn->m_rc;
			
			if( IsHoverMode()&&!rc.IsRectNull())
			{
				InvalidateRect( rc ,FALSE );
				//UpdateWindow();
			}
		}
		UpdateTip();
		if( m_fMouseOver == 0)
		{
			m_fMouseOver = 1;
			//Invalidate();
			//UpdateWindow();
			StartTrackMouseLeave();
		}
		if(::GetCapture() == m_hWnd)
		{
			POINT ptCursor = {0};
			ClientToScreen(&ptCursor);
			RECT rect = { 0 };
			GetWindowRect(&rect);
			unsigned int uPressed = ::PtInRect(&rect, ptCursor) ? 1 : 0;
			if(m_fPressed != uPressed)
			{
				m_fPressed = uPressed;
				Invalidate();
				UpdateWindow();
			}
		}
		
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if(m_fMouseOver == 1)
		{
			m_fMouseOver = 0;
			m_pHoverBtn = NULL;
			Invalidate();
			UpdateTip();
			UpdateWindow();
		}
		return 0;
	}

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam == VK_SPACE && IsHoverMode())
			return 0;   // ignore if in hover mode
		if(wParam == VK_SPACE && m_fPressed == 0)
		{
			m_fPressed = 1;
			Invalidate();
			UpdateWindow();
		}
		bHandled = FALSE;
		return 1;
	}

	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		if(wParam == VK_SPACE && IsHoverMode())
			return 0;   // ignore if in hover mode
		if(wParam == VK_SPACE && m_fPressed == 1)
		{
			m_fPressed = 0;
			Invalidate();
			UpdateWindow();
		}
		bHandled = FALSE;
		return 1;
	}


	LRESULT OnUpdateUiState(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		// If the control is subclassed or superclassed, this message can cause
		// repainting without WM_PAINT. We don't use this state, so just do nothing.
		return 0;
	}

	// Implementation
	void Init()
	{
		// We need this style to prevent Windows from painting the button
		ModifyStyle(0, BS_OWNERDRAW);

		// create a tool tip
		m_tip.Create(m_hWnd);
		ATLASSERT(m_tip.IsWindow());
		
	}

	BOOL StartTrackMouseLeave()
	{
		TRACKMOUSEEVENT tme = { 0 };
		tme.cbSize = sizeof(tme);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_hWnd;
		return _TrackMouseEvent(&tme);
	}

	bool IsHoverMode() const
	{
		return ((m_dwExtendedStyle & BMPBTNS_HOVER) != 0);
	}
	bool IsHorizon() const
	{
		return ((m_dwExtendedStyle & BMPBTNS_HORIZON) != 0);
	}
	bool IsTabButtons() const
	{
		return ((m_dwExtendedStyle & BMPBTNS_TAB) != 0);
	}
};


class CBitmapButtons : public CBitmapButtonsImpl<CBitmapButtons>
{
public:
	DECLARE_WND_SUPERCLASS(_T("WTL_BitmapButtons"), GetWndClassName())

	CBitmapButtons(DWORD dwExtendedStyle = BMPBTNS_HORIZON, HIMAGELIST hImageList = NULL) : 
	CBitmapButtonsImpl<CBitmapButtons>(dwExtendedStyle, hImageList)
	{ }
};
