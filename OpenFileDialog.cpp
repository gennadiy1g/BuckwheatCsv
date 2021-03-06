// By some reason, the linker fails with error about undefined reference in boost::log when the next line is not the first one
#include "CsvTable/log.h"
#include "CsvTable/utilities.h"
#include "OpenFileDialog.h"
#ifdef __WXGTK__
#include "folder_table.xpm"
#endif

OpenFileDialog::OpenFileDialog(wxWindow* parent, wxString path, wxChar separator, wxChar quote, wxChar escape)
    : wxDialog(parent, wxID_ANY, "Open UTF-8 encoded file")
    , mPath(path)
    , mSeparator(separator)
    , mEscape(escape)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    switch (mSeparator) {
    case L',':
        mSeparatorId = ID_COMMA;
        break;
    case L'\t':
        mSeparatorId = ID_TAB;
        break;
    case L';':
        mSeparatorId = ID_SEMICOLON;
        break;
    case L'|':
        mSeparatorId = ID_VERTICAL_BAR;
        break;
    case L' ':
        mSeparatorId = ID_SPACE;
        break;
    default:
        mSeparatorId = ID_OTHER_SEPARATOR;
        break;
    }

    switch (quote) {
    case L'\"':
        mQuoteId = ID_DOUBLE;
        break;
    case L'\'':
        mQuoteId = ID_SINGLE;
        break;
    default:
        wxASSERT(false);
        break;
    }

    switch (mEscape) {
    case L'\0':
        mEscapeId = ID_NO_ESCAPE;
        break;
    default:
        mEscapeId = ID_OTHER_ESCAPE;
        break;
    }

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    auto filePickerCtrl
        = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, "Open file", "Delimited text (*.csv;*.txt;*.tab)|*.csv;*.txt;*.tab",
            wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE | wxFLP_USE_TEXTCTRL | wxFLP_SMALL, FilePathValidator(mPath));
    dialogSizer->Add(filePickerCtrl, wxSizerFlags(0).Expand().Border());

    auto separatorSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Separator character");
    separatorSizer->Add(
        new wxRadioButton(this, ID_COMMA, "Comma", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_TAB, "Tab", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_SEMICOLON, "Semicolon", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_VERTICAL_BAR, "Vertical bar", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(new wxRadioButton(this, ID_SPACE, "Space", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    separatorSizer->Add(
        new wxRadioButton(this, ID_OTHER_SEPARATOR, "Other", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mSeparatorId)),
        wxSizerFlags(0).Border().Center());
    mSeparatorTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(30, -1), 0,
        TextCtrlValidator(mSeparatorId == ID_OTHER_SEPARATOR, mSeparator, "separator"));
    mSeparatorTextCtrl->SetMaxLength(1);
    separatorSizer->Add(mSeparatorTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    dialogSizer->Add(separatorSizer, wxSizerFlags(0).Expand().Border());

    auto quoteEscapeSizer = new wxBoxSizer(wxHORIZONTAL);
    auto quoteSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Quote character");
    quoteSizer->Add(
        new wxRadioButton(this, ID_DOUBLE, "Double", wxDefaultPosition, wxDefaultSize, wxRB_GROUP, RadioButtonValidator(mQuoteId)),
        wxSizerFlags(0).Border().Center());
    quoteSizer->Add(new wxRadioButton(this, ID_SINGLE, "Single", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mQuoteId)),
        wxSizerFlags(0).Border().Center());
    quoteEscapeSizer->Add(quoteSizer, wxSizerFlags(1).Border().Center());

    auto escapeSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Escape character");
    escapeSizer->Add(new wxRadioButton(this, ID_NO_ESCAPE, "No escape character", wxDefaultPosition, wxDefaultSize, wxRB_GROUP,
                         RadioButtonValidator(mEscapeId)),
        wxSizerFlags(0).Border().Center());
    escapeSizer->Add(
        new wxRadioButton(this, ID_OTHER_ESCAPE, "Other", wxDefaultPosition, wxDefaultSize, 0, RadioButtonValidator(mEscapeId)),
        wxSizerFlags(0).Border().Center());
    mEscapeTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(30, -1), 0,
        TextCtrlValidator(mEscapeId == ID_OTHER_ESCAPE, mEscape, "escape"));
    mEscapeTextCtrl->SetMaxLength(1);
    escapeSizer->Add(mEscapeTextCtrl, wxSizerFlags(0).Border(wxLEFT | wxRIGHT).Center());
    quoteEscapeSizer->Add(escapeSizer, wxSizerFlags(1).Border().Center());
    dialogSizer->Add(quoteEscapeSizer, wxSizerFlags(0).Expand());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(folder_table));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnSeparatorRadioButton, this, ID_COMMA, ID_OTHER_SEPARATOR);
    Bind(wxEVT_RADIOBUTTON, &OpenFileDialog::OnEscapeRadioButton, this, ID_NO_ESCAPE, ID_OTHER_ESCAPE);
    Bind(wxEVT_FILEPICKER_CHANGED, &OpenFileDialog::OnFilePicker, this);

    filePickerCtrl->GetTextCtrl()->SetFocus();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

void OpenFileDialog::OnSeparatorRadioButton(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "event.GetId()=" << event.GetId() << FUNCTION_FILE_LINE;
    mSeparatorTextCtrl->Enable(event.GetId() == ID_OTHER_SEPARATOR);
}

void OpenFileDialog::OnEscapeRadioButton(wxCommandEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "event.GetId()=" << event.GetId() << FUNCTION_FILE_LINE;
    mEscapeTextCtrl->Enable(event.GetId() == ID_OTHER_ESCAPE);
}

void OpenFileDialog::OnFilePicker(wxFileDirPickerEvent& event)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << "event.GetId()=" << event.GetId() << FUNCTION_FILE_LINE;

    std::optional<wchar_t> separator;
    std::optional<wchar_t> quote;
    bool failed { false };

    try {
        detectSeparatorAndQuote(bfs::path(event.GetPath()), separator, quote);
    } catch (const std::exception& e) {
        failed = true;
    }

    if (!failed) {
        BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
        wxWindow* window { nullptr };
        if (separator) {
            switch (separator.value()) {
            case L',':
                window = FindWindow(ID_COMMA);
                break;
            case L'\t':
                window = FindWindow(ID_TAB);
                break;
            case L';':
                window = FindWindow(ID_SEMICOLON);
                break;
            case L'|':
                window = FindWindow(ID_VERTICAL_BAR);
                break;
            case L' ':
                window = FindWindow(ID_SPACE);
                break;
            default:
                wxASSERT(false);
                break;
            }
            wxASSERT(window);
            auto radioButton = dynamic_cast<wxRadioButton*>(window);
            wxASSERT(radioButton);
            radioButton->SetValue(true);
        }

        window = nullptr;
        if (quote) {
            switch (quote.value()) {
            case L'\"':
                window = FindWindow(ID_DOUBLE);
                break;
            case L'\'':
                window = FindWindow(ID_SINGLE);
                break;
            default:
                wxASSERT(false);
                break;
            }
            wxASSERT(window);
            auto radioButton = dynamic_cast<wxRadioButton*>(window);
            wxASSERT(radioButton);
            radioButton->SetValue(true);
        }
    }
}

wxChar OpenFileDialog::getSeparator() const
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    wxChar separator { L'\0' };
    switch (mSeparatorId) {
    case ID_COMMA:
        separator = L',';
        break;
    case ID_TAB:
        separator = L'\t';
        break;
    case ID_SEMICOLON:
        separator = L';';
        break;
    case ID_VERTICAL_BAR:
        separator = L'|';
        break;
    case ID_SPACE:
        separator = L' ';
        break;
    case ID_OTHER_SEPARATOR:
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
    case ID_NO_ESCAPE:
        escape = L'\0';
        break;
    case ID_OTHER_ESCAPE:
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
    case ID_DOUBLE:
        quote = L'\"';
        break;
    case ID_SINGLE:
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

TextCtrlValidator::TextCtrlValidator(bool textCtrlEnabled, wxChar& ch, wxString description)
    : mTextCtrlEnabled(textCtrlEnabled)
    , mCh(ch)
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
    if (mTextCtrlEnabled) {
        textCtrl->SetValue(mCh);
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
        mCh = textCtrl->GetValue()[0];
    } else {
        mCh = L'\0';
    }
    return true;
}
