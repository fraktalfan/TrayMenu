# TrayMenu
An alternative, freely configurable Start menu for Windows
## Short Description
TrayMenu creates a hierarchical menu from the contents of any folder for quick access to applications, files, scripts, etc. The menu, which is based on the classic folder-based Start menu of earlier versions of Windows, can be accessed via an icon in the taskbar. No administrator rights are required for installation, which means that it can be used on computers with limited user rights.

![01-mainmenu](https://github.com/fraktalfan/TrayMenu/assets/93649344/642bdbd0-56f4-4a9d-8458-1ec7abd5548c)

## Background
In my opinion, the folder-based Start menu introduced with Windows 95 was ideal for clearly categorizing all installed applications and frequently used files in one central location and for quick access to them. After Microsoft unfortunately abolished this extremely practical form of the Start menu with Windows 8 and replaced it with a confusing "Start page", thank God there were tools such as "Open-Shell" that brought the classic Start menu back again. Unfortunately, its installation requires administrator rights, so it cannot be used on company computers with limited user rights, for example. There you could at least help yourself with a self-created toolbar, which behaved similarly to a Start menu, except that it could not be opened via the keyboard (Windows key or Ctrl+Esc, but only by clicking in the taskbar. Unfortunately, with the introduction of Windows 11, Microsoft also abolished this last option of creating your own selection menus. With TrayMenu, an alternative is available that doesn't quite come close to the old Start menu, but still fulfills two important criteria: 1.) Any folder contents can be displayed in the form of a hierarchically structured menu, and 2.) it does not require admin rights on the computer.

## Build / Download
The source code can be compiled with Visual Studio 2019-2022 (with MSVC and MFC packages). A fully compiled, executable file can be downloaded from the developer's homepage at https://www.stefanbion.de/traymenu/.

## Installation
An installation in the usual sense is not necessary; the "TrayMenu.exe" program file can simply be copied to any folder and started from there. A blue 3×3 square icon will appear in the notification area of the task list at the bottom right near the clock:

![00d-taskbar-overflow](https://github.com/fraktalfan/TrayMenu/assets/93649344/bb32dfb8-22b8-4d89-8245-43082e2d02a7)

Unfortunately, this symbol "disappears" after a short time into the "overflow area", which can be opened by clicking on the small arrow pointing up:

![00e-taskbar-overflow-en](https://github.com/fraktalfan/TrayMenu/assets/93649344/50e9c907-de74-41d5-b2c9-f49a692ed97c)

To permanently display the TrayMenu icon in the notification area of the taskbar, simply drag it down to the taskbar with the mouse:

![00b-taskbar-overflow-en](https://github.com/fraktalfan/TrayMenu/assets/93649344/789f8726-316a-4d97-ae2b-0b7a28e3b3db)

![00c-taskbar-overflow-en](https://github.com/fraktalfan/TrayMenu/assets/93649344/38f2a987-8709-42bc-a343-c22536eed144)

![00e-taskbar-overflow](https://github.com/fraktalfan/TrayMenu/assets/93649344/c13eb04c-c0e6-46cb-bf87-b060a7709063)

## Usage
A normal click on the TrayMenu icon or pressing the key combination Ctrl+F1 (changeable) opens the Start menu for selecting the applications and files stored there. Initially, only a few entries are visible there, e.g. the "Programs" submenu, if it has not yet been supplemented by own entries.

A right-click on the TrayMenu icon opens the context menu through which various settings can be made:

![02-settings](https://github.com/fraktalfan/TrayMenu/assets/93649344/846b5f10-1560-4acb-a3a8-c798db24c54e)

The individual setting options are explained below:

### Explore
Selecting "Explore" opens the Start Menu folder in Windows Explorer. Start menu entries can be added, changed, moved or removed there.

### Reload
"Reload" reloads the contents of the Start menu folder so that changed folder contents are also visible in the Start menu.

### Settings
Behind "Settings" there is a submenu with further setting options:

### Settings > Select folder
Another folder can be selected via "Select folder", the content of which should be displayed in the Start menu:

![03-menu-folder](https://github.com/fraktalfan/TrayMenu/assets/93649344/cae4cce9-8000-4c08-9be6-c8940c219740)

By default, the personal Start Menu folder (C:\Users\username\AppData\Roaming\Microsoft\Windows\Start Menu) is selected. A new folder can be searched for using "Browse". The program remembers the previously searched folders. They are displayed below and can be quickly selected with a single click. The currently selected folder has a check mark next to it. If a folder is to be removed from the list of remembered folders, simply click on the corresponding "Forget" entry in the area below.

### Settings > Include
With "Include" you can select which Windows system folders (Desktop or Start menu) should also be included in the start menu:

![04a-include](https://github.com/fraktalfan/TrayMenu/assets/93649344/596c1c1a-9a78-43c8-b93c-fa40064f076d)

### Settings > Tray icon
Another icon, which should be displayed for the currently selected folder in the taskbar, can be selected via the "Tray icon" entry in the Settings menu:

![04-tray-icon](https://github.com/fraktalfan/TrayMenu/assets/93649344/8278bb25-8bbb-44a7-acf0-47426cfc7904)

### Settings > Menu colors
The colors of the menu can be individually adjusted via "Menu colors". The following presets are available: "Default" (standard Windows colors), "Light", "Dark" (as can be seen in the screenshots) and "Custom" (user-defined):

![05-menu-colors](https://github.com/fraktalfan/TrayMenu/assets/93649344/aa481e40-4661-434c-8be0-e6879dd0c288)

The custom colors can be changed via "Customize":

![06-menu-colors-customize](https://github.com/fraktalfan/TrayMenu/assets/93649344/730c12e4-0055-4755-a913-6f4af7d170ca)

The set colors apply equally to all Start menu folders.

### Settings > Autostart
"Autostart" can be used to set TrayMenu to be started automatically when Windows starts:

![07-autostart](https://github.com/fraktalfan/TrayMenu/assets/93649344/c94b27a1-f1e0-47e0-9104-4d8e3a0a9d2e)

Selecting the "Add" entry will add TrayMenu to the autostart for the currently selected folder. After selecting another folder (via "Select folder"), this folder can also be added to autostart. Any number of instances of TrayMenu for different folders can be opened at the same time or started automatically with Windows. For better differentiation, a different icon color can be selected for each instance (i.e. for each Start menu).

To remove a Start menu folder from Autostart, simply click on the corresponding "Remove" entry.

### Settings > Create shortcut
Of course, TrayMenu can also be started manually for individual folders. To do this, you can put a shortcut on the desktop (or in any other folder) via "Create Shortcut" in the Settings menu.

![12-desktop-shortcut](https://github.com/fraktalfan/TrayMenu/assets/93649344/7c7b7c39-3119-4f63-8845-4f944c19aed3)

### Settings > Define hotkey
Instead of a mouse click, the start menu can also be opened with a key combination. The default is Ctrl+F1, but this can be changed via "Define hotkey":

![11-define-hotkey](https://github.com/fraktalfan/TrayMenu/assets/93649344/517dc0ae-fd9b-447d-a60f-5199be5c1abe)

For each instance of TrayMenu (i.e. for each Start menu folder) a separate hotkey can (and should) be defined.

Also the following options are not globally valid (like the menu colors), but individually for each start menu folder:

### Settings > Localized names
The "Localized names" option determines whether the names of certain folders and files are displayed in the original English notation (e.g. "Startup" and "Programs") or in the local language notation (e.g. "Autostart" and "Programme" in German).

### Settings > Folders first
The "Folders first" option determines whether folders (submenus) should be displayed first or in alphabetical order along with the files.

### Settings > Hover to open
If the "Hover to open" option is enabled, to open the Start menu it is sufficient to hold the mouse over the tray icon without clicking on it.

### Settings > Open leftmost
With the "Open leftmost" option enabled, the Start menu opens, when activated via the hotkey, on the left edge of the screen instead of at the tray icon.

### About
Finally, there is the "About" item, which displays the program version and contains a link to access this website:

![09-about](https://github.com/fraktalfan/TrayMenu/assets/93649344/e361ac7f-dae3-4ac4-ad65-195fc2105b18)

### Exit and Close
"Exit" exits TrayMenu, "Close" just closes the menu.

## Tipps

With the help of specially named folder shortcuts, the contents of several folders can be merged in the menu. For example, the contents of the two "Programs" folders for all users (C:\ProgramData\Microsoft\Windows\Start Menu) and for your own user (C:\Users\username\AppData\Roaming\Microsoft\Windows\Start Menu) are displayed together in the "Programs" submenu. To achieve this, simply use Windows Explorer to create a shortcut to the desired target folder in the menu folder and give this shortcut a name with "target" at the beginning:

![10-explorer](https://github.com/fraktalfan/TrayMenu/assets/93649344/6d8dbcc5-8e26-4d27-9b0d-d07283febe60)

In the example above, a shortcut to the folder "C:\ProgramData\Microsoft\Windows\Start Menu" named "target - Startmenü (Alle Benutzer)" was created in the personal Start Menu folder. The menu will then show the content of the target folder instead of this shortcut. If folders of the same name already exist there, the contents are added to the existing folders. In the case of files of the same name, the names of the additional files are extended by a consecutive number in brackets. Any number of such "target" links can be created.

To include the Windows system folders "Desktop" and "Start menu" there is the setting "Include" in the "Settings" menu since version 1.1.

## Updates

### Version 1.1 (02.08.2023)

* New: Function "Include" to include the Windows system folders "Desktop" and "Start Menu" in the start menu.
* New: Function "Define hotkey" to open the start menu with a key combination.
* New: Option "Open leftmost" to open the start menu at the left edge of the screen instead of at the tray icon when activated via the hotkey.

### Version 1.0 (30.07.2023)

* First release

