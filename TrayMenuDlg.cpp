
// TrayMenuDlg.cpp: Implementierungsdatei
//

#include "pch.h"
#include "framework.h"
#include "TrayMenu.h"
#include "TrayMenuDlg.h"
#include "afxdialogex.h"
#include "shobjidl.h"
#include "shlguid.h"
#include "strsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MENU_ID_START 33100

// CTrayMenuDlg-Dialogfeld

CTrayMenuDlg::CTrayMenuDlg(CString strFolder, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRAYMENU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MIXED);

	m_hrCoInitialize = CoInitialize(NULL);
	m_DlgMode = DlgMode::INIT;
	m_bIsVisible = FALSE;
	m_bIsReady = FALSE;
	m_bIsMenuOpened = FALSE;
	m_bIncludeDesktop = FALSE;
	m_bIncludeStartmenu = FALSE;
	m_bLocalizedNames = TRUE;
	m_bFoldersFirst = TRUE;
	m_bHoverToOpen = FALSE;
	m_bOpenLeftmost = TRUE;
	m_hbrMenuBackground = 0;
	m_uHotkeyModifiers = MOD_CONTROL;
	m_uHotkeyKeyCode = VK_F1;

	m_menuStyleLight.SetMenuStyle
	(
		MenuStyle::LIGHT, 
		RGB(0, 0, 0),		// Menu text
		RGB(255, 255, 255),	// Menu background
		RGB(128, 128, 128),	// Separator
		RGB(123, 160, 204),	// Selection - outer border
		RGB(232, 241, 250),	// Selection - inner border
		RGB(189, 216, 248)	// Selection - background
	);

	m_menuStyleDark.SetMenuStyle
	(
		MenuStyle::DARK,
		RGB(255, 255, 255),	// Menu text
		RGB(64, 64, 64),	// Menu background
		RGB(128, 128, 128),	// Separator
		RGB(92, 92, 92),	// Selection - outer border
		RGB(92, 92, 92),	// Selection - inner border
		RGB(92, 92, 92)		// Selection - background
	);

	m_menuStyleCustom.SetMenuStyle
	(
		MenuStyle::CUSTOM,
		RGB(0, 0, 0),		// Menu text
		RGB(243, 245, 243),	// Menu background
		RGB(128, 128, 128),	// Separator
		RGB(108, 226, 108),	// Selection - outer border
		RGB(233, 255, 233),	// Selection - inner border
		RGB(191, 229, 191)	// Selection - background
	);

	m_hIconBlue = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_BLUE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconBlack = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_BLACK), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconGreen = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_GREEN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconGrey = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_GREY), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconLime = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_LIME), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconMixed = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_MIXED), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconOrange = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_ORANGE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconPink = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_PINK), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconRed = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_RED), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconViolet = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_VIOLET), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconWhite = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_WHITE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconYellow = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_YELLOW), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

	m_hIconFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_FOLDER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconFile = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_FILE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconAbout = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_ABOUT), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconClose = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CLOSE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconExit = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_EXIT), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconExplorer = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_EXPLORER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconReload = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_RELOAD), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettings = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SETTINGS), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconWebsite = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_WEBSITE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettingsFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SETTINGS_FOLD), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettingsIcon = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SETTINGS_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettingsLocalized = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SETTINGS_LOC), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettingsFoldersFirst = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SETTINGS_FOLDF), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSelectFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_SELECT_FOLDER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconAutostart = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_AUTOSTART), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconAddFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_ADD_FOLDER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconRemoveFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_REM_FOLDER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconBrowseFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_BROWSE_FOLDER), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconShortcut = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CREATE_LNK), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconSettingsClick = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_CLICK), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColors = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColorsDefault = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS_DEF), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColorsLight = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS_LIG), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColorsDark = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS_DRK), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColorsCustom = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS_OWN), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconColorsCustomize = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_COLORS_CUST), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconIncludeFolder = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_INCLUDE), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconHotkey = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_HOTKEY), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconLeftmost = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_LEFTMOST), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);
	m_hIconExportSettings = (HICON)::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_ICON_EXP_SETTINGS), IMAGE_ICON, 0, 0, LR_DEFAULTCOLOR);

	m_hIconSelected = m_hIconBlue;
	m_nIconIndex = 0;
	m_hIconBusy = m_hIconGrey;
	m_strFolder = strFolder;
}

CTrayMenuDlg::~CTrayMenuDlg()
{
	DeleteData();
}

void CTrayMenuDlg::DeleteData()
{
	// Objekte in umgekehrter Reihenfolge ihrer Erstellung zerstören:

	m_entryRoot.Delete();
	m_arrEntries.clear();

	CItemData::Delete(m_arrItemDataMainMenu);

	if (m_hbrMenuBackground)
	{
		::DeleteObject(m_hbrMenuBackground);
		m_hbrMenuBackground = 0;
	}

	if (SUCCEEDED(m_hrCoInitialize))
		CoUninitialize();
}

void CTrayMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY, m_hotKeyCtrl);
	DDX_Control(pDX, IDC_STATIC_MESSAGE, m_stcDlgMessage);
}

BEGIN_MESSAGE_MAP(CTrayMenuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_USER + 1, OnClickedTrayIcon)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_INITMENUPOPUP()
	ON_WM_HOTKEY()
	ON_WM_MENURBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MENUSELECT()
END_MESSAGE_MAP()


// CTrayMenuDlg-Meldungshandler

BOOL CTrayMenuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen.  Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden

	// TODO: Hier zusätzliche Initialisierung einfügen
	
	ModifyStyleEx(WS_EX_APPWINDOW, 0);

	if (!SUCCEEDED(m_hrCoInitialize))
		AfxMessageBox(_T("CoInitialize failed."));

	LoadSettings();
	CreateTrayIcon();
	ReloadMenu();

	return TRUE;  // TRUE zurückgeben, wenn der Fokus nicht auf ein Steuerelement gesetzt wird
}

void CTrayMenuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// Minimieren abfangen
	if (nID == SC_MINIMIZE)
	{
		// Stattdessen Fenster verstecken
		m_DlgMode = DlgMode::INIT;
		ShowWindow(SW_HIDE);
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// Wenn Sie dem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie
//  den nachstehenden Code, um das Symbol zu zeichnen.  Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch ausgeführt.

void CTrayMenuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext zum Zeichnen

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Symbol in Clientrechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Die System ruft diese Funktion auf, um den Cursor abzufragen, der angezeigt wird, während der Benutzer
//  das minimierte Fenster mit der Maus zieht.
HCURSOR CTrayMenuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTrayMenuDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (!m_bIsVisible)
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}

	CDialogEx::OnWindowPosChanging(lpwndpos);
}

void CTrayMenuDlg::OnOK()
{
	switch (m_DlgMode)
	{
	case DlgMode::HOTKEY:
		DefineHotkey();
		break;
	}
	m_DlgMode = DlgMode::INIT;
	ShowWindow(SW_HIDE);
}

void CTrayMenuDlg::OnCancel()
{
	m_DlgMode = DlgMode::INIT;
	ShowWindow(SW_HIDE);
}

CString CTrayMenuDlg::GetFolderName()
{
	int nPos = m_strFolder.ReverseFind('\\');
	if (nPos > 0)
		return m_strFolder.Right(m_strFolder.GetLength() - nPos - 1);
	else
		return _T("TrayMenu");
}

CString CTrayMenuDlg::GetProfileName()
{
	CString strProfile = m_strFolder;
	strProfile.Replace(_T('\\'), _T('/'));
	return _T("Folders\\") + strProfile;
}

void CTrayMenuDlg::LoadSettings()
{
	CString strProfile = GetProfileName();

	if (strProfile.IsEmpty())
		return;

	m_dwMenuIdIconSelected = AfxGetApp()->GetProfileInt(strProfile, _T("TrayIcon"), ID_ICON_BLUE);
	m_bIncludeDesktop = AfxGetApp()->GetProfileInt(strProfile, _T("IncludeDesktop"), FALSE);
	m_bIncludeStartmenu = AfxGetApp()->GetProfileInt(strProfile, _T("IncludeStartmenu"), FALSE);
	m_bLocalizedNames = AfxGetApp()->GetProfileInt(strProfile, _T("LocalizedNames"), TRUE);
	m_bFoldersFirst = AfxGetApp()->GetProfileInt(strProfile, _T("FoldersFirst"), TRUE);
	m_bHoverToOpen = AfxGetApp()->GetProfileInt(strProfile, _T("HoverToOpen"), FALSE);
	m_bOpenLeftmost = AfxGetApp()->GetProfileInt(strProfile, _T("OpenLeftmost"), TRUE);
	m_uHotkeyModifiers = AfxGetApp()->GetProfileInt(strProfile, _T("HotkeyModifiers"), MOD_CONTROL);
	m_uHotkeyKeyCode = AfxGetApp()->GetProfileInt(strProfile, _T("HotkeyKeyCode"), VK_F1);

	int iMenuStyle = AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("Style"), (int)MenuStyle::DEFAULT);
	m_menuStyleCustom.colMenuText = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("TextColor"), (int)m_menuStyleCustom.colMenuText);
	m_menuStyleCustom.colMenuBackground = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("BackgroundColor"), (int)m_menuStyleCustom.colMenuBackground);
	m_menuStyleCustom.colMenuSeparator = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("SeparatorColor"), (int)m_menuStyleCustom.colMenuSeparator);
	m_menuStyleCustom.colMenuSelectionOuter = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("SelectionOuterColor"), (int)m_menuStyleCustom.colMenuSelectionOuter);
	m_menuStyleCustom.colMenuSelectionInner = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("SelectionInnerColor"), (int)m_menuStyleCustom.colMenuSelectionInner);
	m_menuStyleCustom.colMenuSelectionBackg = (COLORREF)AfxGetApp()->GetProfileInt(_T("MenuStyles"), _T("SelectionBackgColor"), (int)m_menuStyleCustom.colMenuSelectionBackg);

	SelectIcon(m_dwMenuIdIconSelected);
	SetMenuStyle((MenuStyle)iMenuStyle, FALSE);
	RegisterHotkey();
}

void CTrayMenuDlg::SaveSettings()
{
	CString strProfile = GetProfileName();

	if (strProfile.IsEmpty())
		return;

	AfxGetApp()->WriteProfileInt(strProfile, _T("TrayIcon"), m_dwMenuIdIconSelected);
	AfxGetApp()->WriteProfileInt(strProfile, _T("IncludeDesktop"), m_bIncludeDesktop);
	AfxGetApp()->WriteProfileInt(strProfile, _T("IncludeStartmenu"), m_bIncludeStartmenu);
	AfxGetApp()->WriteProfileInt(strProfile, _T("LocalizedNames"), m_bLocalizedNames);
	AfxGetApp()->WriteProfileInt(strProfile, _T("FoldersFirst"), m_bFoldersFirst);
	AfxGetApp()->WriteProfileInt(strProfile, _T("HoverToOpen"), m_bHoverToOpen);
	AfxGetApp()->WriteProfileInt(strProfile, _T("OpenLeftmost"), m_bOpenLeftmost);
	AfxGetApp()->WriteProfileInt(strProfile, _T("HotkeyModifiers"), m_uHotkeyModifiers);
	AfxGetApp()->WriteProfileInt(strProfile, _T("HotkeyKeyCode"), m_uHotkeyKeyCode);

	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("Style"), (int)m_menuStyleController.menuStyle);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("TextColor"), (int)m_menuStyleCustom.colMenuText);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("BackgroundColor"), (int)m_menuStyleCustom.colMenuBackground);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("SeparatorColor"), (int)m_menuStyleCustom.colMenuSeparator);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("SelectionOuterColor"), (int)m_menuStyleCustom.colMenuSelectionOuter);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("SelectionInnerColor"), (int)m_menuStyleCustom.colMenuSelectionInner);
	AfxGetApp()->WriteProfileInt(_T("MenuStyles"), _T("SelectionBackgColor"), (int)m_menuStyleCustom.colMenuSelectionBackg);
}

void CTrayMenuDlg::SetMenuStyle(MenuStyle eMenuStyle, BOOL bReloadAndSave)
{
	//BOOL bOwnerDrawOld = m_menuStyleController.bOwnerDraw;

	switch (eMenuStyle)
	{
		case MenuStyle::DEFAULT:
		{
			m_menuStyleController.SetMenuStyle(m_menuStyleDefault);
			break;
		}
		case MenuStyle::LIGHT:
		{
			m_menuStyleController.SetMenuStyle(m_menuStyleLight);
			break;
		}
		case MenuStyle::DARK:
		{
			m_menuStyleController.SetMenuStyle(m_menuStyleDark);
			break;
		}
		case MenuStyle::CUSTOM:
		{
			m_menuStyleController.SetMenuStyle(m_menuStyleCustom);
			break;
		}
	}

	if (m_hbrMenuBackground)
	{
		::DeleteObject(m_hbrMenuBackground);
		m_hbrMenuBackground = 0;
	}

//	m_hbrMenuBackground = ::CreateSolidBrush(m_menuStyleController.colMenuBackground);

	if (bReloadAndSave)
	{
		//if (bOwnerDrawOld != m_menuStyleController.bOwnerDraw)
			ReloadMenu();
		SaveSettings();
	}
}

BOOL CTrayMenuDlg::SelectColor(COLORREF& color)
{
	//CColorDialog dlg(color);
	CMFCColorDialog dlg(color, 0, this);
	if (dlg.DoModal() == IDOK)
	{
		color = dlg.GetColor();
		return TRUE;
	}
	return FALSE;
}

COLORREF CTrayMenuDlg::GetCustomColor(UINT uMenuID)
{
	switch (uMenuID)
	{
		case ID_SETTINGS_COLORS_OWN_TXT:
		{
			return m_menuStyleCustom.colMenuText;
		}
		case ID_SETTINGS_COLORS_OWN_BGD:
		{
			return m_menuStyleCustom.colMenuBackground;
		}
		case ID_SETTINGS_COLORS_OWN_SEP:
		{
			return m_menuStyleCustom.colMenuSeparator;
		}
		case ID_SETTINGS_COLORS_OWN_IOB:
		{
			return m_menuStyleCustom.colMenuSelectionOuter;
		}
		case ID_SETTINGS_COLORS_OWN_IIB:
		{
			return m_menuStyleCustom.colMenuSelectionInner;
		}
		case ID_SETTINGS_COLORS_OWN_IBG:
		{
			return m_menuStyleCustom.colMenuSelectionBackg;
		}
	}
	return -1;
}

void CTrayMenuDlg::SelectIcon(DWORD dwSelection)
{
	switch (dwSelection)
	{
	case ID_ICON_BLUE: m_hIconSelected = m_hIconBlue; m_nIconIndex = 1; break;
	case ID_ICON_BLACK: m_hIconSelected = m_hIconBlack; m_nIconIndex = 2; break;
	case ID_ICON_GREEN: m_hIconSelected = m_hIconGreen; m_nIconIndex = 3; break;
	case ID_ICON_GREY: m_hIconSelected = m_hIconGrey; m_nIconIndex = 4; break;
	case ID_ICON_LIME: m_hIconSelected = m_hIconLime; m_nIconIndex = 11; break;
	case ID_ICON_MULTICOLOR: m_hIconSelected = m_hIconMixed; m_nIconIndex = 0; break;
	case ID_ICON_ORANGE: m_hIconSelected = m_hIconOrange; m_nIconIndex = 5; break;
	case ID_ICON_PINK: m_hIconSelected = m_hIconPink; m_nIconIndex = 6; break;
	case ID_ICON_RED: m_hIconSelected = m_hIconRed; m_nIconIndex = 7; break;
	case ID_ICON_VIOLET: m_hIconSelected = m_hIconViolet; m_nIconIndex = 8; break;
	case ID_ICON_WHITE: m_hIconSelected = m_hIconWhite; m_nIconIndex = 9; break;
	case ID_ICON_YELLOW: m_hIconSelected = m_hIconYellow; m_nIconIndex = 10; break;
	default: m_nIconIndex = 0; return;
	}

	//m_hIconSelected = m_hIconBlue; // default
	m_dwMenuIdIconSelected = dwSelection;

	m_hIconBusy = (m_hIconSelected == m_hIconGrey ? m_hIconBlack : m_hIconGrey);
	UpdateTrayIcon(m_bIsReady ? m_hIconSelected : m_hIconBusy);
}

void CTrayMenuDlg::CreateTrayIcon()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = 100;
	nid.uCallbackMessage = (WM_USER + 1);
	nid.hIcon = m_hIconSelected;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	wcscpy_s(nid.szTip, GetFolderName());

	Shell_NotifyIcon(NIM_ADD, &nid);
}

void CTrayMenuDlg::UpdateTrayIcon(HICON hIcon, LPCWSTR szTooltip)
{
	NOTIFYICONDATA nid;

	nid.uFlags = 0;

	if (hIcon != 0)
	{
		nid.uFlags |= NIF_ICON;
		nid.hIcon = hIcon;
	}

	if (szTooltip != NULL)
	{
		nid.uFlags |= NIF_TIP;
		wcscpy_s(nid.szTip, szTooltip);
	}

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = 100;

	Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void CTrayMenuDlg::DeleteTrayIcon()
{
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = m_hWnd;
	nid.uID = 100;

	Shell_NotifyIcon(NIM_DELETE, &nid);
}

LRESULT CTrayMenuDlg::OnClickedTrayIcon(WPARAM wParam, LPARAM lParam)
{
	if (lParam == (m_bHoverToOpen ? WM_MOUSEMOVE : WM_LBUTTONDOWN))
	{
		OpenMenu();
	}
	else if (lParam == WM_RBUTTONDOWN)
	{
		OpenContextMenu();
	}

	// "Ok" zurückliefern
	return 0;
}

void CTrayMenuDlg::OpenMenu(BOOL bOpenLeftmost)
{
	if (!m_bIsReady)
		return;

	CMenu* pmnuSubMenu = &m_entryRoot.menu;
	if (pmnuSubMenu != NULL)
	{
		CRect rcIcon;
		NOTIFYICONIDENTIFIER niid;
		ZeroMemory(&niid, sizeof(NOTIFYICONIDENTIFIER));
		niid.cbSize = sizeof(NOTIFYICONIDENTIFIER);
		niid.hWnd = m_hWnd;
		niid.uID = 100;
		Shell_NotifyIconGetRect(&niid, rcIcon);

		CRect rcDesktop;
		CWnd::GetDesktopWindow()->GetWindowRect(rcDesktop);

		// Icon rechts oder links von der Bildschirmmitte
		UINT uMenuAlignFlag = ((rcIcon.left + rcIcon.right) / 2 > (rcDesktop.left + rcDesktop.right) / 2) ? TPM_RIGHTALIGN : TPM_LEFTALIGN;
		int nMenuPosX = (uMenuAlignFlag == TPM_RIGHTALIGN) ? rcIcon.right : rcIcon.left;

		// Icon oberhalb oder unterhalb der Bildschirmmitte
		int nMenuPosY = ((rcIcon.top + rcIcon.bottom) / 2 > (rcDesktop.top + rcDesktop.bottom) / 2) ? rcIcon.top : rcIcon.bottom;

		if (bOpenLeftmost)
		{
			uMenuAlignFlag = TPM_LEFTALIGN;
			nMenuPosX = 0;
		}

		SetForegroundWindow();
		m_bIsMenuOpened = TRUE;
		DWORD dwSelection = pmnuSubMenu->TrackPopupMenu(uMenuAlignFlag | TPM_RETURNCMD, nMenuPosX, nMenuPosY, this);
		m_bIsMenuOpened = FALSE;
		if (dwSelection >= MENU_ID_START && dwSelection - MENU_ID_START < m_arrEntries.size())
		{
			CEntry* pEntry = m_arrEntries[dwSelection - MENU_ID_START];
			OpenPath(pEntry->strPath);
		}
	}
}

void CTrayMenuDlg::OpenContextMenu()
{
	// Settings-Submenü "Select folder"
	CMenu mnuSelectFolder;
	mnuSelectFolder.CreatePopupMenu();
	AppendMenuItem(&mnuSelectFolder, ID_BROWSE_FOLDER, _T("Browse..."));
	// Existierende Folder hinzufügen (Auswahl)
	CString strFolder;
	DWORD dwIndex = 0;
	UINT uMenuID = ID_SEL_FOLDER;
	while ((strFolder = FindNextProfile(dwIndex)) != _T("") && uMenuID < ID_SEL_FOLDER_MAX)
	{
		if (uMenuID == ID_SEL_FOLDER) AppendMenuItem(&mnuSelectFolder);
		AppendMenuItem(&mnuSelectFolder, uMenuID, strFolder + (strFolder == m_strFolder ? _T(" \u2714") : _T("")));
		uMenuID++;
	}
	// Existierende Folder hinzufügen (Löschen)
	dwIndex = 0;
	uMenuID = ID_DEL_FOLDER;
	while ((strFolder = FindNextProfile(dwIndex)) != _T("") && uMenuID < ID_DEL_FOLDER_MAX)
	{
		if (strFolder != m_strFolder)
		{
			if (uMenuID == ID_DEL_FOLDER) AppendMenuItem(&mnuSelectFolder);
			AppendMenuItem(&mnuSelectFolder, uMenuID++, _T("Forget ") + strFolder);
		}
	}

	// Settings-Submenü "Include"
	CMenu mnuSettingsInclude;
	mnuSettingsInclude.CreatePopupMenu();
	AppendMenuItem(&mnuSettingsInclude, ID_INC_DESKTOP, _T("Desktop") + CString(m_bIncludeDesktop ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsInclude, ID_INC_STARTMENU, _T("Start menu") + CString(m_bIncludeStartmenu ? _T(" \u2714") : _T("")));

	// Settings-Submenü "Icon"
	CMenu mnuSettingsIcon;
	mnuSettingsIcon.CreatePopupMenu();
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_RED, _T("Red") + CString(m_hIconSelected == m_hIconRed ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_ORANGE, _T("Orange") + CString(m_hIconSelected == m_hIconOrange ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_YELLOW, _T("Yellow") + CString(m_hIconSelected == m_hIconYellow ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_LIME, _T("Lime") + CString(m_hIconSelected == m_hIconLime ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_GREEN, _T("Green") + CString(m_hIconSelected == m_hIconGreen ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_BLUE, _T("Blue") + CString(m_hIconSelected == m_hIconBlue ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_VIOLET, _T("Violet") + CString(m_hIconSelected == m_hIconViolet ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_PINK, _T("Pink") + CString(m_hIconSelected == m_hIconPink ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_MULTICOLOR, _T("Multicolor") + CString(m_hIconSelected == m_hIconMixed ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_BLACK, _T("Black") + CString(m_hIconSelected == m_hIconBlack ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_GREY, _T("Grey") + CString(m_hIconSelected == m_hIconGrey ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsIcon, ID_ICON_WHITE, _T("White") + CString(m_hIconSelected == m_hIconWhite ? _T(" \u2714") : _T("")));

	// Settings-Submenü "Custom"
	CMenu mnuSettingsCustom;
	mnuSettingsCustom.CreatePopupMenu();
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_BGD, _T("Background"));
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_TXT, _T("Item text"));
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_SEP, _T("Separator"));
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_IBG, _T("Selection - background"));
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_IOB, _T("Selection - outer border"));
	AppendMenuItem(&mnuSettingsCustom, ID_SETTINGS_COLORS_OWN_IIB, _T("Selection - inner border"));

	// Settings-Submenü "Menu colors"
	CMenu mnuSettingsColors;
	mnuSettingsColors.CreatePopupMenu();
	AppendMenuItem(&mnuSettingsColors, ID_SETTINGS_COLORS_DEF, _T("Default") + CString (m_menuStyleController.menuStyle == MenuStyle::DEFAULT ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsColors, ID_SETTINGS_COLORS_LIG, _T("Light") + CString(m_menuStyleController.menuStyle == MenuStyle::LIGHT ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsColors, ID_SETTINGS_COLORS_DRK, _T("Dark") + CString(m_menuStyleController.menuStyle == MenuStyle::DARK ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsColors, ID_SETTINGS_COLORS_OWN, _T("Custom") + CString(m_menuStyleController.menuStyle == MenuStyle::CUSTOM ? _T(" \u2714") : _T("")));
	AppendMenuItem(&mnuSettingsColors);
	AppendMenuItem(&mnuSettingsColors, ID_SETTINGS_COLORS_CUST, _T("Customize"), &mnuSettingsCustom);

	// Settings-Submenü "Autostart"
	CMenu mnuAutostart;
	mnuAutostart.CreatePopupMenu();
	// Item zum Hinzufügen des aktuellen Ordners zum Autostart
	BOOL bAddItem = !FindInAutostart(m_strFolder, FALSE);
	if (bAddItem) AppendMenuItem(&mnuAutostart, ID_ADD_FOLDER, _T("Add ") + m_strFolder);
	// Items zum Entfernen von Ordnern aus dem Autostart
	dwIndex = 0;
	uMenuID = ID_REM_FOLDER;
	m_arrAutostartFolders.clear();
	while ((strFolder = FindNextAutostartFolder(dwIndex)) != _T("") && uMenuID < ID_REM_FOLDER_MAX)
	{
		if (bAddItem && uMenuID == ID_REM_FOLDER) AppendMenuItem(&mnuAutostart);
		AppendMenuItem(&mnuAutostart, uMenuID, _T("Remove ") + strFolder + (strFolder == m_strFolder ? _T(" \u2714") : _T("")));
		m_arrAutostartFolders.push_back(strFolder);
		uMenuID++;
	}

	// Submenü "Settings"
	CMenu mnuSettings;

	mnuSettings.CreatePopupMenu();
	AppendMenuItem(&mnuSettings, ID_SELECT_FOLDER, _T("Select folder"), &mnuSelectFolder);
	AppendMenuItem(&mnuSettings, ID_INCLUDE_FOLDER, _T("Include"), &mnuSettingsInclude);
	AppendMenuItem(&mnuSettings, ID_SETTINGS_ICON, _T("Tray icon"), &mnuSettingsIcon);
	AppendMenuItem(&mnuSettings, ID_SETTINGS_COLORS, _T("Menu colors"), &mnuSettingsColors);
	AppendMenuItem(&mnuSettings, ID_AUTOSTART, _T("Autostart"), &mnuAutostart);
	AppendMenuItem(&mnuSettings);
	AppendMenuItem(&mnuSettings, ID_CREATE_SHORTCUT, _T("Create shortcut..."));
	AppendMenuItem(&mnuSettings, ID_DEFINE_HOTKEY, _T("Define hotkey..."));
	AppendMenuItem(&mnuSettings, ID_EXPORT_SETTINGS, _T("Export settings..."));
	AppendMenuItem(&mnuSettings);
	AppendMenuItem(&mnuSettings, ID_SETTINGS_LOCALIZE, m_bLocalizedNames ? _T("Localized names \u2714") : _T("Localized names"));
	AppendMenuItem(&mnuSettings, ID_SETTINGS_FOLDF, m_bFoldersFirst ? _T("Folders first \u2714") : _T("Folders first"));
	AppendMenuItem(&mnuSettings, ID_SETTINGS_CLICK, m_bHoverToOpen ? _T("Hover to open \u2714") : _T("Hover to open"));
	AppendMenuItem(&mnuSettings, ID_SETTINGS_LEFTMOST, m_bOpenLeftmost ? _T("Open leftmost \u2714") : _T("Open leftmost"));

	// Submenü "About"
	CMenu mnuAbout;
	mnuAbout.CreatePopupMenu();

	AppendMenuItem(&mnuAbout, ID_ABOUT_TITLE, _T("TrayMenu"));
	AppendMenuItem(&mnuAbout, ID_ABOUT_VERSION, _T("Version 1.2"));
	AppendMenuItem(&mnuAbout, ID_ABOUT_WEBSITE, _T("Go to website"));

	mnuAbout.SetDefaultItem(ID_ABOUT_TITLE);
	mnuAbout.EnableMenuItem(ID_ABOUT_TITLE, MF_DISABLED);
	mnuAbout.EnableMenuItem(ID_ABOUT_VERSION, MF_DISABLED);

	// Tray-Icon-Kontextmenü
	CMenu mnuContextMenu;
	mnuContextMenu.CreatePopupMenu();

	AppendMenuItem(&mnuContextMenu, ID_OPEN_FOLDER, _T("Explore"));
	AppendMenuItem(&mnuContextMenu, ID_RELOAD, _T("Reload"));
	AppendMenuItem(&mnuContextMenu, ID_SETTINGS, _T("Settings"), &mnuSettings);
	AppendMenuItem(&mnuContextMenu, ID_ABOUT, _T("About"), &mnuAbout);
	AppendMenuItem(&mnuContextMenu, ID_EXIT, _T("Exit"));
	AppendMenuItem(&mnuContextMenu);
	AppendMenuItem(&mnuContextMenu, ID_CLOSE, _T("Close"));

	CPoint ptMenuPos;
	GetCursorPos(&ptMenuPos);
	SetForegroundWindow();
	DWORD dwSelection = mnuContextMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD, ptMenuPos.x, ptMenuPos.y, this);
	switch (dwSelection)
	{
		case ID_RELOAD:
		{
			ReloadMenu();
			break;
		}
		case ID_SETTINGS:
		{
			m_bIsVisible = TRUE;
			ShowWindow(SW_RESTORE);
			break;
		}
		case ID_OPEN_FOLDER:
		{
			ExploreFolder(m_strFolder);
			break;
		}
		case ID_ABOUT_WEBSITE:
		{
			BrowseToWebsite();
			break;
		}
		case ID_EXIT:
		{
			DeleteTrayIcon();
			EndDialog(IDCANCEL);
			break;
		}
		case ID_INC_DESKTOP:
		{
			m_bIncludeDesktop = !m_bIncludeDesktop;
			ReloadMenu();
			SaveSettings();
			break;
		}
		case ID_INC_STARTMENU:
		{
			m_bIncludeStartmenu = !m_bIncludeStartmenu;
			ReloadMenu();
			SaveSettings();
			break;
		}
		case ID_SETTINGS_LOCALIZE:
		{
			m_bLocalizedNames = !m_bLocalizedNames;
			ReloadMenu();
			SaveSettings();
			break;
		}
		case ID_SETTINGS_FOLDF:
		{
			m_bFoldersFirst = !m_bFoldersFirst;
			ReloadMenu();
			SaveSettings();
			break;
		}
		case ID_BROWSE_FOLDER:
		{
			if (BrowseFolder())
			{
				LoadSettings();
				ReloadMenu();
			}
			break;
		}
		case ID_CREATE_SHORTCUT:
		{
			CreateShortcut();
			break;
		}
		case ID_ADD_FOLDER:
		{
			AddToAutostart(m_strFolder);
			break;
		}
		case ID_SETTINGS_CLICK:
		{
			m_bHoverToOpen = !m_bHoverToOpen;
			SaveSettings();
			break;
		}
		case ID_SETTINGS_LEFTMOST:
		{
			m_bOpenLeftmost = !m_bOpenLeftmost;
			SaveSettings();
			break;
		}
		case ID_SETTINGS_COLORS_DEF:
		{
			SetMenuStyle(MenuStyle::DEFAULT, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_LIG:
		{
			SetMenuStyle(MenuStyle::LIGHT, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_DRK:
		{
			SetMenuStyle(MenuStyle::DARK, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN:
		{
			SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_TXT:
		{
			if (SelectColor(m_menuStyleCustom.colMenuText))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_BGD:
		{
			if (SelectColor(m_menuStyleCustom.colMenuBackground))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_SEP:
		{
			if (SelectColor(m_menuStyleCustom.colMenuSeparator))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_IOB:
		{
			if (SelectColor(m_menuStyleCustom.colMenuSelectionOuter))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_IIB:
		{
			if (SelectColor(m_menuStyleCustom.colMenuSelectionInner))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_SETTINGS_COLORS_OWN_IBG:
		{
			if (SelectColor(m_menuStyleCustom.colMenuSelectionBackg))
				SetMenuStyle(MenuStyle::CUSTOM, TRUE);
			break;
		}
		case ID_DEFINE_HOTKEY:
		{
			ShowDialogHotkey();
			break;
		}
		case ID_EXPORT_SETTINGS:
		{
			ExportSettings();
			break;
		}
		default:
		{
			if (dwSelection >= ID_SEL_FOLDER && dwSelection <= ID_SEL_FOLDER_MAX)
			{
				CItemData* pItemData = CItemData::Find(m_arrItemDataContextMenu, dwSelection);
				if (pItemData && pItemData->strCaption != m_strFolder)
				{
					m_strFolder = pItemData->strCaption;
					LoadSettings();
					ReloadMenu();
				}
			}
			else if (dwSelection >= ID_DEL_FOLDER && dwSelection <= ID_DEL_FOLDER_MAX)
			{
				CItemData* pItemData = CItemData::Find(m_arrItemDataContextMenu, dwSelection);
				if (pItemData)
				{
					RemoveProfile(pItemData->strCaption);
				}
			}
			else if (dwSelection >= ID_REM_FOLDER && dwSelection <= ID_REM_FOLDER_MAX)
			{
				CString strFolder = m_arrAutostartFolders[dwSelection - ID_REM_FOLDER];
				FindInAutostart(strFolder, TRUE); // TRUE = Eintrag löschen
			}
			else
			{
				SelectIcon(dwSelection);
				SaveSettings();
			}
		}
	}
	mnuContextMenu.DestroyMenu();
	CItemData::Delete(m_arrItemDataContextMenu);
}

BOOL CTrayMenuDlg::ReloadMenu()
{
	UpdateTrayIcon(m_hIconBusy, _T("Loading..."));

	if (m_bIsReady)
	{
		m_bIsReady = false;
		DeleteData();
	}

	m_bIsReady = ReadFolder() && CreateMenu();
	UpdateTrayIcon(m_hIconSelected, m_entryRoot.strDisplayName);
	return m_bIsReady;
}

BOOL CTrayMenuDlg::ReadFolder()
{
	m_nCurrentMenuID = MENU_ID_START;

	m_entryRoot.bIsRootElement = TRUE;
	m_entryRoot.uMenuID = m_nCurrentMenuID;
	m_entryRoot.eEntryType = EntryType::DIR;
	m_entryRoot.strPath = m_strFolder;
	m_entryRoot.strName = GetFolderName();
	m_entryRoot.strDisplayName = GetLocalizedName(m_strFolder, m_entryRoot.strName);

	m_nCurrentMenuID++;
	m_arrEntries.push_back(&m_entryRoot);

	BOOL bResult = AddPath(&m_entryRoot, m_strFolder);

	if (m_bIncludeDesktop)
	{
		if (EnabledSystemFolder(CSIDL_DESKTOP)) bResult &= AddSystemFolder(CSIDL_DESKTOP, TRUE);
		if (EnabledSystemFolder(CSIDL_COMMON_DESKTOPDIRECTORY)) bResult &= AddSystemFolder(CSIDL_COMMON_DESKTOPDIRECTORY, TRUE, CSIDL_DESKTOP);
	}
			
	if (m_bIncludeStartmenu)
	{
		if (EnabledSystemFolder(CSIDL_STARTMENU)) bResult &= AddSystemFolder(CSIDL_STARTMENU);
		if (EnabledSystemFolder(CSIDL_COMMON_STARTMENU)) bResult &= AddSystemFolder(CSIDL_COMMON_STARTMENU);
	}

	m_entryRoot.SortChildren();
	return bResult;
}

BOOL CTrayMenuDlg::AddPath(CEntry* pEntry, CString strFolder, CString strPattern, CString strMergeFolder)
{
	WIN32_FIND_DATA data;
	HANDLE hFile = FindFirstFile(strFolder + "\\" + strPattern, &data);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			CString strPath = strFolder + "\\" + CString(data.cFileName);

			if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // gefundener Eintrag ist ein Verzeichnis
			{
				if (CString(data.cFileName) == "." || CString(data.cFileName) == "..")
					continue;

				CString strDisplayName = GetLocalizedName(strPath, data.cFileName);
				CString strExistingFolder = strDisplayName;
				if (!strMergeFolder.IsEmpty()) strExistingFolder = strMergeFolder;
				CEntry* pEntryExistingChild = pEntry->FindChild(strExistingFolder, EntryType::DIR);
				if (pEntryExistingChild)
				{
					// Ein Verzeichnis-Element mit diesem Namen existiert schon (nach .lnk-Auflösung):
					// Einträge aus diesem Verzeichnis dem bestehenden Verzeichnis-Element hinzufügen.
					AddPath(pEntryExistingChild, strPath); // Verzeichnis durchsuchen (rekursiver Aufruf)
				}
				else
				{
					// Neues Verzeichnis-Element erzeugen und Einträge aus dem Verzeichnis hinzufügen.
					CEntry* pNewEntry = new CEntry;
					pNewEntry->uMenuID = m_nCurrentMenuID;
					pNewEntry->eEntryType = EntryType::DIR;
					pNewEntry->strPath = strPath;
					pNewEntry->strName = data.cFileName;
					pNewEntry->strDisplayName = strDisplayName;
					pNewEntry->bMenuStyleOwnerDraw = m_menuStyleController.bOwnerDraw;

					pEntry->children.push_back(pNewEntry);
					m_arrEntries.push_back(pNewEntry);
					m_nCurrentMenuID++;

					AddPath(pNewEntry, strPath); // Verzeichnis durchsuchen (rekursiver Aufruf)
				}
			}
			else if (!(data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) // gefundener Eintrag ist eine Datei (nicht versteckt)
			{
				CEntry* pNewEntry = new CEntry;
				pNewEntry->uMenuID = m_nCurrentMenuID;
				pNewEntry->eEntryType = EntryType::FILE;
				pNewEntry->strPath = strPath;
				pNewEntry->strName = data.cFileName;
				pNewEntry->strDisplayName = GetLocalizedName(strPath, data.cFileName);
				pNewEntry->bMenuStyleOwnerDraw = m_menuStyleController.bOwnerDraw;

				// Datei-Verknüpfungen (*.lnk)
				if (pNewEntry->strDisplayName.Right(4).CompareNoCase(_T(".lnk")) == 0)
				{
					// Dateiendung ".lnk" entfernen
					pNewEntry->strDisplayName = pNewEntry->strDisplayName.Left(pNewEntry->strDisplayName.GetLength() - 4);

					// target*.lnk auflösen
					if (pNewEntry->strName.Left(6).CompareNoCase(_T("target")) == 0)
					{
						WCHAR szLinkPath[MAX_PATH];
						HRESULT hr = ResolveLnk(m_hWnd, CT2A(strPath), szLinkPath, MAX_PATH);
						if (SUCCEEDED(hr)) pNewEntry->strLinkPath = szLinkPath;

						// Statt einen "target*"-Eintrag im Menü zu erzeugen, die Einträge des Verweisziels ins Menü einbauen:
						if (!pNewEntry->strLinkPath.IsEmpty())
						{
							// Weiter rekursiv im Zielverzeichnis des target*.lnk suchen
							delete pNewEntry;
							AddPath(pEntry, szLinkPath);
							continue;
						}
					}
				}
				// URL-Verknüpfungen (*.url)
				else if (pNewEntry->strDisplayName.Right(4).CompareNoCase(_T(".url")) == 0)
				{
					// Dateiendung ".url" entfernen
					pNewEntry->strDisplayName = pNewEntry->strDisplayName.Left(pNewEntry->strDisplayName.GetLength() - 4);
				}

				// Duplikate numerieren
				if (pEntry->FindChild(pNewEntry->strDisplayName, EntryType::FILE))
				{
					CString strDisplayNameNew;
					int n = 2;
					do
					{
						strDisplayNameNew.Format(_T("%s (%d)"), (LPCTSTR)pNewEntry->strDisplayName, n++);
					} while (pEntry->FindChild(strDisplayNameNew, EntryType::FILE) != NULL);
					pNewEntry->strDisplayName = strDisplayNameNew;
				}

				pEntry->children.push_back(pNewEntry);
				m_arrEntries.push_back(pNewEntry);
				m_nCurrentMenuID++;
			}
		}
		while (FindNextFile(hFile, &data));
		FindClose(hFile);
		return TRUE;
	}
	return FALSE;
}

BOOL CTrayMenuDlg::EnabledSystemFolder(int csidl)
{
	WCHAR path[MAX_PATH];
	HRESULT hr = SHGetFolderPath(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, path);
	return SUCCEEDED(hr) && !CString(path).IsEmpty() && m_strFolder.CompareNoCase(path) != 0;
}

BOOL CTrayMenuDlg::AddSystemFolder(int csidl, BOOL bAddFolder, int csidlMerge)
{
	WCHAR path[MAX_PATH];
	if (S_OK == SHGetFolderPath(NULL, csidl, NULL, SHGFP_TYPE_CURRENT, path) && !CString(path).IsEmpty())
	{
		if (bAddFolder)
		{
			CString strFolder = path;
			CString strPattern = _T("*");
			int nPos = strFolder.ReverseFind(_T('\\'));
			if (nPos > 0 && nPos < strFolder.GetLength())
			{
				strPattern = strFolder.Mid(nPos + 1);
				strFolder = strFolder.Left(nPos);
			}

			if (csidlMerge != -1 && S_OK == SHGetFolderPath(NULL, csidlMerge, NULL, SHGFP_TYPE_CURRENT, path) && !CString(path).IsEmpty())
			{
				CString strMergePath = path;
				CString strMergeFolder;
				int nPos = strMergePath.ReverseFind(_T('\\'));
				if (nPos > 0 && nPos < strMergePath.GetLength())
				{
					strMergeFolder = strMergePath.Mid(nPos + 1);
					strMergeFolder = GetLocalizedName(strMergePath, strMergeFolder);
				}

				return AddPath(&m_entryRoot, strFolder, strPattern, strMergeFolder);
			}
			else
			{
				return AddPath(&m_entryRoot, strFolder, strPattern);
			}
		}
		else
		{
			return AddPath(&m_entryRoot, path);
		}
	}
	return TRUE;
}

BOOL CTrayMenuDlg::CreateMenu()
{
	BOOL bResult = AddItem(&m_entryRoot, NULL);

	CMenu* pmnuSubMenu = &m_entryRoot.menu;
	if (pmnuSubMenu != NULL)
	{
		AppendMenuItem(pmnuSubMenu);
		AppendMenuItem(pmnuSubMenu, ID_CLOSE, _T("Close"), NULL, TRUE);
	}

	return bResult;
}

BOOL CTrayMenuDlg::AddItem(CEntry* pEntry, CMenu* pmnuParent)
{
	if (pEntry->children.size() > 0) // Verzeichnis: Submenü erzeugen
	{
		pEntry->menu.CreatePopupMenu();
		CMenu* pmnuSubMenu = &pEntry->menu;

		// Für die Kind-Elemente Unterverzeichnisse erzeugen (rekursiver Aufruf)
		vector<CEntry*>::iterator it;
		if (m_bFoldersFirst)
		{
			BOOL bShowSeparator = FALSE;
			for (it = pEntry->children.begin(); it != pEntry->children.end(); it++)
			{
				if ((*it)->eEntryType == EntryType::DIR && (*it)->children.size() > 0)
				{
					bShowSeparator = TRUE;
					AddItem(*it, pmnuSubMenu);
				}
			}
			for (it = pEntry->children.begin(); it != pEntry->children.end(); it++)
			{
				if ((*it)->eEntryType != EntryType::DIR)
				{
					if (bShowSeparator)
					{
						AppendMenuItem(pmnuSubMenu);
						bShowSeparator = FALSE;
					}
					AddItem(*it, pmnuSubMenu);
				}
			}
		}
		else
		{
			for (it = pEntry->children.begin(); it != pEntry->children.end(); it++)
			{
				if ((*it)->eEntryType != EntryType::DIR || (*it)->children.size() > 0)
					AddItem(*it, pmnuSubMenu);
			}
		}
		AppendMenuItem(pmnuParent, pEntry->uMenuID, pEntry->GetMenuString(), pmnuSubMenu);
	}
	else // Datei: Menü-Item erzeugen
	{
		AppendMenuItem(pmnuParent, pEntry->uMenuID, pEntry->GetMenuString());
	}

	return TRUE;
}

void CTrayMenuDlg::AppendMenuItem(CMenu* pMenu, UINT uMenuID, LPCWSTR szCaption, CMenu* pSubMenu, BOOL bAddToItemDataMainMenu)
{
	if (pMenu == NULL) return;

	UINT uOwnerDrawFlag = (m_menuStyleController.bOwnerDraw ? MF_OWNERDRAW : 0);
	ItemType eItemType = ItemType::INIT;

	if (pSubMenu)
	{
		// Submenü
		eItemType = ItemType::POPUP;
		pMenu->AppendMenu(MF_POPUP | uOwnerDrawFlag, (UINT_PTR)pSubMenu->m_hMenu, szCaption);

		// Da Submenüs mittels AppendMenu() keine IDs zugeordnet werden können, dies nachträglich tun, indem
		// unmittelbar nach dem AppendMenu-Aufruf dem zuletzt hinzugefügten Submenü-Item die Item-Info aktualisiert wird:
		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_ID;
		pMenu->GetMenuItemInfo(pMenu->GetMenuItemCount() - 1, &mii, TRUE);
		mii.wID = uMenuID;
		pMenu->SetMenuItemInfo(pMenu->GetMenuItemCount() - 1, &mii, TRUE);
	}
	else if (uMenuID)
	{
		// normales Item
		eItemType = ItemType::STRING;
		pMenu->AppendMenu(MF_STRING | uOwnerDrawFlag, uMenuID, szCaption);
	}
	else
	{
		// Separator
		eItemType = ItemType::SEPARATOR;
		pMenu->AppendMenu(MF_SEPARATOR | uOwnerDrawFlag, ID_SEPARATOR);
	}

	if (m_menuStyleController.bOwnerDraw)
	{
		// Hintergrundfarbe setzen
		if (!m_hbrMenuBackground)
			m_hbrMenuBackground = ::CreateSolidBrush(m_menuStyleController.colMenuBackground);

		MENUINFO mi = { 0 };
		mi.cbSize = sizeof(MENUINFO);
		pMenu->GetMenuInfo(&mi);
		mi.hbrBack = m_hbrMenuBackground;
		mi.fMask = MIM_BACKGROUND | MIM_STYLE;
//		mi.dwStyle = MIM_APPLYTOSUBMENUS;
		pMenu->SetMenuInfo(&mi);
	}

	if (uMenuID >= MENU_ID_START || bAddToItemDataMainMenu)
		m_arrItemDataMainMenu.push_back(new CItemData(pMenu, uMenuID, szCaption, eItemType));
	else if (uMenuID)
		m_arrItemDataContextMenu.push_back(new CItemData(pMenu, uMenuID, szCaption, eItemType));
}

void CTrayMenuDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis)
{
	if ((lpdis == NULL) || (lpdis->CtlType != ODT_MENU))
	{
		CDialogEx::OnDrawItem(nIDCtl, lpdis);
		return; //not for a menu
	}

	CDC* pDC = CDC::FromHandle(lpdis->hDC);

	if (m_menuStyleController.bOwnerDraw)
	{
		pDC->SelectClipRgn(NULL); // Zeichnen erlauben
		CRect rcItem(lpdis->rcItem);

		// Hintergrund des Items
		pDC->FillSolidRect(rcItem, m_menuStyleController.colMenuBackground);

		// Selektiertes Item
		if ((lpdis->itemState & ODS_SELECTED) &&
			(lpdis->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
		{
			pDC->FillSolidRect(rcItem, m_menuStyleController.colMenuSelectionOuter);
			rcItem.left++, rcItem.top++, rcItem.bottom--, rcItem.right--;
			pDC->FillSolidRect(rcItem, m_menuStyleController.colMenuSelectionInner);
			rcItem.left++, rcItem.top++, rcItem.bottom--, rcItem.right--;
			pDC->FillSolidRect(rcItem, m_menuStyleController.colMenuSelectionBackg);
		}

		if (lpdis->itemID == ID_SEPARATOR)
		{
			CRect rcSeparator(rcItem);
			rcSeparator.top = rcSeparator.top + rcSeparator.Height() / 2;
			rcSeparator.bottom = rcSeparator.top + 1;
			rcSeparator.left += 1;
			rcSeparator.right -= 1;
			pDC->FillSolidRect(rcSeparator, m_menuStyleController.colMenuSeparator);
		}
		else
		{
			CItemData* pItemData = CItemData::Find(m_arrItemDataContextMenu, lpdis->itemID);
			if (!pItemData) pItemData = CItemData::Find(m_arrItemDataMainMenu, lpdis->itemID);
			if (pItemData)
			{
				pDC->SetTextColor(m_menuStyleController.colMenuText);
				pDC->TextOut(lpdis->rcItem.left + 24, lpdis->rcItem.top + 3, pItemData->strCaption);

				if (pItemData->eItemType == ItemType::POPUP)
				{
					//pDC->TextOut(lpdis->rcItem.right - 10, lpdis->rcItem.top + 3, _T(">"));
					pDC->TextOut(lpdis->rcItem.right - 10, lpdis->rcItem.top + 3, _T("\u276F")); // ❯
					//pDC->TextOut(lpdis->rcItem.right - 10, lpdis->rcItem.top + 3, _T("\u2BC8")); // ⯈
					//pDC->TextOut(lpdis->rcItem.right - 10, lpdis->rcItem.top + 3, _T("\u1433")); // ᐳ
				}
			}
		}
	}

	// Farben
	COLORREF color = GetCustomColor(lpdis->itemID);
	if (color != -1 && m_menuStyleController.bOwnerDraw)
	{
		CRect rcIcon(lpdis->rcItem.left + 4, lpdis->rcItem.top + 3, lpdis->rcItem.left + 20, lpdis->rcItem.top + 19);
		pDC->FillSolidRect(rcIcon, m_menuStyleController.colMenuSeparator);
		rcIcon.left++, rcIcon.top++, rcIcon.bottom--, rcIcon.right--;
		pDC->FillSolidRect(rcIcon, color);
	}
	else
	{
		// Icon
		HICON hIcon = GetIconForItem(lpdis->itemID);
		if (hIcon)
		{
			ICONINFO iconinfo;
			::GetIconInfo(hIcon, &iconinfo);

			BITMAP bitmap;
			::GetObject(iconinfo.hbmColor, sizeof(bitmap), &bitmap);
			::DeleteObject(iconinfo.hbmColor);
			::DeleteObject(iconinfo.hbmMask);

			int iIconOffsetX = (m_menuStyleController.bOwnerDraw ? 4 : 0);
			int iIconOffsetY = (m_menuStyleController.bOwnerDraw ? 3 : 0);

			::DrawIconEx(lpdis->hDC, lpdis->rcItem.left + iIconOffsetX, lpdis->rcItem.top + iIconOffsetY,
				hIcon, bitmap.bmWidth, bitmap.bmHeight, 0, NULL, DI_NORMAL);
		}
	}

	if (m_menuStyleController.bOwnerDraw)
		pDC->IntersectClipRect(0, 0, 0, 0); // Zeichnen verbieten (damit Windows keine eigenen Submenü-Pfeile malt)
}

void CTrayMenuDlg::OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialogEx::OnInitMenuPopup(pMenu, nIndex, bSysMenu);

	if (bSysMenu)
	{
		pMenu = GetSystemMenu(FALSE);
	}

	// Icon
	MENUINFO mi;
	mi.cbSize = sizeof(mi);
	mi.fMask = MIM_STYLE;
	mi.dwStyle = MNS_CHECKORBMP | MNS_AUTODISMISS;
	pMenu->SetMenuInfo(&mi);

	MENUITEMINFO mii;
	mii.cbSize = sizeof(mii);

	for (int pos = 0; pos < pMenu->GetMenuItemCount(); pos++)
	{
		mii.fMask = MIIM_FTYPE | MIIM_ID;
		pMenu->GetMenuItemInfo(pos, &mii, TRUE);

		HICON hIcon = GetIconForItem(mii.wID);

		if (hIcon)
		{
			if (!(mii.fType & MFT_OWNERDRAW))
			{
				mii.fMask = MIIM_FTYPE | MIIM_BITMAP;
				mii.hbmpItem = HBMMENU_CALLBACK;
				mii.fType = MFT_STRING;

				pMenu->SetMenuItemInfo(pos, &mii, TRUE);
			}
		}
	}
}

void CTrayMenuDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpmis)
{
	if (lpmis == NULL || lpmis->CtlType != ODT_MENU)
	{
		CDialogEx::OnMeasureItem(nIDCtl, lpmis); //not for a menu
		return;
	}

	lpmis->itemHeight = 16;
	lpmis->itemWidth = 16;

	if (m_menuStyleController.bOwnerDraw)
	{
		if (lpmis->itemID == ID_SEPARATOR)
		{
			lpmis->itemHeight = 9;
		}
		else
		{
			lpmis->itemHeight = 22; // Höhe der Items
			lpmis->itemWidth = 24; // Breite des Platzes für das Icon links vom Itemtext

			CItemData* pItemData = CItemData::Find(m_arrItemDataContextMenu, lpmis->itemID);
			if (!pItemData) pItemData = CItemData::Find(m_arrItemDataMainMenu, lpmis->itemID);
			if (pItemData)
			{
				// Textgröße des Items ermitteln. Vorher muss der für das Menü verwendete Font
				// gesetzt werden, andernfalls ist die ermittelte Textlänge um ca. den Faktor 1,2
				// zu groß (z.B. 90 statt 78 Pixel für den String "Anwendungen").
				NONCLIENTMETRICS metrics = { 0 };
				metrics.cbSize = sizeof(metrics);
				BOOL bResult = SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &metrics, 0);
				if (bResult)
				{
					HFONT fontMenu = CreateFontIndirect(&metrics.lfMenuFont);
					CFont* pFontMenu = CFont::FromHandle(fontMenu);
					CDC* pDC = GetDC();
					CFont* pFontOld = (CFont*)pDC->SelectObject(pFontMenu);
					CSize size(pDC->GetTextExtent(pItemData->strCaption));
					pDC->SelectObject(pFontOld);
					ReleaseDC(pDC);
					DeleteObject(fontMenu);
					lpmis->itemWidth += size.cx;
				}

				// Wenn ein Submenü-Pfeil gezeichnet werden soll, dann 10 px hinzugeben
				if (pItemData->eItemType == ItemType::POPUP)
					lpmis->itemWidth += 10;
			}
		}
	}
}

void CTrayMenuDlg::OnMenuRButtonUp(UINT uMenuPos, CMenu* pMenu)
{
	// Wird nur bei Rechtsklick auf Items aufgerufen, die keine Submenüs öffnen.
	UINT uMenuId = pMenu->GetMenuItemID(uMenuPos);
	if (uMenuId >= MENU_ID_START && uMenuId - MENU_ID_START < m_arrEntries.size())
	{
		CEntry* pEntry = m_arrEntries[uMenuId - MENU_ID_START];
		CPoint ptMenuPos;
		GetCursorPos(&ptMenuPos);
		OpenShellContextMenu(pEntry->strPath, ptMenuPos.x, ptMenuPos.y, m_hWnd);
	}
}

void CTrayMenuDlg::OnRButtonUp(UINT nFlags,	CPoint point)
{
	// Rechtsklick auf ein zuvor gemerktes Submenü-Item:
	if (m_rcItemMenuSelect.PtInRect(point))
	{
		if (m_uItemIDMenuSelect >= MENU_ID_START && m_uItemIDMenuSelect - MENU_ID_START < m_arrEntries.size())
		{
			CEntry* pEntry = m_arrEntries[m_uItemIDMenuSelect - MENU_ID_START];
			OpenShellContextMenu(pEntry->strPath, point.x, point.y, m_hWnd);
		}
	}
}

void CTrayMenuDlg::OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu)
{
	// Selektiertes Submenü-Item merken:
	if (nFlags & MF_POPUP)
	{
		MENUITEMINFO mii;
		mii.cbSize = sizeof(mii);
		mii.fMask = MIIM_ID;

		if (GetMenuItemInfo(hSysMenu, nItemID, TRUE, &mii))
		{
			GetMenuItemRect(m_hWnd, hSysMenu, nItemID, m_rcItemMenuSelect);
			m_uItemIDMenuSelect = mii.wID;
		}
	}
}

bool CTrayMenuDlg::OpenShellContextMenu(const CString& strPath, int xPos, int yPos, HWND hwndParent)
{
	ITEMIDLIST* idList = 0;
	HRESULT result = SHParseDisplayName(strPath, 0, &idList, 0, 0);
	if (!SUCCEEDED(result) || !idList)
		return false;
	CItemIdListReleaser idListReleaser(idList);

	IShellFolder* iFolder = 0;

	LPCITEMIDLIST idChild = 0;
	result = SHBindToParent(idList, IID_IShellFolder, (void**)&iFolder, &idChild);
	if (!SUCCEEDED(result) || !iFolder)
		return false;
	CComInterfaceReleaser iFolderReleaser(iFolder);

	IContextMenu* iMenu = 0;
	result = iFolder->GetUIObjectOf(hwndParent, 1, (const ITEMIDLIST**)&idChild, IID_IContextMenu, 0, (void**)&iMenu);
	if (!SUCCEEDED(result) || !iFolder)
		return false;
	CComInterfaceReleaser menuReleaser(iMenu);

	HMENU hMenu = CreatePopupMenu();
	if (!hMenu)
		return false;
	if (SUCCEEDED(iMenu->QueryContextMenu(hMenu, 0, 1, 0x7FFF, CMF_NORMAL)))
	{
		int iCmd = TrackPopupMenuEx(hMenu, TPM_RETURNCMD | TPM_RECURSE, xPos, yPos, hwndParent, NULL);
		if (iCmd > 0)
		{
			SendMessage(WM_CANCELMODE); // close TrayMenu menu

			CMINVOKECOMMANDINFOEX info = { 0 };
			info.cbSize = sizeof(info);
			info.fMask = CMIC_MASK_UNICODE;
			info.hwnd = hwndParent;
			info.lpVerb = (LPCSTR)((INT_PTR)iCmd - 1);
			info.lpVerbW = (LPCWSTR)((INT_PTR)iCmd - 1);
			info.nShow = SW_SHOWNORMAL;
			iMenu->InvokeCommand((LPCMINVOKECOMMANDINFO)&info);
		}
	}
	DestroyMenu(hMenu);

	return true;
}

HICON CTrayMenuDlg::GetIconForItem(UINT itemID)
{
	switch (itemID)
	{
	case 0: return 0;

	// Kontextmenü-Icons
	case ID_OPEN_FOLDER: return m_hIconExplorer;
	case ID_INCLUDE_FOLDER: return m_hIconIncludeFolder;
	case ID_INC_DESKTOP: return m_hIconIncludeFolder;
	case ID_INC_STARTMENU: return m_hIconIncludeFolder;
	case ID_DEFINE_HOTKEY: return m_hIconHotkey;
	case ID_RELOAD: return m_hIconReload;
	case ID_SETTINGS: return m_hIconSettings;
	case ID_SETTINGS_ICON: return m_hIconSettingsIcon;
	case ID_SETTINGS_COLORS: return m_hIconColors;
	case ID_SETTINGS_COLORS_DEF: return m_hIconColorsDefault;
	case ID_SETTINGS_COLORS_LIG: return m_hIconColorsLight;
	case ID_SETTINGS_COLORS_DRK: return m_hIconColorsDark;
	case ID_SETTINGS_COLORS_OWN: return m_hIconColorsCustom;
	case ID_SETTINGS_COLORS_CUST: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_TXT: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_BGD: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_SEP: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_IOB: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_IIB: return m_hIconColorsCustomize;
	case ID_SETTINGS_COLORS_OWN_IBG: return m_hIconColorsCustomize;
	case ID_SETTINGS_LOCALIZE: return m_hIconSettingsLocalized;
	case ID_SETTINGS_FOLDF: return m_hIconSettingsFoldersFirst;
	case ID_SETTINGS_CLICK: return m_hIconSettingsClick;
	case ID_SETTINGS_LEFTMOST: return m_hIconLeftmost;
	case ID_EXPORT_SETTINGS: return m_hIconExportSettings;
	case ID_SELECT_FOLDER: return m_hIconSelectFolder;
	case ID_BROWSE_FOLDER: return m_hIconBrowseFolder;
	case ID_AUTOSTART: return m_hIconAutostart;
	case ID_CREATE_SHORTCUT: return m_hIconShortcut;
	case ID_ADD_FOLDER: return m_hIconAddFolder;
	case ID_ICON_BLUE: return m_hIconBlue;
	case ID_ICON_BLACK: return m_hIconBlack;
	case ID_ICON_GREEN: return m_hIconGreen;
	case ID_ICON_GREY: return m_hIconGrey;
	case ID_ICON_LIME: return m_hIconLime;
	case ID_ICON_MULTICOLOR: return m_hIconMixed;
	case ID_ICON_ORANGE: return m_hIconOrange;
	case ID_ICON_PINK: return m_hIconPink;
	case ID_ICON_RED: return m_hIconRed;
	case ID_ICON_VIOLET: return m_hIconViolet;
	case ID_ICON_WHITE: return m_hIconWhite;
	case ID_ICON_YELLOW: return m_hIconYellow;
	case ID_ABOUT: return m_hIconAbout;
	case ID_ABOUT_TITLE: return 0;
	case ID_ABOUT_VERSION: return 0;
	case ID_ABOUT_WEBSITE: return m_hIconWebsite;
	case ID_EXIT: return m_hIconExit;
	case ID_CLOSE: return m_hIconClose;
	}

	// Dynamisch erzeugte Items des Menüs "Select folder"
	if (itemID >= ID_SEL_FOLDER && itemID <= ID_SEL_FOLDER_MAX)
	{
		return m_hIconSelectFolder;
	}

	// Dynamisch erzeugte Items des Menüs "Select folder"
	if (itemID >= ID_DEL_FOLDER && itemID <= ID_DEL_FOLDER_MAX)
	{
		return m_hIconRemoveFolder;
	}

	// Dynamisch erzeugte Items des Menüs "Autostart"
	if (itemID >= ID_REM_FOLDER && itemID <= ID_REM_FOLDER_MAX)
	{
		return m_hIconRemoveFolder;
	}

	// Startmenü-Items
	if (itemID >= MENU_ID_START && itemID - MENU_ID_START < m_arrEntries.size())
	{
		CEntry* pEntry = m_arrEntries[itemID - MENU_ID_START];
		if (pEntry->hIcon == 0)
		{
			HICON hIconDefault = pEntry->eEntryType == EntryType::DIR ? m_hIconFolder : m_hIconFile;
			pEntry->hIcon = GetFileIcon(pEntry->strPath, hIconDefault);
		}
		return pEntry->hIcon;
	}

	return 0;
}

HRESULT CTrayMenuDlg::ResolveLnk(HWND hwnd, LPCSTR szLinkPath, LPWSTR szTargetPath, int iTargetPathSize)
{
	HRESULT hres;
	IShellLink* psl;
	WIN32_FIND_DATA wfd;

	WCHAR szGotPath[MAX_PATH];
	*szTargetPath = 0;

	// Get a pointer to the IShellLink interface. It is assumed that CoInitialize
	// has already been called. 
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		IPersistFile* ppf;

		// Get a pointer to the IPersistFile interface. 
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);

		if (SUCCEEDED(hres))
		{
			WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			MultiByteToWideChar(CP_ACP, 0, szLinkPath, -1, wsz, MAX_PATH);

			// Add code here to check return value from MultiByteWideChar 
			// for success.

			// Load the shortcut. 
			hres = ppf->Load(wsz, STGM_READ);

			if (SUCCEEDED(hres))
			{
				// Resolve the link. 
				hres = psl->Resolve(hwnd, SLR_NO_UI | SLR_NOUPDATE);

				if (SUCCEEDED(hres))
				{
					// Get the path to the link target. 
					ZeroMemory(&wfd, sizeof(WIN32_FIND_DATA));
					hres = psl->GetPath(szGotPath, MAX_PATH, &wfd, 0);
					if (SUCCEEDED(hres)) StringCbCopy(szTargetPath, iTargetPathSize, szGotPath);
				}
			}

			// Release the pointer to the IPersistFile interface. 
			ppf->Release();
		}

		// Release the pointer to the IShellLink interface. 
		psl->Release();
	}
	return hres;
}

HICON CTrayMenuDlg::GetFileIcon(CString strPath, HICON hIconDefault)
{
	HICON hIcon = hIconDefault;

/*
	// Zeichnet Link-Overlays über den Icons:
	SHFILEINFOW sfi = { 0 };
	if (SUCCEEDED(SHGetFileInfo(strPath, 0, &sfi, sizeof(sfi), SHGFI_ICON | SHGFI_SMALLICON)))
	{
		hIcon = sfi.hIcon;
	}
*/

	SHFILEINFO sfi = { 0 };
	HIMAGELIST list = (HIMAGELIST)SHGetFileInfo(strPath, 0, &sfi, sizeof(sfi), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
	if (list)
	{
		hIcon = ImageList_GetIcon(list, sfi.iIcon, ILD_NORMAL);
	}

	return hIcon;
}

CString CTrayMenuDlg::GetLocalizedName(CString strPath, CString strDefaultName)
{
	if (!m_bLocalizedNames)
		return strDefaultName;

	WCHAR szResModulePath[MAX_PATH];
	int iIDsRes;

	if (SUCCEEDED(SHGetLocalizedName(strPath, szResModulePath, MAX_PATH, &iIDsRes)))
	{
		CString strResModulePathExpanded = Expand(szResModulePath);
		if (!strResModulePathExpanded.IsEmpty())
		{
			HMODULE hMod = LoadLibraryEx(strResModulePathExpanded, NULL, LOAD_LIBRARY_AS_DATAFILE);
			WCHAR szLocalizedPath[MAX_PATH];
			if (hMod && LoadString(hMod, iIDsRes, szLocalizedPath, MAX_PATH))
			{
				strDefaultName = szLocalizedPath;
			}
		}
	}

	return strDefaultName;
}

CString CTrayMenuDlg::Expand(CString strString)
{
	WCHAR* pszExpanded = new WCHAR[MAX_PATH];
	DWORD dwSize = ExpandEnvironmentStrings(strString, pszExpanded, MAX_PATH);
	if (dwSize > 0)
	{
		if (dwSize > MAX_PATH)
		{
			delete[] pszExpanded;
			pszExpanded = new WCHAR[dwSize];
			dwSize = ExpandEnvironmentStrings(strString, pszExpanded, MAX_PATH);
		}
	}
	strString = pszExpanded;
	delete[] pszExpanded;
	return strString;
}

CString CTrayMenuDlg::GetKnownFolderName(int csidl)
{
	CString strName;
	PWSTR szName = NULL;
	PIDLIST_ABSOLUTE pidl;
	HRESULT hr = SHGetFolderLocation(0, csidl, 0, 0, &pidl);
	if (S_OK == hr)
	{
		IShellItem* pItem;
		hr = SHCreateItemFromIDList(pidl, IID_PPV_ARGS(&pItem));
		ILFree(pidl);
		if (S_OK == hr)
		{
			hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &szName);
			strName = szName;
			CoTaskMemFree(&szName);
			pItem->Release();
		}
	}
	return strName;
}

void CTrayMenuDlg::BrowseToWebsite()
{
	ShellExecute(m_hWnd, _T("open"), _T("https://www.stefanbion.de/traymenu/"), NULL, NULL, SW_SHOWNORMAL);
}

void CTrayMenuDlg::ExploreFolder(CString strPath)
{
	ShellExecute(m_hWnd, _T("explore"), strPath, NULL, NULL, SW_SHOWNORMAL);
}

void CTrayMenuDlg::OpenPath(CString strPath)
{
	ShellExecute(m_hWnd, _T("open"), strPath, NULL, NULL, SW_SHOWNORMAL);
}

BOOL CTrayMenuDlg::BrowseFolder()
{
	CFolderPickerDialog dlg(m_strFolder, OFN_FORCESHOWHIDDEN);
	if (dlg.DoModal() == IDOK)
	{
		CString strSelectedFolder = dlg.GetPathName();
		if (strSelectedFolder != m_strFolder)
		{
			m_strFolder = strSelectedFolder;
			return TRUE;
		}
	}
	return FALSE;
}

CString CTrayMenuDlg::FindNextProfile(DWORD& dwIndex)
{
	CRegKey keyProfiles;
	LRESULT res = keyProfiles.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Stefan Bion\\TrayMenu\\Folders"), KEY_READ);
	while (res == ERROR_SUCCESS)
	{
		CString strProfile;
		res = RegEnumKey(keyProfiles, dwIndex++, strProfile.GetBuffer(MAX_PATH), MAX_PATH);
		strProfile.ReleaseBuffer();
		strProfile.Replace(_T('/'), _T('\\'));
		return strProfile;
	}
	keyProfiles.Close();
	return _T("");
}

BOOL CTrayMenuDlg::RemoveProfile(CString strFolderName)
{
	strFolderName = strFolderName.Mid(7); // "Forget " am Anfang entfernen
	strFolderName.Replace(_T('\\'), _T('/'));

	CRegKey keyProfiles;
	LRESULT res = keyProfiles.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Stefan Bion\\TrayMenu\\Folders"), KEY_ALL_ACCESS);
	if (res == ERROR_SUCCESS)
	{
		res = keyProfiles.DeleteSubKey(strFolderName);
		keyProfiles.Close();
	}
	return res == ERROR_SUCCESS;
}

BOOL CTrayMenuDlg::FindInAutostart(CString strFolder, BOOL bDeleteValue)
{
	CRegKey keyAutostart;
	LRESULT res = keyAutostart.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 
		bDeleteValue ? KEY_ALL_ACCESS : KEY_READ);
	DWORD dwIndex = 0;
	while (res  == ERROR_SUCCESS)
	{
		CString strValueName;
		DWORD dwLengthValueName = MAX_PATH;
		res = RegEnumValue(keyAutostart, dwIndex++, strValueName.GetBuffer(MAX_PATH), &dwLengthValueName, NULL, NULL, NULL, NULL);
		strValueName.ReleaseBuffer();
		if (strValueName.Left(10) == _T("TrayMenu #"))
		{
			CString strValue;
			DWORD dwLengthValue = MAX_PATH;
			res = keyAutostart.QueryStringValue(strValueName, strValue.GetBuffer(MAX_PATH), &dwLengthValue);
			if (res == ERROR_SUCCESS)
			{
				if (strValue.Find(_T("\\TrayMenu.exe\" \"") + strFolder + _T("\"")) != -1)
				{
					if (bDeleteValue) keyAutostart.DeleteValue(strValueName);
					keyAutostart.Close();
					return TRUE;
				}
			}
		}
	}
	keyAutostart.Close();
	return FALSE;
}

CString CTrayMenuDlg::FindNextAutostartFolder(DWORD& dwIndex)
{
	CRegKey keyAutostart;
	LRESULT res = keyAutostart.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), KEY_READ);
	while (res == ERROR_SUCCESS)
	{
		CString strValueName;
		DWORD dwLengthValueName = MAX_PATH;
		res = RegEnumValue(keyAutostart, dwIndex++, strValueName.GetBuffer(MAX_PATH), &dwLengthValueName, NULL, NULL, NULL, NULL);
		strValueName.ReleaseBuffer();
		if (strValueName.Left(10) == _T("TrayMenu #"))
		{
			CString strValue;
			DWORD dwLengthValue = MAX_PATH;
			res = keyAutostart.QueryStringValue(strValueName, strValue.GetBuffer(MAX_PATH), &dwLengthValue);
			strValue.ReleaseBuffer();
			if (res == ERROR_SUCCESS)
			{
				CString strSearch = _T("\\TrayMenu.exe\" \"");
				int nPos = strValue.Find(strSearch);
				if (nPos != -1)
				{
					CString strFolder = strValue.Mid(nPos + strSearch.GetLength());
					strFolder = strFolder.Left(strFolder.GetLength() - 1); // remove trailing quotes
					keyAutostart.Close();
					return strFolder;
				}
			}
		}
	}
	keyAutostart.Close();
	return _T("");
}

BOOL CTrayMenuDlg::AddToAutostart(CString strFolder)
{
	CString strAppPath;
	GetModuleFileName(GetModuleHandle(NULL), strAppPath.GetBuffer(MAX_PATH), MAX_PATH);
	strAppPath.ReleaseBuffer();

	CString strAutostartCommand;
	strAutostartCommand.Format(_T("\"%s\" \"%s\""), (LPCTSTR)strAppPath, (LPCTSTR)strFolder);

	int mEntryID = 1;
	CRegKey keyAutostart;
	LRESULT res = keyAutostart.Open(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), KEY_ALL_ACCESS);
	while (res == ERROR_SUCCESS)
	{
		CString strValueName;
		strValueName.Format(_T("TrayMenu #%d"), mEntryID++);
		CString strValue;
		DWORD dwLengthValue = MAX_PATH;
		res = keyAutostart.QueryStringValue(strValueName, strValue.GetBuffer(MAX_PATH), &dwLengthValue);
		strValue.ReleaseBuffer();
		if (res != ERROR_SUCCESS)
			keyAutostart.SetStringValue(strValueName, strAutostartCommand);
	}
	keyAutostart.Close();
	return TRUE;
}

BOOL CTrayMenuDlg::CreateShortcut()
{
	// Pfad der .exe-Datei ermitteln
	CString strAppPath;
	GetModuleFileName(GetModuleHandle(NULL), strAppPath.GetBuffer(MAX_PATH), MAX_PATH);
	strAppPath.ReleaseBuffer();

	// Desktop-Verzeichnis ermitteln
	PWSTR szDesktopPath = NULL;
	if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Desktop, 0, NULL, &szDesktopPath)))
		return FALSE;
	CString strDesktopPath(szDesktopPath);
	CoTaskMemFree(szDesktopPath);

	CString strLinkPath = strDesktopPath + _T("\\") + m_entryRoot.strDisplayName + _T(".lnk");
	CFileDialog dlg(FALSE, _T(".lnk"), strLinkPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Shortcut (*.lnk)|*.lnk||"));
	if (dlg.DoModal() != IDOK)
		return FALSE;

	strLinkPath = dlg.GetPathName();

	// Get a pointer to the IShellLink interface.
	IShellLink* psl;
	HRESULT hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&psl);
	if (SUCCEEDED(hres))
	{
		psl->SetPath(_T("\"") + strAppPath + _T("\""));
		psl->SetArguments(_T("\"") + m_strFolder + _T("\""));
		psl->SetDescription(m_strFolder);
		psl->SetIconLocation(strAppPath, m_nIconIndex);

		// Query IShellLink for the IPersistFile interface, used for saving the 
		// shortcut in persistent storage.
		IPersistFile* ppf;
		hres = psl->QueryInterface(IID_IPersistFile, (LPVOID*)&ppf);
		if (SUCCEEDED(hres))
		{
			//WCHAR wsz[MAX_PATH];

			// Ensure that the string is Unicode. 
			//MultiByteToWideChar(CP_ACP, 0, strLinkPath, -1, wsz, MAX_PATH);

			// Save the link by calling IPersistFile::Save. 
			hres = ppf->Save(strLinkPath, TRUE);
			ppf->Release();
		}
		psl->Release();
	}
	return SUCCEEDED(hres);
}

BOOL CTrayMenuDlg::RegisterHotkey()
{
	UnregisterHotKey(m_hWnd, 1);
	return RegisterHotKey(m_hWnd, 1, m_uHotkeyModifiers | MOD_NOREPEAT, m_uHotkeyKeyCode);
}

void CTrayMenuDlg::OnHotKey(UINT id, UINT fsModifiers, UINT vk)
{
	switch (id)
	{
	case 1:
		if (m_bIsMenuOpened)
			SendMessage(WM_CANCELMODE);
		else
			OpenMenu(m_bOpenLeftmost);
		break;
	}
}

CString CTrayMenuDlg::GetHotkeyName()
{
	m_hotKeyCtrl.SetHotKey((WORD&)m_uHotkeyKeyCode, (WORD&)m_uHotkeyModifiers);
	return m_hotKeyCtrl.GetHotKeyName();
}

void CTrayMenuDlg::ShowDialogHotkey()
{
	m_DlgMode = DlgMode::HOTKEY;
	m_bIsVisible = TRUE;
	SetWindowText(_T("Define hotkey"));
	m_stcDlgMessage.SetWindowText(_T("Please press any combination of keys:"));
	m_hotKeyCtrl.SetHotKey((WORD&)m_uHotkeyKeyCode, (WORD&)m_uHotkeyModifiers);
	m_hotKeyCtrl.SetFocus();
	CenterWindow();
	ShowWindow(SW_RESTORE);
}

void CTrayMenuDlg::DefineHotkey()
{
	UINT uHotkeyModifiersOld = m_uHotkeyModifiers;
	UINT uHotkeyKeyCodeOld = m_uHotkeyKeyCode;

	m_hotKeyCtrl.GetHotKey((WORD&)m_uHotkeyKeyCode, (WORD&)m_uHotkeyModifiers);
	if (RegisterHotkey())
	{
		SaveSettings();
	}
	else
	{
		m_uHotkeyModifiers = uHotkeyModifiersOld;
		m_uHotkeyKeyCode = uHotkeyKeyCodeOld;
		RegisterHotkey();
	}
}

BOOL CTrayMenuDlg::ShellExecuteWait(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd, DWORD dwMilliseconds)
{
	BOOL bSuccess = TRUE;
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = hwnd;
	ShExecInfo.lpVerb = lpOperation;
	ShExecInfo.lpFile = lpFile;
	ShExecInfo.lpParameters = lpParameters;
	ShExecInfo.lpDirectory = lpDirectory;
	ShExecInfo.nShow = nShowCmd;
	ShExecInfo.hInstApp = NULL;
	bSuccess = bSuccess && ShellExecuteEx(&ShExecInfo);
	bSuccess = bSuccess && (ShExecInfo.hProcess && WAIT_OBJECT_0 == WaitForSingleObject(ShExecInfo.hProcess, dwMilliseconds));
	if (ShExecInfo.hProcess) CloseHandle(ShExecInfo.hProcess);
	return bSuccess;
}

BOOL CTrayMenuDlg::ExportSettings()
{
	CFileDialog dlg(FALSE, _T(".reg"), _T("TrayMenuSettings.reg"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Registration files (*.reg)|*.reg||"));
	if (dlg.DoModal() != IDOK)
		return FALSE;

	BOOL bResult = FALSE;

	CString strRegExePath = Expand(_T("%windir%\\system32\\reg.exe"));

	FILE* pRegFile = NULL;
	FILE* pTmpFile = NULL;

	CString strRegFile = dlg.GetPathName();
	CString strTmpFile = dlg.GetPathName() + _T(".tmp");

	do
	{
		if (PathFileExists(strRegFile)) DeleteFile(strRegFile);
		if (PathFileExists(strTmpFile)) DeleteFile(strTmpFile);

		if (PathFileExists(strRegFile)) break;
		if (PathFileExists(strTmpFile)) break;

		CString strArgsProfile = _T(" export \"HKCU\\SOFTWARE\\Stefan Bion\\TrayMenu\" \"") + strRegFile + _T("\" /y");
		CString strArgsAutorun = _T(" export \"HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run\" \"") + strTmpFile + _T("\" /y");

		if (!ShellExecuteWait(m_hWnd, _T("open"), strRegExePath, strArgsProfile, NULL, SW_HIDE)) break;
		if (!ShellExecuteWait(m_hWnd, _T("open"), strRegExePath, strArgsAutorun, NULL, SW_HIDE)) break;

		if (!PathFileExists(strRegFile)) break;
		if (!PathFileExists(strTmpFile)) break;

		if (_wfopen_s(&pRegFile, strRegFile, _T("at, ccs=UNICODE"))) break;
		if (_wfopen_s(&pTmpFile, strTmpFile, _T("rt, ccs=UNICODE"))) break;

		CString strLine;
		try
		{
			CStdioFile fileReg(pRegFile);
			CStdioFile fileTmp(pTmpFile);

			while (fileTmp.ReadString(strLine))
			{
				if (strLine.Left(1) == _T('[') || strLine.Left(11) == _T("\"TrayMenu #"))
					fileReg.WriteString(strLine + _T("\n"));
			}
			bResult = TRUE;
		}
		catch (CException* e)
		{
			e->ReportError();
			break;
		}

	}
	while (FALSE);

	if (pRegFile) fclose(pRegFile);
	if (pTmpFile) fclose(pTmpFile);
	DeleteFile(strTmpFile);

	if (!bResult) AfxMessageBox(_T("Export failed."));

	return bResult;
}