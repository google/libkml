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

// This is a simple demo application using the wxWidgets framework to
// exercise some aspects of libkml.
//
// The current implementation simply parses a given KML file into a tree
// control to visualize the parsed DOM.

#ifndef WXVIEWER_H__
#define WXVIEWER_H__

#include <string>
#include "kml/dom.h"
#include "wx/treectrl.h"
#include "wx/wx.h"

// The main application class.
class WXViewer : public wxApp {
 public:
  // Called on application startup.
  virtual bool OnInit();
};

// The ListView class is a tree control that displays the structure of the
// parsed KML DOM.
class ListView : public wxTreeCtrl {
 public:
  ListView(wxWindow* parent, const wxWindowID id, const wxPoint& pos,
           const wxSize& size, long style);
  virtual ~ListView() {}

  // Set the contents of the listview from a wxString of KML. The existing
  // contents, if any, are destroyed.
  void SetKmlData(const wxString& str);

  // The icon for files and folders.
  enum {
    ICON_FILE,
    ICON_FOLDER_OPENED,
    ICON_FOLDER_CLOSED
  };

 private:
  // A helper to construct the wxImageList used to hole the listview icons.
  void CreateImageList();
  // Populate the tree control from a stored KML DOM object.
  void Populate();
  // Helper for Populate. Walks a KML DOM container calling AppendFeature
  // for all children.
  void AppendContainer(const kmldom::ContainerPtr& container,
                       wxTreeItemId parent_id);
  // Helper for Populate. Takes a KML DOM Feature and appends it to the
  // listview control. Checks if the Feature is a container, and calls
  // AppendContainer if so.
  void AppendFeature(const kmldom::FeaturePtr&, wxTreeItemId parent_id);
  // A smart pointer to a parsed KML DOM object.
  kmldom::ElementPtr kml_root_;
  DECLARE_EVENT_TABLE()
};

// The main window frame.
class MainFrame : public wxFrame {
 public:
  MainFrame(const wxString& title, wxWindowID id, const wxPoint& pos,
            const wxSize& size, long style);
  virtual ~MainFrame();

  // Menu callbacks.
  void OnAbout(wxCommandEvent& event);
  void OnFileOpen(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);

  // Fits the listview control to the size of the main window frame.
  void Resize();

 private:
  wxPanel* panel_;
  ListView* listview_;
  DECLARE_EVENT_TABLE()
};

// Menu and control IDs.
enum {
  DIALOGS_FILE_OPEN = 1,  // 0 is reserved.
  WINDOW_LISTVIEW,
  WINDOW_MAINWINDOW
};

#endif  // WXVIEWER_H__

