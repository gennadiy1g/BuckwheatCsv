#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    mFilePickerCtrl = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
        wxDefaultPosition, wxSize(500, -1), wxFLP_DEFAULT_STYLE);

    auto topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(mFilePickerCtrl, wxSizerFlags(0).Align(wxALIGN_CENTER_HORIZONTAL).Border(wxALL));

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center());

    topSizer->AddSpacer(5);

    SetSizerAndFit(topSizer); // use the sizer for layout and set size and hints
}
