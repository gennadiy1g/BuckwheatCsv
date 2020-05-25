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

    mDataViewList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 200));
    dialogSizer->Add(mDataViewList, wxSizerFlags(1).Expand().Border());

    mDataViewList->AppendTextColumn("#", wxDATAVIEW_CELL_INERT, -1, wxALIGN_RIGHT);
    mDataViewList->AppendTextColumn("Name");
    populateColumnsListCtrl(L"");

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_SEARCH, &FindColumnDialog::OnSearch, this);

    searchCtrl->SetFocus();
}

void FindColumnDialog::populateColumnsListCtrl(const wxString& partOfName)
{
    std::wstring partOfNameTrim = boost::trim_copy(partOfName.ToStdWstring());

    mDataViewList->DeleteAllItems();
    wxVector<wxVariant> data;

    for (auto i = 0; i < mGridTable->GetColsCount(); ++i) {
        if (partOfNameTrim == L"" || boost::icontains(mGridTable->GetColLabelValue(i).ToStdWstring(), partOfNameTrim)) {
            data.push_back(wxVariant(std::to_string(i + 1)));
            data.push_back(wxVariant(mGridTable->GetColLabelValue(i)));
            mDataViewList->AppendItem(data);
            data.clear();
        }
    }
}

void FindColumnDialog::OnSearch(wxCommandEvent& event)
{
    auto s = event.GetString();
    populateColumnsListCtrl(s);
}
