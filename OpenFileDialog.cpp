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

    auto radioButton = new wxRadioButton();
    radioButton->Create(this, ID_Comma, "Comma");
    radioButton->SetValue(true);
    separatorSizer->Add(radioButton, sizerFlags);

    separatorSizer->Add(new wxRadioButton(this, ID_Tab, "Tab"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Semicolon, "Semicolon"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_VerticalBar, "Vertical bar"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Space, "Space"), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Other, "Other"), sizerFlags);

    auto textCtrl = new wxTextCtrl();
    /* A window can be created initially disabled by calling wxWindow::Enable() on it before calling wxWindow::Create()
     * to create the actual underlying window.
     * See https://docs.wxwidgets.org/3.1.3/classwx_window.html#a4e933aa891f42fbb3b87438057c573af for the example */
    textCtrl->Enable(false);
    textCtrl->Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1));
    separatorSizer->Add(textCtrl, sizerFlags);

    topSizer->Add(separatorSizer, wxSizerFlags(0).Border().Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetSizerAndFit(topSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_BUTTON, &OpenFileDialog::OnOK, this, wxID_OK);
    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnRadioButton, this, wxID_ANY);
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

void OpenFileDialog::OnRadioButton(wxCommandEvent& event)
{
    wxLogDebug("%i (%s %s:%i)", event.GetId(), __FUNCTION__, __FILE__, __LINE__);
}
