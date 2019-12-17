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

    Bind(wxEVT_BUTTON, &OpenFileDialog::OnOK, this, wxID_OK);
}

void OpenFileDialog::OnOK(wxCommandEvent& event)
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    if (!wxFile::Exists(GetPath())) {
        wxMessageBox(GetPath() + "\nFile not found.\nCheck the file name and try again.", "Open file", wxOK | wxICON_EXCLAMATION | wxCENTRE, this);
        return;
    }
    event.Skip(true);
}
