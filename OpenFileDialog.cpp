#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Open file")
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
                         wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE, FilePathValidator(mPath)),
        wxSizerFlags(0).Expand().Border());

    auto separatorSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Separator character");
    separatorSizer->Add(new wxRadioButton(this, ID_Comma, "Comma", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_Tab, "Tab", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_Semicolon, "Semicolon", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_VerticalBar, "Vertical bar", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_Space, "Space", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_OtherSeparator, "Other", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)), wxSizerFlags(0).Border().Center());
    mSeparatorTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1), 0, TextCtrlValidator(mSeparatorId, mSeparator));
    mSeparatorTextCtrl->SetMaxLength(1);
    separatorSizer->Add(mSeparatorTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    dialogSizer->Add(separatorSizer, wxSizerFlags(0).Expand().Border());

    auto quoteEscapeSizer = new wxBoxSizer(wxHORIZONTAL);
    auto quoteSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Quote character");
    quoteSizer->Add(new wxRadioButton(this, wxID_ANY, "Double", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator), wxSizerFlags(0).Border().Center());
    quoteSizer->Add(new wxRadioButton(this, wxID_ANY, "Single", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator), wxSizerFlags(0).Border().Center());
    quoteEscapeSizer->Add(quoteSizer, wxSizerFlags(1).Border().Center());

    auto escapeSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Escape character");
    escapeSizer->Add(new wxRadioButton(this, ID_Backslash, "Backslash (\\)", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, wxDefaultValidator), wxSizerFlags(0).Border().Center());
    escapeSizer->Add(new wxRadioButton(this, ID_OtherEscape, "Other", wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator), wxSizerFlags(0).Border().Center());
    mEscapeTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1), 0, wxDefaultValidator);
    mEscapeTextCtrl->SetMaxLength(1);
    escapeSizer->Add(mEscapeTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    quoteEscapeSizer->Add(escapeSizer, wxSizerFlags(1).Border().Center());
    dialogSizer->Add(quoteEscapeSizer, wxSizerFlags(0).Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnSeparatorRadioButton, this, ID_Comma, ID_OtherSeparator);
    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnEscapeRadioButton, this, ID_Backslash, ID_OtherEscape);
}

void OpenFileDialog::OnSeparatorRadioButton(wxCommandEvent& event)
{
    wxLogDebug("%i (%s %s:%i)", event.GetId(), __FUNCTION__, __FILE__, __LINE__);
    mSeparatorTextCtrl->Enable(event.GetId() == ID_OtherSeparator);
}

void OpenFileDialog::OnEscapeRadioButton(wxCommandEvent& event)
{
    wxLogDebug("%i (%s %s:%i)", event.GetId(), __FUNCTION__, __FILE__, __LINE__);
    mEscapeTextCtrl->Enable(event.GetId() == ID_OtherEscape);
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
    case ID_OtherSeparator:
        separator = mSeparator;
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
}

bool FilePathValidator::TransferFromWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto filePickerCtrl = static_cast<wxFilePickerCtrl*>(GetWindow());
    mPath = filePickerCtrl->GetPath();
    return true;
}

RadioButtonValidator::RadioButtonValidator(int& radioButtonId)
    : mRadioButtonId(radioButtonId)
{
}

bool RadioButtonValidator::TransferToWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto radioButton = static_cast<wxRadioButton*>(GetWindow());
    radioButton->SetValue(radioButton->GetId() == mRadioButtonId);
    return true;
}

bool RadioButtonValidator::TransferFromWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto radioButton = static_cast<wxRadioButton*>(GetWindow());
    if (radioButton->GetValue()) {
        mRadioButtonId = radioButton->GetId();
    }
    return true;
}

TextCtrlValidator::TextCtrlValidator(const int& radioButtonId, wxChar& text)
    : mRadioButtonId(radioButtonId)
    , mChar(text)
{
}

bool TextCtrlValidator::Validate(wxWindow* parent)
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto textCtrl = static_cast<wxTextCtrl*>(GetWindow());
    if (textCtrl->IsEnabled()) {
        if (textCtrl->GetValue().IsEmpty()) {
            wxMessageBox("Please enter separator character.", "Open file", wxOK | wxICON_EXCLAMATION | wxCENTRE, parent);
            return false;
        } else {
            return true;
        }
    } else {
        return true;
    }
}

bool TextCtrlValidator::TransferToWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto textCtrl = static_cast<wxTextCtrl*>(GetWindow());
    if (mRadioButtonId == ID_OtherSeparator) {
        textCtrl->SetValue(mChar);
        textCtrl->Enable(true);
    } else {
        textCtrl->SetValue(L"");
        textCtrl->Enable(false);
    }
    return true;
}

bool TextCtrlValidator::TransferFromWindow()
{
    wxLogDebug("(%s %s:%i)", __FUNCTION__, __FILE__, __LINE__);
    auto textCtrl = static_cast<wxTextCtrl*>(GetWindow());
    if (textCtrl->IsEnabled()) {
        wxASSERT(!textCtrl->GetValue().IsEmpty());
        mChar = textCtrl->GetValue()[0];
    } else {
        mChar = L'\0';
    }
    return true;
}
