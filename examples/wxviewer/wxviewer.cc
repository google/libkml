// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "wxviewer.h"
#include "wx/filedlg.h"
#include "wx/ffile.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/log.h"
#include "icon_file.xpm"
#include "icon_folder_opened.xpm"
#include "icon_folder_closed.xpm"

using kmldom::ContainerPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;

// Event table for MainFrame menus.
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
  EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
  EVT_MENU(DIALOGS_FILE_OPEN, MainFrame::OnFileOpen)
END_EVENT_TABLE()

// Event table for ListView mouse actions.
BEGIN_EVENT_TABLE(ListView, wxTreeCtrl)
  // TODO.
END_EVENT_TABLE()

// Implements WXViewer& GetApp().
DECLARE_APP(WXViewer)

// Give wxWidgets the means to create a WXViewer object.
IMPLEMENT_APP(WXViewer)

// Helper to find the root element of the parsed KML.
static const FeaturePtr GetRootFeature(const ElementPtr& root) {
  const KmlPtr kml = kmldom::AsKml(root);
  if (kml && kml->has_feature()) {
    return kml->get_feature();
  }
  return kmldom::AsFeature(root);
}

// Helper to convert wxString to std::string.
static std::string wx2std(const wxString& wxstr, wxMBConv* conv) {
  if (wxstr.empty()) {
    return "";
  }
  if (!conv) {
    conv = wxConvCurrent;  // TODO: more research on this.
  }
  return std::string(wxstr.mb_str(*conv));
}

// Helper to convert std::string to wxString.
static wxString std2wx(const std::string& stdstr, wxMBConv* conv) {
  if (stdstr.empty()) {
    return wxEmptyString;
  }
  if (!conv) {
    conv = wxConvCurrent;  // TODO: more research on this.
  }
  return wxString(stdstr.c_str(), *conv);
}

bool WXViewer::OnInit() {
  // Create the main application window.
  MainFrame* frame = new MainFrame(wxT("WXViewer"), wxID_ANY,
                                   wxDefaultPosition, wxSize(400, 400),
                                   wxDEFAULT_FRAME_STYLE);
  frame->Show(true);
  return true;
}

ListView::ListView(wxWindow* parent, const wxWindowID id, const wxPoint& pos,
                   const wxSize& size, long style)
    : wxTreeCtrl(parent, id, pos, size, style), kml_root_(NULL) {
  CreateImageList();
}

void ListView::CreateImageList() {
  const int size = 32;
  // The icon list should match the ICON_XXX enum defined in the ListView
  // class.
  wxImageList* images = new wxImageList(size, size, true);
  wxBusyCursor wait;
  wxIcon icons[3];
  icons[0] = wxIcon(icon_file_xpm);
  icons[1] = wxIcon(icon_folder_opened_xpm);
  icons[2] = wxIcon(icon_folder_closed_xpm);
  for (size_t i = 0; i < WXSIZEOF(icons); ++i) {
    images->Add(icons[i]);
  }
  AssignImageList(images);
}

void ListView::SetKmlData(const wxString& str) {
  std::string errors;
  kml_root_ = kmldom::Parse(wx2std(str, NULL), &errors);
  if (!kml_root_) {
    wxLogMessage(wxT("SetKmlData Parse failed: %s"),
                 std2wx(errors, NULL).c_str());
    return;
  }
  DeleteAllItems();
  Populate();
}

void ListView::Populate() {
  if (!kml_root_) {
    return;
  }

  const FeaturePtr root_feature = GetRootFeature(kml_root_);
  wxString name = std2wx(root_feature->get_name(), NULL);

  int icon = ICON_FILE;
  const ContainerPtr container = kmldom::AsContainer(root_feature);
  if (container) {
    icon = container->get_open() ? ICON_FOLDER_OPENED : ICON_FOLDER_CLOSED;
  }

  wxTreeItemId root_id = AddRoot(name, icon, wxTreeItemIcon_Expanded, NULL);
  SetItemImage(root_id, icon, wxTreeItemIcon_Expanded);

  if (container) {
    AppendContainer(container, root_id);
    if (container->get_open()) {
      Expand(root_id);
    }
  }
}

void ListView::AppendContainer(const ContainerPtr& container,
                               wxTreeItemId parent_id) {
  for (size_t i = 0; i < container->get_feature_array_size(); ++i) {
    AppendFeature(container->get_feature_array_at(i), parent_id);
  }
}

void ListView::AppendFeature(const FeaturePtr& feature,
                             wxTreeItemId parent_id) {
  wxString name = std2wx(feature->get_name(), NULL);
  wxTreeItemId appended_id;
  const ContainerPtr container = kmldom::AsContainer(feature);
  if (container) {
    int icon = container->get_open() ? ICON_FOLDER_OPENED : ICON_FOLDER_CLOSED;
    appended_id = AppendItem(parent_id, name, icon, icon, NULL);
    AppendContainer(container, appended_id);
    if (container->get_open()) {
      Expand(appended_id);
    }
  } else {
    AppendItem(parent_id, name, ICON_FILE, ICON_FILE, NULL);
  }
}

MainFrame::MainFrame(const wxString& title, wxWindowID id, const wxPoint& pos,
                     const wxSize& size, long style)
    : wxFrame(NULL, id, title, pos, size, style), listview_(NULL) {

  // Create a menu bar.
  wxMenu* FileMenu = new wxMenu;

  // The "About" item is in the help menu.
  wxMenu* HelpMenu = new wxMenu;
  HelpMenu->Append(wxID_ABOUT, wxT("About"), wxT("Show about dialog"));
  FileMenu->Append(wxID_EXIT, wxT("Quit WXViewer"), wxT("Quit this program"));
  FileMenu->Append(DIALOGS_FILE_OPEN, wxT("Open File..."), wxT("Open a file"));

  // Now append the created menus to the menu bar.
  wxMenuBar* MenuBar = new wxMenuBar();
  MenuBar->Append(FileMenu, wxT("File"));
  MenuBar->Append(HelpMenu, wxT("Help"));

  // And attach the menu bar to the frame.
  SetMenuBar(MenuBar);

  // Create a status bar.
  CreateStatusBar(2);
  SetStatusText(wxT("42"));

  // Create a panel to hold our controls.
  panel_ = new wxPanel(this);

  // Bring up the listview and parent it to the panel.
  listview_ = new ListView(panel_, WINDOW_LISTVIEW, wxDefaultPosition,
                           wxDefaultSize, wxTR_HAS_BUTTONS);

  // Size the listview to the main frame.
  Resize();
}

void MainFrame::Resize() {
  wxSize size = GetClientSize();
  listview_->SetSize(0, 0, size.x, size.y);
}

MainFrame::~MainFrame() {
  delete listview_;
  delete panel_;
}

void MainFrame::OnAbout(wxCommandEvent& event) {
  wxString msg;
  msg.Printf(wxT("A libkml demo application.\nCreated with %s"),
                 wxVERSION_STRING);
  wxMessageBox(msg, wxT("About WXViewer"),
               wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnQuit(wxCommandEvent& event) {
  Close();
}

void MainFrame::OnFileOpen(wxCommandEvent& WXUNUSED(event)) {
  wxFileDialog dialog(this, _T("Select a file to open"), wxEmptyString,
                      wxEmptyString, _T("KML files (*.kml;*.kmz)|*.kml;*.kmz"));
  dialog.CenterOnParent();
  dialog.SetDirectory(wxGetHomeDir());
  wxString file_data;
  if (dialog.ShowModal() == wxID_OK) {
    wxFFile file(dialog.GetPath(), _T("r"));  // TODO KMZ
    if (!file.IsOpened()) {
      wxLogMessage(wxT("Could not open %s"), dialog.GetPath().c_str());
      return;
    }
    file.ReadAll(&file_data);
    file.Close();
  }
  listview_->SetKmlData(file_data);
}

