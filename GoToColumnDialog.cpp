// By some reason, the linker fails with error about undefined reference in boost::log when the next line is not the first one
#include "CsvTable/log.h"
#include <wx/combobox.h>
#include "GoToColumnDialog.h"

GoToColumnDialog::GoToColumnDialog(wxWindow* parent, GridTableBase* mGridTable)
    : wxDialog(parent, wxID_ANY, "Go to Column")
{
    auto& gLogger = GlobalLogger::get();
    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;

    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    auto comboBox = new wxComboBox(this, wxID_ANY);
    for (auto i = 0; i < mGridTable->GetColsCount(); ++i) {
        comboBox->Append(mGridTable->GetColLabelValue(i));
    }
    dialogSizer->Add(comboBox, wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    BOOST_LOG_SEV(gLogger, bltrivial::trace) << FUNCTION_FILE_LINE;
}
