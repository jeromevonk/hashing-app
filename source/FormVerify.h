#pragma once

#include "FormMessageBox.h"

namespace Hashing {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormVerify
	/// </summary>
	public ref class FormVerify : public System::Windows::Forms::Form
	{
	public:
		FormVerify(void)
		{
			InitializeComponent();
         
         // If there is text on clipboard, lets check it
         if ( Clipboard::GetDataObject()->GetDataPresent(DataFormats::Text))
         {
            String^ fromClipboard = Clipboard::GetDataObject()->GetData( DataFormats::Text )->ToString();

            // First, let's remove al spaces
            fromClipboard = fromClipboard->Replace( " ", "" );

            // We are looking for hash of 16 to 64 bytes (so 32 to 128 characters)
            if ( fromClipboard->Length >= 32 && fromClipboard->Length <= 128 )
            {
               bool bLooksLikeAHash = true;

               for each (Char c in fromClipboard)
               {
                  if ( false == ValidateKey( c, false ) )
                  {
                     bLooksLikeAHash = false;
                     break;
                  }
               }

               if ( bLooksLikeAHash )
               {
                  tbHashToVerify->Text = fromClipboard;
                  lbPasted->Text = "Text automatically pasted from the clipboard!";
               }
            }
         }
		}

      String^ getHash(void )
      {
         return tbHashToVerify->Text;
      }

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormVerify()
		{
			if (components)
			{
				delete components;
			}
		}
   private: System::Windows::Forms::Label^  lbInstruction;
   private: System::Windows::Forms::TextBox^  tbHashToVerify;
   private: System::Windows::Forms::Button^  btVerify;
   private: System::Windows::Forms::Button^  btCancel;
   private: System::Windows::Forms::Button^  btPaste;
   private: System::Windows::Forms::Label^  lbVerifyBytes;
   private: System::Windows::Forms::Label^  lbSize;
   private: System::Windows::Forms::Label^  lbPasted;

   private: String^ strLastValidData;



   private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager( FormVerify::typeid ));
         this->lbInstruction = (gcnew System::Windows::Forms::Label());
         this->tbHashToVerify = (gcnew System::Windows::Forms::TextBox());
         this->btVerify = (gcnew System::Windows::Forms::Button());
         this->btCancel = (gcnew System::Windows::Forms::Button());
         this->btPaste = (gcnew System::Windows::Forms::Button());
         this->lbVerifyBytes = (gcnew System::Windows::Forms::Label());
         this->lbSize = (gcnew System::Windows::Forms::Label());
         this->lbPasted = (gcnew System::Windows::Forms::Label());
         this->SuspendLayout();
         // 
         // lbInstruction
         // 
         this->lbInstruction->AutoSize = true;
         this->lbInstruction->Font = (gcnew System::Drawing::Font( L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->lbInstruction->Location = System::Drawing::Point( 4, 13 );
         this->lbInstruction->Name = L"lbInstruction";
         this->lbInstruction->Size = System::Drawing::Size( 360, 14 );
         this->lbInstruction->TabIndex = 0;
         this->lbInstruction->Text = L"Input here to verify against any of the hashes. No spaces, please";
         // 
         // tbHashToVerify
         // 
         this->tbHashToVerify->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->tbHashToVerify->Location = System::Drawing::Point( 35, 58 );
         this->tbHashToVerify->MaxLength = 128;
         this->tbHashToVerify->Name = L"tbHashToVerify";
         this->tbHashToVerify->Size = System::Drawing::Size( 323, 22 );
         this->tbHashToVerify->TabIndex = 1;
         this->tbHashToVerify->TextChanged += gcnew System::EventHandler( this, &FormVerify::tbHashToVerifyTextChanged );
         this->tbHashToVerify->KeyDown += gcnew System::Windows::Forms::KeyEventHandler( this, &FormVerify::tbHashToVerifyKeyDown );
         this->tbHashToVerify->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler( this, &FormVerify::tbHashToVerifyKeyPress );
         this->tbHashToVerify->KeyUp += gcnew System::Windows::Forms::KeyEventHandler( this, &FormVerify::tbHashToVerifyKeyUp );
         // 
         // btVerify
         // 
         this->btVerify->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(128)) );
         this->btVerify->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btVerify->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btVerify->Location = System::Drawing::Point( 109, 89 );
         this->btVerify->Name = L"btVerify";
         this->btVerify->Size = System::Drawing::Size( 54, 23 );
         this->btVerify->TabIndex = 29;
         this->btVerify->Text = L"Verify";
         this->btVerify->UseVisualStyleBackColor = false;
         this->btVerify->Click += gcnew System::EventHandler( this, &FormVerify::btVerify_Click );
         // 
         // btCancel
         // 
         this->btCancel->BackColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)) );
         this->btCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
         this->btCancel->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btCancel->Font = (gcnew System::Drawing::Font( L"Calibri", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->btCancel->Location = System::Drawing::Point( 201, 89 );
         this->btCancel->Name = L"btCancel";
         this->btCancel->Size = System::Drawing::Size( 54, 23 );
         this->btCancel->TabIndex = 30;
         this->btCancel->Text = L"Cancel";
         this->btCancel->UseVisualStyleBackColor = false;
         // 
         // btPaste
         // 
         this->btPaste->BackColor = System::Drawing::Color::White;
         this->btPaste->BackgroundImage = (cli::safe_cast<System::Drawing::Image^>(resources->GetObject( L"btPaste.BackgroundImage" )));
         this->btPaste->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
         this->btPaste->Location = System::Drawing::Point( 7, 54 );
         this->btPaste->Name = L"btPaste";
         this->btPaste->Size = System::Drawing::Size( 24, 24 );
         this->btPaste->TabIndex = 31;
         this->btPaste->UseVisualStyleBackColor = false;
         this->btPaste->Click += gcnew System::EventHandler( this, &FormVerify::btPaste_Click );
         // 
         // lbVerifyBytes
         // 
         this->lbVerifyBytes->AutoSize = true;
         this->lbVerifyBytes->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->lbVerifyBytes->ForeColor = System::Drawing::Color::FromArgb( static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(192)),
            static_cast<System::Int32>(static_cast<System::Byte>(0)) );
         this->lbVerifyBytes->Location = System::Drawing::Point( 322, 84 );
         this->lbVerifyBytes->Name = L"lbVerifyBytes";
         this->lbVerifyBytes->Size = System::Drawing::Size( 14, 14 );
         this->lbVerifyBytes->TabIndex = 32;
         this->lbVerifyBytes->Text = L"0";
         // 
         // lbSize
         // 
         this->lbSize->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
         this->lbSize->AutoSize = true;
         this->lbSize->Font = (gcnew System::Drawing::Font( L"Consolas", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->lbSize->ForeColor = System::Drawing::Color::Blue;
         this->lbSize->Location = System::Drawing::Point( 275, 103 );
         this->lbSize->Name = L"lbSize";
         this->lbSize->Size = System::Drawing::Size( 0, 14 );
         this->lbSize->TabIndex = 33;
         // 
         // lbPasted
         // 
         this->lbPasted->AutoSize = true;
         this->lbPasted->Font = (gcnew System::Drawing::Font( L"Calibri", 9, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0) ));
         this->lbPasted->ForeColor = System::Drawing::SystemColors::AppWorkspace;
         this->lbPasted->Location = System::Drawing::Point( 65, 30 );
         this->lbPasted->Name = L"lbPasted";
         this->lbPasted->Size = System::Drawing::Size( 233, 14 );
         this->lbPasted->TabIndex = 34;
         this->lbPasted->Text = L"";
         // 
         // FormVerify
         // 
         this->AcceptButton = this->btVerify;
         this->AutoScaleDimensions = System::Drawing::SizeF( 6, 13 );
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->BackColor = System::Drawing::Color::White;
         this->CancelButton = this->btCancel;
         this->ClientSize = System::Drawing::Size( 364, 124 );
         this->Controls->Add( this->lbPasted );
         this->Controls->Add( this->lbSize );
         this->Controls->Add( this->lbVerifyBytes );
         this->Controls->Add( this->btPaste );
         this->Controls->Add( this->btCancel );
         this->Controls->Add( this->btVerify );
         this->Controls->Add( this->tbHashToVerify );
         this->Controls->Add( this->lbInstruction );
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
         this->MaximizeBox = false;
         this->MinimizeBox = false;
         this->Name = L"FormVerify";
         this->ShowIcon = false;
         this->ShowInTaskbar = false;
         this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
         this->Text = L"Verify";
         this->ResumeLayout( false );
         this->PerformLayout();

      }
#pragma endregion

// Button clicks
private: System::Void btVerify_Click(System::Object^  sender, System::EventArgs^  e) 
{
   if ( tbHashToVerify->Text == "" )
   {
      this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      Close();
   }
   else if ( tbHashToVerify->Text->Length < 32)
   {
      this->DialogResult = System::Windows::Forms::DialogResult::Cancel;
      FormMessageBox mfb("", "Hashes must have at least 16 bytes (32 characters)", FormMessageBox::HashingRed );
      mfb.ShowDialog();
   }
   else
   {
      this->DialogResult = System::Windows::Forms::DialogResult::OK;
      Close();
   }
}
private: System::Void btPaste_Click(System::Object^  sender, System::EventArgs^  e) 
{
   try
   {
      if ( Clipboard::GetDataObject()->GetDataPresent(DataFormats::Text))
      {
         // First, let's remove al spaces
         String^ fromClipboard = Clipboard::GetDataObject()->GetData(DataFormats::Text)->ToString();
         fromClipboard = fromClipboard->Replace( " ", "" );
         tbHashToVerify->Text = fromClipboard;
      }
      else
      {
         //SetStatusLabel("The clipboad does not contain any text");
      }
   }
   catch(Exception^ e)
   {
      e;
   }
}


// Event handlers
private: System::Void tbHashToVerifyTextChanged(System::Object^  sender, System::EventArgs^  e) 
{
   // DOUBT:
   // Should allow only hexadecimal to be pasted?
   // User might be confused...
   //
   //  String^ strAux = tbHashToVerify->Text;
   //  if ( false == hasInvalidData(strAux) )
   //  {
   //     tbHashToVerify->Text = strLastValidData;
   //     tbHashToVerify->SelectionStart  = tbHashToVerify->Text->Length+1;
   //    return;
   // }
   //
   // strLastValidData = strAux;

   // First, let's remove al spaces
   tbHashToVerify->Text = tbHashToVerify->Text->Replace( " ", "" );




   if ( tbHashToVerify->Text->Length == 0)
   {
      lbVerifyBytes->Text = "0";
   }
   else
   {
      int iLength = tbHashToVerify->Text->Length;
      int iSpaces = tbHashToVerify->Text->Split()->Length -1;

      lbVerifyBytes->Text = ( (iLength - iSpaces)/2 ).ToString();
   }

   if ( lbVerifyBytes->Text == "16")
   {
      lbSize->Text = "MD5 size";
   }
   else if ( lbVerifyBytes->Text == "20")
   {
      lbSize->Text = "SHA1 size";
   }
   else if ( lbVerifyBytes->Text == "32")
   {
      lbSize->Text = "SHA256 size";
   }
   else if ( lbVerifyBytes->Text == "64")
   {
      lbSize->Text = "SHA512 size";
   }
   else
   {
      lbSize->Text = "";
   }
}
private: System::Void tbHashToVerifyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
      // Do not handle anything here
}
private: System::Void tbHashToVerifyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) 
{
   if ( !ValidateKey( e->KeyChar, true) )
   {
      e->Handled = true;
   }
}
private: System::Void tbHashToVerifyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) 
{
      // Do not handle anything here
}

// Auxiliar functions
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


};
}
