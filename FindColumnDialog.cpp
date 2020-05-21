#include <wx/dataview.h>
#include <wx/srchctrl.h>

#include "FindColumnDialog.h"
#if defined(__WXGTK__)
#include "table_select_column.xpm"
#endif

FindColumnDialog::FindColumnDialog(wxWindow* parent)
    : wxDialog(parent, wxID_ANY, "Find Column", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);

    auto staticText = new wxStaticText(this, wxID_ANY, "Filter columns by name");
    dialogSizer->Add(staticText, wxSizerFlags(0).Expand().Border());

    auto searchCtrl = new wxSearchCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    searchCtrl->SetHint("few letters of a column's name, case insensitive, anywhere in the name");
    dialogSizer->Add(searchCtrl, wxSizerFlags(0).Expand().Border());

    auto dataViewList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200));
    dataViewList->AppendTextColumn("#", wxDATAVIEW_CELL_INERT, -1, wxALIGN_RIGHT);
    dataViewList->AppendTextColumn("Name");

    wxVector<wxVariant> data;
    data.push_back(wxVariant("1"));
    data.push_back(wxVariant("column 1"));
    dataViewList->AppendItem(data);

    data.clear();
    data.push_back(wxVariant("2"));
    data.push_back(wxVariant("column 2"));
    dataViewList->AppendItem(data);

    dialogSizer->Add(dataViewList, wxSizerFlags(1).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    searchCtrl->SetFocus();
}
