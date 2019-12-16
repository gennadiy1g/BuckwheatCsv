#include <wx/filepicker.h>

#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    wxBoxSizer* topsizer = new wxBoxSizer(wxVERTICAL);

    topsizer->Add(new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
                      wxDefaultPosition, wxSize(500, -1), wxFLP_DEFAULT_STYLE),
        wxSizerFlags(0).Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL));

    wxBoxSizer* button_sizer = new wxBoxSizer(wxHORIZONTAL);

    button_sizer->Add(new wxButton(this, wxID_OK, "OK"),
        wxSizerFlags(0).Align(wxALIGN_CENTER_VERTICAL).Border(wxALL));

    button_sizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"),
        wxSizerFlags(0).Align(wxALIGN_CENTER_VERTICAL).Border(wxALL));

    topsizer->Add(button_sizer, wxSizerFlags(0).Center());

    SetSizerAndFit(topsizer); // use the sizer for layout and set size and hints
}

wxString OpenFileDialog::GetPath() const { return ""; }
