#pragma once
#include "Contour.h"

namespace ContourMaestro {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
    private: System::Windows::Forms::MenuStrip^  menuStrip;
    protected: 

    protected: 
    private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
    private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
    private: System::Windows::Forms::PictureBox^  pictureBox1;
    private: System::Windows::Forms::DataGridView^  dataGridView;

    private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
    private: System::Windows::Forms::BindingSource^  bindingSource;
    private: System::ComponentModel::IContainer^  components;


	private:
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
            this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
            this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
            this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
            this->dataGridView = (gcnew System::Windows::Forms::DataGridView());
            this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
            this->bindingSource = (gcnew System::Windows::Forms::BindingSource(this->components));
            this->menuStrip->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->BeginInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingSource))->BeginInit();
            this->SuspendLayout();
            // 
            // menuStrip
            // 
            this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
            this->menuStrip->Location = System::Drawing::Point(0, 0);
            this->menuStrip->Name = L"menuStrip";
            this->menuStrip->Size = System::Drawing::Size(284, 24);
            this->menuStrip->TabIndex = 0;
            this->menuStrip->Text = L"menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->openToolStripMenuItem});
            this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
            this->fileToolStripMenuItem->Size = System::Drawing::Size(37, 20);
            this->fileToolStripMenuItem->Text = L"File";
            // 
            // openToolStripMenuItem
            // 
            this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
            this->openToolStripMenuItem->Size = System::Drawing::Size(103, 22);
            this->openToolStripMenuItem->Text = L"Open";
            this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::openToolStripMenuItem_Click);
            // 
            // pictureBox1
            // 
            this->pictureBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->pictureBox1->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->pictureBox1->Location = System::Drawing::Point(12, 27);
            this->pictureBox1->Name = L"pictureBox1";
            this->pictureBox1->Size = System::Drawing::Size(260, 167);
            this->pictureBox1->TabIndex = 1;
            this->pictureBox1->TabStop = false;
            // 
            // dataGridView
            // 
            this->dataGridView->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->dataGridView->AutoSizeColumnsMode = System::Windows::Forms::DataGridViewAutoSizeColumnsMode::AllCells;
            this->dataGridView->ColumnHeadersBorderStyle = System::Windows::Forms::DataGridViewHeaderBorderStyle::None;
            this->dataGridView->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView->Location = System::Drawing::Point(12, 206);
            this->dataGridView->Name = L"dataGridView";
            this->dataGridView->RowHeadersVisible = false;
            this->dataGridView->Size = System::Drawing::Size(260, 92);
            this->dataGridView->TabIndex = 2;
            // 
            // openFileDialog
            // 
            this->openFileDialog->Filter = L"csv file|*.csv|All files|*.*";
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(284, 310);
            this->Controls->Add(this->dataGridView);
            this->Controls->Add(this->pictureBox1);
            this->Controls->Add(this->menuStrip);
            this->MainMenuStrip = this->menuStrip;
            this->Name = L"Form1";
            this->Text = L"Contour Maestro";
            this->menuStrip->ResumeLayout(false);
            this->menuStrip->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView))->EndInit();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->bindingSource))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: array<System::Double, 2>^ m_Data;
    private: System::Collections::Generic::List<array<System::Double, 1>^> ^contour;
    private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
                 if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                 {
                     System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(openFileDialog->OpenFile());
                     array<System::Char>^ comma = {','};
                     array<System::String^>^ split = rdr->ReadLine()->Split(comma);
                     m_Data = gcnew array<System::Double, 2>(System::Convert::ToInt32(split[0]),
                                                             System::Convert::ToInt32(split[1]));
                     dataGridView->RowCount = m_Data->GetLength(0);
                     dataGridView->ColumnCount = m_Data->GetLength(1);
                     for (System::Int32 i=0; i<m_Data->GetLength(0); i++)
                     {
                         split = rdr->ReadLine()->Split(comma);
                         System::Diagnostics::Trace::Assert (split->Length == m_Data->GetLength(1));
                         for (System::Int32 j=0; j<m_Data->GetLength(1); j++)
                         {
                             m_Data[i,j] = System::Convert::ToDouble (split[j]);
                             dataGridView[j,i]->Value = gcnew System::Double (m_Data[i,j]);
                         }
                     }
                     array <Double, 1>^ x_idxs = {0, 1, 2, 3, 4, 5, 6, 7};
                     array <Double, 1>^ y_idxs = {0, 1, 2, 3};
                     array <Double, 1>^ levels = {0, 0.5, 1, 1.5, 2, 2.5};
                     //System::Collections::Generic::List<array<System::Double, 1>^>^
                     contour = conrec (m_Data, x_idxs, y_idxs, levels);
                     //if (pictureBox1->Image == nullptr)
                     //    ((System::Drawing::Bitmap^) pictureBox1->Image)->Dispose();
                     pictureBox1->Image = gcnew System::Drawing::Bitmap(pictureBox1->Width,
                                                                        pictureBox1->Height);
                     System::Drawing::Graphics^ gph = System::Drawing::Graphics::FromImage(pictureBox1->Image);
                     for each (array<System::Double, 1>^ arr in contour)
                     {
                         gph->DrawLine (System::Drawing::Pens::Black,
                                        (float) ((arr[0]/7.0)*((System::Double) pictureBox1->Width)),
                                        (float) ((3.0 - arr[1])/3.0*((System::Double) pictureBox1->Height)),
                                        (float) ((arr[2]/7.0)*((System::Double) pictureBox1->Width)),
                                        (float) ((3.0 - arr[3])/3.0*((System::Double) pictureBox1->Height)));
                     }
                 }
             }
    };
}

