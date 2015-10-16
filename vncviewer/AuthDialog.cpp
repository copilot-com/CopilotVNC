/////////////////////////////////////////////////////////////////////////////
//  Copyright (C) 2002-2013 UltraVNC Team Members. All Rights Reserved.
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307,
//  USA.
//
// If the source code for the program is not available from the place from
// which you received this file, check 
// http://www.uvnc.com/
//
////////////////////////////////////////////////////////////////////////////
 


// AuthDialog.cpp: implementation of the AuthDialog class.

#include "stdhdrs.h"
#include "vncviewer.h"
#include "AuthDialog.h"
#include "Exception.h"
#include <windowsx.h>
#include "common/win32_helpers.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

AuthDialog::AuthDialog()
{
	m_passwd[0]=__T('\0');
	//adzm 2010-05-12 - passphrase
	m_bPassphraseMode = false;
}

AuthDialog::~AuthDialog()
{
}

int AuthDialog::DoDialog(bool ms_logon, TCHAR IN_host[MAX_HOST_NAME_LEN], int IN_port, bool isSecure, bool warning)
{
	TCHAR tempchar[10];
	strcpy_s(_host, IN_host);
	strcat_s(_host, ":");
	strcat_s(_host, itoa(IN_port, tempchar, 10));
	if (isSecure) return DialogBoxParam(pApp->m_instance, DIALOG_MAKEINTRESOURCE(IDD_AUTH_DIALOG), NULL, (DLGPROC) DlgProc, (LONG_PTR) this);
	else if (ms_logon) return DialogBoxParam(pApp->m_instance, DIALOG_MAKEINTRESOURCE(IDD_AUTH_DIALOG2), NULL, (DLGPROC) DlgProc, (LONG_PTR) this);
	else if (warning) return DialogBoxParam(pApp->m_instance, DIALOG_MAKEINTRESOURCE(IDD_AUTH_DIALOG1), NULL, (DLGPROC) DlgProc1, (LONG_PTR) this);
	else return DialogBoxParam(pApp->m_instance, DIALOG_MAKEINTRESOURCE(IDD_AUTH_DIALOG3), NULL, (DLGPROC) DlgProc1, (LONG_PTR) this);
}

BOOL CALLBACK AuthDialog::DlgProc(  HWND hwnd,  UINT uMsg,  
									   WPARAM wParam, LPARAM lParam ) {
	// This is a static method, so we don't know which instantiation we're 
	// dealing with. But we can get a pseudo-this from the parameter to 
	// WM_INITDIALOG, which we therafter store with the window and retrieve
	// as follows:

    AuthDialog *_this = helper::SafeGetWindowUserData<AuthDialog>(hwnd); 

	switch (uMsg) {

	case WM_INITDIALOG:
		{
            helper::SafeSetWindowUserData(hwnd, lParam);

			_this = (AuthDialog *) lParam;

            Edit_LimitText(GetDlgItem(hwnd, IDC_PASSWD_EDIT), 32);
			//CentreWindow(hwnd);
			TCHAR tempchar[MAX_HOST_NAME_LEN];
			GetWindowText(hwnd, tempchar, MAX_HOST_NAME_LEN);
			strcat_s(tempchar, "   ");
			strcat_s(tempchar, _this->_host);
			SetWindowText(hwnd, tempchar);
			SetForegroundWindow(hwnd);
			return TRUE;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			{
				UINT res= GetDlgItemText( hwnd,  IDC_PASSWD_EDIT,
					_this->m_passwd, 256);
				res= GetDlgItemText( hwnd,  IDD_DOMAIN,
					_this->m_domain, 256);
				res= GetDlgItemText( hwnd,  IDD_USER_NAME,
					_this->m_user, 256);
				
				EndDialog(hwnd, TRUE);

				return TRUE;
			}
		case IDCANCEL:
			EndDialog(hwnd, FALSE);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, FALSE);
		return TRUE;
	}
	return 0;
}

BOOL CALLBACK AuthDialog::DlgProc1(  HWND hwnd,  UINT uMsg,  
									   WPARAM wParam, LPARAM lParam ) {
	// This is a static method, so we don't know which instantiation we're 
	// dealing with. But we can get a pseudo-this from the parameter to 
	// WM_INITDIALOG, which we therafter store with the window and retrieve
	// as follows:
    AuthDialog *_this = helper::SafeGetWindowUserData<AuthDialog>(hwnd); 

	switch (uMsg) {

	case WM_INITDIALOG:
		{           
			_this = (AuthDialog *) lParam;
			 helper::SafeSetWindowUserData(hwnd, lParam);
			//adzm 2010-05-12 - passphrase
			Edit_LimitText(GetDlgItem(hwnd, IDC_PASSWD_EDIT), _this->m_bPassphraseMode ? 128 : 8);
			//CentreWindow(hwnd);
			TCHAR tempchar[MAX_HOST_NAME_LEN];
			GetWindowText(hwnd, tempchar, MAX_HOST_NAME_LEN);
			strcat_s(tempchar, "   ");
			strcat_s(tempchar, _this->_host);
			SetWindowText(hwnd, tempchar);
			return TRUE;
		}
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			{
				UINT res= GetDlgItemText( hwnd,  IDC_PASSWD_EDIT,
					_this->m_passwd, 256);
				EndDialog(hwnd, TRUE);

				return TRUE;
			}
		case IDCANCEL:
			EndDialog(hwnd, FALSE);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		EndDialog(hwnd, FALSE);
		return TRUE;
	}
	return 0;
}
