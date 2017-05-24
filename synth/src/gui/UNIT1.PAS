// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI window 1

{$A+,B-,C-,D+,E-,F-,G+,H+,I+,J+,K-,L+,M-,N+,O-,P+,Q-,R-,S-,T-,U-,V+,W-,X+,Y-,Z1}
{$MINSTACKSIZE $00004000}
{$MAXSTACKSIZE $00100000}
{$IMAGEBASE $00400000}
{$APPTYPE GUI}
{$A+,B-,C-,D+,E-,F-,G+,H+,I+,J+,K-,L+,M-,N+,O-,P+,Q-,R-,S-,T-,U-,V+,W-,X+,Y-,Z1}
{$MINSTACKSIZE $00004000}
{$MAXSTACKSIZE $00100000}
{$IMAGEBASE $00400000}
{$APPTYPE GUI}
{DEFINE DELETE_TMP_SCRIPT}
{$define USE_SHAPE_COEF}
unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, StdCtrls, ExtCtrls, MMSystem, ShellAPI;

const
  HELP_KEY = 112;    // F1
  PLAY_KEY = 116;    // F5
  PLAY_KEY2= 117;    // F6
  STOP_KEY = 119;    // F8
  LOAD_KEY = 114;    // F3
  SAVE_KEY = 113;    // F2

  EFF_BUTTON_TAG = 190;
type
  TForm1 = class(TForm)
    Synt_Pages: TPageControl;
    Oscill_Page: TTabSheet;
    Seq_Page: TTabSheet;
    Eff_Page: TTabSheet;
    Sample_View: TImage;
    Play_Button: TButton;
    Load_Button: TButton;
    Save_Button: TButton;
    Sequencer_Notes: TListBox;
    Notes_Num_Select: TScrollBar;
    Notes_Num_Label: TLabel;
    Notes_Num_Show: TLabel;
    Button_Stop: TButton;
    OpenDialog1: TOpenDialog;
    SaveDialog1: TSaveDialog;
    Misc_Page: TTabSheet;
    Sample_Speed_Show: TLabel;
    Sample_Speed_Input: TScrollBar;
    Sample_Tempo_Show: TLabel;
    Sample_Tempo_Input: TScrollBar;
    Sample_Note_Len_Show: TLabel;
    Sample_Note_Len_Input: TScrollBar;
    Sample_Tempo_Label: TLabel;
    Sample_Speed_Label: TLabel;
    Sample_Note_Len_Label: TLabel;
    Cust_Sound_Page: TTabSheet;
    Cust_Sound_Amp_Env_Button: TButton;
    Cust_Sound_Freq_Env_Button: TButton;
    Cust_Sound_Enabled: TCheckBox;
    Clear_Button: TButton;
    Octave_Select: TScrollBar;
    Octave_Select_Show: TLabel;
    Octave_Select_Label: TLabel;
    Osc_Select_Group: TRadioGroup;
    Seq_Amp_Env_Button: TButton;
    Seq_Enabled: TCheckBox;
    Harm_Enable_Button: TCheckBox;
    Eff_List: TListBox;
    Eff_Add_Button: TButton;
    Del_Eff_Button: TButton;
    Eff_Type_Select: TListBox;
    Label1: TLabel;
    Eff_Edit_Box: TGroupBox;
    Frw_Btn: TButton;
    Bk_Btn: TButton;
    Seq_Notes_Rep_Inp: TScrollBar;
    Seq_Notes_Rep_Num: TLabel;
    Seq_Notes_Rep_Num_Lb: TLabel;
    Sample_Notes_Len_Show2: TLabel;
    Osc_Label: TLabel;
    Flang_Coef_Show: TLabel;
    Label2: TLabel;
    Random_Notes_Btn: TButton;
    Dies_Enable: TCheckBox;
    Cust_Wave_Edit_Btn: TButton;
    Shape_Label: TLabel;
    Shape_Show: TLabel;
    Pitch_Speed_Select: TScrollBar;
    Pitch_Speed_Lb: TLabel;
    Pitch_Speed_Show: TLabel;
    Attack_Len_Scroll: TScrollBar;
    Decay_Len_Scroll: TScrollBar;
    Att_Len_Lb: TLabel;
    Dec_Len_Lb: TLabel;
    Att_Len_Show: TLabel;
    Dec_Len_Show: TLabel;
    Pitch_Btn: TButton;
    Sustain_Btn: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure Save_ButtonClick(Sender: TObject);
    procedure Play_ButtonClick(Sender: TObject);
    procedure Button_StopClick(Sender: TObject);
    procedure Load_ButtonClick(Sender: TObject);
    procedure Sample_Speed_InputChange(Sender: TObject);
    procedure Sample_Tempo_InputChange(Sender: TObject);
    procedure Sample_Note_Len_InputChange(Sender: TObject);
    procedure FormKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure Cust_Sound_Amp_Env_ButtonClick(Sender: TObject);
    procedure Cust_Sound_Freq_Env_ButtonClick(Sender: TObject);
    procedure Cust_Sound_EnabledClick(Sender: TObject);
    procedure Clear_ButtonClick(Sender: TObject);
    procedure Octave_SelectChange(Sender: TObject);
    procedure Sequencer_NotesKeyPress(Sender: TObject; var Key: Char);
    procedure Osc_Select_GroupClick(Sender: TObject);
    procedure Seq_Amp_Env_ButtonClick(Sender: TObject);
    procedure Notes_Num_SelectScroll(Sender: TObject;
      ScrollCode: TScrollCode; var ScrollPos: Integer);
    procedure Seq_EnabledClick(Sender: TObject);
    procedure Sequencer_NotesKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure Harm_Enable_ButtonEnter(Sender: TObject);
    procedure Eff_Add_ButtonClick(Sender: TObject);
    procedure Del_Eff_ButtonClick(Sender: TObject);
    procedure Eff_ListClick(Sender: TObject);
    procedure Frw_BtnClick(Sender: TObject);
    procedure Bk_BtnClick(Sender: TObject);
    procedure Seq_Notes_Rep_InpChange(Sender: TObject);
    procedure Random_Notes_BtnClick(Sender: TObject);
    procedure Cust_Wave_Edit_BtnClick(Sender: TObject);
    procedure Pitch_Speed_SelectChange(Sender: TObject);
    procedure Attack_Len_ScrollChange(Sender: TObject);
    procedure Decay_Len_ScrollChange(Sender: TObject);
    procedure Pitch_BtnClick(Sender: TObject);
    procedure Sustain_BtnClick(Sender: TObject);
  private
    procedure Osc_Scroll_Change(Sender: TObject);
    procedure Osc_Enabled_Change(Sender: TObject);
    procedure Osc_Type_Change(Sender: TObject);
    procedure Update_All_Controls;
    procedure Harm_Scroll_Change(Sender: TObject; ScrollCode:
                                         TScrollCode; var ScrollPos: Integer);
    procedure Osc_Select_Change(Sender: TObject);
    procedure Osc_Flang_Change(Sender: TObject);
    procedure Osc_Shape_Change(Sender: TObject);
    { Private declarations }
  public
    { Public declarations }
  end;

  PStr = ^string;
  TSampleArray = array [1..22050*9+1000] of word;
  PSampleArray = ^TSampleArray;
  TGenSample = procedure(Script_Ptr: PStr; Script_Len: integer; Wav_Data: PSampleArray;
  var Wav_Data_Size: integer); stdcall;
var
  Form1: TForm1;
  sample_size: integer;
  GenSample: TGenSample;
  sample: TSampleArray;

implementation

uses Unit2;

{$R *.DFM}

{$I synt_def.pas}
{$I serv.pas}
{$I env_edit.pas}
var
  Oscill_Mix_Scrolls: array[0..OSCILL_NUM-1] of TScrollBar;
  Oscill_Enabled_Buttons: array[0..OSCILL_NUM-1] of TCheckBox;
  Oscill_Select_Buttons: array[0..OSCILL_NUM-1] of TRadioButton;
  Oscill_Type_Selects: array[0..OSCILL_NUM-1] of TComboBox;
  Oscill_Flang_Coefs: array[0..OSCILL_NUM-1] of TTrackBar;
  Oscill_Shape_Coefs: array[0..OSCILL_NUM-1] of TScrollBar;
  Harm_Scrolls: array[0..HARMONICS_NUM-1] of TScrollBar;
  Cur_Oscillator: integer;
  HLib: integer;

procedure TForm1.Osc_Scroll_Change(Sender: TObject);
begin        // В tag храним номер осц.,
             // к которому относится Scrollbar
    Oscillators[(Sender as TScrollBar).Tag].Oscill_Mix_Coef:=
                        (Sender as TScrollBar).Position/100;
end;

procedure TForm1.Osc_Flang_Change(Sender: TObject);
begin        // В tag храним номер осц.,
             // к которому относится Scrollbar
    (Sender as TTrackBar).SetTick(PERCENT3 div 2);
    Oscillators[(Sender as TTrackBar).Tag].Oscill_Flanger_Coef:=
                        (Sender as TTrackBar).Position;
  Flang_Coef_Show.Caption:=FloatToStrF((Sender as TTrackBar).Position/PERCENT3*0.2+0.9,
  ffGeneral,10,10);
end;

procedure TForm1.Osc_Shape_Change(Sender: TObject);
begin        // В tag храним номер осц.,
             // к которому относится Scrollbar
    Oscillators[(Sender as TScrollBar).Tag].Oscill_Shape_Coef:=
                        (Sender as TScrollBar).Position;
  Shape_Show.Caption:=FloatToStrF((Sender as TScrollBar).Position/PERCENT,
  ffGeneral,10,10);
end;

procedure TForm1.Osc_Enabled_Change(Sender: TObject);
{var
  Sn: TCheckBox;}
begin        // В tag context храним номер осц.,
             // к которому относится Scrollbar
    Oscillators[(Sender as TCheckBox).tag].Enabled:=
                                  (Sender as TCheckBox).Checked;
end;
procedure TForm1.Osc_Type_Change(Sender: TObject);
begin        // В tag context храним номер осц.,
             // к которому относится Scrollbar
    Oscillators[(Sender as TComboBox).tag].Oscill_Type:=
                                    (Sender as TComboBox).ItemIndex;
end;

procedure TForm1.Harm_Scroll_Change(Sender: TObject; ScrollCode:
                                         TScrollCode; var ScrollPos: Integer);
var
  i: integer;
begin        // В tag context храним номер осц.,
             // к которому относится Scrollbar
    i:=(Sender as TScrollBar).tag;
    if Cur_Oscillator>-1 then //Если осц. выбран, то
      Harmonizers[Cur_Oscillator].Harmonics_Mix_Coefs[i]:=
                   (100-Harm_Scrolls[i].Position)/100;
end;

procedure TForm1.Osc_Select_Change(Sender: TObject);
var
  i: integer;
begin        // В tag context храним номер осц.,
             // к которому относится Scrollbar
    i:=(Sender as TRadioButton).tag;
    Read_Harmonizer(Harmonizers[i],Harm_Scrolls);
    Cur_Oscillator:=i;
end;


procedure TForm1.Update_All_Controls;
var
  i,j: integer;
begin
  for i:=0 to OSCILL_NUM-1 do      // осцилляторы
  begin
    Oscill_Select_Buttons[i].Checked:=False;
    Oscill_Enabled_Buttons[i].Checked:=Oscillators[i].Enabled;
    Oscill_Mix_Scrolls[i].Position:=Round(
                                    Oscillators[i].Oscill_Mix_Coef*100);
    Oscill_Type_Selects[i].ItemIndex:=Oscillators[i].Oscill_Type;
    Oscill_Flang_Coefs[i].Position:=Oscillators[i].Oscill_Flanger_Coef;
    Oscill_Shape_Coefs[i].Position:=Oscillators[i].Oscill_Shape_Coef;    
  end;
  for j:=0 to HARMONICS_NUM-1 do
      Harm_Scrolls[j].Position:=100;

  Harm_Enable_Button.Checked:=False;
  Flang_Coef_Show.Caption:='';
  Shape_Show.Caption:='';
                                       // Секвенсор
  Notes_Num_Select.Position:=0;
  Notes_Num_Show.Caption:='0';
  Octave_Select.Position:=0;
  Octave_Select_Show.Caption:='0';
  Sequencer_Notes.Items.Clear;
  Seq_Notes_Rep_Inp.Position:=Sequencers[0].Seq_Notes_Repeat_Num;
  Seq_Notes_Rep_Num.Caption:=Num_To_Str(Seq_Notes_Rep_Inp.Position);

  with Sequencers[0] do
  begin
    if Sequencers[0].Enabled then
    begin
      Notes_Num_Select.Position:=Notes_Num;
      Notes_Num_Select.Max:=MAX_NOTES_NUM;
      Notes_Num_Show.Caption:=Num_To_Str(Notes_Num_Select.Position);
      Octave_Select.Position:=Cur_Octave;
      Octave_Select_Show.Caption:=Num_To_Str(Octave_Select.Position);
      if Notes_Num>0 then               // Ежели ноты есть
        for j:=0 to Notes_Num-1 do     // Добавляем их в список
          Sequencer_Notes.Items.Add(Notes[j]);
    end;
  end;
                                  // Эффекты
  Eff_List.Clear;
  for i:=0 to Effects.Count-1 do
    Eff_List.Items.Add(TEffect(Effects.Items[i]).Eff_Name);
    
  if Eff_Edit_Box.ControlCount>0 then
    begin    // Вытираем то, что было
      j:=Eff_Edit_Box.ControlCount-1;
      for i:= 0 to j do
        Eff_Edit_Box.Controls[0].Free;
    end;

  Seq_Enabled.Checked:=Sequencers[0].Enabled;
                                        // Все остальное
  Sample_Tempo_Input.Position:=Sample_Tempo;
  Sample_Tempo_Show.Caption:=Num_To_Str(Sample_Tempo);
  Sample_Speed_Input.Position:=Sample_Speed;
  Sample_Speed_Show.Caption:=Num_To_Str(Sample_Speed);
  Sample_Note_Len_Input.Position:=Sample_Note_Len;
  Sample_Note_Len_Show.Caption:=Num_To_Str(Sample_Note_Len);
  Cust_Sound_Enabled.Checked:=Custom_Sounds[0].Enabled;
  Cur_Oscillator:=-1;
  Pitch_Speed_Select.Min:=1;
  Pitch_Speed_Select.Max:=10;
  Pitch_Speed_Select.Position:=Pitch_Speed;
  Pitch_Speed_Show.Caption:=Num_To_Str(Pitch_Speed);

  Attack_Len_Scroll.Min:=0;
  Attack_Len_Scroll.Max:=100;
  Attack_Len_Scroll.Position:=Attack_Len;
  Att_Len_Show.Caption:=Num_To_Str(Attack_Len)+'%';
  Decay_Len_Scroll.Min:=0;
  Decay_Len_Scroll.Max:=100;
  Decay_Len_Scroll.Position:=Decay_Len;
  Dec_Len_Show.Caption:=Num_To_Str(Decay_Len)+'%';
end;

procedure TForm1.FormCreate(Sender: TObject);
var
  i,j,k :integer;
  ScrollBar, Scroll :TScrollBar;
  CheckBox :TCheckBox;
  ComboBox: TComboBox;
  RadioButton :TRadioButton;
  TrackBar: TTrackBar;
begin
  hLib:=LoadLibrary(SYNTH_DLL_NAME);
  if (hLib <= 0) then
  begin
    raise exception.create('Where''s my DLL (synth.dll) ?');
    halt;
  end;
  @GenSample:= GetProcAddress(hLib,'Generate');
  if not assigned(GenSample) then
    raise exception.Create('Bad DLL (synth.dll)');

  for i:=0 to OSCILL_NUM-1 do      // Создаем массив осцилляторов
  begin
    Oscillators[i]:=TOscillator.Create;
    Harmonizers[i]:=THarmonizer.Create; // Создаем гармонайзеры
  end;
  for i:=0 to CHANNELS_NUM-1 do      // Создаем массив секвенсеров
    Sequencers[i]:=TSequencer.Create;
  for i:=0 to CUST_SOUND_NUM-1 do      // Создаем массив звуков
    Custom_Sounds[i]:=TCust_Sound.Create;

  Effects:=TList.Create;       // Создаем массив указателей на эффекты
  Custom_Wave:=TEnvelope.Create;   //  Создаем custom wave

  for i:=0 to OSCILL_NUM-1 do     // Инитим списки осцилляторов
  begin
    RadioButton:=TRadioButton.Create(Osc_Select_Group);
    RadioButton.Parent:=Osc_Select_Group;
    RadioButton.OnClick:=Osc_Select_Change;
    RadioButton.tag:=i;
    RadioButton.Left:=10-8;
    RadioButton.Top:=22+i*25;
    Oscill_Select_Buttons[i]:=RadioButton;
                                  // Инитим двигалки микса у осцилляторов
    Scroll:=TScrollBar.Create(Osc_Select_Group);
    Scroll.Parent:=Osc_Select_Group;
    Scroll.OnChange:=Osc_Scroll_Change;
    Scroll.tag:=i;
    Scroll.Left:=100-50-15;
    Scroll.Top:=22+i*25;
    Scroll.Width:=110;
    Scroll.ShowHint:=True; Scroll.Hint:='Oscillator '+ Num_To_Str(i+1)+' mix coef';
    Oscill_Mix_Scrolls[i]:=Scroll;
    CheckBox:=TCheckBox.Create(Osc_Select_Group);
    CheckBox.Parent:=Osc_Select_Group;
    CheckBox.OnClick:=Osc_Enabled_Change;
    CheckBox.tag:=i;
    CheckBox.Left:=30-12;
    CheckBox.Width:=15;
    CheckBox.Top:=22+i*25;
    CheckBox.Caption:='';
    CheckBox.ShowHint:=True; CheckBox.Hint:='Enable/disable oscillator '+Num_To_Str(i+1);
    Oscill_Enabled_Buttons[i]:=CheckBox;
    ComboBox:=TComboBox.Create(Osc_Select_Group);
    ComboBox.Parent:=Osc_Select_Group;
    ComboBox.Left:=175-14-12;
    ComboBox.Top:=22+i*25;
    ComboBox.Width:=62;
    ComboBox.OnChange:=Osc_Type_Change;
    for j:=0 to OSCILL_TYPES_NUM-1 do // Инитим список типов осц.
      ComboBox.Items.Add(Osc_Type_Names[j]);
    ComboBox.tag:=i;
    ComboBox.ShowHint:=True; ComboBox.Hint:='Oscillator '+ Num_To_Str(i+1)+' type';
    Oscill_Type_Selects[i]:=ComboBox;
                   // Инитим двигалки для расстройки частоты
    TrackBar:=TTrackBar.Create(Osc_Select_Group);
    TrackBar.Parent:=Osc_Select_Group;
    TrackBar.tag:=i;
    TrackBar.Left:=235-15-12;
    TrackBar.Width:=100;
    TrackBar.Height:=20;
    TrackBar.Max:=PERCENT3;
    TrackBar.TickStyle:=tsManual;
    TrackBar.SetTick(PERCENT3 div 2);
    TrackBar.Top:=22+i*25;
    TrackBar.ShowHint:=True;TrackBar.Hint:='Oscillator '+ Num_To_Str(i+1)+' flanger coef';
    Oscill_Flang_Coefs[i]:=TrackBar;
    TrackBar.OnChange:=Osc_Flang_Change;
                   // Инитим двигалки для изменения волны
    ScrollBar:=TScrollBar.Create(Osc_Select_Group);
    ScrollBar.Parent:=Osc_Select_Group;
    ScrollBar.tag:=i;
    ScrollBar.Left:=310;
    ScrollBar.Width:=90;
    ScrollBar.Height:=10;
    ScrollBar.Max:=PERCENT3;
    ScrollBar.Top:=25+i*25;
    ScrollBar.ShowHint:=True;ScrollBar.Hint:='Oscillator '+ Num_To_Str(i+1)+' shape coef';
    Oscill_Shape_Coefs[i]:=ScrollBar;
    ScrollBar.Min:=0;
    ScrollBar.Max:=200;
    ScrollBar.OnChange:=Osc_Shape_Change;
  end;

  for i:=0 to HARMONICS_NUM-1 do
             // Инитим двигалки у гармонайзера
  begin
    Scroll:=TScrollBar.Create(Osc_Select_Group);
    Scroll.Parent:=Osc_Select_Group;
    Scroll.Kind:=sbVertical;
    Scroll.OnScroll:=Harm_Scroll_Change;
    Scroll.tag:=i;
    Scroll.Left:=335+i*15+70;
    Scroll.Top:=22;
    Scroll.Width:=10;
    Scroll.ShowHint:=True; Scroll.Hint:='Harmonic '+ Num_To_Str(i+1)+' mix coef';
    Harm_Scrolls[i]:=Scroll;
  end;

  for i:=0 to EFFECTS_TYPES_NUM-1 do // Инитим список типов эффектов.
    Eff_Type_Select.Items.Add(Eff_Names2[i]);

  Sample1:=TWavData.Create;

  ALL_NOTES_NAMES[0]:=EMPTY_NOTE_NAME;
  k:=1;
  for i:=1 to OCTAVES_NUM do     // Инитим имена нот
  begin
    for j:=1 to NOTES_IN_OCTAVE_NUM do
    begin
      ALL_NOTES_NAMES[k]:=NOTES_NAMES[j]+' '+Num_To_Str(i)+' ';
      inc(k);
      ALL_NOTES_NAMES[k]:=NOTES_NAMES[j]+NOTE_DIES_SYM+Num_To_Str(i)+' ';
      inc(k);
    end;
  end;

  Sample_Note_Len_Input.Max:=MAX_NOTES_NUM;
  Form1.Caption:=SYNTH_NAME;
  Clear_All_Variables;             // Чиcтим переменные
  Update_All_Controls;
end;

procedure TForm1.FormDestroy(Sender: TObject);
var
  i :integer;
begin
  Stop_Play_Wav;                   // Затыкаем звук
  if (hLib>0) then
    FreeLibrary(HLib);               // Выгружаем ДЛЛ-ку
  for i:=0 to OSCILL_NUM-1 do      // Прибиваем массив осцилляторов
  begin
    Oscillators[i].Free;
  end;
  for i:=0 to CHANNELS_NUM-1 do    // Прибиваем массив секвенсеров
  begin
    Sequencers[i].Free;
  end;
  for i:=0 to CUST_SOUND_NUM-1 do  // Прибиваем массив звуков
  begin
    Custom_Sounds[i].Free;
  end;

  for i:=0 to Effects.Count-1 do   // Прибиваем эффекты
    TEffect(Effects.Items[i]).Free;

  Custom_Wave.Free;   //  Прибиваем custom wave

end;
procedure TForm1.FormShow(Sender: TObject);
begin
  with Sample_View.Canvas do
  begin
    Pen.Color := clGreen;
    Brush.Color := clBlack;
    FillRect(ClipRect);    // Очищаем вид сэмпла
  end;
end;

procedure TForm1.Save_ButtonClick(Sender: TObject);
begin
  Do_Save_Script;
end;

procedure TForm1.Play_ButtonClick(Sender: TObject);
begin
  Do_Play(True);
end;

procedure TForm1.Button_StopClick(Sender: TObject);
begin
  Stop_Play_Wav;
end;

procedure TForm1.Load_ButtonClick(Sender: TObject);
begin
  Do_Load_Script;
  Update_All_Controls;
end;

procedure TForm1.Sample_Speed_InputChange(Sender: TObject);
begin
  Sample_Speed_Show.Caption:=Num_To_Str(Sample_Speed_Input.Position);
  Sample_Speed:=Sample_Speed_Input.Position;
end;

procedure TForm1.Sample_Tempo_InputChange(Sender: TObject);
begin
  Sample_Tempo_Show.Caption:=Num_To_Str(Sample_Tempo_Input.Position);
  Sample_Tempo:=Sample_Tempo_Input.Position;
end;

procedure TForm1.Sample_Note_Len_InputChange(Sender: TObject);
begin
  Sample_Note_Len_Show.Caption:=Num_To_Str(Sample_Note_Len_Input.Position);
  Sample_Note_Len:=Sample_Note_Len_Input.Position;
  Sample_Notes_Len_Show2.Caption:='('+Num_To_Str(Sequencers[0].Seq_Notes_Repeat_Num*Sample_Note_Len)+')';  
end;

procedure TForm1.FormKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
    case key of   
     PLAY_KEY:  begin Do_Play(True);Key:=0; end;
     PLAY_KEY2: begin Do_Play(False);Key:=0; end;    // Играем без loop'А
     STOP_KEY:  begin Stop_Play_Wav;Key:=0; end;
     LOAD_KEY:  begin Do_Load_Script;Update_All_Controls;Key:=0; end;
     SAVE_KEY:  begin Do_Save_Script;Key:=0; end;
     HELP_KEY:;
  end;
end;

procedure TForm1.Cust_Sound_Amp_Env_ButtonClick(Sender: TObject);
begin
    with Custom_Sounds[0].Amp_Env do
      Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
      'Custom Sound Amp Env');
end;

procedure TForm1.Cust_Sound_Freq_Env_ButtonClick(Sender: TObject);
begin
    with Custom_Sounds[0].Freq_Env do
      Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
      'Custom Sound Freq Env');
end;

procedure TForm1.Cust_Sound_EnabledClick(Sender: TObject);
begin
    Custom_Sounds[0].Enabled:=
      Cust_Sound_Enabled.Checked;
end;

procedure TForm1.Clear_ButtonClick(Sender: TObject);
begin
  Clear_All_Variables;
  Update_All_Controls;
end;

procedure TForm1.Octave_SelectChange(Sender: TObject);
begin
  Octave_Select_Show.Caption:=Num_To_Str(Octave_Select.Position);

    with Sequencers[0] do
      Cur_Octave:=Octave_Select.Position;

end;

procedure TForm1.Sequencer_NotesKeyPress(Sender: TObject; var Key: Char);
var
  i,j, Cur_Note: integer;
  Tmp_Key: char;
  Note_Last_Sym: char;
begin
  if (Sequencer_Notes.Items.Count>0) and (Sequencer_Notes.ItemIndex>-1) then
// Ежели ноты есть и какая-нить из них выделена
  begin
    Tmp_Key:=UpCase(Key);                 // Приводим к общему знаменателю ;)
    Cur_Note:=Sequencer_Notes.ItemIndex;
    Note_Last_Sym:=Sequencer_Notes.Items.Strings[Cur_Note]
              [Length(Sequencer_Notes.Items.Strings[Cur_Note])];
    if Sequencer_Notes.Items.Strings[Cur_Note]=EMPTY_NOTE_NAME then
      Note_Last_Sym:=' ';

    j:=Sequencer_Notes.ItemIndex;

    for i:=1 to NOTES_IN_OCTAVE_NUM do
      if Tmp_Key=Seq_Notes_Keys1[i] then
      begin
        Sequencer_Notes.Items.Strings[Cur_Note]:=NOTES_NAMES[i]+
        ' '+Num_To_Str(Sequencers[0].Cur_Octave)+Note_Last_Sym;
        Update_Sequencer;     // Копируем ноты в массив секвенсора
        Sequencer_Notes.Refresh;
        if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
          Sequencer_Notes.ItemIndex:=j+1;
        Key:=#0;             // Обработали
        exit;
      end;

    for i:=1 to NOTES_IN_OCTAVE_NUM do
      if Tmp_Key=Seq_Notes_Keys2[i] then
      begin
        Key:=#0;             // Обработали
            // Ежели октава не <=1, а то будет октава 0
        if Sequencers[0].Cur_Octave>1 then
        begin
          Update_Sequencer;          // Копируем ноты в массив секвенсора
          Sequencer_Notes.Items.Strings[Cur_Note]:=NOTES_NAMES[i]+
          ' '+Num_To_Str(Sequencers[0].Cur_Octave-1)+Note_Last_Sym;
          if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
            Sequencer_Notes.ItemIndex:=j+1;
          exit;
        end;
      end;

    for i:=1 to NOTES_IN_OCTAVE_NUM do
      if Tmp_Key=Seq_Notes_Keys3[i] then
      begin
        Sequencer_Notes.Items.Strings[Cur_Note]:=NOTES_NAMES[i]+
        NOTE_DIES_SYM+Num_To_Str(Sequencers[0].Cur_Octave)+Note_Last_Sym;
        Update_Sequencer;     // Копируем ноты в массив секвенсора
        Sequencer_Notes.Refresh;
        if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
          Sequencer_Notes.ItemIndex:=j+1;
        Key:=#0;             // Обработали
        exit;
      end;

    for i:=1 to NOTES_IN_OCTAVE_NUM do
      if Tmp_Key=Seq_Notes_Keys4[i] then
      begin
        Key:=#0;             // Обработали
            // Ежели октава не <=1, а то будет октава 0
        if Sequencers[0].Cur_Octave>1 then
        begin
          Update_Sequencer;          // Копируем ноты в массив секвенсора
          Sequencer_Notes.Items.Strings[Cur_Note]:=NOTES_NAMES[i]+
          NOTE_DIES_SYM+Num_To_Str(Sequencers[0].Cur_Octave-1)+Note_Last_Sym;
          if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
            Sequencer_Notes.ItemIndex:=j+1;
          exit;
        end;
      end;

    if Tmp_Key=Note_Sustain_Key then
    begin
      Toggle_Note_Sustain;
      Update_Sequencer;          // Копируем ноты в массив секвенсора
      if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
        Sequencer_Notes.ItemIndex:=j+1;
      Key:=#0;             // Обработали
    end;
    if Tmp_Key=Note_Pitch_Key then
    begin
      Toggle_Note_Pitch;
      Update_Sequencer;          // Копируем ноты в массив секвенсора
      if j<Sequencer_Notes.Items.Count then // Двигаем курсор на след. ноту
        Sequencer_Notes.ItemIndex:=j+1;
      Key:=#0;             // Обработали
    end;

  end;
end;

procedure TForm1.Osc_Select_GroupClick(Sender: TObject);
begin
  if Osc_Select_Group.ItemIndex>-1 then
  begin
    with Oscillators[Osc_Select_Group.ItemIndex] do
    begin
//      Oscill_Type_Select.ItemIndex:=Oscill_Type;
    end;
  end;
end;

procedure TForm1.Seq_Amp_Env_ButtonClick(Sender: TObject);
begin
  with Sequencers[0].Amp_Env do
    Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords, NOTE_AMP_ENV_NOTES_LEN,
    'Sequencer Notes Amp Env');
end;

procedure TForm1.Notes_Num_SelectScroll(Sender: TObject;
  ScrollCode: TScrollCode; var ScrollPos: Integer);
var
  i,Notes_Num_Delta :integer;
begin
    Notes_Num_Show.Caption:=Num_To_Str(Notes_Num_Select.Position);
    if Notes_Num_Select.Position<Sequencer_Notes.Items.Count then
    begin                             // Удаляем ноты
      Notes_Num_Delta:=Sequencer_Notes.Items.Count-Notes_Num_Select.Position;
      for i:=1 to Notes_Num_Delta do
        Sequencer_Notes.Items.Delete(Sequencer_Notes.Items.Count-1);
    end;
    if Notes_Num_Select.Position>Sequencer_Notes.Items.Count then
    begin                             // Добавляем ноты
      Notes_Num_Delta:=Notes_Num_Select.Position-Sequencer_Notes.Items.Count;
      for i:=1 to Notes_Num_Delta do
        Sequencer_Notes.Items.Add(ALL_NOTES_NAMES[0]);
    end;
  Update_Sequencer;
end;

procedure TForm1.Seq_EnabledClick(Sender: TObject);
begin
  Sequencers[0].Enabled:=
    Seq_Enabled.Checked;
end;

procedure TForm1.Sequencer_NotesKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
var
  Tmp_Key: word;
  Cur_Note: integer;
begin
  Cur_Note:=Sequencer_Notes.ItemIndex;

  if Key=Note_Insert_Key then
  begin
    Sequencer_Notes.Items.Insert(Cur_Note,EMPTY_NOTE_NAME);
    Key:=0;
  end;

  if Cur_Note>-1 then
  begin
    if Key=Note_Delete_Key then
    begin
      Sequencer_Notes.Items.Delete(Cur_Note);
      Key:=0;
    end;
    if Key=Note_None_Key then
    begin
      i:=Sequencer_Notes.ItemIndex;
      Sequencer_Notes.Items.Strings[Cur_Note]:=EMPTY_NOTE_NAME;
      if Sequencer_Notes.ItemIndex<Sequencer_Notes.Items.Count then
        Sequencer_Notes.ItemIndex:=i+1;
      Key:=0;
    end;
  end;

  Update_Sequencer;     // Копируем ноты в массив секвенсора
                          // Update'им движок кол-ва нот
  Notes_Num_Select.Position:=Sequencer_Notes.Items.Count;
  Notes_Num_Show.Caption:=Num_To_Str(Notes_Num_Select.Position);
end;

{var
  Button: Integer;
begin
  Button := Application.MessageBox('Do you want to exit ?', 'Synth', MB_YESNO);
 if Button = IDNO then
   CanClose:=False;}

procedure TForm1.Harm_Enable_ButtonEnter(Sender: TObject);
begin
  if (Cur_Oscillator>-1) then //Если осц. выбран, то
    Toggle(Harmonizers[Cur_Oscillator].Enabled);
end;

procedure TForm1.Eff_Add_ButtonClick(Sender: TObject);
begin
  if Eff_Type_Select.ItemIndex>-1 then
    if Eff_List.ItemIndex>-1 then
    begin
      Effects.Insert(Eff_List.ItemIndex,Eff_Types[Eff_Type_Select.ItemIndex].Create(
                  Eff_Type_Select.Items[Eff_Type_Select.ItemIndex]));
      Eff_List.Items.Insert(Eff_List.ItemIndex,Eff_Type_Select.Items[Eff_Type_Select.ItemIndex]);
    end
    else
    begin
      Effects.Add(Eff_Types[Eff_Type_Select.ItemIndex].Create(
                  Eff_Type_Select.Items[Eff_Type_Select.ItemIndex]));
      Eff_List.Items.Add(Eff_Type_Select.Items[Eff_Type_Select.ItemIndex]);
    end;
end;

procedure TForm1.Del_Eff_ButtonClick(Sender: TObject);
begin
  if Eff_List.ItemIndex>-1 then
  begin
    Effects.Delete(Eff_List.ItemIndex);
    Eff_List.Items.Delete(Eff_List.ItemIndex);
  end
end;

procedure TForm1.Eff_ListClick(Sender: TObject);
var
  k: integer;
begin
  if Eff_List.ItemIndex>-1 then
  begin
    if Eff_Edit_Box.ControlCount>0 then
    begin    // Вытираем то, что было
      k:=Eff_Edit_Box.ControlCount-1;
      for i:= 0 to k do
        Eff_Edit_Box.Controls[0].Free;
    end;
    TEffect(Effects.Items[Eff_List.ItemIndex]).Edit(Eff_Edit_Box);    
  end;
end;

procedure TForm1.Frw_BtnClick(Sender: TObject);
begin
  if (Eff_List.ItemIndex>-1) and
     (Eff_List.ItemIndex<Eff_List.Items.Count-1) then
  begin
    i:=Eff_List.ItemIndex;
    Effects.Exchange(Eff_List.ItemIndex,Eff_List.ItemIndex+1);
    Eff_List.Items.Exchange(Eff_List.ItemIndex,Eff_List.ItemIndex+1);
    Eff_List.ItemIndex:=i+1;
  end
end;

procedure TForm1.Bk_BtnClick(Sender: TObject);
begin
  if (Eff_List.ItemIndex>0) then
  begin
    i:=Eff_List.ItemIndex;
    Effects.Exchange(Eff_List.ItemIndex,Eff_List.ItemIndex-1);
    Eff_List.Items.Exchange(Eff_List.ItemIndex,Eff_List.ItemIndex-1);
    Eff_List.ItemIndex:=i-1;
  end
end;

procedure TForm1.Seq_Notes_Rep_InpChange(Sender: TObject);
begin
  Seq_Notes_Rep_Num.Caption:=Num_To_Str(Seq_Notes_Rep_Inp.Position);
  Sequencers[0].Seq_Notes_Repeat_Num:=Seq_Notes_Rep_Inp.Position;
  Sample_Notes_Len_Show2.Caption:='('+Num_To_Str(Sequencers[0].Seq_Notes_Repeat_Num*Sample_Note_Len)+')';

end;

procedure TForm1.Random_Notes_BtnClick(Sender: TObject);
var
  i,j,k: integer;
  s,Note_Last_Sym: string;
begin
  with Sequencer_Notes do
    if (Items.Count>0)  then
  // Ежели ноты есть
    begin
      for i:=0 to Items.Count-1 do
      begin
        j:=Random(NOTES_IN_OCTAVE_NUM-1)+1;
        if Items.Strings[i]=EMPTY_NOTE_NAME then
          Note_Last_Sym:=' '
        else
          Note_Last_Sym:=Items.Strings[i]
              [Length(Items.Strings[i])];
        s:=' ';
        if Dies_Enable.Checked then
          if (Random(100) mod 2)=1 then
            s:=NOTE_DIES_SYM;
        Items.Strings[i]:=NOTES_NAMES[j]+
        s+Num_To_Str(Sequencers[0].Cur_Octave)+Note_Last_Sym;
      end;
      Update_Sequencer;     // Копируем ноты в массив секвенсора
  end;
end;

procedure TForm1.Cust_Wave_Edit_BtnClick(Sender: TObject);
begin
  with Custom_Wave do
    Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
    'Custom wave');
end;

procedure TForm1.Pitch_Speed_SelectChange(Sender: TObject);
begin
  Pitch_Speed:=Pitch_Speed_Select.Position;
  Pitch_Speed_Show.Caption:=Num_To_Str(Pitch_Speed);
end;

procedure TForm1.Attack_Len_ScrollChange(Sender: TObject);
begin
  Attack_Len:=Attack_Len_Scroll.Position;
  Att_Len_Show.Caption:=Num_To_Str(Attack_Len)+'%';
  if (Decay_Len+Attack_Len)>100 then
  begin
    Attack_Len:=100-Decay_Len;
    Attack_Len_Scroll.Position:=Attack_Len;
  end

end;

procedure TForm1.Decay_Len_ScrollChange(Sender: TObject);
begin
  Decay_Len:=Decay_Len_Scroll.Position;
  Dec_Len_Show.Caption:=Num_To_Str(Decay_Len)+'%';
  if (Decay_Len+Attack_Len)>100 then
  begin
    Decay_Len:=100-Attack_Len;
    Decay_Len_Scroll.Position:=Decay_Len;
  end
end;

procedure TForm1.Pitch_BtnClick(Sender: TObject);
begin
  if (Sequencer_Notes.Items.Count>0) and (Sequencer_Notes.ItemIndex>-1) then
// Ежели ноты есть и какая-нить из них выделена
  begin
    Toggle_Note_Pitch;
    Update_Sequencer;          // Копируем ноты в массив секвенсора
  end;
end;

procedure TForm1.Sustain_BtnClick(Sender: TObject);
begin
  if (Sequencer_Notes.Items.Count>0) and (Sequencer_Notes.ItemIndex>-1) then
// Ежели ноты есть и какая-нить из них выделена
  begin
    Toggle_Note_Sustain;
    Update_Sequencer;          // Копируем ноты в массив секвенсора
  end;
end;

end.
