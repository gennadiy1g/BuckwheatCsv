#include "CsvTable/log.h"
#include "GoToRowDialog.h"

GoToRowDialog::GoToRowDialog(wxWindow* parent, GridTableBase* mGridTable)
    : wxDialog(parent, wxID_ANY, "Go to Row")
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    mTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
    dialogSizer->Add(mTextCtrl, wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_row));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    mTextCtrl->SetFocus();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}
