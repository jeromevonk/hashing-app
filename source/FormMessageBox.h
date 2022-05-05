#pragma once

namespace Hashing {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Summary for FormMessageBox
	/// </summary>
	public ref class FormMessageBox : public System::Windows::Forms::Form
	{
	public:
		FormMessageBox(String^ strTitle, String^ strMessage, Color color)
		{
			InitializeComponent();
         this->Text      = strTitle;
         lbMessage->Text = strMessage + "   ";

         this->btOK->BackColor = color;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMessageBox()
		{
			if (components)
			{
				delete components;
			}
		}
   private: System::Windows::Forms::Button^  btOK;
   private: System::Windows::Forms::Label^  lbMessage;

   public: static Color HashingGreen = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
            static_cast<System::Int32>(static_cast<System::Byte>(128))); 
           static Color HashingRed = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(0)), 
            static_cast<System::Int32>(static_cast<System::Byte>(0)));

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
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMessageBox::typeid));
         this->btOK = (gcnew System::Windows::Forms::Button());
         this->lbMessage = (gcnew System::Windows::Forms::Label());
         this->SuspendLayout();
         // 
         // btOK
         // 
         this->btOK->Anchor = System::Windows::Forms::AnchorStyles::None;
         this->btOK->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)), 
            static_cast<System::Int32>(static_cast<System::Byte>(128)));
         this->btOK->DialogResult = System::Windows::Forms::DialogResult::Cancel;
         this->btOK->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btOK->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
            static_cast<System::Byte>(0)));
         this->btOK->Location = System::Drawing::Point(30, 54);
         this->btOK->Name = L"btOK";
         this->btOK->Size = System::Drawing::Size(56, 28);
         this->btOK->TabIndex = 31;
         this->btOK->Text = L"OK";
         this->btOK->UseVisualStyleBackColor = false;
         // 
         // lbMessage
         // 
         this->lbMessage->Anchor = System::Windows::Forms::AnchorStyles::Left;
         this->lbMessage->AutoSize = true;
         this->lbMessage->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
            static_cast<System::Byte>(0)));
         this->lbMessage->Location = System::Drawing::Point(13, 22);
         this->lbMessage->Name = L"lbMessage";
         this->lbMessage->Size = System::Drawing::Size(11, 14);
         this->lbMessage->TabIndex = 32;
         this->lbMessage->Text = L"-";
         // 
         // FormMessageBox
         // 
         this->AcceptButton = this->btOK;
         this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->AutoSize = true;
         this->BackColor = System::Drawing::Color::White;
         this->CancelButton = this->btOK;
         this->ClientSize = System::Drawing::Size(116, 94);
         this->Controls->Add(this->lbMessage);
         this->Controls->Add(this->btOK);
         this->Font = (gcnew System::Drawing::Font(L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
            static_cast<System::Byte>(0)));
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
         this->MaximizeBox = false;
         this->MinimizeBox = false;
         this->Name = L"FormMessageBox";
         this->Opacity = 0.95;
         this->ShowInTaskbar = false;
         this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
         this->ResumeLayout(false);
         this->PerformLayout();

      }
#pragma endregion
	};
}
