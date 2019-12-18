#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    mFilePickerCtrl = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)");

    auto topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(mFilePickerCtrl, wxSizerFlags(0).Border().Expand());

    auto separatorSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Separator character");
    wxSizerFlags sizerFlags(0);
    sizerFlags.Border().CenterVertical();
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Comma"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Tab"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Semicolon"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Vertical bar"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Space"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, wxID_ANY, "Other"), sizerFlags);
    separatorSizer->Add(new wxTextCtrl(this, wxID_ANY), sizerFlags);

    topSizer->Add(separatorSizer, wxSizerFlags(0).Border().Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

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
