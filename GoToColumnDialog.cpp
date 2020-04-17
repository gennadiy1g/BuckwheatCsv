#include <wx/combobox.h>

#include "GoToColumnDialog.h"

GoToColumnDialog::GoToColumnDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Go to Column")
{
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);
    dialogSizer->Add(new wxComboBox(this, wxID_ANY), wxSizerFlags(0).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Border().Center());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints
}
