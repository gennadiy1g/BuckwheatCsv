#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    mFilePickerCtrl = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
        wxDefaultPosition, wxSize(500, -1), wxFLP_DEFAULT_STYLE);

    wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);

    topSizer->Add(mFilePickerCtrl, wxSizerFlags(0).Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL));

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    buttonSizer->Add(new wxButton(this, wxID_OK, "OK"),
        wxSizerFlags(0).Align(wxALIGN_CENTER_VERTICAL).Border(wxALL));

    buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"),
        wxSizerFlags(0).Align(wxALIGN_CENTER_VERTICAL).Border(wxALL));

    topSizer->Add(buttonSizer, wxSizerFlags(0).Center());

    SetSizerAndFit(topSizer); // use the sizer for layout and set size and hints
}

wxString OpenFileDialog::GetPath() const { return ""; }
