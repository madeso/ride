// Copyright (2015) Gustav

#include "ride/createnewfiledlg.h"

#include <ride/wx.h>

#include <wx/dir.h>
#include <wx/filename.h>

#include "ride/generated/ui.h"

#include "ride/resources/icons.h"

#include "ride/wxutils.h"

//////////////////////////////////////////////////////////////////////////

class CreateNewFileDlg : public ui::CreateNewFile {
 public:
  CreateNewFileDlg(wxWindow* parent, const wxString& project_folder,
                   const wxString& fodler_hint);

  const wxString GetFilePath() const;
  const wxString GetTemplateSource() const;

 protected:
  void OnTextChanged(wxCommandEvent& event) override;
  void OnComboChanged(wxCommandEvent& event) override;
  void OnCheckChanged(wxCommandEvent& event) override;

  void OnNameEnter(wxCommandEvent& event) override;
  void OnCancel(wxCommandEvent& event) override;
  void OnOk(wxCommandEvent& event) override;

 private:
  void UpdateTemplateSource();
  wxString project_folder_;
};

//////////////////////////////////////////////////////////////////////////

CreateNewFileDlgHandler::CreateNewFileDlgHandler(wxWindow* parent,
                                                 const wxString& project_folder,
                                                 const wxString& fodler_hint)
    : parent_(parent),
      project_folder_(project_folder),
      fodler_hint_(fodler_hint) {}

bool CreateNewFileDlgHandler::ShowModal() {
  CreateNewFileDlg dlg(parent_, project_folder_, fodler_hint_);
  const bool ret = wxOK == dlg.ShowModal();
  if (ret) {
    file_path_ = dlg.GetFilePath();
    template_source_ = dlg.GetTemplateSource();
  }
  return ret;
}

const wxString CreateNewFileDlgHandler::file_path() const { return file_path_; }

const wxString CreateNewFileDlgHandler::template_source() const {
  return template_source_;
}

//////////////////////////////////////////////////////////////////////////

class FileTemplate {
 public:
  virtual ~FileTemplate() {}
  virtual const wxString GenerateContent(const wxString& name) = 0;
};

void AddFileTemplate(wxListCtrl* list, const wxString& text, int image,
                     FileTemplate* ft) {
  auto i = list->InsertItem(0, "", image);
  list->SetItemColumnImage(i, 0, image);
  list->SetItem(i, 1, text);
  list->SetItem(i, 2, "Rust");
  list->SetItemPtrData(i, reinterpret_cast<wxUIntPtr>(ft));
}

class EmptyFileTemplate : public FileTemplate {
 public:
  const wxString GenerateContent(const wxString& name) override {
    return wxEmptyString;
  }
} g_empty_file_template;

class RustClassFileTemplate : public FileTemplate {
 public:
  const wxString GenerateContent(const wxString& name) override {
    return wxString::Format(
        "struct %s {\n"
        "}\n"
        "\n"
        "impl %s {\n"
        "}\n"
        "\n",
        name, name);
  }
} g_rust_class_file_template;

enum {
  REPLACE_SPACE_NOT,
  REPLACE_SPACE_WITH_NONE,
  REPLACE_SPACE_WITH_UNDERSCORE,
  REPLACE_SPACE_WITH_DASH
};

CreateNewFileDlg::CreateNewFileDlg(wxWindow* parent,
                                   const wxString& project_folder,
                                   const wxString& fodler_hint)
    : ui::CreateNewFile(parent), project_folder_(project_folder) {
  wxImageList* images = new wxImageList(16, 16);
  images->Add(wxIcon(file_normal_xpm));
  uiTemplates->AssignImageList(images, wxIMAGE_LIST_SMALL);

  const auto icon_index = uiTemplates->InsertColumn(0, "Icon");
  const auto language_index = uiTemplates->InsertColumn(2, "Language");
  const auto name_index = uiTemplates->InsertColumn(1, "Name");
  uiTemplates->SetColumnWidth(icon_index, 20);
  uiTemplates->SetColumnWidth(language_index, 60);
  uiTemplates->SetColumnWidth(name_index, 200);

  AddFileTemplate(uiTemplates, "Empty Rust file", 0, &g_empty_file_template);
  AddFileTemplate(uiTemplates, "Rust class", 0, &g_rust_class_file_template);

  uiPath->SetValue(fodler_hint);

  uiReplaceAction->AppendString("Don't replace spaces");
  uiReplaceAction->AppendString("Remove spaces");
  uiReplaceAction->AppendString("Replaces spaces with a _ (underscore)");
  uiReplaceAction->AppendString("Replaces spaces with a - (dash)");
  uiReplaceAction->Select(3);

  uiTemplates->SetItemState(0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
  uiName->SetFocus();

  UpdateTemplateSource();
}

void CreateNewFileDlg::OnNameEnter(wxCommandEvent& event) { OnOk(event); }

void CreateNewFileDlg::OnCancel(wxCommandEvent& event) { EndModal(wxCANCEL); }

void CreateNewFileDlg::OnOk(wxCommandEvent& event) { EndModal(wxOK); }

const wxString CreateNewFileDlg::GetFilePath() const {
  wxString file_name = uiName->GetValue();
  if (uiLowerCase->IsChecked()) {
    file_name.MakeLower();
  }

  const int action = uiReplaceAction->GetSelection();
  switch (action) {
    case -1:
    case REPLACE_SPACE_NOT:
      break;
    case REPLACE_SPACE_WITH_NONE:
      file_name.Replace(" ", "");
      break;
    case REPLACE_SPACE_WITH_UNDERSCORE:
      file_name.Replace(" ", "_");
      break;
    case REPLACE_SPACE_WITH_DASH:
      file_name.Replace(" ", "-");
      break;
    default:
      assert(false && "Invalid replace action!");
  }

  wxFileName fn(project_folder_);
  const wxString path = uiPath->GetValue();
  if (path != wxEmptyString) {
    auto dirs = Split(path, '/');
    for (auto d : dirs) {
      if (d != wxEmptyString) {
        fn.AppendDir(d);
      }
    }
  }
  if (file_name != wxEmptyString) {
    fn.SetFullName(file_name);
    if (fn.GetExt() == wxEmptyString) {
      fn.SetExt("rs");  // todo: use the extension from the template instead..
    }
  }

  return fn.GetFullPath();
}

FileTemplate* GetFt(wxListCtrl* list) {
  const auto selection =
      list->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
  if (selection == -1) return NULL;
  wxUIntPtr data = list->GetItemData(selection);
  FileTemplate* ret = reinterpret_cast<FileTemplate*>(data);
  return ret;
}

const wxString CreateNewFileDlg::GetTemplateSource() const {
  FileTemplate* file_template = GetFt(uiTemplates);
  if (file_template == NULL)
    return wxEmptyString;
  else
    return file_template->GenerateContent(uiName->GetValue());
}

void CreateNewFileDlg::OnTextChanged(wxCommandEvent& event) {
  UpdateTemplateSource();
}
void CreateNewFileDlg::OnComboChanged(wxCommandEvent& event) {
  UpdateTemplateSource();
}
void CreateNewFileDlg::OnCheckChanged(wxCommandEvent& event) {
  UpdateTemplateSource();
}

void CreateNewFileDlg::UpdateTemplateSource() {
  uiSuggestedFilePath->SetLabel(GetFilePath());
}
