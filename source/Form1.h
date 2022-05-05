#pragma once

#include "stdafx.h"
#include "FormVerify.h"
#include "FormAboutEx.h"
#include "FormMessageBox.h"

namespace Hashing {

   using namespace System;
   using namespace System::ComponentModel;
   using namespace System::Collections;
   using namespace System::Collections::Generic;
   using namespace System::Diagnostics;
   using namespace System::Windows::Forms;
   using namespace System::Data;
   using namespace System::Drawing;
   using namespace System::Runtime::InteropServices;
   using namespace System::IO;
   using namespace System::Text;


   /// <summary>
   /// Summary for Form1
   /// </summary>
   public ref class Form1 : public System::Windows::Forms::Form
   {
   public:
      Form1(array<System::String ^> ^args)
      {
         InitializeComponent();

         // Do it here, generated code will erase
         cbTabMultiple->SelectedIndex = 1;
         cbTabManual->SelectedIndex   = 1;

         // Create our temporary files list
          L_TempFiles = gcnew List<String^>();

         // Allocate working buffer
         try
         {
            btWorkBuffer = new BYTE[128*1024];
            iChunkSize = 128*1024;
         }
         catch(Exception^ e)
         {
            e;
            try
            {
               btWorkBuffer = new BYTE[64*1024];
               iChunkSize = 64*1024;
            }
            catch(Exception^ e)
            {
               e;
               try
               {
                  btWorkBuffer = new BYTE[4*1024];
                  iChunkSize = 4*1024;
               }
               catch(Exception^ e)
               {
                  e;

                  FormMessageBox mfb("Bad memory allocation", "Memory allocation failed. Program will abort", FormMessageBox::HashingRed);
                  mfb.ShowDialog();

                  Environment::Exit(-1); // Application::Exit() does not work here
               }
            }
         }

         // Parse the input arguments
         if ( args->Length == 0 )
         {
            tabControl->SelectTab(0);
         }
         else if ( args->Length == 1 )
         {
            if ( File::Exists(args[0]) )
            {
               if ( (File::GetAttributes(args[0]) & FileAttributes::Directory)  == FileAttributes::Directory )
               {
                  // this is a folder, ignore it
                  SetStatusLabel("Ignoring directories as input arguments");
               }
               else if (Path::GetExtension(args[0]) == ".lnk")
               {
                  SetStatusLabel("Ignoring shortcuts as input arguments");
               }
               else
               {
                  SetTabSingleFilePath(args[0]);
                  TabSingleOpenFileDialog->FileName = args[0];
               }
            }
            else
            {
               // this file does not exist
               SetStatusLabel("Ignoring inexistent files as input arguments");
            }

            tabControl->SelectTab(0);
         }
         else if ( args->Length > 1 )
         {
            List<String^>^ L_MultipleFiles = gcnew List<String^>();
            int iCount = args->Length;

            for (int i = 0; i < args->Length; i++ )
            {
               if ( File::Exists(args[i]) )
               {
                  if ( (File::GetAttributes(args[i]) & FileAttributes::Directory)  == FileAttributes::Directory )
                  {
                     // this is a folder, ignore it
                     SetStatusLabel("Ignoring directories as input arguments");
                     iCount--;
                  }
                  else if (Path::GetExtension(args[0]) == ".lnk")
                  {
                     SetStatusLabel("Ignoring shortcuts as input arguments");
                     iCount--;
                  }
                  else
                  {
                     L_MultipleFiles->Add( args[i] );
                  }
               }
               else
               {
                  // this file does not exist
                  SetStatusLabel("Ignoring inexistent files as input arguments");
                  iCount--;
               }
            }

            if ( iCount == 0 )
            {
               tbTabMultipleFilePath->Text = "";
               TabMultipleOpenFileDialog->Tag = nullptr;
            }
            else if ( iCount == 1 )
            {
               tbTabMultipleFilePath->Text = iCount + " file selected";
               TabMultipleOpenFileDialog->Tag = L_MultipleFiles;
            }
            else
            {
               tbTabMultipleFilePath->Text = iCount + " files selected";
               TabMultipleOpenFileDialog->Tag = L_MultipleFiles;
            }

            tabControl->SelectTab(1);
         }
      }

   protected:
      /// <summary>
      /// Clean up any resources being used.
      /// </summary>
      ~Form1()
      {
         // Delete our working buffer
         if (btWorkBuffer)
         {
            delete[] btWorkBuffer;
         }

         if (components)
         {
            delete components;
         }
      }
   private: System::Windows::Forms::OpenFileDialog^  TabSingleOpenFileDialog;
   private: System::Windows::Forms::StatusStrip^  statusStrip;
   private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel;
   private: System::Windows::Forms::TabControl^  tabControl;
   private: System::Windows::Forms::TabPage^  tabSingle;
   private: System::Windows::Forms::TextBox^  tbSHA512;
   private: System::Windows::Forms::TextBox^  tbSHA256;
   private: System::Windows::Forms::TextBox^  tbSHA1;
   private: System::Windows::Forms::TextBox^  tbMD5;
   private: System::Windows::Forms::Button^  btTabSingleSHA512;
   private: System::Windows::Forms::Button^  btTabSingleSHA256;
   private: System::Windows::Forms::Button^  btTabSingleSHA1;
   private: System::Windows::Forms::Button^  btTabSingleMD5;
   private: System::Windows::Forms::TextBox^  tbTabSingleFilePath;
   private: System::Windows::Forms::Button^  btTabSingleBrowse;
   private: System::Windows::Forms::TabPage^  tabMultiple;
   private: System::Windows::Forms::TabPage^  tabManual;
   private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
   private: System::Windows::Forms::MenuStrip^  menuStrip1;
   private: System::Windows::Forms::RichTextBox^  rtbTabMultiple;
   private: System::Windows::Forms::Button^  btTabMutipleBrowse;
   private: System::Windows::Forms::Label^  labelTabManualResult;
   private: System::Windows::Forms::TextBox^  tbTabManualResult;
   private: System::Windows::Forms::Button^  btTabManualCalculate;
   private: System::Windows::Forms::Label^  labelTabMultipleChoose;
   private: System::Windows::Forms::ComboBox^  cbTabMultiple;
   private: System::Windows::Forms::TextBox^  tbTabMultipleFilePath;
   private: System::Windows::Forms::OpenFileDialog^  TabMultipleOpenFileDialog;
   private: System::Windows::Forms::Button^  btTabMultipleCopy;
   private: System::Windows::Forms::Label^  labelTabManualChoose;
   private: System::Windows::Forms::ComboBox^  cbTabManual;
   private: System::Windows::Forms::Button^  btTabMutipleCalculate;
   private: System::ComponentModel::BackgroundWorker^  bgWorker;
   private: System::Windows::Forms::ToolStripProgressBar^  tsProgressBar;
   private: System::Windows::Forms::Button^  btTabMultipleSaveAs;
   private: System::Windows::Forms::Button^  btTabSingleSaveAll;
   private: System::Windows::Forms::SaveFileDialog^  SaveFileDialog;
   private: System::Windows::Forms::Button^  btCopyMD5;
   private: System::Windows::Forms::Button^  btCopySHA512;
   private: System::Windows::Forms::Button^  btCopySHA256;
   private: System::Windows::Forms::Button^  btCopySHA1;
   private: System::Windows::Forms::Button^  btTabManualCopy;
   private: System::Windows::Forms::Button^  btTabSingleVerify;
   private: System::Windows::Forms::Button^  btTabMultipleOpenNotepad;
   private: System::Windows::Forms::ContextMenuStrip^  ctxMenu;
   private: System::Windows::Forms::ToolStripMenuItem^  ctxMenuClear;
   private: System::Windows::Forms::ToolStripMenuItem^  ctxMenuPaste;
   private: System::Windows::Forms::RichTextBox^  rtbTabManualInput;
   private: System::Windows::Forms::Label^  lbTabManualBytes;
   private: System::ComponentModel::IContainer^  components;
   private: System::Windows::Forms::ToolStripMenuItem^  donateToolStripMenuItem;
   private: System::Windows::Forms::ToolStripMenuItem^  websiteToolStripMenuItem;
   private: System::Windows::Forms::Label^  label1;
   private: System::Windows::Forms::ToolStripSplitButton^  tsCancel;
   private: System::Windows::Forms::ToolStripMenuItem^  tsiCancel;
   private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem1;
   private: System::Windows::Forms::CheckBox^  checkBoxTabManual;

   // My global variables
   private: BYTE* btWorkBuffer;
   private: int   iChunkSize;

   private: String^ strLastValidData;
   private: List<String^>^ L_TempFiles;

   // My enums
   private: enum class EAlgorithm : int { MD5, SHA1, SHA224, SHA256, SHA384, SHA512};
   private: enum class ETab : int { SINGLE_FILE, MULTIPLE_FILES };

   private: ref struct InputArguments
   {
      List<String^>^ L_Files;
      EAlgorithm alg;
      ETab tab;
   };

   private: ref struct OutputArguments
   {
      List<String^>^ L_Files;
      List<String^>^ L_Hashes;
      EAlgorithm alg;
      ETab tab;
   };

   private: ref struct Progress
   {
      int iNumberOfFiles;
      int iProcessing;
      ETab tab;
   };

      /// <summary>
      /// Required designer variable.
      /// </summary>


#pragma region Windows Form Designer generated code
      /// <summary>
      /// Required method for Designer support - do not modify
      /// the contents of this method with the code editor.
      /// </summary>
      void InitializeComponent(void)
      {
         this->components = (gcnew System::ComponentModel::Container());
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager( Form1::typeid ));
         this->TabSingleOpenFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
         this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
         this->toolStripStatusLabel = (gcnew System::Windows::Forms::ToolStripStatusLabel());
         this->tsCancel = (gcnew System::Windows::Forms::ToolStripSplitButton());
         this->tsiCancel = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->tsProgressBar = (gcnew System::Windows::Forms::ToolStripProgressBar());
         this->tabControl = (gcnew System::Windows::Forms::TabControl());
         this->tabSingle = (gcnew System::Windows::Forms::TabPage());
         this->btTabSingleVerify = (gcnew System::Windows::Forms::Button());
         this->btCopySHA512 = (gcnew System::Windows::Forms::Button());
         this->btCopySHA256 = (gcnew System::Windows::Forms::Button());
         this->btCopySHA1 = (gcnew System::Windows::Forms::Button());
         this->btCopyMD5 = (gcnew System::Windows::Forms::Button());
         this->btTabSingleSaveAll = (gcnew System::Windows::Forms::Button());
         this->tbSHA512 = (gcnew System::Windows::Forms::TextBox());
         this->tbSHA256 = (gcnew System::Windows::Forms::TextBox());
         this->tbSHA1 = (gcnew System::Windows::Forms::TextBox());
         this->tbMD5 = (gcnew System::Windows::Forms::TextBox());
         this->btTabSingleSHA512 = (gcnew System::Windows::Forms::Button());
         this->btTabSingleSHA256 = (gcnew System::Windows::Forms::Button());
         this->btTabSingleSHA1 = (gcnew System::Windows::Forms::Button());
         this->btTabSingleMD5 = (gcnew System::Windows::Forms::Button());
         this->tbTabSingleFilePath = (gcnew System::Windows::Forms::TextBox());
         this->btTabSingleBrowse = (gcnew System::Windows::Forms::Button());
         this->tabMultiple = (gcnew System::Windows::Forms::TabPage());
         this->btTabMultipleOpenNotepad = (gcnew System::Windows::Forms::Button());
         this->btTabMultipleSaveAs = (gcnew System::Windows::Forms::Button());
         this->btTabMutipleCalculate = (gcnew System::Windows::Forms::Button());
         this->btTabMultipleCopy = (gcnew System::Windows::Forms::Button());
         this->tbTabMultipleFilePath = (gcnew System::Windows::Forms::TextBox());
         this->labelTabMultipleChoose = (gcnew System::Windows::Forms::Label());
         this->cbTabMultiple = (gcnew System::Windows::Forms::ComboBox());
         this->rtbTabMultiple = (gcnew System::Windows::Forms::RichTextBox());
         this->btTabMutipleBrowse = (gcnew System::Windows::Forms::Button());
         this->tabManual = (gcnew System::Windows::Forms::TabPage());
         this->checkBoxTabManual = (gcnew System::Windows::Forms::CheckBox());
         this->label1 = (gcnew System::Windows::Forms::Label());
         this->lbTabManualBytes = (gcnew System::Windows::Forms::Label());
         this->rtbTabManualInput = (gcnew System::Windows::Forms::RichTextBox());
         this->ctxMenu = (gcnew System::Windows::Forms::ContextMenuStrip( this->components ));
         this->ctxMenuClear = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->ctxMenuPaste = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->btTabManualCopy = (gcnew System::Windows::Forms::Button());
         this->labelTabManualChoose = (gcnew System::Windows::Forms::Label());
         this->cbTabManual = (gcnew System::Windows::Forms::ComboBox());
         this->labelTabManualResult = (gcnew System::Windows::Forms::Label());
         this->tbTabManualResult = (gcnew System::Windows::Forms::TextBox());
         this->btTabManualCalculate = (gcnew System::Windows::Forms::Button());
         this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->aboutToolStripMenuItem1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->donateToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->websiteToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
         this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
         this->TabMultipleOpenFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
         this->bgWorker = (gcnew System::ComponentModel::BackgroundWorker());
         this->SaveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
         this->statusStrip->SuspendLayout();
         this->tabControl->SuspendLayout();
         this->tabSingle->SuspendLayout();
         this->tabMultiple->SuspendLayout();
         this->tabManual->SuspendLayout();
         this->ctxMenu->SuspendLayout();
         this->menuStrip1->SuspendLayout();
         this->SuspendLayout();
         // 
         // TabSingleOpenFileDialog
         // 
         this->TabSingleOpenFileDialog->Title = L"Select a file to hash";
         // 
         // statusStrip
         // 
         this->statusStrip->ImageScalingSize = System::Drawing::Size( 20, 20 );
         this->statusStrip->Items->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 3 ) {
            this->toolStripStatusLabel,
               this->tsCancel, this->tsProgressBar
         } );
         this->statusStrip->Location = System::Drawing::Point( 0, 325 );
         this->statusStrip->Name = L"statusStrip";
         this->statusStrip->Size = System::Drawing::Size( 610, 26 );
         this->statusStrip->SizingGrip = false;
         this->statusStrip->TabIndex = 11;
         // 
         // toolStripStatusLabel
         // 
         this->toolStripStatusLabel->BackColor = System::Drawing::Color::Transparent;
         this->toolStripStatusLabel->Name = L"toolStripStatusLabel";
         this->toolStripStatusLabel->Size = System::Drawing::Size( 327, 21 );
         this->toolStripStatusLabel->Spring = true;
         this->toolStripStatusLabel->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
         // 
         // tsCancel
         // 
         this->tsCancel->BackColor = System::Drawing::Color::LightGray;
         this->tsCancel->DisplayStyle = System::Windows::Forms::ToolStripItemDisplayStyle::Image;
         this->tsCancel->DropDownItems->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 1 ) { this->tsiCancel } );
         this->tsCancel->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"tsCancel.Image" )));
         this->tsCancel->ImageTransparentColor = System::Drawing::Color::Magenta;
         this->tsCancel->Name = L"tsCancel";
         this->tsCancel->Size = System::Drawing::Size( 36, 24 );
         this->tsCancel->Text = L"Options";
         this->tsCancel->ButtonClick += gcnew System::EventHandler( this, &Form1::tsCancel_ButtonClick );
         // 
         // tsiCancel
         // 
         this->tsiCancel->Font = (gcnew System::Drawing::Font( L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tsiCancel->Image = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"tsiCancel.Image" )));
         this->tsiCancel->Name = L"tsiCancel";
         this->tsiCancel->Size = System::Drawing::Size( 110, 22 );
         this->tsiCancel->Text = L"Cancel";
         this->tsiCancel->Click += gcnew System::EventHandler( this, &Form1::tsiCancel_Click );
         // 
         // tsProgressBar
         // 
         this->tsProgressBar->Alignment = System::Windows::Forms::ToolStripItemAlignment::Right;
         this->tsProgressBar->Margin = System::Windows::Forms::Padding( 1, 3, 0, 3 );
         this->tsProgressBar->MarqueeAnimationSpeed = 1;
         this->tsProgressBar->Name = L"tsProgressBar";
         this->tsProgressBar->Size = System::Drawing::Size( 200, 20 );
         this->tsProgressBar->Style = System::Windows::Forms::ProgressBarStyle::Continuous;
         // 
         // tabControl
         // 
         this->tabControl->Controls->Add( this->tabSingle );
         this->tabControl->Controls->Add( this->tabMultiple );
         this->tabControl->Controls->Add( this->tabManual );
         this->tabControl->Font = (gcnew System::Drawing::Font( L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tabControl->Location = System::Drawing::Point( 0, 27 );
         this->tabControl->Name = L"tabControl";
         this->tabControl->SelectedIndex = 0;
         this->tabControl->Size = System::Drawing::Size( 613, 302 );
         this->tabControl->TabIndex = 12;
         // 
         // tabSingle
         // 
         this->tabSingle->AllowDrop = true;
         this->tabSingle->Controls->Add( this->btTabSingleVerify );
         this->tabSingle->Controls->Add( this->btCopySHA512 );
         this->tabSingle->Controls->Add( this->btCopySHA256 );
         this->tabSingle->Controls->Add( this->btCopySHA1 );
         this->tabSingle->Controls->Add( this->btCopyMD5 );
         this->tabSingle->Controls->Add( this->btTabSingleSaveAll );
         this->tabSingle->Controls->Add( this->tbSHA512 );
         this->tabSingle->Controls->Add( this->tbSHA256 );
         this->tabSingle->Controls->Add( this->tbSHA1 );
         this->tabSingle->Controls->Add( this->tbMD5 );
         this->tabSingle->Controls->Add( this->btTabSingleSHA512 );
         this->tabSingle->Controls->Add( this->btTabSingleSHA256 );
         this->tabSingle->Controls->Add( this->btTabSingleSHA1 );
         this->tabSingle->Controls->Add( this->btTabSingleMD5 );
         this->tabSingle->Controls->Add( this->tbTabSingleFilePath );
         this->tabSingle->Controls->Add( this->btTabSingleBrowse );
         this->tabSingle->Font = (gcnew System::Drawing::Font( L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tabSingle->Location = System::Drawing::Point( 4, 23 );
         this->tabSingle->Name = L"tabSingle";
         this->tabSingle->Padding = System::Windows::Forms::Padding( 3 );
         this->tabSingle->Size = System::Drawing::Size( 605, 275 );
         this->tabSingle->TabIndex = 0;
         this->tabSingle->Text = L"Single file";
         this->tabSingle->UseVisualStyleBackColor = true;
         this->tabSingle->DragDrop += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnDrop );
         this->tabSingle->DragEnter += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnEnter );
         // 
         // btTabSingleVerify
         // 
         this->btTabSingleVerify->BackColor = System::Drawing::Color::Transparent;
         this->btTabSingleVerify->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleVerify->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleVerify->Location = System::Drawing::Point( 9, 233 );
         this->btTabSingleVerify->Name = L"btTabSingleVerify";
         this->btTabSingleVerify->Size = System::Drawing::Size( 54, 23 );
         this->btTabSingleVerify->TabIndex = 10;
         this->btTabSingleVerify->Text = L"Verify";
         this->btTabSingleVerify->UseVisualStyleBackColor = false;
         this->btTabSingleVerify->Click += gcnew System::EventHandler( this, &Form1::btTabSingleVerify_Click );
         // 
         // btCopySHA512
         // 
         this->btCopySHA512->BackColor = System::Drawing::Color::White;
         this->btCopySHA512->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btCopySHA512.BackgroundImage" )));
         this->btCopySHA512->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btCopySHA512->Location = System::Drawing::Point( 571, 190 );
         this->btCopySHA512->Name = L"btCopySHA512";
         this->btCopySHA512->Size = System::Drawing::Size( 24, 24 );
         this->btCopySHA512->TabIndex = 9;
         this->btCopySHA512->UseVisualStyleBackColor = false;
         this->btCopySHA512->Click += gcnew System::EventHandler( this, &Form1::btCopySHA512_Click );
         // 
         // btCopySHA256
         // 
         this->btCopySHA256->BackColor = System::Drawing::Color::White;
         this->btCopySHA256->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btCopySHA256.BackgroundImage" )));
         this->btCopySHA256->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btCopySHA256->Location = System::Drawing::Point( 571, 164 );
         this->btCopySHA256->Name = L"btCopySHA256";
         this->btCopySHA256->Size = System::Drawing::Size( 24, 24 );
         this->btCopySHA256->TabIndex = 8;
         this->btCopySHA256->UseVisualStyleBackColor = false;
         this->btCopySHA256->Click += gcnew System::EventHandler( this, &Form1::btCopySHA256_Click );
         // 
         // btCopySHA1
         // 
         this->btCopySHA1->BackColor = System::Drawing::Color::White;
         this->btCopySHA1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btCopySHA1.BackgroundImage" )));
         this->btCopySHA1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btCopySHA1->Location = System::Drawing::Point( 571, 134 );
         this->btCopySHA1->Name = L"btCopySHA1";
         this->btCopySHA1->Size = System::Drawing::Size( 24, 24 );
         this->btCopySHA1->TabIndex = 7;
         this->btCopySHA1->UseVisualStyleBackColor = false;
         this->btCopySHA1->Click += gcnew System::EventHandler( this, &Form1::btCopySHA1_Click );
         // 
         // btCopyMD5
         // 
         this->btCopyMD5->BackColor = System::Drawing::Color::White;
         this->btCopyMD5->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btCopyMD5.BackgroundImage" )));
         this->btCopyMD5->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btCopyMD5->Location = System::Drawing::Point( 571, 104 );
         this->btCopyMD5->Name = L"btCopyMD5";
         this->btCopyMD5->Size = System::Drawing::Size( 24, 24 );
         this->btCopyMD5->TabIndex = 6;
         this->btCopyMD5->UseVisualStyleBackColor = false;
         this->btCopyMD5->Click += gcnew System::EventHandler( this, &Form1::btCopyMD5_Click );
         // 
         // btTabSingleSaveAll
         // 
         this->btTabSingleSaveAll->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabSingleSaveAll->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleSaveAll->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleSaveAll->Location = System::Drawing::Point( 236, 228 );
         this->btTabSingleSaveAll->Name = L"btTabSingleSaveAll";
         this->btTabSingleSaveAll->Size = System::Drawing::Size( 133, 32 );
         this->btTabSingleSaveAll->TabIndex = 11;
         this->btTabSingleSaveAll->Text = L"Save all to text file";
         this->btTabSingleSaveAll->UseVisualStyleBackColor = false;
         this->btTabSingleSaveAll->Click += gcnew System::EventHandler( this, &Form1::btTabSingleSaveAll_Click );
         // 
         // tbSHA512
         // 
         this->tbSHA512->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbSHA512->Location = System::Drawing::Point( 74, 193 );
         this->tbSHA512->Name = L"tbSHA512";
         this->tbSHA512->ReadOnly = true;
         this->tbSHA512->Size = System::Drawing::Size( 489, 22 );
         this->tbSHA512->TabIndex = 0;
         this->tbSHA512->TabStop = false;
         // 
         // tbSHA256
         // 
         this->tbSHA256->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbSHA256->Location = System::Drawing::Point( 74, 167 );
         this->tbSHA256->Name = L"tbSHA256";
         this->tbSHA256->ReadOnly = true;
         this->tbSHA256->Size = System::Drawing::Size( 489, 22 );
         this->tbSHA256->TabIndex = 0;
         this->tbSHA256->TabStop = false;
         // 
         // tbSHA1
         // 
         this->tbSHA1->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbSHA1->Location = System::Drawing::Point( 74, 138 );
         this->tbSHA1->Name = L"tbSHA1";
         this->tbSHA1->ReadOnly = true;
         this->tbSHA1->Size = System::Drawing::Size( 489, 22 );
         this->tbSHA1->TabIndex = 0;
         this->tbSHA1->TabStop = false;
         // 
         // tbMD5
         // 
         this->tbMD5->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbMD5->Location = System::Drawing::Point( 74, 107 );
         this->tbMD5->Name = L"tbMD5";
         this->tbMD5->ReadOnly = true;
         this->tbMD5->Size = System::Drawing::Size( 489, 22 );
         this->tbMD5->TabIndex = 0;
         this->tbMD5->TabStop = false;
         // 
         // btTabSingleSHA512
         // 
         this->btTabSingleSHA512->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabSingleSHA512->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleSHA512->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleSHA512->Location = System::Drawing::Point( 9, 193 );
         this->btTabSingleSHA512->Name = L"btTabSingleSHA512";
         this->btTabSingleSHA512->Size = System::Drawing::Size( 54, 23 );
         this->btTabSingleSHA512->TabIndex = 5;
         this->btTabSingleSHA512->Text = L"SHA-512";
         this->btTabSingleSHA512->UseVisualStyleBackColor = false;
         this->btTabSingleSHA512->Click += gcnew System::EventHandler( this, &Form1::btTabSingleSHA512_Click );
         // 
         // btTabSingleSHA256
         // 
         this->btTabSingleSHA256->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabSingleSHA256->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleSHA256->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleSHA256->Location = System::Drawing::Point( 9, 164 );
         this->btTabSingleSHA256->Name = L"btTabSingleSHA256";
         this->btTabSingleSHA256->Size = System::Drawing::Size( 54, 23 );
         this->btTabSingleSHA256->TabIndex = 4;
         this->btTabSingleSHA256->Text = L"SHA-256";
         this->btTabSingleSHA256->UseVisualStyleBackColor = false;
         this->btTabSingleSHA256->Click += gcnew System::EventHandler( this, &Form1::btTabSingleSHA256_Click );
         // 
         // btTabSingleSHA1
         // 
         this->btTabSingleSHA1->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabSingleSHA1->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleSHA1->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleSHA1->Location = System::Drawing::Point( 9, 135 );
         this->btTabSingleSHA1->Name = L"btTabSingleSHA1";
         this->btTabSingleSHA1->Size = System::Drawing::Size( 54, 23 );
         this->btTabSingleSHA1->TabIndex = 3;
         this->btTabSingleSHA1->Text = L"SHA-1";
         this->btTabSingleSHA1->UseVisualStyleBackColor = false;
         this->btTabSingleSHA1->Click += gcnew System::EventHandler( this, &Form1::btTabSingleSHA1_Click );
         // 
         // btTabSingleMD5
         // 
         this->btTabSingleMD5->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabSingleMD5->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleMD5->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleMD5->Location = System::Drawing::Point( 9, 106 );
         this->btTabSingleMD5->Name = L"btTabSingleMD5";
         this->btTabSingleMD5->Size = System::Drawing::Size( 54, 23 );
         this->btTabSingleMD5->TabIndex = 2;
         this->btTabSingleMD5->Text = L"MD5";
         this->btTabSingleMD5->UseVisualStyleBackColor = false;
         this->btTabSingleMD5->Click += gcnew System::EventHandler( this, &Form1::btTabSingleMD5_Click );
         // 
         // tbTabSingleFilePath
         // 
         this->tbTabSingleFilePath->AllowDrop = true;
         this->tbTabSingleFilePath->BackColor = System::Drawing::SystemColors::ActiveCaption;
         this->tbTabSingleFilePath->Enabled = false;
         this->tbTabSingleFilePath->Font = (gcnew System::Drawing::Font( L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbTabSingleFilePath->Location = System::Drawing::Point( 1, 58 );
         this->tbTabSingleFilePath->Name = L"tbTabSingleFilePath";
         this->tbTabSingleFilePath->ReadOnly = true;
         this->tbTabSingleFilePath->Size = System::Drawing::Size( 600, 23 );
         this->tbTabSingleFilePath->TabIndex = 0;
         this->tbTabSingleFilePath->TabStop = false;
         this->tbTabSingleFilePath->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         this->tbTabSingleFilePath->DragDrop += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnDrop );
         this->tbTabSingleFilePath->DragEnter += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnEnter );
         // 
         // btTabSingleBrowse
         // 
         this->btTabSingleBrowse->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabSingleBrowse->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabSingleBrowse->Location = System::Drawing::Point( 258, 16 );
         this->btTabSingleBrowse->Name = L"btTabSingleBrowse";
         this->btTabSingleBrowse->Size = System::Drawing::Size( 89, 33 );
         this->btTabSingleBrowse->TabIndex = 1;
         this->btTabSingleBrowse->Text = L"Browse File";
         this->btTabSingleBrowse->UseVisualStyleBackColor = true;
         this->btTabSingleBrowse->Click += gcnew System::EventHandler( this, &Form1::btTabSingleBrowse_Click );
         // 
         // tabMultiple
         // 
         this->tabMultiple->AllowDrop = true;
         this->tabMultiple->Controls->Add( this->btTabMultipleOpenNotepad );
         this->tabMultiple->Controls->Add( this->btTabMultipleSaveAs );
         this->tabMultiple->Controls->Add( this->btTabMutipleCalculate );
         this->tabMultiple->Controls->Add( this->btTabMultipleCopy );
         this->tabMultiple->Controls->Add( this->tbTabMultipleFilePath );
         this->tabMultiple->Controls->Add( this->labelTabMultipleChoose );
         this->tabMultiple->Controls->Add( this->cbTabMultiple );
         this->tabMultiple->Controls->Add( this->rtbTabMultiple );
         this->tabMultiple->Controls->Add( this->btTabMutipleBrowse );
         this->tabMultiple->Font = (gcnew System::Drawing::Font( L"Segoe UI", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tabMultiple->Location = System::Drawing::Point( 4, 23 );
         this->tabMultiple->Name = L"tabMultiple";
         this->tabMultiple->Padding = System::Windows::Forms::Padding( 3 );
         this->tabMultiple->Size = System::Drawing::Size( 605, 275 );
         this->tabMultiple->TabIndex = 1;
         this->tabMultiple->Text = L"Multiple files";
         this->tabMultiple->UseVisualStyleBackColor = true;
         this->tabMultiple->DragDrop += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnDrop );
         this->tabMultiple->DragEnter += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnEnter );
         // 
         // btTabMultipleOpenNotepad
         // 
         this->btTabMultipleOpenNotepad->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabMultipleOpenNotepad->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabMultipleOpenNotepad->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabMultipleOpenNotepad->Location = System::Drawing::Point( 515, 227 );
         this->btTabMultipleOpenNotepad->Name = L"btTabMultipleOpenNotepad";
         this->btTabMultipleOpenNotepad->Size = System::Drawing::Size( 76, 41 );
         this->btTabMultipleOpenNotepad->TabIndex = 6;
         this->btTabMultipleOpenNotepad->Text = L"Open in notepad";
         this->btTabMultipleOpenNotepad->UseVisualStyleBackColor = false;
         this->btTabMultipleOpenNotepad->Click += gcnew System::EventHandler( this, &Form1::btTabMultipleOpenNotepad_Click );
         // 
         // btTabMultipleSaveAs
         // 
         this->btTabMultipleSaveAs->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabMultipleSaveAs->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabMultipleSaveAs->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabMultipleSaveAs->Location = System::Drawing::Point( 515, 181 );
         this->btTabMultipleSaveAs->Name = L"btTabMultipleSaveAs";
         this->btTabMultipleSaveAs->Size = System::Drawing::Size( 76, 41 );
         this->btTabMultipleSaveAs->TabIndex = 5;
         this->btTabMultipleSaveAs->Text = L"Save as text file";
         this->btTabMultipleSaveAs->UseVisualStyleBackColor = false;
         this->btTabMultipleSaveAs->Click += gcnew System::EventHandler( this, &Form1::btTabMultipleSaveAs_Click );
         // 
         // btTabMutipleCalculate
         // 
         this->btTabMutipleCalculate->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabMutipleCalculate->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabMutipleCalculate->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabMutipleCalculate->Location = System::Drawing::Point( 258, 91 );
         this->btTabMutipleCalculate->Name = L"btTabMutipleCalculate";
         this->btTabMutipleCalculate->Size = System::Drawing::Size( 89, 33 );
         this->btTabMutipleCalculate->TabIndex = 3;
         this->btTabMutipleCalculate->Text = L"Calculate";
         this->btTabMutipleCalculate->UseVisualStyleBackColor = false;
         this->btTabMutipleCalculate->Click += gcnew System::EventHandler( this, &Form1::btTabMutipleCalculate_Click );
         // 
         // btTabMultipleCopy
         // 
         this->btTabMultipleCopy->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabMultipleCopy->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabMultipleCopy->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabMultipleCopy->Location = System::Drawing::Point( 515, 136 );
         this->btTabMultipleCopy->Name = L"btTabMultipleCopy";
         this->btTabMultipleCopy->Size = System::Drawing::Size( 76, 41 );
         this->btTabMultipleCopy->TabIndex = 4;
         this->btTabMultipleCopy->Text = L"Copy to clipboard";
         this->btTabMultipleCopy->UseVisualStyleBackColor = false;
         this->btTabMultipleCopy->Click += gcnew System::EventHandler( this, &Form1::btTabMultipleCopy_Click );
         // 
         // tbTabMultipleFilePath
         // 
         this->tbTabMultipleFilePath->BackColor = System::Drawing::SystemColors::ActiveCaption;
         this->tbTabMultipleFilePath->Enabled = false;
         this->tbTabMultipleFilePath->Font = (gcnew System::Drawing::Font( L"Consolas", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbTabMultipleFilePath->Location = System::Drawing::Point( 1, 58 );
         this->tbTabMultipleFilePath->Name = L"tbTabMultipleFilePath";
         this->tbTabMultipleFilePath->ReadOnly = true;
         this->tbTabMultipleFilePath->Size = System::Drawing::Size( 600, 23 );
         this->tbTabMultipleFilePath->TabIndex = 21;
         this->tbTabMultipleFilePath->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
         // 
         // labelTabMultipleChoose
         // 
         this->labelTabMultipleChoose->AutoSize = true;
         this->labelTabMultipleChoose->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->labelTabMultipleChoose->Location = System::Drawing::Point( 42, 100 );
         this->labelTabMultipleChoose->Name = L"labelTabMultipleChoose";
         this->labelTabMultipleChoose->Size = System::Drawing::Size( 79, 15 );
         this->labelTabMultipleChoose->TabIndex = 20;
         this->labelTabMultipleChoose->Text = L"Choose hash:";
         // 
         // cbTabMultiple
         // 
         this->cbTabMultiple->BackColor = System::Drawing::Color::Silver;
         this->cbTabMultiple->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
         this->cbTabMultiple->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
         this->cbTabMultiple->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->cbTabMultiple->FormattingEnabled = true;
         this->cbTabMultiple->Items->AddRange( gcnew cli::array< System::Object^  >( 6 ) {
            L"MD5", L"SHA-1", L"SHA-224", L"SHA-256", L"SHA-384",
               L"SHA-512"
         } );
         this->cbTabMultiple->Location = System::Drawing::Point( 127, 99 );
         this->cbTabMultiple->Name = L"cbTabMultiple";
         this->cbTabMultiple->Size = System::Drawing::Size( 85, 21 );
         this->cbTabMultiple->TabIndex = 2;
         // 
         // rtbTabMultiple
         // 
         this->rtbTabMultiple->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->rtbTabMultiple->Location = System::Drawing::Point( 8, 136 );
         this->rtbTabMultiple->Name = L"rtbTabMultiple";
         this->rtbTabMultiple->ReadOnly = true;
         this->rtbTabMultiple->Size = System::Drawing::Size( 496, 132 );
         this->rtbTabMultiple->TabIndex = 7;
         this->rtbTabMultiple->TabStop = false;
         this->rtbTabMultiple->Text = L"";
         // 
         // btTabMutipleBrowse
         // 
         this->btTabMutipleBrowse->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabMutipleBrowse->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabMutipleBrowse->Location = System::Drawing::Point( 258, 16 );
         this->btTabMutipleBrowse->Name = L"btTabMutipleBrowse";
         this->btTabMutipleBrowse->Size = System::Drawing::Size( 89, 33 );
         this->btTabMutipleBrowse->TabIndex = 1;
         this->btTabMutipleBrowse->Text = L"Browse Files";
         this->btTabMutipleBrowse->UseVisualStyleBackColor = true;
         this->btTabMutipleBrowse->Click += gcnew System::EventHandler( this, &Form1::btTabMutipleBrowse_Click );
         // 
         // tabManual
         // 
         this->tabManual->Controls->Add( this->checkBoxTabManual );
         this->tabManual->Controls->Add( this->label1 );
         this->tabManual->Controls->Add( this->lbTabManualBytes );
         this->tabManual->Controls->Add( this->rtbTabManualInput );
         this->tabManual->Controls->Add( this->btTabManualCopy );
         this->tabManual->Controls->Add( this->labelTabManualChoose );
         this->tabManual->Controls->Add( this->cbTabManual );
         this->tabManual->Controls->Add( this->labelTabManualResult );
         this->tabManual->Controls->Add( this->tbTabManualResult );
         this->tabManual->Controls->Add( this->btTabManualCalculate );
         this->tabManual->Location = System::Drawing::Point( 4, 23 );
         this->tabManual->Name = L"tabManual";
         this->tabManual->Padding = System::Windows::Forms::Padding( 3 );
         this->tabManual->Size = System::Drawing::Size( 605, 275 );
         this->tabManual->TabIndex = 2;
         this->tabManual->Text = L"Manual input";
         this->tabManual->UseVisualStyleBackColor = true;
         // 
         // checkBoxTabManual
         // 
         this->checkBoxTabManual->AutoSize = true;
         this->checkBoxTabManual->Checked = true;
         this->checkBoxTabManual->CheckState = System::Windows::Forms::CheckState::Checked;
         this->checkBoxTabManual->Location = System::Drawing::Point( 20, 137 );
         this->checkBoxTabManual->Name = L"checkBoxTabManual";
         this->checkBoxTabManual->Size = System::Drawing::Size( 143, 18 );
         this->checkBoxTabManual->TabIndex = 3;
         this->checkBoxTabManual->Text = L"Automatic calculation";
         this->checkBoxTabManual->UseVisualStyleBackColor = true;
         this->checkBoxTabManual->CheckedChanged += gcnew System::EventHandler( this, &Form1::checkBoxTabManual_CheckedChanged );
         // 
         // label1
         // 
         this->label1->AutoSize = true;
         this->label1->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->label1->Location = System::Drawing::Point( 101, 6 );
         this->label1->Name = L"label1";
         this->label1->Size = System::Drawing::Size( 405, 15 );
         this->label1->TabIndex = 28;
         this->label1->Text = L"Input bytes in ASC representation of hexadecimal (2 characters per byte):";
         // 
         // lbTabManualBytes
         // 
         this->lbTabManualBytes->AutoSize = true;
         this->lbTabManualBytes->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->lbTabManualBytes->ForeColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)) );
         this->lbTabManualBytes->Location = System::Drawing::Point( 562, 137 );
         this->lbTabManualBytes->Name = L"lbTabManualBytes";
         this->lbTabManualBytes->Size = System::Drawing::Size( 14, 14 );
         this->lbTabManualBytes->TabIndex = 27;
         this->lbTabManualBytes->Text = L"0";
         // 
         // rtbTabManualInput
         // 
         this->rtbTabManualInput->ContextMenuStrip = this->ctxMenu;
         this->rtbTabManualInput->DetectUrls = false;
         this->rtbTabManualInput->Font = (gcnew System::Drawing::Font( L"Consolas", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->rtbTabManualInput->Location = System::Drawing::Point( 6, 24 );
         this->rtbTabManualInput->MaxLength = 6143;
         this->rtbTabManualInput->Name = L"rtbTabManualInput";
         this->rtbTabManualInput->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::ForcedVertical;
         this->rtbTabManualInput->Size = System::Drawing::Size( 595, 105 );
         this->rtbTabManualInput->TabIndex = 1;
         this->rtbTabManualInput->Text = L"";
         this->rtbTabManualInput->TextChanged += gcnew System::EventHandler( this, &Form1::TabManualTextChanged );
         this->rtbTabManualInput->KeyDown += gcnew System::Windows::Forms::KeyEventHandler( this, &Form1::TabManualKeyDown );
         this->rtbTabManualInput->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler( this, &Form1::TabManualKeyPressed );
         this->rtbTabManualInput->KeyUp += gcnew System::Windows::Forms::KeyEventHandler( this, &Form1::TabManualKeyUp );
         // 
         // ctxMenu
         // 
         this->ctxMenu->Items->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 2 ) { this->ctxMenuClear, this->ctxMenuPaste } );
         this->ctxMenu->Name = L"ctxMenu";
         this->ctxMenu->Size = System::Drawing::Size( 103, 48 );
         // 
         // ctxMenuClear
         // 
         this->ctxMenuClear->Name = L"ctxMenuClear";
         this->ctxMenuClear->Size = System::Drawing::Size( 102, 22 );
         this->ctxMenuClear->Text = L"Clear";
         this->ctxMenuClear->Click += gcnew System::EventHandler( this, &Form1::ctxMenuClear_Click );
         // 
         // ctxMenuPaste
         // 
         this->ctxMenuPaste->Name = L"ctxMenuPaste";
         this->ctxMenuPaste->Size = System::Drawing::Size( 102, 22 );
         this->ctxMenuPaste->Text = L"Paste";
         this->ctxMenuPaste->Click += gcnew System::EventHandler( this, &Form1::ctxMenuPaste_Click );
         // 
         // btTabManualCopy
         // 
         this->btTabManualCopy->BackColor = System::Drawing::Color::White;
         this->btTabManualCopy->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btTabManualCopy.BackgroundImage" )));
         this->btTabManualCopy->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btTabManualCopy->Location = System::Drawing::Point( 566, 224 );
         this->btTabManualCopy->Name = L"btTabManualCopy";
         this->btTabManualCopy->Size = System::Drawing::Size( 24, 24 );
         this->btTabManualCopy->TabIndex = 5;
         this->btTabManualCopy->UseVisualStyleBackColor = false;
         this->btTabManualCopy->Click += gcnew System::EventHandler( this, &Form1::btTabManualCopy_Click );
         // 
         // labelTabManualChoose
         // 
         this->labelTabManualChoose->AutoSize = true;
         this->labelTabManualChoose->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->labelTabManualChoose->Location = System::Drawing::Point( 175, 137 );
         this->labelTabManualChoose->Name = L"labelTabManualChoose";
         this->labelTabManualChoose->Size = System::Drawing::Size( 79, 15 );
         this->labelTabManualChoose->TabIndex = 22;
         this->labelTabManualChoose->Text = L"Choose hash:";
         // 
         // cbTabManual
         // 
         this->cbTabManual->BackColor = System::Drawing::Color::Silver;
         this->cbTabManual->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
         this->cbTabManual->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
         this->cbTabManual->Font = (gcnew System::Drawing::Font( L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->cbTabManual->FormattingEnabled = true;
         this->cbTabManual->Items->AddRange( gcnew cli::array< System::Object^  >( 6 ) {
            L"MD5", L"SHA-1", L"SHA-224", L"SHA-256", L"SHA-384",
               L"SHA-512"
         } );
         this->cbTabManual->Location = System::Drawing::Point( 260, 135 );
         this->cbTabManual->Name = L"cbTabManual";
         this->cbTabManual->Size = System::Drawing::Size( 85, 21 );
         this->cbTabManual->TabIndex = 2;
         this->cbTabManual->SelectedIndexChanged += gcnew System::EventHandler( this, &Form1::cbTabManual_SelectedIndexChanged );
         // 
         // labelTabManualResult
         // 
         this->labelTabManualResult->AutoSize = true;
         this->labelTabManualResult->Location = System::Drawing::Point( 13, 229 );
         this->labelTabManualResult->Name = L"labelTabManualResult";
         this->labelTabManualResult->Size = System::Drawing::Size( 45, 14 );
         this->labelTabManualResult->TabIndex = 18;
         this->labelTabManualResult->Text = L"Result:";
         // 
         // tbTabManualResult
         // 
         this->tbTabManualResult->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbTabManualResult->Location = System::Drawing::Point( 64, 227 );
         this->tbTabManualResult->Name = L"tbTabManualResult";
         this->tbTabManualResult->ReadOnly = true;
         this->tbTabManualResult->Size = System::Drawing::Size( 496, 22 );
         this->tbTabManualResult->TabIndex = 17;
         this->tbTabManualResult->TabStop = false;
         // 
         // btTabManualCalculate
         // 
         this->btTabManualCalculate->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(192)) );
         this->btTabManualCalculate->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btTabManualCalculate->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btTabManualCalculate->Location = System::Drawing::Point( 258, 174 );
         this->btTabManualCalculate->Name = L"btTabManualCalculate";
         this->btTabManualCalculate->Size = System::Drawing::Size( 89, 33 );
         this->btTabManualCalculate->TabIndex = 4;
         this->btTabManualCalculate->Text = L"Calculate";
         this->btTabManualCalculate->UseVisualStyleBackColor = false;
         this->btTabManualCalculate->Click += gcnew System::EventHandler( this, &Form1::btTabManualCalculate_Click );
         // 
         // aboutToolStripMenuItem
         // 
         this->aboutToolStripMenuItem->DropDownItems->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 3 ) {
            this->aboutToolStripMenuItem1,
               this->donateToolStripMenuItem, this->websiteToolStripMenuItem
         } );
         this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
         this->aboutToolStripMenuItem->Size = System::Drawing::Size( 44, 20 );
         this->aboutToolStripMenuItem->Text = L"Help";
         // 
         // aboutToolStripMenuItem1
         // 
         this->aboutToolStripMenuItem1->Name = L"aboutToolStripMenuItem1";
         this->aboutToolStripMenuItem1->ShortcutKeys = System::Windows::Forms::Keys::F12;
         this->aboutToolStripMenuItem1->ShowShortcutKeys = false;
         this->aboutToolStripMenuItem1->Size = System::Drawing::Size( 112, 22 );
         this->aboutToolStripMenuItem1->Text = L"About";
         this->aboutToolStripMenuItem1->Click += gcnew System::EventHandler( this, &Form1::aboutToolStripMenuItem1_Click );
         // 
         // donateToolStripMenuItem
         // 
         this->donateToolStripMenuItem->Name = L"donateToolStripMenuItem";
         this->donateToolStripMenuItem->Size = System::Drawing::Size( 112, 22 );
         this->donateToolStripMenuItem->Text = L"Donate";
         this->donateToolStripMenuItem->Click += gcnew System::EventHandler( this, &Form1::donateToolStripMenuItem_Click );
         // 
         // websiteToolStripMenuItem
         // 
         this->websiteToolStripMenuItem->Name = L"websiteToolStripMenuItem";
         this->websiteToolStripMenuItem->ShortcutKeys = System::Windows::Forms::Keys::F10;
         this->websiteToolStripMenuItem->ShowShortcutKeys = false;
         this->websiteToolStripMenuItem->Size = System::Drawing::Size( 112, 22 );
         this->websiteToolStripMenuItem->Text = L"Website";
         this->websiteToolStripMenuItem->Click += gcnew System::EventHandler( this, &Form1::websiteToolStripMenuItem_Click );
         // 
         // menuStrip1
         // 
         this->menuStrip1->ImageScalingSize = System::Drawing::Size( 20, 20 );
         this->menuStrip1->Items->AddRange( gcnew cli::array< System::Windows::Forms::ToolStripItem^  >( 1 ) { this->aboutToolStripMenuItem } );
         this->menuStrip1->Location = System::Drawing::Point( 0, 0 );
         this->menuStrip1->Name = L"menuStrip1";
         this->menuStrip1->Size = System::Drawing::Size( 610, 24 );
         this->menuStrip1->TabIndex = 10;
         this->menuStrip1->Text = L"menuStrip1";
         // 
         // TabMultipleOpenFileDialog
         // 
         this->TabMultipleOpenFileDialog->Multiselect = true;
         this->TabMultipleOpenFileDialog->Title = L"Select files to hash";
         // 
         // bgWorker
         // 
         this->bgWorker->WorkerReportsProgress = true;
         this->bgWorker->WorkerSupportsCancellation = true;
         this->bgWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler( this, &Form1::bgWorker_DoWork );
         this->bgWorker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler( this, &Form1::bgWorker_ProgressChanged );
         this->bgWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler( this, &Form1::bgWorker_RunWorkerCompleted );
         // 
         // Form1
         // 
         this->AllowDrop = true;
         this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->BackColor = System::Drawing::Color::White;
         this->ClientSize = System::Drawing::Size( 610, 351 );
         this->Controls->Add( this->tabControl );
         this->Controls->Add( this->statusStrip );
         this->Controls->Add( this->menuStrip1 );
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject( L"$this.Icon" )));
         this->MainMenuStrip = this->menuStrip1;
         this->MaximizeBox = false;
         this->Name = L"Form1";
         this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
         this->Text = L"#ashing";
         this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler( this, &Form1::Form_Closing );
         this->DragDrop += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnDrop );
         this->DragEnter += gcnew System::Windows::Forms::DragEventHandler( this, &Form1::Drag_OnEnter );
         this->statusStrip->ResumeLayout( false );
         this->statusStrip->PerformLayout();
         this->tabControl->ResumeLayout( false );
         this->tabSingle->ResumeLayout( false );
         this->tabSingle->PerformLayout();
         this->tabMultiple->ResumeLayout( false );
         this->tabMultiple->PerformLayout();
         this->tabManual->ResumeLayout( false );
         this->tabManual->PerformLayout();
         this->ctxMenu->ResumeLayout( false );
         this->menuStrip1->ResumeLayout( false );
         this->menuStrip1->PerformLayout();
         this->ResumeLayout( false );
         this->PerformLayout();

      }
#pragma endregion


// ----------------------------------------------------------------------------------
// Tab 1: Single File View
// ----------------------------------------------------------------------------------
private: System::Void btTabSingleBrowse_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabSingleOpenFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
   {
      // Clear all the boxes
      tbMD5->Clear();
      tbSHA1->Clear();
      tbSHA256->Clear();
      tbSHA512->Clear();

      // Clear the progress bar
      tsProgressBar->Value = 0;

      SetTabSingleFilePath(TabSingleOpenFileDialog->FileName);
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void btTabSingleMD5_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabSingleOpenFileDialog->FileName != "" )
   {
      InputArguments^ args = gcnew InputArguments();
      args->alg = EAlgorithm::MD5;
      args->tab = ETab::SINGLE_FILE;
      args->L_Files = gcnew List<String^>();
      args->L_Files->Add(TabSingleOpenFileDialog->FileName);

      SetStatusLabel("Performing MD5...");
      tbMD5->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }
   else
   {
      SetStatusLabel("No file selected");
   }
}
private: System::Void btTabSingleSHA1_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabSingleOpenFileDialog->FileName != "" )
   {
      InputArguments^ args = gcnew InputArguments();
      args->alg = EAlgorithm::SHA1;
      args->tab = ETab::SINGLE_FILE;
      args->L_Files = gcnew List<String^>();
      args->L_Files->Add(TabSingleOpenFileDialog->FileName);

      SetStatusLabel("Performing SHA1...");
      tbSHA1->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }
   else
   {
      SetStatusLabel("No file selected");
   }
}
private: System::Void btTabSingleSHA256_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabSingleOpenFileDialog->FileName != "" )
   {
      InputArguments^ args = gcnew InputArguments();
      args->alg = EAlgorithm::SHA256;
      args->tab = ETab::SINGLE_FILE;
      args->L_Files = gcnew List<String^>();
      args->L_Files->Add(TabSingleOpenFileDialog->FileName);

      SetStatusLabel("Performing SHA256...");
      tbSHA256->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }
   else
   {
      SetStatusLabel("No file selected");
   }
}
private: System::Void btTabSingleSHA512_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabSingleOpenFileDialog->FileName != "" )
   {
      InputArguments^ args = gcnew InputArguments();
      args->alg = EAlgorithm::SHA512;
      args->tab = ETab::SINGLE_FILE;
      args->L_Files = gcnew List<String^>();
      args->L_Files->Add(TabSingleOpenFileDialog->FileName);

      SetStatusLabel("Performing SHA512...");
      tbSHA512->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }
   else
   {
      SetStatusLabel("No file selected");
   }
}
private: System::Void btTabSingleSaveAll_Click(System::Object^  sender, System::EventArgs^  e)
{
   SaveFileDialog->FileName = "";
   SaveFileDialog->Filter   = "Text files|*.txt";

   if ( tbMD5->Text == "" && tbSHA1->Text == "" && tbSHA256->Text == "" && tbSHA512->Text == "" )
   {
      SetStatusLabel("No data to save. Perform a hash operation");
      return;
   }

   if( SaveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
   {
      StreamWriter^ fs = gcnew StreamWriter( SaveFileDialog->FileName );

      fs->Write( "File    : "    + Path::GetFileName(TabSingleOpenFileDialog->FileName)      + "\r\n");
      fs->Write( "Path    : "    + Path::GetDirectoryName(TabSingleOpenFileDialog->FileName) + "\r\n" );

      if ( tbMD5->Text != "" )
      {
         fs->Write( "MD5     : "    + tbMD5->Text                                      + "\r\n" );
      }

      if ( tbSHA1->Text != "" )
      {
         fs->Write( "SHA-1   : "    + tbSHA1->Text                                     + "\r\n");
      }

      if ( tbSHA256->Text != "" )
      {
         fs->Write( "SHA-256 : "    + tbSHA256->Text                                   + "\r\n");
      }

      if ( tbSHA512->Text != "" )
      {
         fs->Write( "SHA-512 : "    + tbSHA512->Text                                   + "\r\n");
      }

      fs->Close( );
   }
}
private: System::Void btCopyMD5_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( tbMD5->Text != "" )
   {
      try
      {
         Clipboard::SetDataObject(tbMD5->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         e;
         SetStatusLabel("Failed copying to clipboard, please try again");
      }
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void btCopySHA1_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( tbSHA1->Text != "" )
   {
      try
      {
         Clipboard::SetDataObject(tbSHA1->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         e;
         SetStatusLabel("Failed copying to clipboard, please try again");
      }
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void btCopySHA256_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( tbSHA256->Text != "" )
   {
      try
      {
         Clipboard::SetDataObject(tbSHA256->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         e;
         SetStatusLabel("Failed copying to clipboard, please try again");
      }
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void btCopySHA512_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( tbSHA512->Text != "" )
   {
      try
      {
         Clipboard::SetDataObject(tbSHA512->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         e;
         SetStatusLabel("Failed copying to clipboard, please try again");
      }
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void btTabSingleVerify_Click(System::Object^  sender, System::EventArgs^  e)
{
   FormVerify fv;

   if ( tbMD5->Text == "" && tbSHA1->Text == "" && tbSHA256->Text == "" && tbSHA512->Text == "" )
   {
      SetStatusLabel("Nothing to verify against. Perform a hash operation first");
      return;
   }

   if ( fv.ShowDialog() == System::Windows::Forms::DialogResult::OK)
   {
      String^ strHashToVerify = fv.getHash()->ToUpper();

      // Just to be sure
      if ( strHashToVerify == "" )
      {
         return;
      }

      if ( strHashToVerify == tbMD5->Text )
      {
         FormMessageBox mfb("Match!", "Matches with MD5", FormMessageBox::HashingGreen );
         mfb.ShowDialog();
      }
      else if ( strHashToVerify == tbSHA1->Text )
      {
         FormMessageBox mfb("Match!", "Matches with SHA1", FormMessageBox::HashingGreen );
         mfb.ShowDialog();
      }
      else if ( strHashToVerify == tbSHA256->Text )
      {
         FormMessageBox mfb("Match!", "Matches with SHA256", FormMessageBox::HashingGreen );
         mfb.ShowDialog();
      }
      else if ( strHashToVerify == tbSHA512->Text )
      {
         FormMessageBox mfb("Match!", "Matches with SHA512", FormMessageBox::HashingGreen);
         mfb.ShowDialog();
      }
      else
      {
         FormMessageBox mfb("No match", "The hash you entered did not match any of the calculated hashes", FormMessageBox::HashingRed);
         mfb.ShowDialog();
      }
      SetStatusLabel("");
   }
   else
   {
      SetStatusLabel("");
   }
}

// ----------------------------------------------------------------------------------
// Tab 2: Multiple Files View
// ----------------------------------------------------------------------------------
private: System::Void btTabMutipleBrowse_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( TabMultipleOpenFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
   {
      if ( TabMultipleOpenFileDialog->FileNames->Length == 1 )
      {
         tbTabMultipleFilePath->Text = TabMultipleOpenFileDialog->FileNames->Length + " file selected";
      }
      else
      {
         tbTabMultipleFilePath->Text = TabMultipleOpenFileDialog->FileNames->Length + " files selected";
      }

      // Clear previous results
      rtbTabMultiple->Clear();

      // Clear the progress bar
      tsProgressBar->Value = 0;

      // The selection of anything in this file dialog cancels whatever was in the FileDialog tag
      // i.e., if the program was started with input arguments, lose it here
      TabMultipleOpenFileDialog->Tag = nullptr;
   }
}
private: System::Void btTabMutipleCalculate_Click(System::Object^  sender, System::EventArgs^  e)
{
   InputArguments^ args = gcnew InputArguments();
   args->tab = ETab::MULTIPLE_FILES;
   args->L_Files = gcnew List<String^>();

   // Verificar qual hash o usuario selecionou
   if ( cbTabMultiple->SelectedIndex == 0 )
   {
      args->alg = EAlgorithm::MD5;
   }
   else if ( cbTabMultiple->SelectedIndex == 1 )
   {
      args->alg = EAlgorithm::SHA1;
   }
   else if ( cbTabMultiple->SelectedIndex == 2 )
   {
      args->alg = EAlgorithm::SHA224;
   }
   else if ( cbTabMultiple->SelectedIndex == 3 )
   {
      args->alg = EAlgorithm::SHA256;
   }
   else if ( cbTabMultiple->SelectedIndex == 4 )
   {
      args->alg = EAlgorithm::SHA384;
   }
   else if ( cbTabMultiple->SelectedIndex == 5 )
   {
      args->alg = EAlgorithm::SHA512;
   }
   else
   {
      // Error
      SetStatusLabel("Please, choose a hash function");
      return;
   }

   if ( TabMultipleOpenFileDialog->Tag != nullptr )
   {
      // Temporary list
      List<String^>^ L_temp;
      L_temp = dynamic_cast<List<String^>^> (TabMultipleOpenFileDialog->Tag);
      if ( L_temp == nullptr)
      {
         SetStatusLabel("There was a problem retrieving the files. Please, try again");
         return;
      }

      // Adicionar os arquivos na struct
      for each (String^ file in L_temp)
      {
         args->L_Files->Add(file);
      }

      SetStatusLabel("Hash multiple files");
      rtbTabMultiple->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }
   else
   {
      // Adicionar os arquivos na struct
      for each (String^ file in TabMultipleOpenFileDialog->FileNames)
      {
         args->L_Files->Add(file);
      }

      if ( args->L_Files->Count == 0)
      {
         SetStatusLabel("No file selected");
         return;
      }

      SetStatusLabel("Hash multiple files");
      rtbTabMultiple->Clear();
      DisableAll();
      bgWorker->RunWorkerAsync(args);
   }


}
private: System::Void btTabMultipleCopy_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( rtbTabMultiple->Text != "")
   {
      try
      {
         Clipboard::SetDataObject(rtbTabMultiple->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         // MSDN: https://msdn.microsoft.com/en-us/library/ms158293.aspx
         // Adding data to the Clipboard can occasionally fail if the Clipboard is busy with another thread or application. 
         //This method is useful to work around this issue in environments with heavy Clipboard use. 
         e;
         SetStatusLabel("Failed copying to clipboard, please try again"); //SetStatusLabel( e->ToString() );
      }
   }
}
private: System::Void btTabMultipleSaveAs_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( rtbTabMultiple->Text != "")
   {
      SaveFileDialog->FileName = "";
      SaveFileDialog->Filter   = "Text files|*.txt";

      if( SaveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK )
      {
         StreamWriter^ fs = gcnew StreamWriter( SaveFileDialog->FileName );

         // StreamWriter precisa receber \r\n ou Environment::NewLine, apenas \n nao funciona...
         fs->Write("Generated at " + DateTime::Now + Environment::NewLine + Environment::NewLine);

         array<String^>^ astrLines;
         array<Char>^sep = gcnew array<Char>{0x0A};
         astrLines = rtbTabMultiple->Text->Split( sep, StringSplitOptions::None  );

         for each( String^ line in astrLines )
         {
            fs->Write( line + Environment::NewLine );
         }


         fs->Close();

         SetStatusLabel("Saved");
      }
      else
      {
         SetStatusLabel("Cancelled");
      }
   }
}
private: System::Void btTabMultipleOpenNotepad_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( rtbTabMultiple->Text != "")
   {
      try
      {
         // Cria um arquivo temporario e recebe o path para ele
         String^ strTmpFile = Path::GetTempFileName();

         // Escreve o conteudo da rich-text-box no arquivo temporario
         StreamWriter^ fs = gcnew StreamWriter( strTmpFile );

         array<String^>^ astrLines;
         array<Char>^sep = gcnew array<Char>{0x0A};
         astrLines = rtbTabMultiple->Text->Split( sep, StringSplitOptions::None ); // String::IsNullOrEmpty(s) // StringSplitOptions::RemoveEmptyEntries

         for each( String^ line in astrLines )
         {
            fs->Write( line + Environment::NewLine);
         }

         fs->Close();

         Process::Start( "notepad.exe", strTmpFile );

         //System::Threading::Thread::Sleep(250); 
         //File::Delete(strTmpFile); // opcao: guardar uma lista e deletar quando a aplicacao terminar

         L_TempFiles->Add(strTmpFile);

      }
      catch(IOException^ e)
      {
         SetStatusLabel( e->ToString() );
      }
   }
}


// ----------------------------------------------------------------------------------
// Tab 3: Manual input View
// ----------------------------------------------------------------------------------
private: System::Void btTabManualCalculate_Click(System::Object^  sender, System::EventArgs^  e)
{
   if ( rtbTabManualInput->Text->Length < 2 )
   {
      tbTabManualResult->Clear();
      SetStatusLabel("Null input");
   }
   else
   {
      if ( 2 == TabManualPerformHash() )
      {
         FormMessageBox mfb("Invalid input", "The input can only contain hexadecimal numbers: 0-9, a-f, A-F", FormMessageBox::HashingRed);
         mfb.ShowDialog();
      }
   }
}
private: System::Void btTabManualCopy_Click(System::Object^  sender, System::EventArgs^  e)
{
      

   if ( tbTabManualResult->Text != "" )
   {
      try
      {
         Clipboard::SetDataObject(tbTabManualResult->Text, true, 3, 100);
         SetStatusLabel("Copied!");
      }
      catch(Exception^ e)
      {
         e;
         SetStatusLabel("Failed copying to clipboard, please try again");
      }
   }
   else
   {
      SetStatusLabel("");
   }
}
private: System::Void HashFromMemory( BYTE* abyBufferToHash, int iBufferLen, EAlgorithm alg, BYTE* abyDigest )
{
   switch(alg)
   {
      case EAlgorithm::MD5:
      {
         MD5(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      case EAlgorithm::SHA1:
      {
         SHA1(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      case EAlgorithm::SHA224:
      {
         SHA224(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      case EAlgorithm::SHA256:
      {
         SHA256(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      case EAlgorithm::SHA384:
      {
         SHA384(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      case EAlgorithm::SHA512:
      {
         SHA512(abyBufferToHash, iBufferLen, abyDigest);
      }
      break;

      default:
      {

      }
      break;
   }
}
private: System::Void ctxMenuPaste_Click(System::Object^  sender, System::EventArgs^  e) 
{
   try
   {
      if ( Clipboard::GetDataObject()->GetDataPresent(DataFormats::Text))
      {
         rtbTabManualInput->Text += Clipboard::GetDataObject()->GetData(DataFormats::Text)->ToString();
      }
      else
      {
         SetStatusLabel("No text on clipboard to paste");
      }
   }
   catch(Exception^ e)
   {
      SetStatusLabel(e->ToString());
   }
}
private: System::Void ctxMenuClear_Click(System::Object^  sender, System::EventArgs^  e) 
{
   rtbTabManualInput->Text = "";
}
private: System::Void TabManualTextChanged(System::Object^  sender, System::EventArgs^  e) 
{
   // Check if text is valid
   String^ strAux = rtbTabManualInput->Text;
   if ( false == hasInvalidData(strAux) )
   {
      rtbTabManualInput->Text = strLastValidData;
      rtbTabManualInput->SelectionStart  = rtbTabManualInput->Text->Length+1;
      return;
   }

   // Too large?
   

   // Save the caret position
   int  iCaretPos      =  rtbTabManualInput->SelectionStart;
   bool bCaretInTheEnd = (rtbTabManualInput->SelectionStart == rtbTabManualInput->Text->Length)? true : false;

   if ( bCaretInTheEnd == false)
   {
      if ( strAux[iCaretPos] == 0x20 )
      {
         iCaretPos++;
      }
   }

   // Format two chars followed by a space
   strAux = formatBytes(strAux);

   //Save this
   strLastValidData        = strAux;
   rtbTabManualInput->Text = strAux;

   // Update byte count
   int iLength = rtbTabManualInput->Text->Length;
   int iSpaces = rtbTabManualInput->Text->Split()->Length -1;
   lbTabManualBytes->Text = ((iLength - iSpaces)/2 ).ToString();

   // Move the caret to where it was
   if ( bCaretInTheEnd )
   {
      rtbTabManualInput->SelectionStart  = iLength+1;
   }
   else
   {
      rtbTabManualInput->SelectionStart  = iCaretPos;
   }
   
   // If 'automatic' is selected, perform the operation
   if ( checkBoxTabManual->Checked == true )
   {
      if ( iLength <= 1 )
      {
         // Nothing to hash
         tbTabManualResult->Clear();
         SetStatusLabel("");
      }
      else
      {
         if ( 0 != TabManualPerformHash() )
         {
            tbTabManualResult->Clear();
         }
      }
   }
}
private: System::Void TabManualKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{    
   // Handle only the paste action
   if (e->Control && e->KeyCode == Keys::V)
   {
      //String^ toPaste = ;
      if ( true )
      {
         ((RichTextBox^)sender)->Paste(DataFormats::GetFormat("Text"));
         e->Handled = true;
      }
   }
}
private: System::Void TabManualKeyPressed(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
{
   if ( !ValidateKey( e->KeyChar, true) )
   {
      e->Handled = true;
   }
}   
private: System::Void TabManualKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
   // Do not handle anything here
}
private: bool ValidateKey(int key, bool noSpace)
{
   if ( ( key == 0x0D ||  key == 0x0A  )
   ||   ( key >= 0x21 &&  key <= 0x24  )
   ||   ( key >= 0x25 &&  key <= 0x2F  )
   ||   ( key >= 0x3A &&  key <= 0x40  )
   ||   ( key >= 0x47 &&  key <= 0x60  )
   ||   ( key >= 0x67                  ) )
   {
      return false;
   }
   else if ( noSpace == true && key == 0x20 )
   {
      return false;
   }
   else
   {
      return true;
   }
   
}
private: int TabManualPerformHash()
{
   EAlgorithm alg;
   BYTE* byDigest;
   unsigned int  uiDigestLen;

   // Verificar qual hash o usuario selecionou
   if ( cbTabManual->SelectedIndex == 0 )
   {
      alg = EAlgorithm::MD5;
      uiDigestLen = 16;
      byDigest = new BYTE[uiDigestLen];
   }
   else if ( cbTabManual->SelectedIndex == 1 )
   {
      alg = EAlgorithm::SHA1;
      uiDigestLen = 20;
      byDigest = new BYTE[uiDigestLen];
   }
   else if ( cbTabManual->SelectedIndex == 2 )
   {
      alg = EAlgorithm::SHA224;
      uiDigestLen = 28;
      byDigest = new BYTE[uiDigestLen];
   }
   else if ( cbTabManual->SelectedIndex == 3 )
   {
      alg = EAlgorithm::SHA256;
      uiDigestLen = 32;
      byDigest = new BYTE[uiDigestLen];
   }
   else if ( cbTabManual->SelectedIndex == 4 )
   {
      alg = EAlgorithm::SHA384;
      uiDigestLen = 48;
      byDigest = new BYTE[uiDigestLen];
   }
   else if ( cbTabManual->SelectedIndex == 5 )
   {
      alg = EAlgorithm::SHA512;
      uiDigestLen = 64;
      byDigest = new BYTE[uiDigestLen];
   }
   else
   {
      // Error
      SetStatusLabel("Please, choose a hash function");
      return 1;
   }

   // Size ok?

   String^ strAux = rtbTabManualInput->Text->Replace(" ", "" );

   if ( 2048 < (strAux->Length)/2 )
   {
      FormMessageBox mfb("Input is too long", "For inputs bigger than 2048 bytes, please use the file view", FormMessageBox::HashingRed);
      mfb.ShowDialog();

      SetStatusLabel("Input too long"); //TODO: teoricamente nao passa mais aqui
      delete[] byDigest;
      return 3;
   }

   // Converter para hex
   BYTE abyBufferToHash[2048] = {0};
   int iLength = ASCII_to_Hex( rtbTabManualInput->Text, sizeof(abyBufferToHash), abyBufferToHash );
   if ( iLength < 0 )
   {
      SetStatusLabel("Invalid input"); //TODO: teoricamente nao passa mais aqui
      delete[] byDigest;
      return 2;
   }
   else if ( iLength > 2048 )
   {
      // Nao passa mais aqui!!! //TODO
      FormMessageBox mfb("Input is too long", "For inputs bigger than 2048 bytes, please use the file view", FormMessageBox::HashingRed);
      mfb.ShowDialog();

      SetStatusLabel("Input too long");
      delete[] byDigest;
      return 3;
   }

   // Calcule o hash
   HashFromMemory( abyBufferToHash, iLength, alg, byDigest );

   // Update in screen
   tbTabManualResult->Text = Hex_To_String(byDigest, uiDigestLen);
   SetStatusLabel("Done");
   tsProgressBar->Value = 0;
   delete[] byDigest;

   return 0;
}
private: System::Void checkBoxTabManual_CheckedChanged(System::Object^  sender, System::EventArgs^  e) 
{
   if ( checkBoxTabManual->Checked == true )
   {
      if ( rtbTabManualInput->Text->Length < 2 )
      {
         tbTabManualResult->Clear();
         SetStatusLabel("");
      }
      else
      {
         TabManualPerformHash();
      }
   }
}
private: System::Void cbTabManual_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) 
{
   // If 'automatic' is selected, perform the operation
   if ( checkBoxTabManual->Checked == true )
   {
      if ( rtbTabManualInput->Text->Length <= 1 )
      {
         // Nothing to hash
         tbTabManualResult->Clear();
         SetStatusLabel("");
      }
      else
      {
         if ( 0 != TabManualPerformHash() )
         {
            tbTabManualResult->Clear();
         }
      }
   }
}
private: bool hasInvalidData(String^ str)
{
   for each (char c in str)
   {
      if ( false == ValidateKey(c, false) )
      {
         return false;
      }
   }

   return true;
}
private: String^ formatBytes(String^ str)
{
   int i = 0;
   str = str->Replace(" ", "");
   StringBuilder^ builder = gcnew StringBuilder;
   for each (auto c in str)
   {
      builder->Append(c);
      if ( 0 == (++i % 2 ) && i != str->Length )
      {
         builder->Append(" ");
      }
   }

   return builder->ToString();
}

// ----------------------------------------------------------------------------------
// Background worker for single and mutiple file views
// ----------------------------------------------------------------------------------
private: System::Void bgWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
{
   BYTE abyDigest[64] = {0}; // a size of 64 fits all the hash types

   // Get the input arguments
   InputArguments^ input = (InputArguments^) e->Argument;

   // Prepare the output arguments
   OutputArguments^ output = gcnew OutputArguments();
   output->alg = input->alg;
   output->tab = input->tab;
   output->L_Files  = gcnew List<String^>();
   output->L_Hashes = gcnew List<String^>();

   // Progress
   Progress^ progress = gcnew Progress();
   progress->iNumberOfFiles = input->L_Files->Count;
   progress->iProcessing = 0;
   progress->tab = input->tab;

   // The trick: we sort aphabetically here
   input->L_Files->Sort();

   for each (String^ file in input->L_Files )
   {
      progress->iProcessing++;
      output->L_Files->Add(file);
      char* szFileToHash = (char*)(void*)Marshal::StringToHGlobalAnsi( file );

      FILE* fp = NULL;
      if ( 0 != fopen_s(&fp, szFileToHash, "rb") )
      {
         output->L_Hashes->Add( String::Format("Error ({0}) opening ", GetLastError() ) + Path::GetFileName(file));
         continue;
      }

      // Retrieve the size of the file
      _fseeki64(fp , 0, SEEK_END);
      unsigned long long ullSize = _ftelli64(fp);
      rewind(fp);

      unsigned long long ullProcessed  = 0;
      int  iRead = 0;

      switch(input->alg)
      {
         case EAlgorithm::MD5:
         {
            // Init the hash context
            MD5_CTX  ctx_md5;
            if ( 1 != MD5_Init(&ctx_md5) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal MD5_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != MD5_Update(&ctx_md5, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal MD5_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
               }
            }

            // Finalize the hash
            if ( 1 != MD5_Final(abyDigest, &ctx_md5) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal MD5_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 16) );
         }
         break;

         case EAlgorithm::SHA1:
         {
            // Init the hash context
            SHA_CTX  ctx_sha1;
            if ( 1 != SHA1_Init(&ctx_sha1) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA1_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != SHA1_Update(&ctx_sha1, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal SHA1_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
                  System::Threading::Thread::Sleep(1);
               }
            }

            // Finalize the hash
            if ( 1 != SHA1_Final(abyDigest, &ctx_sha1) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA1_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 20) );
         }
         break;

         case EAlgorithm::SHA224:
         {
            // Init the hash context (SHA224 uses SHA256_CTX)
            SHA256_CTX  ctx_sha224;
            if ( 1 != SHA224_Init(&ctx_sha224) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA224_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != SHA224_Update(&ctx_sha224, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal SHA224_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
                  System::Threading::Thread::Sleep(1);
               }
            }

            // Finalize the hash
            if ( 1 != SHA224_Final(abyDigest, &ctx_sha224) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA224_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 28) );
         }
         break;

         case EAlgorithm::SHA256:
         {
            // Init the hash context
            SHA256_CTX  ctx_sha256;
            if ( 1 != SHA256_Init(&ctx_sha256) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA256_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != SHA256_Update(&ctx_sha256, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal SHA256_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
                  System::Threading::Thread::Sleep(1);
               }
            }

            // Finalize the hash
            if ( 1 != SHA256_Final(abyDigest, &ctx_sha256) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA256_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 32) );
         }
         break;

         case EAlgorithm::SHA384:
         {
            // Init the hash context
            SHA512_CTX  ctx_sha384;
            if ( 1 != SHA384_Init(&ctx_sha384) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA384_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != SHA384_Update(&ctx_sha384, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal SHA384_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
                  System::Threading::Thread::Sleep(1);
               }
            }

            // Finalize the hash
            if ( 1 != SHA384_Final(abyDigest, &ctx_sha384) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA384_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 48) );
         }
         break;

         case EAlgorithm::SHA512:
         {
            // Init the hash context
            SHA512_CTX  ctx_sha512;
            if ( 1 != SHA512_Init(&ctx_sha512) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA512_Init error | " + Path::GetFileName(file);
               return;
            }

            // Update the hash
            while ( iRead = fread(btWorkBuffer, 1, iChunkSize, fp) )
            {
               if ( 1 != SHA512_Update(&ctx_sha512, btWorkBuffer, iRead) )
               {
                  // Unexpected error
                  fclose(fp);
                  e->Result = "Internal SHA512_Update error | " + Path::GetFileName(file);
                  return;
               }
               else if (bgWorker->CancellationPending)
               {
                  e->Cancel = true;
                  fclose(fp);
                  return;
               }
               else
               {
                  ullProcessed += iRead;
                  double dPercentage = ((double) ullProcessed / ullSize) * 100;
                  bgWorker->ReportProgress( (int) Math::Floor(dPercentage), progress );
                  System::Threading::Thread::Sleep(1);
               }
            }

            // Finalize the hash
            if ( 1 != SHA512_Final(abyDigest, &ctx_sha512) )
            {
               // Unexpected error
               fclose(fp);
               e->Result = "Internal SHA512_Final error | " + Path::GetFileName(file);
               return;
            }
            output->L_Hashes->Add( Hex_To_String(abyDigest, 64) );
         }
         break;

         default:
         {

         }
         break;
      }

      // Finally, release the file pointer
      fclose(fp);

      // Sleep a little bit
      System::Threading::Thread::Sleep(1);
   }

   e->Result = output;
}
private: System::Void bgWorker_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e)
{
   Progress^ progress = dynamic_cast<Progress^>(e->UserState);
   try
   {
      if ( progress->tab == ETab::SINGLE_FILE )
      {
         tsProgressBar->Value = e->ProgressPercentage;
      }
      else
      {
         SetStatusLabel("Hashing file " + progress->iProcessing.ToString() + " of " + progress->iNumberOfFiles.ToString() );
         tsProgressBar->Value = e->ProgressPercentage;
      }
   }
   catch(...)
   {

   }
}
private: System::Void bgWorker_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e)
{
   if ( e->Error != nullptr )
   {
      SetStatusLabel("Erro!");
   }
   else if ( e->Cancelled )
   {
      tsProgressBar->Value = 0;
      SetStatusLabel("Cancelled");
   }
   else if ( nullptr != dynamic_cast<String^> (e->Result) )
   {
      SetStatusLabel(dynamic_cast<String^> (e->Result));
   }
   else
   {
      // 
      int iProblems   = 0;
      int iNumOfFiles = 0;

      // Get the output arguments
      OutputArguments^ output = dynamic_cast<OutputArguments^> (e->Result);
      if ( nullptr == output )
      {
         SetStatusLabel("No data to display");
      }
      else
      {
         // Single or mutiple files were hashed?
         if ( output->tab == ETab::SINGLE_FILE )
         {
            switch(output->alg)
            {
               case EAlgorithm::MD5:
               {
                  tbMD5->Text = output->L_Hashes[0];
               }
               break;

               case EAlgorithm::SHA1:
               {
                  tbSHA1->Text = output->L_Hashes[0];
               }
               break;

               case EAlgorithm::SHA256:
               {
                  tbSHA256->Text = output->L_Hashes[0];
               }
               break;

               case EAlgorithm::SHA512:
               {
                  tbSHA512->Text = output->L_Hashes[0];
               }
               break;
            }
         }
         else // if ( output->tab == ETab::MULTIPLE_FILES )
         {
            if ( output->L_Files->Count != output->L_Hashes->Count )
            {
               SetStatusLabel("Not all files were hashed. Something wrong occurred");
               EnableAll();
               return;
            }
            else
            {
               // Inform user that we are processing
               SetStatusLabel("Processing results...");
               tsProgressBar->Value = 0;
               iNumOfFiles = output->L_Files->Count;
               Application::DoEvents();

               for (int i = 0; i < iNumOfFiles; i++ )
               {
                  // Get file information
                  try
                  {
                     FileInfo^ fi = gcnew FileInfo( output->L_Files[i] );
                     FileVersionInfo^ myFileVersionInfo = FileVersionInfo::GetVersionInfo(output->L_Files[i]);

                     if (fi->Extension == ".dll" || fi->Extension == ".exe" )
                     {
                        rtbTabMultiple->AppendText( "File      : " + Path::GetFileName(output->L_Files[i]) + "\n" +
                                                    "Version   : " + myFileVersionInfo->FileVersion        + "\n" +
                                                    "Size      : " + fi->Length + " bytes"                 + "\n" +
                                                    "Last write: " +  fi->LastWriteTime.ToString()         + "\n");
                     }
                     else
                     {
                        rtbTabMultiple->AppendText( "File      : " + Path::GetFileName(output->L_Files[i]) + "\n" +
                                                    "Size      : " + fi->Length + " bytes"                 + "\n" +
                                                    "Last write: " + fi->LastWriteTime.ToString()          + "\n"  );
                     }

                     switch(output->alg)
                     {
                        case EAlgorithm::MD5:
                        {
                           rtbTabMultiple->AppendText("MD5       : ");
                        }
                        break;

                        case EAlgorithm::SHA1:
                        {
                           rtbTabMultiple->AppendText("SHA1      : ");
                        }
                        break;

                        case EAlgorithm::SHA224:
                        {
                           rtbTabMultiple->AppendText("SHA224    : ");
                        }
                        break;

                        case EAlgorithm::SHA256:
                        {
                           rtbTabMultiple->AppendText("SHA256    : ");
                        }
                        break;

                        case EAlgorithm::SHA384:
                        {
                           rtbTabMultiple->AppendText("SHA384    : ");
                        }
                        break;

                        case EAlgorithm::SHA512:
                        {
                           rtbTabMultiple->AppendText("SHA512    : ");
                        }
                        break;
                     }

                     // Write the hash
                     rtbTabMultiple->AppendText(output->L_Hashes[i]);

                     // If it is not the last file, skip lines
                     if ( i != output->L_Files->Count -1)
                     {
                        rtbTabMultiple->AppendText("\r\n\r\n");
                     }
                  }
                  catch(Exception^ e)
                  {
                     e;
                     iProblems++;

                     // Arquivo nao existe?
                     rtbTabMultiple->AppendText( "File      : " + Path::GetFileName(output->L_Files[i]) + "\n");
                     rtbTabMultiple->AppendText( "!Failed!  : ");
                     rtbTabMultiple->AppendText(output->L_Hashes[i]);

                     // If it is not the last file, skip lines
                     if ( i != iNumOfFiles -1)
                     {
                        rtbTabMultiple->AppendText("\r\n\r\n");
                     }
                  }

                  // Update screen
                  double dPercentage   = ((double)(i+1)/iNumOfFiles) * 100;
                  tsProgressBar->Value = (int)Math::Floor(dPercentage);
                  Application::DoEvents();
               }
            }
         }
         if ( 0 == iProblems )
         {
            SetStatusLabel("Done");
         }
         else if ( 1 == iProblems )
         {
            SetStatusLabel("Done - unable to hash " + iProblems.ToString() + " file");
         }
         else
         {
            SetStatusLabel("Done - unable to hash " + iProblems.ToString() + " files");
         }
      }
   }

   EnableAll();
}


// ----------------------------------------------------------------------------------
// Status Bar
// ----------------------------------------------------------------------------------
private: System::Void SetStatusLabel( String^ strStatus)
{
   // The actual width of the form Window fits about 80 characters
   if (strStatus->Length < 80)
   {
      toolStripStatusLabel->Text = strStatus;
   }
   else
   {
      toolStripStatusLabel->Text = gcnew String( strStatus->Substring(0, 80) ) + "...";
   }
}
private: System::Void SetTabSingleFilePath( String^ filePath)
{
   // The actual width of the text box fits about 84 characters, but we consider 80
   int iPathLen = filePath->Length;

   try
   {
      if ( iPathLen < 80)
      {
         tbTabSingleFilePath->Text = filePath;
      }
      else
      {
         String^ fileName= Path::GetFileName( filePath );
         int iNameLen = fileName->Length;

         // If the filename, without the path, is really long, it might exceed that space, but that's OK
         tbTabSingleFilePath->Text = gcnew String( filePath->Substring(0, (75-iNameLen > 3 ? 75-iNameLen: 3) ) )
                                     + "[...]\\"
                                     + fileName ;
      }
   }
   catch(...)
   {
      SetStatusLabel("Error getting file name");
   }
}
private: System::Void tsCancel_ButtonClick(System::Object^  sender, System::EventArgs^  e)
{
   bgWorker->CancelAsync();
}
private: System::Void tsiCancel_Click(System::Object^  sender, System::EventArgs^  e) 
{
   bgWorker->CancelAsync();
}

// ----------------------------------------------------------------------------------
// Util
// ----------------------------------------------------------------------------------
private: System::String^ Hex_To_String( unsigned char* ucBuffer, int iLength )
{
   String ^strBuffer = String::Empty;
   for (int i = 0; i < iLength; i++)
   {
      strBuffer += ucBuffer[i].ToString("X2");
   }

   return strBuffer;
}
private: int ASCII_to_Hex( String^ strInputAscii, int iOutputMax, unsigned char* ucOutput )
{
   String^ strAscii = strInputAscii->ToUpper();
   strAscii = strAscii->Replace(" ", "" );

   int Length = (strAscii->Length)/2;
   if( Length > iOutputMax )
   {
      Length = iOutputMax;
   }

   for( int i = 0; i < Length; i++ )
   {
      if( strAscii[2*i] >= '0' && strAscii[2*i] <= '9' )
      {
         ucOutput[i] = 16*(strAscii[2*i] - '0');
      }
      else if( strAscii[2*i] >= 'A' && strAscii[2*i] <= 'F' )
      {
         ucOutput[i] = 16*(10 + (strAscii[2*i] - 'A'));
      }
      else
      {
         return -1;
      }

      if( strAscii[2*i + 1] >= '0' && strAscii[2*i + 1] <= '9' )
      {
         ucOutput[i] += (strAscii[2*i + 1] - '0');
      }
      else if( strAscii[2*i + 1] >= 'A' && strAscii[2*i + 1] <= 'F' )
      {
         ucOutput[i] += 10 + (strAscii[2*i + 1] - 'A');
      }
      else
      {
         return -1;
      }
   }

   return Length;
}
private: System::Void DisableAll()
{
   // Tab single-file
   btTabSingleMD5->Enabled    = false;
   btTabSingleSHA1->Enabled   = false;
   btTabSingleSHA256->Enabled = false;
   btTabSingleSHA512->Enabled = false;
   btTabSingleBrowse->Enabled = false;

   // Tab multiple-files
   btTabMutipleCalculate->Enabled = false;
   btTabMutipleBrowse->Enabled    = false;

   tsProgressBar->Value = 0;
}
private: System::Void EnableAll()
{
   // Tab single-file
   btTabSingleMD5->Enabled    = true;
   btTabSingleSHA1->Enabled   = true;
   btTabSingleSHA256->Enabled = true;
   btTabSingleSHA512->Enabled = true;
   btTabSingleBrowse->Enabled = true;

   // Tab multiple-files
   btTabMutipleCalculate->Enabled = true;
   btTabMutipleBrowse->Enabled    = true;
}
private: System::Void Drag_OnDrop(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
{
   try
   {
      // Is there any data?
      if ( false == e->Data->GetDataPresent( DataFormats::FileDrop ) )
      {
         SetStatusLabel("Error dropping file: no data present");
         return;
      }

      // Declare an array of String to receive the file names
      array<String^>^ filenames = dynamic_cast<array<String^>^>( e->Data->GetData(DataFormats::FileDrop) );
      if ( filenames == nullptr)
      {
         SetStatusLabel("Error getting dropped files");
         return;
      }

      // How many files were dropped?
      if ( filenames->Length == 1 )
      {
         SetTabSingleFilePath(filenames[0]);
         TabSingleOpenFileDialog->FileName = filenames[0];
         tabControl->SelectTab(0);

         // Clear whatever was there
         tbMD5->Clear();
         tbSHA1->Clear();
         tbSHA256->Clear();
         tbSHA512->Clear();

         // Clear the progress bar
         tsProgressBar->Value = 0;
      }
      else
      {
         List<String^>^ L_MultipleFiles = gcnew List<String^>();
         for (int i = 0; i < filenames->Length; i++ )
         {
            L_MultipleFiles->Add( filenames[i] );
         }

         TabMultipleOpenFileDialog->Tag = L_MultipleFiles;
         tbTabMultipleFilePath->Text = filenames->Length + " files selected";
         tabControl->SelectTab(1);

         // Clear whatever was there
         rtbTabMultiple->Clear();

         // Clear the progress bar
        tsProgressBar->Value = 0;
      }
   }
   catch(Exception^ ex)
   {
      SetStatusLabel(ex->ToString() );
   }
}
private: System::Void Drag_OnEnter(System::Object^  sender, System::Windows::Forms::DragEventArgs^  e)
{
   // We must accept only files, not folders
   array<String^>^ filenames = dynamic_cast<array<String^>^>( e->Data->GetData(DataFormats::FileDrop) );
   if ( filenames == nullptr)
   {
      SetStatusLabel("Error analyzing files");
      return;
   }

   bool bValid = true;

   for each (String^ file in filenames)
   {
      if ( (File::GetAttributes(file) & FileAttributes::Directory)  == FileAttributes::Directory )
      {
         // Not valid, it is a folder
         bValid = false;
      }
      else if (Path::GetExtension(file) == ".lnk")
      {
         // Not valid, it is a shortcut
         bValid = false;
      }
   }

   if ( true == bValid)
   {
      e->Effect = DragDropEffects::Link;
   }
   else
   {
      e->Effect = DragDropEffects::None;
   }

}
private: System::Void Form_Closing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) 
{
   for each (String^ s in L_TempFiles )
   {
      try
      {
         File::Delete(s);
      }
      catch(Exception^ e)
      {
         e;
      }
   }
}

// ----------------------------------------------------------------------------------
// Menu
// ----------------------------------------------------------------------------------
private: System::Void aboutToolStripMenuItem1_Click(System::Object^  sender, System::EventArgs^  e)
{
   FormAboutEx fax(iChunkSize/1024);
   fax.ShowDialog();
}
private: System::Void donateToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
   ProcessStartInfo^ sInfo = gcnew ProcessStartInfo("https://hashingapp.github.io/#donate");  
   Process::Start(sInfo);
}
private: System::Void websiteToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) 
{
   ProcessStartInfo^ sInfo = gcnew ProcessStartInfo("https://hashingapp.github.io/");  
   Process::Start(sInfo);
}


};
}

