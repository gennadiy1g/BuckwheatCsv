// By some reason, the linker fails with error about undefined reference in boost::log when the next line is not the first one
#include "CsvTable/log.h"
#include "GoToColumnDialog.h"
#ifdef __WXGTK__
#include "table_select_column.xpm"
#endif

GoToColumnDialog::GoToColumnDialog(wxWindow* parent, GridTableBase* gridTable)
    : wxDialog(parent, wxID_ANY, "Go to Column")
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    mComboBox = new wxComboBox(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0, ComboBoxValidator());
    for (auto i = 0; i < gridTable->GetColsCount(); ++i) {
        mComboBox->Append(gridTable->GetColLabelValue(i));
    }
    dialogSizer->Add(mComboBox, wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    mComboBox->SetFocus();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}

bool ComboBoxValidator::Validate(wxWindow* parent)
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto comboBox = dynamic_cast<wxComboBox*>(GetWindow());
    wxASSERT(comboBox);
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << ", comboBox->GetSelection()=" << comboBox->GetSelection()
                                             << ", comboBox->GetValue()=" << comboBox->GetValue() << FUNCTION_FILE_LINE;
    if (comboBox->GetSelection() == wxNOT_FOUND) {
        auto userInput = comboBox->GetValue().Trim(true).Trim(false);
        if (!userInput.IsEmpty()) {
            auto position = comboBox->FindString(userInput, true);
            if (position == wxNOT_FOUND) {
                position = comboBox->FindString(userInput, false);
            }
            if (position != wxNOT_FOUND) {
                comboBox->SetSelection(position);
                return true;
            }
            wxMessageBox(userInput + "\nColumn not found.\nCheck the column name and try again.", "Go to Column",
                wxOK | wxICON_EXCLAMATION | wxCENTRE, parent);
        } else {
            wxMessageBox("Enter column name or select it from the list.", "Go to Column", wxOK | wxICON_EXCLAMATION | wxCENTRE, parent);
        }
        return false;
    } else {
        return true;
    }
}
