// By some reason, the linker fails with error about undefined reference in boost::log when the next line is not the first one
#include "CsvTable/log.h"
#include "GoToRowDialog.h"
#include "table_select_row.xpm"

#include <wx/event.h>
#include <wx/valnum.h>

GoToRowDialog::GoToRowDialog(wxWindow* parent, GridTableBase* gridTable)
    : wxDialog(parent, wxID_ANY, "Go to Row")
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    wxIntegerValidator<unsigned int> intValidator(&mRow, wxNUM_VAL_ZERO_AS_BLANK);
    intValidator.SetRange(1, gridTable->GetRowsCount());
    mTextCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, intValidator);
    dialogSizer->Add(mTextCtrl, wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_row));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    mTextCtrl->SetFocus();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}
