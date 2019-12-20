#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    auto topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
                      wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE, FilePathValidator(mPath)),
        wxSizerFlags(0).Border().Expand());

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

    mTextCtrl = new wxTextCtrl();
    /* A window can be created initially disabled by calling wxWindow::Enable() on it before calling wxWindow::Create()
     * to create the actual underlying window.
     * See https://docs.wxwidgets.org/3.1.3/classwx_window.html#a4e933aa891f42fbb3b87438057c573af for the example */
    mTextCtrl->Enable(false);
    mTextCtrl->Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1));
    separatorSizer->Add(mTextCtrl, sizerFlags);

    topSizer->Add(separatorSizer, wxSizerFlags(0).Border().Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    topSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetSizerAndFit(topSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnRadioButton, this, wxID_ANY);
}

void OpenFileDialog::OnRadioButton(wxCommandEvent& event)
{
    wxLogDebug("%i (%s %s:%i)", event.GetId(), __FUNCTION__, __FILE__, __LINE__);
    mSeparatorID = event.GetId();
    mTextCtrl->Enable(mSeparatorID == ID_Other);
}

FilePathValidator::FilePathValidator(wxString& path)
    : mPath(path)
{
}

bool FilePathValidator::Validate(wxWindow* parent)
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto filePickerCtrl = static_cast<wxFilePickerCtrl*>(GetWindow());
    if (!wxFile::Exists(filePickerCtrl->GetPath())) {
        wxMessageBox(filePickerCtrl->GetPath() + "\nFile not found.\nCheck the file name and try again.", "Open file",
            wxOK | wxICON_EXCLAMATION | wxCENTRE, parent);
        return false;
    } else {
        return true;
    }
}

bool FilePathValidator::TransferToWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto filePickerCtrl = static_cast<wxFilePickerCtrl*>(GetWindow());
    filePickerCtrl->SetPath(mPath);
    return true;
};

bool FilePathValidator::TransferFromWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto filePickerCtrl = static_cast<wxFilePickerCtrl*>(GetWindow());
    mPath = filePickerCtrl->GetPath();
    return true;
};
