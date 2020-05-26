#include <boost/algorithm/string.hpp>

#include "FindColumnDialog.h"
#if defined(__WXGTK__)
#include "table_select_column.xpm"
#endif

FindColumnDialog::FindColumnDialog(wxWindow* parent, GridTableBase* gridTable)
    : wxDialog(parent, wxID_ANY, "Find Column", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
    , mGridTable(gridTable)
    , mTimer(this)
{
    auto dialogSizer = new wxBoxSizer(wxVERTICAL);

    mSearchCtrl = new wxSearchCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    mSearchCtrl->SetHint("few letters of a column's name, case insensitive, anywhere in the name");
    dialogSizer->Add(mSearchCtrl, wxSizerFlags(0).Expand().Border());

    mDataViewList = new wxDataViewListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(300, 200));
    dialogSizer->Add(mDataViewList, wxSizerFlags(1).Expand().Border());

    mDataViewList->AppendTextColumn("Name");
    mDataViewList->AppendTextColumn("Position");
    populateColumnsListCtrl(L"");

    auto buttonSizer = CreateButtonSizer(wxOK | wxCANCEL);
    dialogSizer->Add(buttonSizer, wxSizerFlags(0).Center().Border());

    SetIcon(wxICON(table_select_column));
    SetSizerAndFit(dialogSizer); // use the sizer for layout and set size and hints

    Bind(wxEVT_SEARCH, &FindColumnDialog::OnSearch, this);
    Bind(wxEVT_TEXT, &FindColumnDialog::OnTextChange, this);
    Bind(wxEVT_TIMER, &FindColumnDialog::OnTimer, this);

    mSearchCtrl->SetFocus();
}

void FindColumnDialog::populateColumnsListCtrl(const wxString& textToSearch)
{
    std::wstring textToSearchTrim = boost::trim_copy(textToSearch.ToStdWstring());

    mDataViewList->DeleteAllItems();
    wxVector<wxVariant> data;

    for (auto i = 0; i < mGridTable->GetColsCount(); ++i) {
        if (textToSearchTrim == L"" || boost::icontains(mGridTable->GetColLabelValue(i).ToStdWstring(), textToSearchTrim)) {
            data.push_back(wxVariant(mGridTable->GetColLabelValue(i)));
            data.push_back(wxVariant(std::to_string(i + 1)));
            mDataViewList->AppendItem(data);
            data.clear();
        }
    }
}

void FindColumnDialog::OnSearch(wxCommandEvent& event)
{
    auto textToSearch = event.GetString();
    populateColumnsListCtrl(textToSearch);
}

void FindColumnDialog::OnTimer(wxTimerEvent& event)
{
    auto textToSearch = mSearchCtrl->GetValue();
    populateColumnsListCtrl(textToSearch);
}

void FindColumnDialog::OnTextChange(wxCommandEvent& event)
{
    mTimer.Stop();
    mTimer.StartOnce(500);
}
