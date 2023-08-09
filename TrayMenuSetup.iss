; Script for Inno Setup Compiler 6.1.2

#define MyAppName "TrayMenu"
#define MyAppPublisher "Stefan Bion"
#define MyAppURL "https://www.stefanbion.de/traymenu/"
#define MyAppExeName "TrayMenu.exe"
#define MyAppExePath "x64\Release\TrayMenu.exe"
#define MyFileVersion GetVersionNumbersString(MyAppExePath)
#define MyProductVersion RemoveFileExt(MyFileVersion)
#define MyAppCopyright "An alternative Start menu for Windows"

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{982A9090-6CCA-415C-B49C-7BD1EDD1BDE8}
AppName={#MyAppName}
AppVersion={#MyFileVersion}
AppVerName={#MyAppName} {#MyProductVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
AppComments={#MyAppCopyright}
AppCopyright={#MyAppPublisher}
VersionInfoVersion={#MyFileVersion}
VersionInfoDescription={#MyAppCopyright}

DefaultDirName={autopf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=.
OutputBaseFilename=TrayMenuSetup
SetupIconFile=res\TrayMenuMixed.ico
Compression=lzma
SolidCompression=yes
UninstallDisplayIcon={app}\{#MyAppExeName}
ArchitecturesInstallIn64BitMode=x64
;WizardStyle=modern
;DisableWelcomePage=no

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppExePath}"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Languages: en; Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "Run {#MyAppName}"; Tasks: desktopicon
Languages: de; Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "{#MyAppName} starten"; Tasks: desktopicon
Languages: en; Name: "{autoprograms}\{#MyAppName}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "Run {#MyAppName}"
Languages: de; Name: "{autoprograms}\{#MyAppName}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Comment: "{#MyAppName} starten"
Languages: en; Name: "{autoprograms}\{#MyAppName}\Uninstall {#MyAppName}"; Filename: "{uninstallexe}"; Comment: "Uninstall {#MyAppName}"
Languages: de; Name: "{autoprograms}\{#MyAppName}\{#MyAppName} deinstallieren"; Filename: "{uninstallexe}"; Comment: "{#MyAppName} deinstallieren"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

