#pragma once
#include "MathUtils.h"

namespace RetroMaestro {

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
        ref class EventData
        {
        public:
            System::Int32 BatID;
            System::Int32 PitID;
            System::Int32 OutDelta;
            System::Double Value;
        };
    private: System::Windows::Forms::Button^  button_QueryAll;
    public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
            dataGridView_Results->Rows->Add(4);
            System::Diagnostics::Debug::Assert (Value(3,0) == 2);
            System::Diagnostics::Debug::Assert (Value(3,4) == 1);
            System::Diagnostics::Debug::Assert (Value(3,6) == 0);
            System::Diagnostics::Debug::Assert (Value(3,8) == 1);
            System::Diagnostics::Debug::Assert (Value(3,10) == 0);
            System::Diagnostics::Debug::Assert (Value(3,12) == 0);
            System::Diagnostics::Debug::Assert (Value(3,16) == 1);
            System::Diagnostics::Debug::Assert (Value(3,18) == 0);
            System::Diagnostics::Debug::Assert (Value(3,20) == 0);
            System::Diagnostics::Debug::Assert (Value(3,24) == 0);
            System::Diagnostics::Debug::Assert (Value(3,32) == 0);
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
    private: System::Windows::Forms::MenuStrip^  menuStrip1;
    private: System::Windows::Forms::Button^  button_OpenDB;
    private: System::Windows::Forms::Button^  button_Dir;
    private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog;
    private: System::Windows::Forms::TextBox^  textBox_Folder;
    private: System::Windows::Forms::TextBox^  textBox_Connection;


    private: System::ComponentModel::BackgroundWorker^  backgroundWorker;
    private: System::Windows::Forms::Button^  button_DoWork;
    private: System::Windows::Forms::StatusStrip^  statusStrip;
    private: System::Windows::Forms::ToolStripProgressBar^  toolStripProgressBar;
    private: System::Windows::Forms::Button^  button_CloseDB;
    private: System::Windows::Forms::Button^  button_StopWork;


    private: System::Windows::Forms::Button^  button_Query;
    private: System::Windows::Forms::TextBox^  textBox_Where;
    private: System::Windows::Forms::Label^  label_Where;
    private: System::Windows::Forms::Label^  label_Result;
    private: System::Windows::Forms::TextBox^  textBox_Result;
    private: System::Windows::Forms::DataGridView^  dataGridView_Results;








    private: System::Windows::Forms::Button^  button_FileCRCs;
    private: System::Windows::Forms::TextBox^  textBox_timespan;
    private: System::Windows::Forms::TextBox^  textBox_numlines;
    private: System::Windows::Forms::Button^  button_Query2;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0000;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0001;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0010;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0011;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0100;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0101;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0110;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_0111;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1000;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1001;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1010;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1011;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1100;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1101;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1110;
    private: System::Windows::Forms::DataGridViewTextBoxColumn^  Column_1111;
    private: System::Windows::Forms::Button^  button_LoadValues;
    private: System::Windows::Forms::TextBox^  textBox_ValueFile;
    private: System::Windows::Forms::Button^  button_LoadEvents;

    private: System::Windows::Forms::TextBox^  textBox_Events;
    private: System::Windows::Forms::Button^  button_Calculate;
    private: System::Windows::Forms::Button^  button_Calculate2;
    private: System::Windows::Forms::Button^  button_BasesPer;





















    protected: 


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
            this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
            this->button_OpenDB = (gcnew System::Windows::Forms::Button());
            this->button_Dir = (gcnew System::Windows::Forms::Button());
            this->folderBrowserDialog = (gcnew System::Windows::Forms::FolderBrowserDialog());
            this->textBox_Folder = (gcnew System::Windows::Forms::TextBox());
            this->textBox_Connection = (gcnew System::Windows::Forms::TextBox());
            this->backgroundWorker = (gcnew System::ComponentModel::BackgroundWorker());
            this->button_DoWork = (gcnew System::Windows::Forms::Button());
            this->statusStrip = (gcnew System::Windows::Forms::StatusStrip());
            this->toolStripProgressBar = (gcnew System::Windows::Forms::ToolStripProgressBar());
            this->button_CloseDB = (gcnew System::Windows::Forms::Button());
            this->button_StopWork = (gcnew System::Windows::Forms::Button());
            this->button_Query = (gcnew System::Windows::Forms::Button());
            this->textBox_Where = (gcnew System::Windows::Forms::TextBox());
            this->label_Where = (gcnew System::Windows::Forms::Label());
            this->label_Result = (gcnew System::Windows::Forms::Label());
            this->textBox_Result = (gcnew System::Windows::Forms::TextBox());
            this->dataGridView_Results = (gcnew System::Windows::Forms::DataGridView());
            this->Column_0000 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0001 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0010 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0011 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0100 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0101 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0110 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_0111 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1000 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1001 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1010 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1011 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1100 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1101 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1110 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->Column_1111 = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
            this->button_FileCRCs = (gcnew System::Windows::Forms::Button());
            this->textBox_timespan = (gcnew System::Windows::Forms::TextBox());
            this->textBox_numlines = (gcnew System::Windows::Forms::TextBox());
            this->button_Query2 = (gcnew System::Windows::Forms::Button());
            this->button_QueryAll = (gcnew System::Windows::Forms::Button());
            this->button_LoadValues = (gcnew System::Windows::Forms::Button());
            this->textBox_ValueFile = (gcnew System::Windows::Forms::TextBox());
            this->button_LoadEvents = (gcnew System::Windows::Forms::Button());
            this->textBox_Events = (gcnew System::Windows::Forms::TextBox());
            this->button_Calculate = (gcnew System::Windows::Forms::Button());
            this->button_Calculate2 = (gcnew System::Windows::Forms::Button());
            this->button_BasesPer = (gcnew System::Windows::Forms::Button());
            this->statusStrip->SuspendLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Results))->BeginInit();
            this->SuspendLayout();
            // 
            // menuStrip1
            // 
            this->menuStrip1->Location = System::Drawing::Point(0, 0);
            this->menuStrip1->Name = L"menuStrip1";
            this->menuStrip1->Size = System::Drawing::Size(819, 24);
            this->menuStrip1->TabIndex = 0;
            this->menuStrip1->Text = L"menuStrip1";
            // 
            // button_OpenDB
            // 
            this->button_OpenDB->Location = System::Drawing::Point(12, 27);
            this->button_OpenDB->Name = L"button_OpenDB";
            this->button_OpenDB->Size = System::Drawing::Size(75, 23);
            this->button_OpenDB->TabIndex = 1;
            this->button_OpenDB->Text = L"OpenDB";
            this->button_OpenDB->UseVisualStyleBackColor = true;
            this->button_OpenDB->Click += gcnew System::EventHandler(this, &Form1::button_OpenDB_Click);
            // 
            // button_Dir
            // 
            this->button_Dir->Location = System::Drawing::Point(12, 81);
            this->button_Dir->Name = L"button_Dir";
            this->button_Dir->Size = System::Drawing::Size(75, 23);
            this->button_Dir->TabIndex = 2;
            this->button_Dir->Text = L"Dir";
            this->button_Dir->UseVisualStyleBackColor = true;
            this->button_Dir->Click += gcnew System::EventHandler(this, &Form1::button_Dir_Click);
            // 
            // textBox_Folder
            // 
            this->textBox_Folder->Location = System::Drawing::Point(12, 110);
            this->textBox_Folder->Name = L"textBox_Folder";
            this->textBox_Folder->Size = System::Drawing::Size(260, 20);
            this->textBox_Folder->TabIndex = 3;
            this->textBox_Folder->Text = L"C:\\Users\\Kevin\\Documents\\retrosheet\\data";
            // 
            // textBox_Connection
            // 
            this->textBox_Connection->Location = System::Drawing::Point(12, 55);
            this->textBox_Connection->Name = L"textBox_Connection";
            this->textBox_Connection->Size = System::Drawing::Size(260, 20);
            this->textBox_Connection->TabIndex = 4;
            this->textBox_Connection->Text = L"Database=Retrosheet;Trusted_Connection=yes;Server=BABYG\\SQLEXPRESS;Provider=SQLOL" 
                L"EDB";
            // 
            // backgroundWorker
            // 
            this->backgroundWorker->WorkerReportsProgress = true;
            this->backgroundWorker->WorkerSupportsCancellation = true;
            this->backgroundWorker->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker_DoWork);
            this->backgroundWorker->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker_RunWorkerCompleted);
            this->backgroundWorker->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker_ProgressChanged);
            // 
            // button_DoWork
            // 
            this->button_DoWork->Location = System::Drawing::Point(93, 81);
            this->button_DoWork->Name = L"button_DoWork";
            this->button_DoWork->Size = System::Drawing::Size(75, 23);
            this->button_DoWork->TabIndex = 6;
            this->button_DoWork->Text = L"Do Work";
            this->button_DoWork->UseVisualStyleBackColor = true;
            this->button_DoWork->Click += gcnew System::EventHandler(this, &Form1::button_DoWork_Click);
            // 
            // statusStrip
            // 
            this->statusStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->toolStripProgressBar});
            this->statusStrip->Location = System::Drawing::Point(0, 314);
            this->statusStrip->Name = L"statusStrip";
            this->statusStrip->Size = System::Drawing::Size(819, 22);
            this->statusStrip->TabIndex = 8;
            this->statusStrip->Text = L"statusStrip1";
            // 
            // toolStripProgressBar
            // 
            this->toolStripProgressBar->Name = L"toolStripProgressBar";
            this->toolStripProgressBar->Size = System::Drawing::Size(260, 16);
            // 
            // button_CloseDB
            // 
            this->button_CloseDB->Location = System::Drawing::Point(93, 27);
            this->button_CloseDB->Name = L"button_CloseDB";
            this->button_CloseDB->Size = System::Drawing::Size(75, 23);
            this->button_CloseDB->TabIndex = 9;
            this->button_CloseDB->Text = L"CloseDB";
            this->button_CloseDB->UseVisualStyleBackColor = true;
            this->button_CloseDB->Click += gcnew System::EventHandler(this, &Form1::button_CloseDB_Click);
            // 
            // button_StopWork
            // 
            this->button_StopWork->Location = System::Drawing::Point(174, 81);
            this->button_StopWork->Name = L"button_StopWork";
            this->button_StopWork->Size = System::Drawing::Size(75, 23);
            this->button_StopWork->TabIndex = 10;
            this->button_StopWork->Text = L"Stop Work";
            this->button_StopWork->UseVisualStyleBackColor = true;
            this->button_StopWork->Click += gcnew System::EventHandler(this, &Form1::button_StopWork_Click);
            // 
            // button_Query
            // 
            this->button_Query->Location = System::Drawing::Point(12, 136);
            this->button_Query->Name = L"button_Query";
            this->button_Query->Size = System::Drawing::Size(75, 23);
            this->button_Query->TabIndex = 13;
            this->button_Query->Text = L"Query";
            this->button_Query->UseVisualStyleBackColor = true;
            this->button_Query->Click += gcnew System::EventHandler(this, &Form1::button_Query_Click);
            // 
            // textBox_Where
            // 
            this->textBox_Where->Location = System::Drawing::Point(290, 55);
            this->textBox_Where->Multiline = true;
            this->textBox_Where->Name = L"textBox_Where";
            this->textBox_Where->Size = System::Drawing::Size(200, 49);
            this->textBox_Where->TabIndex = 11;
            this->textBox_Where->Text = L"[gameID] like \'WAS2011%\'";
            // 
            // label_Where
            // 
            this->label_Where->AutoSize = true;
            this->label_Where->Location = System::Drawing::Point(290, 40);
            this->label_Where->Name = L"label_Where";
            this->label_Where->Size = System::Drawing::Size(42, 13);
            this->label_Where->TabIndex = 12;
            this->label_Where->Text = L"Where:";
            // 
            // label_Result
            // 
            this->label_Result->AutoSize = true;
            this->label_Result->Location = System::Drawing::Point(290, 107);
            this->label_Result->Name = L"label_Result";
            this->label_Result->Size = System::Drawing::Size(40, 13);
            this->label_Result->TabIndex = 15;
            this->label_Result->Text = L"Result:";
            // 
            // textBox_Result
            // 
            this->textBox_Result->Location = System::Drawing::Point(290, 122);
            this->textBox_Result->Multiline = true;
            this->textBox_Result->Name = L"textBox_Result";
            this->textBox_Result->Size = System::Drawing::Size(200, 49);
            this->textBox_Result->TabIndex = 14;
            // 
            // dataGridView_Results
            // 
            this->dataGridView_Results->AllowUserToAddRows = false;
            this->dataGridView_Results->AllowUserToDeleteRows = false;
            this->dataGridView_Results->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
            this->dataGridView_Results->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(16) {this->Column_0000, 
                this->Column_0001, this->Column_0010, this->Column_0011, this->Column_0100, this->Column_0101, this->Column_0110, this->Column_0111, 
                this->Column_1000, this->Column_1001, this->Column_1010, this->Column_1011, this->Column_1100, this->Column_1101, this->Column_1110, 
                this->Column_1111});
            this->dataGridView_Results->Location = System::Drawing::Point(12, 194);
            this->dataGridView_Results->Name = L"dataGridView_Results";
            this->dataGridView_Results->ReadOnly = true;
            this->dataGridView_Results->RowHeadersVisible = false;
            this->dataGridView_Results->Size = System::Drawing::Size(807, 117);
            this->dataGridView_Results->TabIndex = 16;
            // 
            // Column_0000
            // 
            this->Column_0000->HeaderText = L"0000";
            this->Column_0000->Name = L"Column_0000";
            this->Column_0000->ReadOnly = true;
            this->Column_0000->Width = 50;
            // 
            // Column_0001
            // 
            this->Column_0001->HeaderText = L"0001";
            this->Column_0001->Name = L"Column_0001";
            this->Column_0001->ReadOnly = true;
            this->Column_0001->Width = 50;
            // 
            // Column_0010
            // 
            this->Column_0010->HeaderText = L"0010";
            this->Column_0010->Name = L"Column_0010";
            this->Column_0010->ReadOnly = true;
            this->Column_0010->Width = 50;
            // 
            // Column_0011
            // 
            this->Column_0011->HeaderText = L"0011";
            this->Column_0011->Name = L"Column_0011";
            this->Column_0011->ReadOnly = true;
            this->Column_0011->Width = 50;
            // 
            // Column_0100
            // 
            this->Column_0100->HeaderText = L"0100";
            this->Column_0100->Name = L"Column_0100";
            this->Column_0100->ReadOnly = true;
            this->Column_0100->Width = 50;
            // 
            // Column_0101
            // 
            this->Column_0101->HeaderText = L"0101";
            this->Column_0101->Name = L"Column_0101";
            this->Column_0101->ReadOnly = true;
            this->Column_0101->Width = 50;
            // 
            // Column_0110
            // 
            this->Column_0110->HeaderText = L"0110";
            this->Column_0110->Name = L"Column_0110";
            this->Column_0110->ReadOnly = true;
            this->Column_0110->Width = 50;
            // 
            // Column_0111
            // 
            this->Column_0111->HeaderText = L"0111";
            this->Column_0111->Name = L"Column_0111";
            this->Column_0111->ReadOnly = true;
            this->Column_0111->Width = 50;
            // 
            // Column_1000
            // 
            this->Column_1000->HeaderText = L"1000";
            this->Column_1000->Name = L"Column_1000";
            this->Column_1000->ReadOnly = true;
            this->Column_1000->Width = 50;
            // 
            // Column_1001
            // 
            this->Column_1001->HeaderText = L"1001";
            this->Column_1001->Name = L"Column_1001";
            this->Column_1001->ReadOnly = true;
            this->Column_1001->Width = 50;
            // 
            // Column_1010
            // 
            this->Column_1010->HeaderText = L"1010";
            this->Column_1010->Name = L"Column_1010";
            this->Column_1010->ReadOnly = true;
            this->Column_1010->Width = 50;
            // 
            // Column_1011
            // 
            this->Column_1011->HeaderText = L"1011";
            this->Column_1011->Name = L"Column_1011";
            this->Column_1011->ReadOnly = true;
            this->Column_1011->Width = 50;
            // 
            // Column_1100
            // 
            this->Column_1100->HeaderText = L"1100";
            this->Column_1100->Name = L"Column_1100";
            this->Column_1100->ReadOnly = true;
            this->Column_1100->Width = 50;
            // 
            // Column_1101
            // 
            this->Column_1101->HeaderText = L"1101";
            this->Column_1101->Name = L"Column_1101";
            this->Column_1101->ReadOnly = true;
            this->Column_1101->Width = 50;
            // 
            // Column_1110
            // 
            this->Column_1110->HeaderText = L"1110";
            this->Column_1110->Name = L"Column_1110";
            this->Column_1110->ReadOnly = true;
            this->Column_1110->Width = 50;
            // 
            // Column_1111
            // 
            this->Column_1111->HeaderText = L"1111";
            this->Column_1111->Name = L"Column_1111";
            this->Column_1111->ReadOnly = true;
            this->Column_1111->Width = 50;
            // 
            // button_FileCRCs
            // 
            this->button_FileCRCs->Location = System::Drawing::Point(496, 81);
            this->button_FileCRCs->Name = L"button_FileCRCs";
            this->button_FileCRCs->Size = System::Drawing::Size(75, 23);
            this->button_FileCRCs->TabIndex = 17;
            this->button_FileCRCs->Text = L"FileCRCs";
            this->button_FileCRCs->UseVisualStyleBackColor = true;
            this->button_FileCRCs->Click += gcnew System::EventHandler(this, &Form1::button_FileCRCs_Click);
            // 
            // textBox_timespan
            // 
            this->textBox_timespan->Location = System::Drawing::Point(496, 110);
            this->textBox_timespan->Name = L"textBox_timespan";
            this->textBox_timespan->Size = System::Drawing::Size(75, 20);
            this->textBox_timespan->TabIndex = 18;
            // 
            // textBox_numlines
            // 
            this->textBox_numlines->Location = System::Drawing::Point(496, 136);
            this->textBox_numlines->Name = L"textBox_numlines";
            this->textBox_numlines->Size = System::Drawing::Size(75, 20);
            this->textBox_numlines->TabIndex = 19;
            // 
            // button_Query2
            // 
            this->button_Query2->Location = System::Drawing::Point(93, 136);
            this->button_Query2->Name = L"button_Query2";
            this->button_Query2->Size = System::Drawing::Size(75, 23);
            this->button_Query2->TabIndex = 20;
            this->button_Query2->Text = L"Query2";
            this->button_Query2->UseVisualStyleBackColor = true;
            this->button_Query2->Click += gcnew System::EventHandler(this, &Form1::button_Query2_Click);
            // 
            // button_QueryAll
            // 
            this->button_QueryAll->Location = System::Drawing::Point(174, 136);
            this->button_QueryAll->Name = L"button_QueryAll";
            this->button_QueryAll->Size = System::Drawing::Size(75, 23);
            this->button_QueryAll->TabIndex = 21;
            this->button_QueryAll->Text = L"Query All";
            this->button_QueryAll->UseVisualStyleBackColor = true;
            this->button_QueryAll->Click += gcnew System::EventHandler(this, &Form1::button_QueryAll_Click);
            // 
            // button_LoadValues
            // 
            this->button_LoadValues->Location = System::Drawing::Point(732, 55);
            this->button_LoadValues->Name = L"button_LoadValues";
            this->button_LoadValues->Size = System::Drawing::Size(75, 23);
            this->button_LoadValues->TabIndex = 23;
            this->button_LoadValues->Text = L"Load Values";
            this->button_LoadValues->UseVisualStyleBackColor = true;
            this->button_LoadValues->Click += gcnew System::EventHandler(this, &Form1::button_LoadValues_Click);
            // 
            // textBox_ValueFile
            // 
            this->textBox_ValueFile->Location = System::Drawing::Point(577, 58);
            this->textBox_ValueFile->Name = L"textBox_ValueFile";
            this->textBox_ValueFile->Size = System::Drawing::Size(155, 20);
            this->textBox_ValueFile->TabIndex = 22;
            this->textBox_ValueFile->Text = L"20160903_161256.txt";
            // 
            // button_LoadEvents
            // 
            this->button_LoadEvents->Location = System::Drawing::Point(732, 84);
            this->button_LoadEvents->Name = L"button_LoadEvents";
            this->button_LoadEvents->Size = System::Drawing::Size(75, 23);
            this->button_LoadEvents->TabIndex = 25;
            this->button_LoadEvents->Text = L"Load Events";
            this->button_LoadEvents->UseVisualStyleBackColor = true;
            this->button_LoadEvents->Click += gcnew System::EventHandler(this, &Form1::button_LoadEvents_Click);
            // 
            // textBox_Events
            // 
            this->textBox_Events->Location = System::Drawing::Point(577, 84);
            this->textBox_Events->Name = L"textBox_Events";
            this->textBox_Events->Size = System::Drawing::Size(155, 20);
            this->textBox_Events->TabIndex = 24;
            this->textBox_Events->Text = L"C:\\Users\\Kevin\\Documents\\retrosheet\\data\\procEvents.txt";
            // 
            // button_Calculate
            // 
            this->button_Calculate->Location = System::Drawing::Point(732, 113);
            this->button_Calculate->Name = L"button_Calculate";
            this->button_Calculate->Size = System::Drawing::Size(75, 23);
            this->button_Calculate->TabIndex = 26;
            this->button_Calculate->Text = L"Calculate";
            this->button_Calculate->UseVisualStyleBackColor = true;
            this->button_Calculate->Click += gcnew System::EventHandler(this, &Form1::button_Calculate_Click);
            // 
            // button_Calculate2
            // 
            this->button_Calculate2->Location = System::Drawing::Point(732, 142);
            this->button_Calculate2->Name = L"button_Calculate2";
            this->button_Calculate2->Size = System::Drawing::Size(75, 23);
            this->button_Calculate2->TabIndex = 27;
            this->button_Calculate2->Text = L"Calculate 2";
            this->button_Calculate2->UseVisualStyleBackColor = true;
            this->button_Calculate2->Click += gcnew System::EventHandler(this, &Form1::button_Calculate2_Click);
            // 
            // button_BasesPer
            // 
            this->button_BasesPer->Location = System::Drawing::Point(12, 165);
            this->button_BasesPer->Name = L"button_BasesPer";
            this->button_BasesPer->Size = System::Drawing::Size(75, 23);
            this->button_BasesPer->TabIndex = 28;
            this->button_BasesPer->Text = L"Bases Per";
            this->button_BasesPer->UseVisualStyleBackColor = true;
            this->button_BasesPer->Click += gcnew System::EventHandler(this, &Form1::button_BasesPer_Click);
            // 
            // Form1
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(819, 336);
            this->Controls->Add(this->button_BasesPer);
            this->Controls->Add(this->button_Calculate2);
            this->Controls->Add(this->button_Calculate);
            this->Controls->Add(this->button_LoadEvents);
            this->Controls->Add(this->textBox_Events);
            this->Controls->Add(this->button_LoadValues);
            this->Controls->Add(this->textBox_ValueFile);
            this->Controls->Add(this->button_QueryAll);
            this->Controls->Add(this->button_Query2);
            this->Controls->Add(this->textBox_numlines);
            this->Controls->Add(this->textBox_timespan);
            this->Controls->Add(this->button_FileCRCs);
            this->Controls->Add(this->dataGridView_Results);
            this->Controls->Add(this->label_Result);
            this->Controls->Add(this->textBox_Result);
            this->Controls->Add(this->button_Query);
            this->Controls->Add(this->label_Where);
            this->Controls->Add(this->textBox_Where);
            this->Controls->Add(this->button_StopWork);
            this->Controls->Add(this->button_CloseDB);
            this->Controls->Add(this->statusStrip);
            this->Controls->Add(this->button_DoWork);
            this->Controls->Add(this->textBox_Connection);
            this->Controls->Add(this->textBox_Folder);
            this->Controls->Add(this->button_Dir);
            this->Controls->Add(this->button_OpenDB);
            this->Controls->Add(this->menuStrip1);
            this->MainMenuStrip = this->menuStrip1;
            this->Name = L"Form1";
            this->Text = L"Form1";
            this->statusStrip->ResumeLayout(false);
            this->statusStrip->PerformLayout();
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->dataGridView_Results))->EndInit();
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
    private: static const System::String^ State0Val =
                 "Outs * 16 +"
                 "iif(thirdRunner is null,0,8) +"
                 "iif(secondRunner is null,0,4) +"
                 "iif(firstRunner is null,0,2) + 1";
    private: static const System::String^ State1Val =
                 "(Outs + OutsOnPlay) * 16 +"
                 "iif(batterEventFlag='T',iif(batterDest>0 and batterDest<4,power(2,batterDest),0),1) +"
                 "iif(runnerOn1stDest>0 and runnerOn1stDest<4,power(2,runnerOn1stDest),0) +"
                 "iif(runnerOn2ndDest>0 and runnerOn2ndDest<4,power(2,runnerOn2ndDest),0) +"
                 "iif(runnerOn3rdDest>0 and runnerOn3rdDest<4,power(2,runnerOn3rdDest),0)";
    private: Retro::RetroDB^ m_DB;
    private: System::Void button_OpenDB_Click(System::Object^  sender, System::EventArgs^  e) {
                 m_DB = gcnew Retro::RetroDB(textBox_Connection->Text, 0);
/*
                                             (System::Int32) Retro::RetroDB::EntityType::Files |
                                             (System::Int32) Retro::RetroDB::EntityType::Games |
                                             (System::Int32) Retro::RetroDB::EntityType::Players |
                                             (System::Int32) Retro::RetroDB::EntityType::Teams);
*/
             }
    private: System::Void button_Dir_Click(System::Object^  sender, System::EventArgs^  e) {
                 if (folderBrowserDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                 {
                     textBox_Folder->Text = folderBrowserDialog->SelectedPath;
                 }
             }
    private: System::Void backgroundWorker_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
             System::DateTime start = System::DateTime::Now;
             array<System::String^>^ files = System::IO::Directory::GetFiles(e->Argument->ToString(), "2015*.EV?");
             System::Diagnostics::Debug::Print ("files->Length = " + files->Length);
             System::Diagnostics::ProcessStartInfo^ ps =
                 gcnew System::Diagnostics::ProcessStartInfo
                       (L"C:\\Users\\Kevin\\Documents\\retrosheet\\data\\BEVENT.EXE");
             ps->CreateNoWindow         = true;
             ps->RedirectStandardOutput = true;
             ps->UseShellExecute        = false;
             ps->WorkingDirectory       = textBox_Folder->Text;
             System::Diagnostics::Debug::Print (ps->FileName);
             System::Diagnostics::Debug::Print (ps->WorkingDirectory);
             m_KeepWorking = true;
             numlines = 0;
             for (System::Int32 i=0; m_KeepWorking && i<files->Length; i++)
             {
                 backgroundWorker->ReportProgress(100*i/files->Length);
                 System::IO::BinaryReader^ rdr = gcnew System::IO::BinaryReader(System::IO::File::OpenRead(files[i]));
                 array<System::Byte>^ bytes = rdr->ReadBytes((System::Int32) rdr->BaseStream->Length);
                 System::Int32 CRC = CRC32::CRC32::calc(bytes);
                 rdr->BaseStream->Seek(0,System::IO::SeekOrigin::Begin);
                 rdr->Close();
                 System::IO::TextReader^ trdr = gcnew System::IO::StreamReader(files[i]);
                 while (trdr->ReadLine() != nullptr) numlines++;
                 trdr->Close();
                 //goto end;
                 System::String^ fname = files[i]->Substring(files[i]->LastIndexOf('\\')+1);
                 System::Int32 fID;
                 if (m_DB->AddEventFile (fname, fID, CRC))
                 {
                     ps->Arguments = "-y " +
                                     files[i]->Substring(files[i]->LastIndexOf('\\')+1,4) +
                                     " -f 0-96 " +
                                     fname;
                     System::Diagnostics::Debug::Print ("i = " + i + " args = " + ps->Arguments);
                     System::Diagnostics::Process ^p = System::Diagnostics::Process::Start (ps);
                     System::String^ str = p->StandardOutput->ReadToEnd();
                     p->WaitForExit();
                     System::Int32 count = 1;
#if 1
                     array<System::String^>^ delimit = gcnew array<System::String^>(1);
                     delimit[0] = System::Environment::NewLine;
                     array<System::String^>^ events = str->Split(delimit, System::StringSplitOptions::RemoveEmptyEntries);
                     for each (System::String^ str in events)
                     {
                         System::Diagnostics::Trace::Assert
                           (m_DB->AddRetroEvent
                            (fID,
                             gcnew Retro::RetroEvent(str,count)));
                         count++;
                     }
#else
                     System::Int32 start = 0, end;
                     while ((end = str->IndexOf(System::Environment::NewLine, start)) != -1)
                     {
                         System::Diagnostics::Trace::Assert
                           (m_DB->AddRetroEvent
                            (fID,
                             gcnew Retro::RetroEvent(str->Substring(start, end-start),count)));
                         count++;
                         start = end + System::Environment::NewLine->Length;
                     }
#endif
                     //m_DB->Clear();
                 }
end:;
             }
             System::DateTime end = System::DateTime::Now;
             span = end - start;
         }
    System::TimeSpan span;
    System::Int32 numlines;
    private: System::Void button_DoWork_Click(System::Object^  sender, System::EventArgs^  e) {
             backgroundWorker->RunWorkerAsync(textBox_Folder->Text);
         }
    private: System::Void backgroundWorker_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
             toolStripProgressBar->Value = e->ProgressPercentage;
         }
    private: System::Void backgroundWorker_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
             textBox_timespan->Text = span.ToString();
             textBox_numlines->Text = numlines.ToString();
             toolStripProgressBar->Value = 0;
         }
    private: System::Void button_CloseDB_Click(System::Object^  sender, System::EventArgs^  e) {
                 m_DB->Close();
             }
    private: System::Boolean m_KeepWorking;
    private: System::Void button_StopWork_Click(System::Object^  sender, System::EventArgs^  e) {
                 m_KeepWorking = false;
         }
    private: System::Void Dump (array<System::Double, 2>^ a)
        {
            for (System::Int32 i=0; i<a->GetLength(0); i++)
            {
                for (System::Int32 j=0; j<a->GetLength(1); j++)
                {
                    System::Diagnostics::Debug::Write (a[i,j] + "\t");
                }
                System::Diagnostics::Debug::Print("");
            }
        }
    private: System::Void Dump (array<System::Double, 1>^ r)
        {
            for (System::Int32 j=0; j<r->GetLength(0); j++)
            {
                System::Diagnostics::Debug::Print ("x[" + j.ToString("D2") + "]\t" +
                                                   r[j].ToString("F3"));
            }
        }
private: array<System::Double, 1>^ b;
private: System::Void button_Query_Click(System::Object^  sender, System::EventArgs^  e) {
             System::String^ SQL = "SELECT " + State0Val + " AS State0," + System::Environment::NewLine +
                                               State1Val + " AS State1," + System::Environment::NewLine +
                                               "COUNT(" + State0Val + ") AS Freq" + System::Environment::NewLine +
                                  " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(textBox_Where->Text))
             {
                SQL += " WHERE (" + textBox_Where->Text + ")" + System::Environment::NewLine;
             }
             SQL += " GROUP BY " + State0Val + "," + State1Val + System::Environment::NewLine +
                    " ORDER BY " + State0Val + "," + State1Val;

             System::Diagnostics::Debug::Print (SQL);
             System::Collections::Generic::List<Retro::ResultType^>^ list = m_DB->Query(SQL);
             textBox_Result->Clear();
             array<System::Double,2>^ freq = gcnew array<System::Double,2>(64,64);
             freq->Initialize();
             b = gcnew array<System::Double, 1>(freq->GetLength(0));
             array<System::Double, 1>^ start =
                                      gcnew array<System::Double, 1>(freq->GetLength(0));
             b->Initialize();
             start->Initialize();
             for each (Retro::ResultType^ r in list)
             {
                 System::String^ txt = r->m_Start.ToString() + "," +
                                       r->m_End.ToString() + "," +
                                       r->m_Freq.ToString();
                 textBox_Result->Text += (txt + System::Environment::NewLine);
                 //System::Diagnostics::Debug::Print(txt);
                 freq[r->m_Start,r->m_End] = (System::Double) r->m_Freq;
                 b[r->m_Start] -= freq[r->m_Start,r->m_End] * Value(r->m_Start, r->m_End);
                 //System::Diagnostics::Debug::Assert ((r->m_End  & 1) == 0);
                 if (((r->m_End  & 1) == 0) &&
                     ((r->m_End >> 4) < 3 ))
                 {
                     freq[r->m_End, r->m_End | 1] += r->m_Freq;
                     start[r->m_End] += r->m_Freq;
                 }
                 start[r->m_Start] += freq[r->m_Start,r->m_End];
             }
             for (System::Int32 i=0; i<start->Length; i++)
             {
                 freq[i,i] -= start[i];
                 if (start[i] == 0)
                 {
                     freq[i,i] = -1.0;
                 }
             }
             Dump (freq);
             //Dump (b);
             GJ_Solve (freq, b);
             //Dump (b); // this is replaced below
             System::String^ line = "";
             System::String^ name = System::DateTime::Now.ToString("yyyyMMdd_HHmmss") + ".txt";
             System::IO::TextWriter ^wtr = gcnew System::IO::StreamWriter(name);
             for (System::Int32 i=0; i<b->Length; i++)
             {
                 dataGridView_Results->Rows[i>>4]->Cells[i&15]->Value = b[i].ToString("0.000");
                 wtr->Write (b[i].ToString() + "\t");
                 if ((i&1)==0) continue;
                 //line += b[i].ToString("0.000");
                 line += b[i].ToString("0.00");
                 System::Diagnostics::Debug::Assert (b[i] == b[i-1]);
                 if (((i/*>>1*/) & 15) == 15)
                 {
                     System::Diagnostics::Debug::Print (line);
                     line = "";
                     wtr->WriteLine();
                 }
                 else
                 {
                     //wtr->Write("\t");
                     line += "\t";
                 }
             }
             wtr->WriteLine (SQL);
             wtr->Close();
         }
         System::Int32 aValue (System::Int32 s)
         {
             return (s>>4) + ((s&1)?1:0) + ((s&2)?1:0) + ((s&4)?1:0) + ((s&8)?1:0);
         }
         System::Int32 Value (System::Int32 From, System::Int32 To)
         {
              System::Int32 ToUsed = (To>>4) + (To&1?1:0) + (To&2?1:0) + (To&4?1:0) + (To&8?1:0);
              System::Int32 FrUsed = (From>>4) + (From&1?1:0) + (From&2?1:0) + (From&4?1:0) + (From&8?1:0);
              System::Diagnostics::Debug::Assert (ToUsed == aValue(To));
              System::Diagnostics::Debug::Assert (FrUsed == aValue(From));
              System::Diagnostics::Debug::Assert ((From&1) == 1);
              //System::Diagnostics::Debug::Assert ((To&1)   == 0);
              //if (((From & 1) == 0) &&
              //    ((To   & 1) == 1))
              //    FrUsed++;
              return FrUsed-ToUsed;
         }
private: System::Void button_FileCRCs_Click(System::Object^  sender, System::EventArgs^  e) {
             array<System::String^>^ files = System::IO::Directory::GetFiles(textBox_Folder->Text, "*.EV?");
             System::Diagnostics::Debug::Print ("files->Length = " + files->Length);
             for each (System::String^ str in files)
             {
                 System::String^ fname = str->Substring(str->LastIndexOf('\\')+1);
                 System::IO::BinaryReader^ rdr = gcnew System::IO::BinaryReader(System::IO::File::OpenRead(str));
                 System::Diagnostics::Debug::Assert
                        (m_DB->UpdateFileCRC
                        (fname, CRC32::
                        CRC32::
                        calc(rdr->ReadBytes((System::Int32) rdr->BaseStream->Length))));
                 rdr->Close();
             }
         }
private: System::Double WhereValue (System::String^ wh)
         {
             System::String^ SQL = "SELECT " + State0Val + " AS State0," + System::Environment::NewLine +
                                               "COUNT(" + State0Val + ") AS Freq," + System::Environment::NewLine +
                                               "0 AS S" + System::Environment::NewLine +
#if 0
                                  " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine +
                                  " INNER JOIN [Retrosheet].[dbo].[rawPlayers] ON " + System::Environment::NewLine +
                                  " [Retrosheet].[dbo].[rawEvents].[Batter]=[Retrosheet].[dbo].[RetroID]";
             SQL += " WHERE rawPlayers.Value is not null" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(wh))
             {
                SQL += " AND (" + wh + ")" + System::Environment::NewLine;
             }
#else
             " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(wh))
             {
                SQL += " WHERE (" + wh + ")" + System::Environment::NewLine;
             }
#endif
             SQL += " GROUP BY " + State0Val + System::Environment::NewLine +
                    " UNION" + System::Environment::NewLine +
                     "SELECT " + State1Val + " AS State1," + System::Environment::NewLine +
                                 "COUNT(" + State1Val + ") AS Freq," + System::Environment::NewLine +
                                 "1 AS S" + System::Environment::NewLine +
#if 0
                    " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine +
                    " INNER JOIN [Retrosheet].[dbo].[rawPlayers] ON " + System::Environment::NewLine +
                    " [Retrosheet].[dbo].[rawEvents].[Batter]=[Retrosheet].[dbo].[RetroID]";
             SQL += " WHERE rawPlayers.Value is not null" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(wh))
             {
                SQL += " AND (" + wh + ")" + System::Environment::NewLine;
             }
#else
             " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(wh))
             {
                SQL += " WHERE (" + wh + ")" + System::Environment::NewLine;
             }
#endif
             SQL += " GROUP BY " + State1Val;
             //System::Diagnostics::Debug::Print (SQL);
             System::Collections::Generic::List<Retro::StateFreqType^>^ list = m_DB->QueryFreq(SQL);
             array<System::Int32>^ f = gcnew array<System::Int32>(2);
             System::Int32 Runs = 0;
             System::Double Val = 0.;
             f->Initialize();
             for each (Retro::StateFreqType^ r in list)
             {
                 Runs += aValue(r->m_State) * r->m_Freq * (1-2*r->m_StartEnd);
                 f[r->m_StartEnd] += r->m_Freq;
                 Val += b[r->m_State] * r->m_Freq * (2*r->m_StartEnd-1);
             }
             System::Diagnostics::Debug::Assert (f[0] == f[1]);
             Val += Runs;
             Val /= f[0];
             return Val;
         }
private: System::Void button_Query2_Click(System::Object^  sender, System::EventArgs^ e) {
             WhereValue(textBox_Where->Text);
         }
private: System::Void button_QueryAll_Click(System::Object^  sender, System::EventArgs^  e) {
             System::Collections::Generic::List<System::String^>^ playerList = m_DB->AllPlayers();
             //textBox_Result->Clear();
             for each (System::String^ p in playerList)
             {
                 m_DB->AddIndivResult(p, WhereValue("batter='" + p + "'"));
                 //System::String^ Out = p + "= " + WhereValue("batter='" + p + "'");
                 //System::Diagnostics::Debug::WriteLine (Out);
                 //textBox_Result->Text += Out + System::Environment::NewLine;
             }
         }
private: array<System::Double,2>^ TransitionValues;
private: System::Void button_LoadValues_Click(System::Object^  sender, System::EventArgs^  e) {
             System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(textBox_ValueFile->Text);
             b = gcnew array<System::Double, 1>(64);
             for (System::Int32 i=0; i<4; i++)
             {
                 array<System::String^>^ strs = rdr->ReadLine()->Split('\t');
                 for (System::Int32 j=0; j<16; j++)
                 {
                     dataGridView_Results->Rows[i]->Cells[j]->Value = strs[j];
                     b[16*i+j] = System::Convert::ToDouble(strs[j]);
                 }
             }
             rdr->Close();
             TransitionValues = gcnew array<System::Double,2>   (64,64);
             for (System::Int32 i=0; i<64; i++)
             {
                 for (System::Int32 j=0; j<64; j++)
                 {
                     TransitionValues[i,j] = b[j] - b[i] + aValue(i) - aValue(j);
                 }
             }
         }
         private: System::Collections::Generic::List<EventData^>^ EventList;
private: System::Void button_LoadEvents_Click(System::Object^  sender, System::EventArgs^  e) {
             System::IO::TextReader^ rdr = gcnew System::IO::StreamReader(textBox_Events->Text);
             EventList = gcnew System::Collections::Generic::List<EventData^>;
             System::String^ line;
             while ((line = rdr->ReadLine()) != nullptr)
             {
                 array<System::String^>^ split = line->Split('\t');
                 EventData^ ev = gcnew EventData;
                 ev->BatID = System::Convert::ToInt32 (split[8]);
                 ev->PitID = System::Convert::ToInt32 (split[9]);
                 ev->Value = TransitionValues[System::Convert::ToInt32 (split[4]),
                                              System::Convert::ToInt32 (split[5])];
                 ev->OutDelta = ((System::Convert::ToInt32 (split[5]))>>4) -
                                ((System::Convert::ToInt32 (split[4]))>>4);
                 EventList->Add(ev);
             }
             rdr->Close();
             System::Diagnostics::Debug::Print ("button_LoadEvents_Click complete");
         }
private: System::Void button_Calculate_Click(System::Object^  sender, System::EventArgs^  e) {
             System::Collections::Generic::List<System::Int32>^ playerList = m_DB->AllPlayerIDs();
             for each (System::Int32 p in playerList)
             {
                 System::Int32 freq = 0;
                 System::Double total = 0.;
                 for each (EventData^ ev in EventList)
                 {
                     if (ev->BatID == p)
                     {
                         freq++;
                         total += ev->Value;
                     }
                 }
                 m_DB->AddIndivIDResult(p, freq, total);
             }
         }
private: System::Void button_Calculate2_Click(System::Object^  sender, System::EventArgs^  e) {
             System::Collections::Generic::List<System::Int32>^ playerList = m_DB->AllPlayerIDs();
             for each (System::Int32 p in playerList)
             {
                 System::Int32 freq = 0;
                 System::Double total = 0.;
                 for each (EventData^ ev in EventList)
                 {
                     if (ev->PitID == p)
                     {
                         freq += ev->OutDelta;
                         total += ev->Value;
                     }
                 }
                 m_DB->AddIndivIDPitchResult(p, freq, total);
             }
         }
private: System::Int32 numBases (System::Int32 s)
         {
             return ((s&2)?1:0) + ((s&4)?2:0) + ((s&8)?3:0);
         }
private: System::Int32 BaseDiff (System::Int32 from, System::Int32 to)
         {
             return numBases(to)-numBases(from)+4*Value(from, to);
         }
private: System::Void button_BasesPer_Click(System::Object^  sender, System::EventArgs^  e) {
             System::String^ SQL = "SELECT " + State0Val + " AS State0," + System::Environment::NewLine +
                                               State1Val + " AS State1," + System::Environment::NewLine +
                                               "COUNT(" + State0Val + ") AS Freq" + System::Environment::NewLine +
                                  " FROM [Retrosheet].[dbo].[rawEvents]" + System::Environment::NewLine;
             if (!System::String::IsNullOrEmpty(textBox_Where->Text))
             {
                SQL += " WHERE (" + textBox_Where->Text + ")" + System::Environment::NewLine;
             }
             SQL += " GROUP BY " + State0Val + "," + State1Val + System::Environment::NewLine +
                    " ORDER BY " + State0Val + "," + State1Val;
             System::Diagnostics::Debug::Print (SQL);
             System::Collections::Generic::List<Retro::ResultType^>^ list = m_DB->Query(SQL);
             System::Int32 bases, freq;
             bases = 0;
             freq = 0;
             for each (Retro::ResultType^ r in list)
             {
                 freq  += r->m_Freq;
                 bases += BaseDiff(r->m_Start, r->m_End)*r->m_Freq;
             }
             System::Diagnostics::Debug::Print (((System::Double) bases / (System::Double) freq).ToString());
         }
};
}
