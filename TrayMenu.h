
// TrayMenu.h: Hauptheaderdatei für die PROJECT_NAME-Anwendung
//

#pragma once

#ifndef __AFXWIN_H__
	#error "'pch.h' vor dieser Datei für PCH einschließen"
#endif

#include "resource.h"		// Hauptsymbole


// CTrayMenuApp:
// Siehe TrayMenu.cpp für die Implementierung dieser Klasse
//

class CTrayMenuApp : public CWinApp
{
public:
	CTrayMenuApp();

// Überschreibungen
public:
	virtual BOOL InitInstance();

// Implementierung

	DECLARE_MESSAGE_MAP()
};

extern CTrayMenuApp theApp;
