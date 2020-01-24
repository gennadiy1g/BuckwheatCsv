#pragma once

#include <wx/filepicker.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum WindowID {
    ID_COMMA = wxID_HIGHEST + 1,
    ID_TAB,
    ID_SEMICOLON,
    ID_VERTICAL_BAR,
    ID_SPACE,
    ID_OTHER_SEPARATOR,

    ID_BACKSLASH,
    ID_OTHER_ESCAPE,

    ID_DOUBLE,
    ID_SINGLE
};

class OpenFileDialog : public wxDialog {
public:
    OpenFileDialog(wxWindow* parent, wxString path, wxChar separator, wxChar quote, wxChar escape); // Constructor
    virtual ~OpenFileDialog() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    OpenFileDialog(const OpenFileDialog& src) = delete;
    OpenFileDialog& operator=(const OpenFileDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    OpenFileDialog(OpenFileDialog&& src) = default;
    OpenFileDialog& operator=(OpenFileDialog&& rhs) = default;

    wxString getPath() const { return mPath; };
    wxChar getSeparator() const;
    wxChar getQuote() const;
    wxChar getEscape() const;

private:
    void OnSeparatorRadioButton(wxCommandEvent& event);
    void OnEscapeRadioButton(wxCommandEvent& event);
    void OnFilePicker(wxFileDirPickerEvent& event);

    wxTextCtrl* mSeparatorTextCtrl { nullptr };
    wxTextCtrl* mEscapeTextCtrl { nullptr };

    wxString mPath { wxEmptyString };
    int mSeparatorId { ID_COMMA };
    wxChar mSeparator { L',' };
    int mEscapeId { ID_BACKSLASH };
    wxChar mEscape { L'\\' };
    int mQuoteId { ID_DOUBLE };
};

class FilePathValidator : public wxValidator {
public:
    explicit FilePathValidator(wxString& path);
    FilePathValidator(const FilePathValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new FilePathValidator(*this); }

private:
    wxString& mPath;
};

class RadioButtonValidator : public wxValidator {
public:
    explicit RadioButtonValidator(int& radioButtonId);
    RadioButtonValidator(const RadioButtonValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override { return true; };
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new RadioButtonValidator(*this); }

private:
    int& mRadioButtonId;
};

class TextCtrlValidator : public wxValidator {
public:
    explicit TextCtrlValidator(int radioButtonId, int otherButtonId, wxChar& ch, wxString description);
    TextCtrlValidator(const TextCtrlValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new TextCtrlValidator(*this); }

private:
    int mRadioButtonId; // ID of the button selected by the user
    int mOtherButtonId; // ID of the "Other" button
    wxChar& mCh;
    wxString mDescription;
};
