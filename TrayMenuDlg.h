#pragma once

#include <string>
#include <vector>
#include <map>
#include <locale>
#include <algorithm> // std::sort
#include <iostream>

using namespace std;

// Wenn viele Menü-Icons gleichzeitig geladen werden, werden auch ziemlich viele GDI-Handles
// verbraten, von denen pro Prozess standardmäßig allerdings nur ca. 10000 verfügbar sind.
// Dieser Wert kann durch Anpassen des folgenden Registry-Wertes auf max. 65536 gesetzt werden:
// HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows\GDIProcessHandleQuota 

class CItemIdListReleaser
{
public:
	explicit CItemIdListReleaser(ITEMIDLIST* idList) : _idList(idList) {}
	~CItemIdListReleaser() { if (_idList) CoTaskMemFree(_idList); }
private:
	ITEMIDLIST* _idList;
};

class CComInterfaceReleaser
{
public:
	explicit CComInterfaceReleaser(IUnknown* i) : _i(i) {}
	~CComInterfaceReleaser() { if (_i) _i->Release(); }
private:
	IUnknown* _i;
};

enum class EntryType { INIT, DIR, FILE };

class CEntry
{
public:
	CEntry()
	{
		iGroup = 0;
		iSort = 0;
		eEntryType = EntryType::INIT;
		uMenuID = 0;
		hIcon = 0;
		iLinkIconIndex = -1;
		bIsRootElement = FALSE;
	}

	~CEntry()
	{
		Delete();
	}

	void Delete()
	{
		if (hIcon != 0)
			DestroyIcon(hIcon);

		for (vector<CEntry*>::iterator it = children.begin(); it != children.end(); it++)
			delete* it;

		children.clear();

		if (bIsRootElement && menu.m_hMenu)
			menu.DestroyMenu();
	}

	CEntry* FindChild(const CString& strDisplayName, EntryType eEntryType)
	{
		for (vector<CEntry*>::iterator it = children.begin(); it != children.end(); it++)
		{
			if ((*it)->strDisplayName.CompareNoCase(strDisplayName) == 0 && (*it)->eEntryType == eEntryType)
				return *it;
		}
		return NULL;
	}

	CString GetMenuString()
	{
		return strDisplayName;
	}

	void SortChildren()
	{
		// Sortierstring setzen für Gruppierung entsprechend der Angaben in traymenu.ini
		map<CString, int> mapGroups;
		ReadFolderInfo(mapGroups);

		for (vector<CEntry*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->SetSortName(mapGroups);
		}

		// Zuerst die eigenen Kinder sortieren
		if (children.size() > 1) // Sortieren macht natürlich erst ab 2 Kind-Elementen Sinn
		{
			sort(children.begin(), children.end(), CompareChildren); // gilt für vector
		}

		// Dann die Enkel, Urenkel usw. sortieren (rekursiver Aufruf)
		for (vector<CEntry*>::iterator it = children.begin(); it != children.end(); it++)
		{
			(*it)->SortChildren();
		}
	}

	BOOL ReadFolderInfo(map<CString, int>& mapGroups)
	{
		FILE* pFile = NULL;
		if (_wfopen_s(&pFile, strPath + L"\\traymenu.ini", L"rt, ccs=UTF-8")) return FALSE;
		CStdioFile file(pFile);
		CString strLine;
		int iGroup = 0, iSort = 0;
		enum class Section { NONE, GROUP };
		Section section = Section::NONE;
		while (file.ReadString(strLine))
		{
			if (strLine.IsEmpty() || strLine.Left(1) == L'#') continue;
			if (strLine.CompareNoCase(L"[GROUP]") == 0)
			{
				section = Section::GROUP;
				iGroup++; iSort = 0;
			}
			else if (section == Section::GROUP)
			{
				mapGroups[strLine] = (iGroup << 16) | iSort;
				iSort++;
			}
		}
		fclose(pFile);
		return TRUE;
	}

	void SetSortName(map<CString, int>& mapGroups)
	{
		iGroup = 0;
		iSort = 0;

		if (mapGroups.find(strDisplayName) != mapGroups.end())
		{
			int iGroupSort = mapGroups[strDisplayName];
			iGroup = iGroupSort >> 16;
			iSort = iGroupSort & 0xFFFF;
		}

		strSortName.Empty();
		strSortName.Format(L"%03d%03d", iGroup, iSort);
		strSortName += strDisplayName;
	}

	static bool CompareChildren(CEntry* a, CEntry* b)
	{
		return *a < *b;
	}

	bool operator < (const CEntry& a) const
	{
		return CSTR_LESS_THAN == CompareString(LOCALE_USER_DEFAULT, 0, strSortName, -1, a.strSortName, -1);
	}

	vector<CEntry*> children;
	EntryType eEntryType;
	CString strPath;
	vector<CString> strMergedPaths;
	CString strName;
	CString strDisplayName;
	CString strSortName;
	int iGroup;
	int iSort;

	// Infos aus der .lnk-Datei:
	CString strLinkPath;
	CString strLinkArgs;
	CString strLinkDesc;
	CString strLinkDir;
	CString strLinkIconPath;
	int iLinkIconIndex;

	CMenu menu;		// Das zu diesem Element gehörige Submenü (Popup)
	UINT uMenuID;	// Fortlaufende Nummer als Menu-ID (ab MENU_ID_START).
	HICON hIcon;	// Handle des Icons, das im Menü angezeigt wird.
	BOOL bIsRootElement;
};

enum class ItemType { INIT, SEPARATOR, STRING, POPUP };

class CItemData
{
public:
	CItemData(CMenu* a_pMenu, UINT a_uMenuID, LPCWSTR a_szCaption, ItemType a_eItemType)
	{
		eItemType = a_eItemType;
		pMenu = a_pMenu;
		uMenuID = a_uMenuID;
		hIcon = 0;
		strCaption = a_szCaption;
	}

	static CItemData* Find(vector<CItemData*>& arrItemData, UINT a_uMenuID)
	{
		for (vector<CItemData*>::iterator it = arrItemData.begin(); it != arrItemData.end(); it++)
		{
			if ((*it)->uMenuID == a_uMenuID)
				return *it;
		}
		return NULL;
	}

	static void Delete(vector<CItemData*>& arrItemData, BOOL bDestroyIcon = FALSE, HICON hExcept = 0)
	{
		while (arrItemData.size() > 0)
		{
			CItemData* pItemData = arrItemData.back();
			if (bDestroyIcon && pItemData->hIcon != hExcept)
				DestroyIcon(pItemData->hIcon);
			delete pItemData;
			arrItemData.pop_back();
		}

		arrItemData.shrink_to_fit();
	}

	ItemType eItemType;
	CMenu* pMenu;
	UINT uMenuID;
	HICON hIcon;
	CString strCaption;
};

class CMenuStyle
{
public:
	CMenuStyle()
	{
		colMenuText = 0;
		colMenuBackground = 0;
		colMenuSeparator = 0;
		colMenuSelectionText = 0;
		colMenuSelectionOuter = 0;
		colMenuSelectionInner = 0;
		colMenuSelectionBackg = 0;
	}

	void SetMenuStyle
	(
		COLORREF a_colMenuText,
		COLORREF a_colMenuBackground,
		COLORREF a_colMenuSeparator,
		COLORREF a_colMenuSelectionText,
		COLORREF a_colMenuSelectionOuter,
		COLORREF a_colMenuSelectionInner,
		COLORREF a_colMenuSelectionBackg
	)
	{
		colMenuText = a_colMenuText;
		colMenuBackground = a_colMenuBackground;
		colMenuSeparator = a_colMenuSeparator;
		colMenuSelectionText = a_colMenuSelectionText;
		colMenuSelectionOuter = a_colMenuSelectionOuter;
		colMenuSelectionInner = a_colMenuSelectionInner;
		colMenuSelectionBackg = a_colMenuSelectionBackg;
	};

	void SetMenuStyle(const CMenuStyle& a_menuStyle)
	{
		colMenuText = a_menuStyle.colMenuText;
		colMenuBackground = a_menuStyle.colMenuBackground;
		colMenuSeparator = a_menuStyle.colMenuSeparator;
		colMenuSelectionText = a_menuStyle.colMenuSelectionText;
		colMenuSelectionOuter = a_menuStyle.colMenuSelectionOuter;
		colMenuSelectionInner = a_menuStyle.colMenuSelectionInner;
		colMenuSelectionBackg = a_menuStyle.colMenuSelectionBackg;
	}

	void CreateDefaultMenuStyles(BOOL bForce, BOOL bSetDefault)
	{
		BOOL bInitialized = AfxGetApp()->GetProfileInt(L"MenuStyles", L"Initialized", FALSE);
		BOOL bSave = !bInitialized || bForce;

		CMenuStyle menuStyle;

		menuStyle.colMenuText = RGB(240, 240, 240);
		menuStyle.colMenuBackground = RGB(32, 32, 32);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(255, 215, 0);
		menuStyle.colMenuSelectionInner = RGB(190, 156, 0);
		menuStyle.colMenuSelectionBackg = RGB(190, 156, 0);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Black/Gold");

		menuStyle.colMenuText = RGB(240, 240, 240);
		menuStyle.colMenuBackground = RGB(32, 32, 32);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(128, 255, 0);
		menuStyle.colMenuSelectionInner = RGB(77, 154, 0);
		menuStyle.colMenuSelectionBackg = RGB(77, 154, 0);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Black/Lime");

		menuStyle.colMenuText = RGB(240, 240, 240);
		menuStyle.colMenuBackground = RGB(57, 54, 54);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(238, 140, 0);
		menuStyle.colMenuSelectionInner = RGB(190, 112, 0);
		menuStyle.colMenuSelectionBackg = RGB(190, 112, 0);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Brown/Orange");

		menuStyle.colMenuText = RGB(240, 240, 240);
		menuStyle.colMenuBackground = RGB(64, 64, 64);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(92, 92, 92);
		menuStyle.colMenuSelectionInner = RGB(92, 92, 92);
		menuStyle.colMenuSelectionBackg = RGB(92, 92, 92);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Dark Grey");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(207, 207, 207);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(192, 0, 0);
		menuStyle.colMenuSelectionInner = RGB(192, 0, 0);
		menuStyle.colMenuSelectionBackg = RGB(192, 0, 0);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Grey/Red");

		menuStyle.colMenuText = RGB(238, 239, 241);
		menuStyle.colMenuBackground = RGB(64, 77, 110);
		menuStyle.colMenuSeparator = RGB(132, 137, 146);
		menuStyle.colMenuSelectionText = RGB(255, 255, 255);
		menuStyle.colMenuSelectionOuter = RGB(201, 130, 77);
		menuStyle.colMenuSelectionInner = RGB(162, 92, 44);
		menuStyle.colMenuSelectionBackg = RGB(162, 92, 44);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Jeans");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(236, 241, 250);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(0, 0, 0);
		menuStyle.colMenuSelectionOuter = RGB(106, 161, 222);
		menuStyle.colMenuSelectionInner = RGB(189, 216, 248);
		menuStyle.colMenuSelectionBackg = RGB(189, 216, 248);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Light Blue");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(243, 245, 243);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(0, 0, 0);
		menuStyle.colMenuSelectionOuter = RGB(108, 226, 108);
		menuStyle.colMenuSelectionInner = RGB(233, 255, 233);
		menuStyle.colMenuSelectionBackg = RGB(191, 229, 191);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Light Green");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(247, 241, 241);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(0, 0, 0);
		menuStyle.colMenuSelectionOuter = RGB(255, 161, 161);
		menuStyle.colMenuSelectionInner = RGB(255, 183, 191);
		menuStyle.colMenuSelectionBackg = RGB(255, 183, 191);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Pink");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(255, 255, 255);
		menuStyle.colMenuSeparator = RGB(128, 128, 128);
		menuStyle.colMenuSelectionText = RGB(0, 0, 0);
		menuStyle.colMenuSelectionOuter = RGB(123, 160, 204);
		menuStyle.colMenuSelectionInner = RGB(232, 241, 250);
		menuStyle.colMenuSelectionBackg = RGB(189, 216, 248);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\White/Blue");

		if (bSetDefault)
			(*this) = menuStyle; // set above menuStyle as the default style

		menuStyle.colMenuText = RGB(99, 75, 75);
		menuStyle.colMenuBackground = RGB(251, 243, 225);
		menuStyle.colMenuSeparator = RGB(138, 118, 74);
		menuStyle.colMenuSelectionText = RGB(40, 31, 27);
		menuStyle.colMenuSelectionOuter = RGB(204, 146, 8);
		menuStyle.colMenuSelectionInner = RGB(255, 240, 181);
		menuStyle.colMenuSelectionBackg = RGB(252, 230, 163);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Wood");

		menuStyle.colMenuText = RGB(0, 0, 0);
		menuStyle.colMenuBackground = RGB(254, 254, 228);
		menuStyle.colMenuSeparator = RGB(0, 0, 0);
		menuStyle.colMenuSelectionText = RGB(18, 55, 0);
		menuStyle.colMenuSelectionOuter = RGB(57, 174, 0);
		menuStyle.colMenuSelectionInner = RGB(57, 174, 0);
		menuStyle.colMenuSelectionBackg = RGB(192, 255, 160);
		if (bSave) menuStyle.SaveMenuStyle(L"MenuStyles\\Yellow/Green");

		AfxGetApp()->WriteProfileInt(L"MenuStyles", L"Initialized", TRUE);
	}

	void LoadMenuStyle(CString strProfile)
	{
		colMenuText = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"TextColor", (int)colMenuText);
		colMenuBackground = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"BackgroundColor", (int)colMenuBackground);
		colMenuSeparator = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"SeparatorColor", (int)colMenuSeparator);
		colMenuSelectionText = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"SelectionText", (int)colMenuSelectionText);
		colMenuSelectionOuter = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"SelectionOuterColor", (int)colMenuSelectionOuter);
		colMenuSelectionInner = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"SelectionInnerColor", (int)colMenuSelectionInner);
		colMenuSelectionBackg = (COLORREF)AfxGetApp()->GetProfileInt(strProfile, L"SelectionBackgColor", (int)colMenuSelectionBackg);
	}

	void SaveMenuStyle(CString strProfile)
	{
		AfxGetApp()->WriteProfileInt(strProfile, L"TextColor", (int)colMenuText);
		AfxGetApp()->WriteProfileInt(strProfile, L"BackgroundColor", (int)colMenuBackground);
		AfxGetApp()->WriteProfileInt(strProfile, L"SeparatorColor", (int)colMenuSeparator);
		AfxGetApp()->WriteProfileInt(strProfile, L"SelectionText", (int)colMenuSelectionText);
		AfxGetApp()->WriteProfileInt(strProfile, L"SelectionOuterColor", (int)colMenuSelectionOuter);
		AfxGetApp()->WriteProfileInt(strProfile, L"SelectionInnerColor", (int)colMenuSelectionInner);
		AfxGetApp()->WriteProfileInt(strProfile, L"SelectionBackgColor", (int)colMenuSelectionBackg);
	}

	bool operator == (const CMenuStyle& a) const
	{
		// Nur die Farben vergleichen, nicht den Namen:
		return 
			colMenuText == a.colMenuText
		&&	colMenuBackground == a.colMenuBackground
		&&	colMenuSeparator == a.colMenuSeparator
		&&	colMenuSelectionText == a.colMenuSelectionText
		&&	colMenuSelectionOuter == a.colMenuSelectionOuter
		&&	colMenuSelectionInner == a.colMenuSelectionInner
		&&	colMenuSelectionBackg == a.colMenuSelectionBackg;
	}

	CMenuStyle& operator = (const CMenuStyle& a)
	{
		// Nur die Farben setzen, nicht den Namen:
		colMenuText = a.colMenuText;
		colMenuBackground = a.colMenuBackground;
		colMenuSeparator = a.colMenuSeparator;
		colMenuSelectionText = a.colMenuSelectionText;
		colMenuSelectionOuter = a.colMenuSelectionOuter;
		colMenuSelectionInner = a.colMenuSelectionInner;
		colMenuSelectionBackg = a.colMenuSelectionBackg;
		return (*this);
	}

	COLORREF colMenuText;
	COLORREF colMenuBackground;
	COLORREF colMenuSeparator;
	COLORREF colMenuSelectionText;
	COLORREF colMenuSelectionOuter;
	COLORREF colMenuSelectionInner;
	COLORREF colMenuSelectionBackg;
};

enum class DlgMode { INIT, HOTKEY, COLORNAME };

// CTrayMenuDlg-Dialogfeld
class CTrayMenuDlg : public CDialogEx
{
	// Konstruktion
public:
	CTrayMenuDlg(CString strFolder, CWnd* pParent = nullptr);	// Standardkonstruktor
	~CTrayMenuDlg();

	void DeleteData();

// Dialogfelddaten
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRAYMENU_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung

// Implementierung
protected:
	HICON m_hIcon;
	HICON m_hIconFolderSelected; // Icon des ausgewählten Folders

	HICON m_hIconBlue;
	HICON m_hIconBlack;
	HICON m_hIconGreen;
	HICON m_hIconGrey;
	HICON m_hIconLime;
	HICON m_hIconMixed;
	HICON m_hIconOrange;
	HICON m_hIconPink;
	HICON m_hIconRed;
	HICON m_hIconViolet;
	HICON m_hIconWhite;
	HICON m_hIconYellow;

	HICON m_hIconFolder;
	HICON m_hIconFile;
	HICON m_hIconAbout;
	HICON m_hIconClose;
	HICON m_hIconExit;
	HICON m_hIconExplorer;
	HICON m_hIconReload;
	HICON m_hIconSettings;
	HICON m_hIconWebsite;
	HICON m_hIconSettingsIcon;
	HICON m_hIconSettingsLocalized;
	HICON m_hIconSettingsFoldersFirst;
	HICON m_hIconSettingsClick;
	HICON m_hIconSelectFolder;
	HICON m_hIconAutostart;
	HICON m_hIconAddFolder;
	HICON m_hIconRemoveFolder;
	HICON m_hIconBrowseFolder;
	HICON m_hIconShortcut;
	HICON m_hIconColors;
	HICON m_hIconColorsDefault;
	HICON m_hIconColorsLight;
	HICON m_hIconColorsDark;
	HICON m_hIconColorsCustom;
	HICON m_hIconColorsCustomize;
	HICON m_hIconIncludeFolder;
	HICON m_hIconHotkey;
	HICON m_hIconLeftmost;
	HICON m_hIconExportSettings;
	HICON m_hIconSave;
	HICON m_hIconDelete;
	HICON m_hIconRestore;

	// Generierte Funktionen für die Meldungstabellen
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	void ResetDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CString GetFolderName();
	void SetFolder(CString strFolder);
	CString GetProfileName();
	void LoadSettings();
	void SaveSettings();
	void SetMenuStyle(UINT uMenuID = 0, BOOL bReloadAndSave = FALSE);
	BOOL SelectColor(COLORREF& color);
	COLORREF GetMenuColor(UINT uMenuID);
	void SelectIcon(DWORD dwSelection);
	void CreateTrayIcon();
	void UpdateTrayIcon(HICON hIcon = (HICON)0, LPCWSTR szTooltip = NULL);
	void DeleteTrayIcon();
	LRESULT OnClickedTrayIcon(WPARAM wParam, LPARAM lParam);
	void OpenMenu(BOOL bOpenLeftmost = FALSE);
	void OpenContextMenu();
	BOOL ReloadMenu();
	BOOL ReadFolder();
	BOOL AddPath(CEntry *pEntry, CString strFolder, CString strPattern = L"*", CString strMergeFolder = L"");
	BOOL IsReadable(CString strPath);
	BOOL EnabledSystemFolder(int csidl);
	BOOL AddSystemFolder(int csidl, BOOL bAddFolder = FALSE, int csidlMerge = -1);
	BOOL CreateMenu();
	BOOL AddItem(CEntry* pEntry, CMenu* pmnuParent);
	void AppendMenuItem(CMenu* pMenu, UINT uMenuID = 0, LPCWSTR szCaption = NULL, CMenu* pSubMenu = NULL, BOOL bAddToItemDataMainMenu = FALSE);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpdis);
	afx_msg void OnInitMenuPopup(CMenu* pMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpmis);
	afx_msg void OnMenuRButtonUp(UINT uMenuPos, CMenu* pMenu);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSubMenu);
	bool OpenShellContextMenu(CEntry* pEntry, CPoint point);
	bool OpenShellContextMenu(const CString& strPath, CPoint point);
	afx_msg void OnHotKey(UINT id, UINT fsModifiers, UINT vk);
	CString GetHotkeyName();
	void ShowDialogHotkey();
	void DefineHotkey();
	void ShowDialogColorName();
	void SaveMenuStyle();
	BOOL ShellExecuteWait(HWND hwnd, LPCWSTR lpOperation, LPCWSTR lpFile, LPCWSTR lpParameters, LPCWSTR lpDirectory, INT nShowCmd, DWORD dwMilliseconds = 5000);
	BOOL ExportSettings();
	BOOL GetProductAndVersion(CString& strProductName, CString& strProductVersion);
	HICON GetIconForItem(UINT itemID);
	HRESULT ResolveLnk(HWND hwnd, LPCSTR szLinkPath, LPWSTR szTargetPath, int iTargetPathSize);
	HICON GetFileIcon(CString strPath, HICON hIconDefault);
	CString GetLocalizedName(CString strPath, CString strDefaultName);
	CString Expand(CString strString);
	CString GetKnownFolderName(int csidl);
	void BrowseToWebsite();
	void ExploreFolder(CString strPath);
	void OpenPath(CString strPath);
	BOOL BrowseFolder();
	CString FindNextFolder(DWORD& dwIndex);
	BOOL RemoveFolder(CString strFolder);
	BOOL FindInAutostart(CString strFolder, BOOL bDeleteValue);
	CString FindNextAutostartFolder(DWORD& dwIndex);
	BOOL AddToAutostart(CString strFolder);
	CString FindNextMenuStyle(DWORD& dwIndex);
	BOOL RemoveMenuStyle(UINT uMenuID);
	BOOL CreateShortcut();
	BOOL RegisterHotkey();
	DECLARE_MESSAGE_MAP()

private:
	HRESULT m_hrCoInitialize;
	DlgMode m_DlgMode;
	BOOL m_bIsVisible;
	BOOL m_bIsReady;
	BOOL m_bIsMenuOpened;
	CString m_strFolder;
	CEntry m_entryRoot;
	vector<CEntry*> m_arrEntries; // enthält die Daten für alle Einträge des Hauptmenüs in hierarchischer Form
	vector<CItemData*> m_arrItemDataMainMenu; // enthält Item-Daten zum Zeichnen des Hauptmenüs
	vector<CItemData*> m_arrItemDataContextMenu; // enthält Item-Daten zum Zeichnen des Kontextmenüs
	vector<CItemData*> m_arrItemDataShellMenu; // enthält Item-Daten zum Zeichnen des Item-Shell-Kontextmenüs
	vector<CString> m_arrAutostartFolders;
	UINT m_nCurrentMenuID;
	HICON m_hIconSelected;
	HICON m_hIconBusy;
	DWORD m_dwMenuIdIconSelected;
	int m_nIconIndex;
	BOOL m_bIncludeDesktop;
	BOOL m_bIncludeStartmenu;
	BOOL m_bLocalizedNames;
	BOOL m_bFoldersFirst;
	BOOL m_bHoverToOpen;
	BOOL m_bOpenLeftmost;
	CMenuStyle m_menuStyle;
	HBRUSH m_hbrMenuBackground;
	UINT m_uHotkeyModifiers;
	UINT m_uHotkeyKeyCode;
	CHotKeyCtrl m_hotKeyCtrl;
	CEdit m_edtInput;
	CStatic m_stcDlgMessage;
	CRect m_rcItemMenuSelect;
	UINT m_uItemIDMenuSelect;
};
