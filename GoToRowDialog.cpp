#include "CsvTable/log.h"
#include "GoToRowDialog.h"

GoToRowDialog::GoToRowDialog(wxWindow* parent, GridTableBase* mGridTable)
    : wxDialog(parent, wxID_ANY, "Go to Row")
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    mSpinCtrl = new wxSpinCtrl(
        this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, mGridTable->GetRowsCount(), 1);
    dialogSizer->Add(mSpinCtrl, wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_row));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    mSpinCtrl->SetFocus();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}
