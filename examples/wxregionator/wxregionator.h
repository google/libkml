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

// This file contains the definition of the main frame used in the application.

#ifndef WXREGIONATOR_H__
#define WXREGIONATOR_H__

#include <wx/wx.h>
#include <wx/filepicker.h>

class wxProgressDialog;

// The main frame class.
class MainFrame : public wxFrame {
 public:
  MainFrame(const wxString& title);
  // Event handlers.
  void GenerateRbnl(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  // kmlconvenience::FeatureListReginator progress callback method.
  bool RegionatorProgress(unsigned int completed, unsigned int total);
 private:
  bool ValidateFormFields(const wxChar* input_file, const wxString& output_dir);
  bool AskIfOutputDirNotEmpty(const wxString& output_dir);
  wxPanel* panel_;
  wxFilePickerCtrl* input_file_ctrl_;
  wxDirPickerCtrl* output_dir_ctrl_;
  wxProgressDialog* progress_dialog_;
  DECLARE_EVENT_TABLE()
};

#endif  // WXREGIONATOR_H__
