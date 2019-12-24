#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto topSizer = new wxBoxSizer(wxVERTICAL);
    topSizer->Add(new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
                      wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE, FilePathValidator(mPath)),
        wxSizerFlags(0).Border().Expand());

    auto separatorSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Separator character");
    wxSizerFlags sizerFlags(0);
    sizerFlags.Border().Center();

    separatorSizer->Add(new wxRadioButton(this, ID_Comma, "Comma", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Tab, "Tab", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Semicolon, "Semicolon", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_VerticalBar, "Vertical bar", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Space, "Space", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);
    separatorSizer->Add(new wxRadioButton(this, ID_Other, "Other", wxDefaultPosition, wxDefaultSize, 0, SeparatorIdValidator(mSeparatorId)), sizerFlags);

    mTextCtrl = new wxTextCtrl();
    /* A window can be created initially disabled by calling wxWindow::Enable() on it before calling wxWindow::Create()
     * to create the actual underlying window.
     * See https://docs.wxwidgets.org/3.1.3/classwx_window.html#a4e933aa891f42fbb3b87438057c573af for the example */
    mTextCtrl->Enable(false);
    mTextCtrl->Create(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1));
    mTextCtrl->SetMaxLength(1);
    separatorSizer->Add(mTextCtrl, sizerFlags);

    topSizer->Add(separatorSizer, wxSizerFlags(0).Border().Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    topSizer->Add(buttonSizer, sizerFlags);

    SetSizerAndFit(topSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnRadioButton, this, wxID_ANY);
}

void OpenFileDialog::OnRadioButton(wxCommandEvent& event)
{
    wxLogDebug("%i (%s %s:%i)", event.GetId(), __FUNCTION__, __FILE__, __LINE__);
    mSeparatorId = event.GetId();
    mTextCtrl->Enable(mSeparatorId == ID_Other);
}

wxChar OpenFileDialog::GetSeparator() const
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    wxChar separator { L'?' };
    switch (mSeparatorId) {
    case ID_Comma:
        separator = L',';
        break;
    case ID_Tab:
        separator = L'\t';
        break;
    case ID_Semicolon:
        separator = L';';
        break;
    case ID_VerticalBar:
        separator = L'|';
        break;
    case ID_Space:
        separator = L' ';
        break;
    case ID_Other:
        separator = mTextCtrl->GetValue()[0];
        break;
    }
    return separator;
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

SeparatorIdValidator::SeparatorIdValidator(int& separatorId)
    : mSeparatorId(separatorId)
{
}

bool SeparatorIdValidator::TransferToWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto radioButton = static_cast<wxRadioButton*>(GetWindow());
    radioButton->SetValue(radioButton->GetId() == mSeparatorId);
    return true;
};

bool SeparatorIdValidator::TransferFromWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto radioButton = static_cast<wxRadioButton*>(GetWindow());
    if (radioButton->GetValue()) {
        mSeparatorId = radioButton->GetId();
    }
    return true;
};
