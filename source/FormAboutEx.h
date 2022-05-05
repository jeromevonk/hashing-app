#pragma once

namespace Hashing {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for FormAboutEx
	/// </summary>
	public ref class FormAboutEx : public System::Windows::Forms::Form
	{
	public:
		FormAboutEx(int iChunkSize)
		{
			InitializeComponent();
         label3->Text = "using chunk size of " + iChunkSize.ToString() + " kb";
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormAboutEx()
		{
			if (components)
			{
				delete components;
			}
		}
   private: System::Windows::Forms::Label^  label1;
   private: System::Windows::Forms::Label^  label3;
   private: System::Windows::Forms::Button^  btOK;
   private: System::Windows::Forms::RichTextBox^  richTextBox1;

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
         System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormAboutEx::typeid));
         this->label1 = (gcnew System::Windows::Forms::Label());
         this->label3 = (gcnew System::Windows::Forms::Label());
         this->btOK = (gcnew System::Windows::Forms::Button());
         this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
         this->SuspendLayout();
         // 
         // label1
         // 
         this->label1->AutoSize = true;
         this->label1->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
         this->label1->Location = System::Drawing::Point(12, 22);
         this->label1->Name = L"label1";
         this->label1->Size = System::Drawing::Size(208, 18);
         this->label1->TabIndex = 1;
         this->label1->Text = L"Hashing | Version 1.5 | May 2022";
         // 
         // label3
         // 
         this->label3->AutoSize = true;
         this->label3->Font = (gcnew System::Drawing::Font(L"Calibri", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
         this->label3->Location = System::Drawing::Point(223, 22);
         this->label3->Name = L"label3";
         this->label3->Size = System::Drawing::Size(166, 18);
         this->label3->TabIndex = 3;
         this->label3->Text = L"using chunk size of 128 kb";
         // 
         // btOK
         // 
         this->btOK->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(128)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
            static_cast<System::Int32>(static_cast<System::Byte>(128)));
         this->btOK->DialogResult = System::Windows::Forms::DialogResult::Cancel;
         this->btOK->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
         this->btOK->Font = (gcnew System::Drawing::Font(L"Calibri", 11.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
         this->btOK->Location = System::Drawing::Point(161, 233);
         this->btOK->Name = L"btOK";
         this->btOK->Size = System::Drawing::Size(65, 28);
         this->btOK->TabIndex = 30;
         this->btOK->Text = L"OK";
         this->btOK->UseVisualStyleBackColor = false;
         // 
         // richTextBox1
         // 
         this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Calibri", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
         this->richTextBox1->Location = System::Drawing::Point(15, 52);
         this->richTextBox1->Name = L"richTextBox1";
         this->richTextBox1->ReadOnly = true;
         this->richTextBox1->Size = System::Drawing::Size(356, 167);
         this->richTextBox1->TabIndex = 32;
         this->richTextBox1->Text = resources->GetString(L"richTextBox1.Text");
         // 
         // FormAboutEx
         // 
         this->AcceptButton = this->btOK;
         this->AutoScaleDimensions = System::Drawing::SizeF(8, 19);
         this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
         this->BackColor = System::Drawing::Color::White;
         this->CancelButton = this->btOK;
         this->ClientSize = System::Drawing::Size(386, 270);
         this->Controls->Add(this->richTextBox1);
         this->Controls->Add(this->btOK);
         this->Controls->Add(this->label3);
         this->Controls->Add(this->label1);
         this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
            static_cast<System::Byte>(0)));
         this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
         this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
         this->MaximizeBox = false;
         this->MinimizeBox = false;
         this->Name = L"FormAboutEx";
         this->Opacity = 0.95;
         this->ShowInTaskbar = false;
         this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
         this->Text = L"About";
         this->ResumeLayout(false);
         this->PerformLayout();

      }
#pragma endregion
	};
}
