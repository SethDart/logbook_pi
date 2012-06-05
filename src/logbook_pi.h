/******************************************************************************
 * $Id: logbook_pi.h, v0.3 2011-08-31 SethDart Exp $
 *
 * Project:  OpenCPN
 * Purpose:  Logbook Plugin
 * Author:   Jean-Eudes Onfray
 *
 ***************************************************************************
 *   Copyright (C) 2011 by Jean-Eudes Onfray   *
 *   $EMAIL$   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#ifndef _LogbookPI_H_
#define _LogbookPI_H_

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#define     PLUGIN_VERSION_MAJOR    0
#define     PLUGIN_VERSION_MINOR    3

#define     MY_API_VERSION_MAJOR    1
#define     MY_API_VERSION_MINOR    6

#include <wx/aui/aui.h>
#include <wx/fileconf.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include "../../../include/ocpn_plugin.h"

#include "nmea0183/nmea0183.h"
#include "tinyxml.h"

// Data must be fresher thant this delay to be saved
#define     DATA_VALIDITY    60
#define     LOGBOOK_EMPTY_VALUE    999.
#define     LOGBOOK_TOOL_POSITION -1          // Request default positioning of toolbar tool

//----------------------------------------------------------------------------------------------------------
//    The PlugIn Class Definition
//----------------------------------------------------------------------------------------------------------

enum
{
      OCPN_LBI_MAIN,
      OCPN_LBI_MIN,
      OCPN_LBI_MAX,
      OCPN_LBI_AVG
};

class LogbookUserInput;

class LogbookItem
{
      public:
            LogbookItem();

            void SetValue(double);
            void Reset();
            bool IsValid();
            double GetValue(short which, bool reset);
            wxString GetFormattedValue(short which, wxString format, bool reset);

      private:
            double      m_value, m_min, m_max, m_avg;
            int         m_count;
            wxDateTime  m_lastReceived;

};

class logbook_pi : public opencpn_plugin_16, wxTimer
{
public:
      logbook_pi(void *ppimgr);

      int Init(void);
      bool DeInit(void);

      int GetAPIVersionMajor();
      int GetAPIVersionMinor();
      int GetPlugInVersionMajor();
      int GetPlugInVersionMinor();
      wxBitmap *GetPlugInBitmap();
      wxString GetCommonName();
      wxString GetShortDescription();
      wxString GetLongDescription();
      int GetToolbarToolCount(void);
      void OnToolbarToolCallback(int id);

      void SetColorScheme( PI_ColorScheme cs );

      void SetNote( wxString note );
      void Notify();
      void CloseUserInput();

      void SetNMEASentence(wxString &sentence);
      void ShowPreferencesDialog( wxWindow* parent );

private:
      bool LoadConfig(void);
      bool SaveConfig(void);
      void ApplyConfig(void);

      void WriteLogEntryCSV(void);
      TiXmlElement *Item2XML( wxString value, LogbookItem &item, wxString format );
      void WriteLogEntryXML(void);

      wxFileConfig     *m_pconfig;
      wxAuiManager     *m_pauimgr;
      int               m_toolbar_item_id;
      LogbookUserInput *m_puserinput;
      wxString          m_filename;
      wxString          m_format;
      int               m_interval;

      NMEA0183          m_NMEA0183;                 // Used to parse NMEA Sentences
      short             mPriPosition, mPriCOGSOG, mPriHeadingM, mPriHeadingT, mPriVar, mPriDateTime, mPriWindR, mPriWindT, mPriDepth;
      LogbookItem       mLat, mLon, mCOG, mSOG, mHeadingM, mHeadingT, mSTW, mAWA, mAWS, mTWA, mTWS, mDepth, mTemp, mVar;
      wxString          m_note;

};

class LogbookPreferencesDialog : public wxDialog
{
public:
      LogbookPreferencesDialog( wxWindow *pparent, wxWindowID id, int interval, wxString format, wxString filename );
      ~LogbookPreferencesDialog() {}

      void OnCloseDialog(wxCloseEvent& event);
      void SaveLogbookConfig();

      wxString m_filename;
      wxString m_format;
      int m_interval;

private:
      wxRadioBox       *m_pFormat;
      wxFilePickerCtrl *m_pFilename;
      wxSpinCtrl       *m_pInterval;
};

class LogbookUserInput : public wxWindow
{
public:
      LogbookUserInput( wxWindow *pparent, wxWindowID id, logbook_pi *vdr, wxString note );
      void SetColorScheme( PI_ColorScheme cs );
      void SetNote( wxString note );
      void OnButtonOK( wxCommandEvent &event );
      void OnButtonCancel( wxCommandEvent &event );

private:
      logbook_pi           *m_plogbook;
      wxTextCtrl           *m_pnote;
      wxCheckBox           *m_pimmediately;
};

#endif
