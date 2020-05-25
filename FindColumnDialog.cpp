#include <wx/srchctrl.h>

#include <boost/algorithm/string.hpp>

#include "FindColumnDialog.h"
#if defined(__WXGTK__)
#include "table_select_column.xpm"
#endif

FindColumnDialog::FindColumnDialog(wxWindow* parent, GridTableBase* gridTable)
    : wxDialog(parent, wxID_ANY, "Find Column", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    , mGridTable(gridTable)
{
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);

    auto searchCtrl = new wxSearchCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    searchCtrl->SetHint("few letters of a column's name, case insensitive, anywhere in the name");
    dialogSizer->Add(searchCtrl, wxSizerFlags(0).Expand().Border());

    auto mDataViewList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200));
    mDataViewList->AppendTextColumn("#", wxDATAVIEW_CELL_INERT, -1, wxALIGN_RIGHT);
    mDataViewList->AppendTextColumn("Name");

    wxVector<wxVariant> data;
    data.push_back(wxVariant("1"));
    data.push_back(wxVariant("column 1"));
    mDataViewList->AppendItem(data);

    data.clear();
    data.push_back(wxVariant("2"));
    data.push_back(wxVariant("column 2"));
    mDataViewList->AppendItem(data);

    dialogSizer->Add(mDataViewList, wxSizerFlags(1).Expand().Border());

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    searchCtrl->SetFocus();
}

void FindColumnDialog::populateDataViewListCtrl(const std::wstring_view partOfName)
{
    std::wstring partOfNameTrim(partOfName);
    boost::trim(partOfNameTrim);
}
