#pragma once

#include <wx/filepicker.h>
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/combobox.h>

#include "GridTable.h"

class GoToColumnDialog : public wxDialog {
public:
    GoToColumnDialog(wxWindow* parent, GridTableBase* gridTable);
    virtual ~GoToColumnDialog() = default;

    // Disallow assignment and pass-by-value.
    GoToColumnDialog(const GoToColumnDialog& src) = delete;
    GoToColumnDialog& operator=(const GoToColumnDialog& rhs) = delete;

    // Explicitly default move constructor and move assignment operator.
    GoToColumnDialog(GoToColumnDialog&& src) = default;
    GoToColumnDialog& operator=(GoToColumnDialog&& rhs) = default;

    int getColumn() const { return mComboBox->GetSelection(); }

private:
    wxComboBox* mComboBox { nullptr };
};

class ComboBoxValidator : public wxValidator {
public:
    explicit ComboBoxValidator() = default;
    ComboBoxValidator(const ComboBoxValidator& src) = default;
    virtual bool Validate(wxWindow* parent) override;
    virtual bool TransferToWindow() override { return true; };
    virtual bool TransferFromWindow() override { return true; };
    virtual wxObject* Clone() const override { return new ComboBoxValidator(*this); }
};