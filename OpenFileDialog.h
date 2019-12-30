#pragma once

#include <wx/filepicker.h>
#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum {
    ID_Comma = wxID_HIGHEST + 1,
    ID_Tab,
    ID_Semicolon,
    ID_VerticalBar,
    ID_Space,
    ID_OtherSeparator,

    ID_Backslash,
    ID_OtherEscape,

    ID_Double,
    ID_Single
};

class OpenFileDialog : public wxDialog {
public:
    explicit OpenFileDialog(wxWindow* parent); // Constructor
    virtual ~OpenFileDialog() = default; // Defaulted virtual destructor

    // Disallow assignment and pass-by-value.
    OpenFileDialog(const OpenFileDialog& src) = delete;
    OpenFileDialog& operator=(const OpenFileDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    OpenFileDialog(OpenFileDialog&& src) = default;
    OpenFileDialog& operator=(OpenFileDialog&& rhs) = default;

    wxString getPath() const { return mPath; };
    int getSeparatorId() const { return mSeparatorId; };
    wxChar getSeparator() const;
    int getQuoteId() const { return mQuoteId; };
    int getEscaptId() const { return mEscapeId; };
    wxChar getEscape() const;

private:
    void OnSeparatorRadioButton(wxCommandEvent& event);
    void OnEscapeRadioButton(wxCommandEvent& event);

    wxTextCtrl* mSeparatorTextCtrl { nullptr };
    wxTextCtrl* mEscapeTextCtrl { nullptr };

    wxString mPath { wxEmptyString };
    int mSeparatorId { ID_Comma };
    wxChar mSeparator { L'\0' };
    int mEscapeId { ID_Backslash };
    wxChar mEscape { L'\0' };
    int mQuoteId { ID_Double };
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
    explicit TextCtrlValidator(int radioButtonId, int otherButtonId, wxChar& text, wxString description);
    TextCtrlValidator(const TextCtrlValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override;
    virtual bool TransferFromWindow() override;
    virtual wxObject* Clone() const override { return new TextCtrlValidator(*this); }

private:
    int mRadioButtonId; // ID of the button selected by the user
    int mOtherButtonId; // ID of the "Other" button
    wxChar& mChar;
    wxString mDescription;
};
