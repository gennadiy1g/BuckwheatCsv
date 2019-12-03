#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/grid.h>

enum { ID_Hello = 1 };

class App : public wxApp
{
public:
    virtual bool OnInit();
};

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    wxGrid* mGrid;
    wxGridTableBase* mTable;
};

class EmptyGridTable : public wxGridTableBase
{
public:
    EmptyGridTable() {};
    virtual ~EmptyGridTable() = default; // Defaulted virtual destructor

    int GetNumberRows() override
    {
        return 100;
    }

    int GetNumberCols() override
    {
        return 26;
    }

    wxString GetValue(int row, int col) override
    {
        return L"";
    }

    void SetValue(int, int, const wxString&) override
    { /* ignore */
    }

    bool IsEmptyCell(int, int) override
    {
        return false;
    }
};
