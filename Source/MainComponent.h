#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <list>


class MainComponent   : public Component, 
  public ChangeListener,
  public MenuBarModel,
  public ApplicationCommandTarget
{
public:
  MainComponent();
  ~MainComponent();

  void paint (Graphics&) override;
  void resized() override;
  void changeListenerCallback(ChangeBroadcaster *source) override;

  // ApplicationCommandTarget
  virtual ApplicationCommandTarget * 	getNextCommandTarget() override;
  virtual void 	getAllCommands(Array< CommandID > &commands) override;
  virtual void 	getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override;
  virtual bool 	perform(const InvocationInfo &info) override;

  // MenuBarModel
  virtual StringArray getMenuBarNames() override;
  virtual PopupMenu getMenuForIndex(int topLevelMenuIndex, const String &) override;
  virtual void 	menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override;

  void do_fileopen();
  void do_fileclose();
  void add_document(const File& file);

private:
  struct OpenDocument {
    OpenDocument(const File& f) : file(f), tokenizer(nullptr) {

      String ext = f.getFileExtension();
      if (ext == ".cc" || ext == ".cpp" || ext == ".h" || ext == ".c")
        tokenizer = new CPlusPlusCodeTokeniser;
      else if (ext == ".lua")
        tokenizer = new LuaTokeniser;
      else if (ext == ".xml")
        tokenizer = new 	XmlTokeniser;

      code_document = new CodeDocument;
      editor = new CodeEditorComponent(*code_document, tokenizer);
      String contents = f.loadFileAsString();
      code_document->replaceAllContent(contents);
      editor->setTabSize(2, true);
    }
    ~OpenDocument() {
      delete editor;
      delete code_document;
    }

    File file;
    ScopedPointer<CodeTokeniser> tokenizer;
    CodeEditorComponent* editor;
    CodeDocument* code_document;
  };
  
  typedef std::list<OpenDocument*> OpenDocsList;
  OpenDocsList _opendocs;
  
  // settings
  PropertiesFile::Options _options;
  ApplicationProperties _applicationProperties;
  PropertiesFile* _settings;

  ScopedPointer<Font> _editorFont;
  
  ScopedPointer<MenuBarComponent> _menu;
  
  TabbedComponent _tabs;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


