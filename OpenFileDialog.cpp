#include "CsvTable/log.h"
#include "OpenFileDialog.h"

OpenFileDialog::OpenFileDialog(wxWindow* parent, wxString path, wxChar separator, wxChar quote, wxChar escape)
    : wxDialog(parent, wxID_ANY, "Open file")
    , mPath(path)
    , mSeparator(separator)
    , mEscape(escape)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    switch (mSeparator) {
    case L',':
        mSeparatorId = ID_Comma;
        break;
    case L'\t':
        mSeparatorId = ID_Tab;
        break;
    case L';':
        mSeparatorId = ID_Semicolon;
        break;
    case L'|':
        mSeparatorId = ID_VerticalBar;
        break;
    case L' ':
        mSeparatorId = ID_Space;
        break;
    default:
        mSeparatorId = ID_OtherSeparator;
        break;
    }

    switch (quote) {
    case L'"':
        mQuoteId = ID_Double;
        break;
    case L'\'':
        mQuoteId = ID_Single;
        break;
    default:
        wxASSERT(false);
        break;
    }

    switch (mEscape) {
    case L'\\':
        mEscapeId = ID_Backslash;
        break;
    default:
        mEscapeId = ID_OtherEscape;
        break;
    }

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(
        new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab)",
            wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE, FilePathValidator(mPath)),
        wxSizerFlags(0).Expand().Border());

    auto separatorSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Separator character");
    separatorSizer->Add(
        new wxRadioButton(this, ID_Comma, "Comma", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_Tab, "Tab", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_Semicolon, "Semicolon", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_VerticalBar, "Vertical bar", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_Space, "Space", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_OtherSeparator, "Other", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    mSeparatorTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1), 0,
        TextCtrlValidator(mSeparatorId, ID_OtherSeparator, mSeparator, "separator"));
    mSeparatorTextCtrl->SetMaxLength(1);
    separatorSizer->Add(mSeparatorTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    dialogSizer->Add(separatorSizer, wxSizerFlags(0).Expand().Border());

    auto quoteEscapeSizer = new wxBoxSizer(wxHORIZONTAL);
    auto quoteSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Quote character");
    quoteSizer->Add(
        new wxRadioButton(this, ID_Double, "Double", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, RadioButtonValidator(mQuoteId)),
        wxSizerFlags(0).Border().Center());
    quoteSizer->Add(new wxRadioButton(this, ID_Single, "Single", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mQuoteId)),
        wxSizerFlags(0).Border().Center());
    quoteEscapeSizer->Add(quoteSizer, wxSizerFlags(1).Border().Center());

    auto escapeSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Escape character");
    escapeSizer->Add(new wxRadioButton(this, ID_Backslash, "Backslash (\\)", wxDefaultPosition, wxDefaultSize, wxRB_GROUP,
                         RadioButtonValidator(mEscapeId)),
        wxSizerFlags(0).Border().Center());
    escapeSizer->Add(new wxRadioButton(this, ID_OtherEscape, "Other", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mEscapeId)),
        wxSizerFlags(0).Border().Center());
    mEscapeTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(20, -1), 0,
        TextCtrlValidator(mEscapeId, ID_OtherEscape, mEscape, "escape"));
    mEscapeTextCtrl->SetMaxLength(1);
    escapeSizer->Add(mEscapeTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    quoteEscapeSizer->Add(escapeSizer, wxSizerFlags(1).Border().Center());
    dialogSizer->Add(quoteEscapeSizer, wxSizerFlags(0).Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnSeparatorRadioButton, this, ID_Comma, ID_OtherSeparator);
    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnEscapeRadioButton, this, ID_Backslash, ID_OtherEscape);

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

void OpenFileDialog::OnSeparatorRadioButton(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "event.GetId()=" << event.GetId() << FUNCTION_FILE_LINE;
    mSeparatorTextCtrl->Enable(event.GetId() == ID_OtherSeparator);
}

void OpenFileDialog::OnEscapeRadioButton(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "event.GetId()=" << event.GetId() << FUNCTION_FILE_LINE;
    mEscapeTextCtrl->Enable(event.GetId() == ID_OtherEscape);
}

wxChar OpenFileDialog::getSeparator() const
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    wxChar separator { L'\0' };
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
    default:
        wxASSERT(false);
        break;
    }
    return separator;
}

wxChar OpenFileDialog::getEscape() const
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    wxChar escape { L'\0' };
    switch (mEscapeId) {
    case ID_Backslash:
        escape = L'\\';
        break;
    case ID_OtherEscape:
        escape = mEscape;
        break;
    default:
        wxASSERT(false);
        break;
    }
    return escape;
}

wxChar OpenFileDialog::getQuote() const
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    wxChar quote { L'\0' };
    switch (mQuoteId) {
    case ID_Double:
        quote = L'"';
        break;
    case ID_Single:
        quote = L'\'';
        break;
    default:
        wxASSERT(false);
        break;
    }
    return quote;
}

FilePathValidator::FilePathValidator(wxString& path)
    : mPath(path)
{
}

bool FilePathValidator::Validate(wxWindow* parent)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto filePickerCtrl = dynamic_cast<wxFilePickerCtrl*>(GetWindow());
    wxASSERT(filePickerCtrl);
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
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto filePickerCtrl = dynamic_cast<wxFilePickerCtrl*>(GetWindow());
    wxASSERT(filePickerCtrl);
    filePickerCtrl->SetPath(mPath);
    return true;
}

bool FilePathValidator::TransferFromWindow()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto filePickerCtrl = dynamic_cast<wxFilePickerCtrl*>(GetWindow());
    wxASSERT(filePickerCtrl);
    mPath = filePickerCtrl->GetPath();
    return true;
}

RadioButtonValidator::RadioButtonValidator(int& radioButtonId)
    : mRadioButtonId(radioButtonId)
{
}

bool RadioButtonValidator::TransferToWindow()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto radioButton = dynamic_cast<wxRadioButton*>(GetWindow());
    wxASSERT(radioButton);
    radioButton->SetValue(radioButton->GetId() == mRadioButtonId);
    return true;
}

bool RadioButtonValidator::TransferFromWindow()
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto radioButton = dynamic_cast<wxRadioButton*>(GetWindow());
    wxASSERT(radioButton);
    if (radioButton->GetValue()) {
        mRadioButtonId = radioButton->GetId();
    }
    return true;
}

TextCtrlValidator::TextCtrlValidator(int radioButtonId, int otherButtonId, wxChar& text, wxString description)
    : mRadioButtonId(radioButtonId)
    , mOtherButtonId(otherButtonId)
    , mChar(text)
    , mDescription(description)
{
}

bool TextCtrlValidator::Validate(wxWindow* parent)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto textCtrl = dynamic_cast<wxTextCtrl*>(GetWindow());
    wxASSERT(textCtrl);
    if (textCtrl->IsEnabled()) {
        if (textCtrl->GetValue().IsEmpty()) {
            wxMessageBox("Please enter " + mDescription + " character.", "Open file", wxOK | wxICON_EXCLAMATION | wxCENTRE, parent);
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
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto textCtrl = dynamic_cast<wxTextCtrl*>(GetWindow());
    wxASSERT(textCtrl);
    if (mRadioButtonId == mOtherButtonId) {
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
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto textCtrl = dynamic_cast<wxTextCtrl*>(GetWindow());
    wxASSERT(textCtrl);
    if (textCtrl->IsEnabled()) {
        wxASSERT(!textCtrl->GetValue().IsEmpty());
        mChar = textCtrl->GetValue()[0];
    } else {
        mChar = L'\0';
    }
    return true;
}
