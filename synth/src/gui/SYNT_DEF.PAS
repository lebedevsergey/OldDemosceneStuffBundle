// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI definitions and service objects

const
  SYNTH_NAME = 'Synth';

  OSCILL_NUM = 5;
  OSCILL_TYPES_NUM = 8;

  HARMONICS_NUM = 6;

  FILT_TYPES_NUM = 6;

  MAX_EFFECTS_NUM = 30;
  EFFECTS_TYPES_NUM = 7;

  SAMPLE_RATE = 22050;
  MAX_SAMPLE_SIZE_ = SAMPLE_RATE*9; //  Надо править TSampleArray в Unit1
  MAX_SAMPLE_DATA_VALUE = $FFFF;
  WAV_DATA_SIZE = 2; // 2 байта

  PERCENT = 100;
  PERCENT3 = 254;

  DEFAULT_SAMPLE_NOTE_LEN = 12;
  DEFAULT_SAMPLE_TEMPO = 125;
  DEFAULT_SAMPLE_SPEED = 6;
  DEFAULT_PITCH_SPEED = 1;
  DEFAULT_ATTACK_LEN = 0;
  DEFAULT_DECAY_LEN = 90;  

//  ENVELOPE_LEN = 10000;  // Длина в процентах
  ENVELOPE_MAX_NODES_NUM = 30;  // От этого зависит SPAM_ENVELOPE_MAX_NODES_NUM
                             // в Unit2
  CHANNELS_NUM = 1;

  CUST_SOUND_NUM = 1;

  MAX_NOTES_NUM = 64;
  OCTAVES_NUM = 10;
  NOTES_IN_OCTAVE_NUM = 7;
  NOTES_NAMES : array [1..NOTES_IN_OCTAVE_NUM] of string
         = ('C','D','E','F','G','A','B');
  EMPTY_NOTE_NAME    = 'NONE';
  NULL_SYM           = 'NULL';
  NOTE_SUSTAIN_SYM   = 's';
  NOTE_PITCH_SYM     = 'p';
  NOTE_SUS_PITCH_SYM = 'q';
  NOTE_DIES_SYM      = '#';

  MAX_DELAYS_NUM = 5;
  MAX_DELAYS_NUM_ = 4;

  NOTE_AMP_ENV_NOTES_LEN = 3;

  TMP_SAMPLE_NAME    = 'tmp.wav';
  TMP_SCRIPT_NAME    = 'tmp.scr';
  SYNTH_DLL_NAME     =  'synth.dll'; 

  SETUP_OSC_COM = 'OSCILLATOR';
  SETUP_OSC_COM2 = 'OSCILLATO2';
  SETUP_OSC_COM3 = 'OSCILLATO3';    
  SEQ_COM = 'SEQUENCER';
  SEQ_COM2 = 'SEQUENCE2';
  SET_BPM_SAMPLE_LEN_COM = 'SET_BPM_SAMPLE_LEN';
  CUST_SOUND_COM = 'CUSTOM_SOUND';
  FILTER_COM = 'FILTER';
  FILTER_COM2 = 'FILTE2';  
  DISTORT_COM = 'DISTORT';
  MOD_AMP_COM = 'MOD_AMP';  
  END_COM = 'END';
  HARM_COM = 'HARMONIZER';
  REVERB_COM = 'REVERB';
  DELAY_COM = 'DELAY';
  DELAY2_COM = 'DELA2';
  LEN_MUL_COM = 'LEN_MUL';
  SET_CUST_WAVE_COM='CUST_WAVE';
  SET_PITCH_SPEED_COM='SET_PITCH_SPEED';
  SET_A_D_COM='SET_A_D';  

  Seq_Notes_Keys1 : array [1..NOTES_IN_OCTAVE_NUM]of char =
  ('Q','W','E','R','T','Y','U');
  Seq_Notes_Keys2 : array [1..NOTES_IN_OCTAVE_NUM]of char =
  ('Z','X','C','V','B','N','M');
  Seq_Notes_Keys3 : array [1..NOTES_IN_OCTAVE_NUM]of char =
  ('2','3','4','5','6','7','8');
  Seq_Notes_Keys4 : array [1..NOTES_IN_OCTAVE_NUM]of char =
  ('S','D','F','G','H','H','J');

  Note_Pitch_Key = '=';
  Note_Sustain_Key = '\';
  Note_Insert_Key = 45;  // INSERT
  Note_Delete_Key = 46;  // DELETE
  Note_None_Key = 8;    // BACKSPACE

  FILT_TYPE_SEL_X = 20;
  FILT_TYPE_SEL_Y = 10;
  FILT_BTN_X = 20;
  FILT_BTN_Y = 40;
  FILT_BTN_Y_STEP = 30;
  FILT_ORD_SCROLL_X=FILT_BTN_X;
  FILT_ORD_SCROLL_Y=140;
  MAX_FILT_ORD=10;

  DIST_COEF_SCROLL_X = 5;
  DELAY_COEF_SCROLL_X = 40;
  DELAY_VALUE_X = 0;
  DELAY_Y = 10;
  DELAY_Y_STEP = 30;
  DELAY2_COEF_SCROLL_X = 40;
  DELAY2_Y = 10;
  DELAY2_Y_STEP = 30;
type
  THarmonizer = class(TObject)
    Enabled: boolean;
    Harmonics_Freq_Coefs: array[0..HARMONICS_NUM-1] of real;
    Harmonics_Mix_Coefs: array[0..HARMONICS_NUM-1] of real;
    procedure Save(var f: textfile; Osc_Num: integer);
    procedure Load(var f: textfile; Osc_Num: integer);
    procedure Clear;
    constructor Create;
  end;

  TOscillator = class(TObject)
    Oscill_Type: integer;
    Enabled: boolean;
    Oscill_Mix_Coef: real;
    Oscill_Flanger_Coef: integer;
    Oscill_Shape_Coef: integer;
    procedure Save(var f: textfile; Osc_Num: integer);
    procedure Load(var f: textfile; Osc_Num: integer; Com_Type_Flg: byte);
    procedure Clear;
    constructor Create;
  end;

  TEnvelope = class(TObject)
    Nodes_Num: integer;
    Nodes_Values: array [1..ENVELOPE_MAX_NODES_NUM] of real;
    Nodes_X_Coords: array [1..ENVELOPE_MAX_NODES_NUM] of real;
    procedure Save(var f: textfile);
    procedure Load(var f: textfile);
    procedure Clear;
    constructor Create;
  end;

  TWavData = class(TObject)
    procedure Draw_Wav_Data(Canvas :TCanvas);
    constructor Create;
  end;

  TSequencer = class(TObject)
    Enabled: boolean;
    Notes_Num: integer;
    Notes: array [0..MAX_NOTES_NUM-1] of string[4];
    Cur_Octave: integer;
    Amp_Env: TEnvelope;
    Seq_Notes_Repeat_Num :integer;
    procedure Save(var f: textfile; Channel_Num: integer);
    procedure Load(var f: textfile; Channel_Num: integer; Com_Type_Flg: byte);
    procedure Clear;
    constructor Create;
  end;

  TCust_Sound = class(TObject)
    Enabled: boolean;
    Amp_Env: TEnvelope;
    Freq_Env: TEnvelope;
    procedure Save(var f: textfile; Channel_Num: integer);
    procedure Load(var f: textfile; Channel_Num: integer);
    procedure Clear;
    constructor Create;
  end;

  TEffect = class(TObject)
    Eff_Name: string;
    procedure Edit(Parent_Contr: TGroupBox);virtual;abstract;
    procedure Save(var f: textfile);virtual;abstract;
    procedure Load(var f: textfile);virtual;abstract;
    procedure Clear;virtual;abstract;
    constructor Create(Name: string);virtual;
  end;

  TFilter = class(TEffect)
    Filt_Type: integer;
    Filt_Order: integer;
    Freq_Env: TEnvelope;
    Gain_Env: TEnvelope;
    Q_Env: TEnvelope;
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    procedure Filt_Type_Change(Sender: TObject);
    procedure Filt_Env_Change(Sender: TObject);
    procedure Filt_Ord_Scroll_Change(Sender: TObject);
    constructor Create(Name: string);override;
  end;

  TDistort = class(TEffect)
    Dist_Env: TEnvelope;
    Dry_Coef,Wet_Coef: byte;
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    procedure Dist_Env_Change(Sender: TObject);
    procedure Mix_Coef_Scroll_Change(Sender: TObject);
    constructor Create(Name: string);override;
  end;

  TReverb = class(TEffect)
    Dist_Env: TEnvelope;
    Dry_Coef: byte;
    Delays: array[1..MAX_DELAYS_NUM] of integer;
    Mix_Coefs: array[1..MAX_DELAYS_NUM] of byte;
    procedure Mix_Coef_Scroll_Change(Sender: TObject);
    procedure Delay_Change(Sender: TObject);
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    constructor Create(Name: string);override;
  end;

  TDelay = class(TEffect)
    Dist_Env: TEnvelope;
    Dry_Coef: byte;
    Delays: array[1..MAX_DELAYS_NUM] of integer;
    Mix_Coefs: array[1..MAX_DELAYS_NUM] of byte;
    procedure Mix_Coef_Scroll_Change(Sender: TObject);
    procedure Delay_Change(Sender: TObject);
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    constructor Create(Name: string);override;
  end;

  TMod_Amp = class(TEffect)
    Amp_Env: TEnvelope;
    procedure Amp_Env_Change(Sender: TObject);
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    constructor Create(Name: string);override;
  end;

  TDelay2 = class(TEffect)
    Feedback: byte;
    Step: byte;
    procedure Feedback_Scroll_Change(Sender: TObject);
    procedure Step_Scroll_Change(Sender: TObject);
    procedure Edit(Parent_Contr: TGroupBox);override;
    procedure Save(var f: textfile);override;
    procedure Load(var f: textfile);override;
    constructor Create(Name: string);override;
  end;

  TEffectType = class of TEffect;

var
  i:integer;
var
  Oscillators: array[0..OSCILL_NUM-1] of TOscillator;
  Sequencers: array[0..CHANNELS_NUM-1] of TSequencer;
  Custom_Sounds: array[0..CUST_SOUND_NUM-1] of TCust_Sound;
  Harmonizers: array[0..OSCILL_NUM-1] of THarmonizer;
  Effects:  TList;
  Custom_Wave: TEnvelope;
  Pitch_Speed: integer;
  Attack_Len, Decay_Len: integer;

  Osc_Type_Names: array[0..OSCILL_TYPES_NUM-1] of string =
  ('Sinus','Square','Saw','Triangle','Noise','Saw2','Custom','TBLsaw2');
  Sample1: TWavData;
  ALL_NOTES_NAMES : array [0..NOTES_IN_OCTAVE_NUM*OCTAVES_NUM*2]
                    of string[4];
  Filt_Type_Names: array[0..FILT_TYPES_NUM-1] of string =
  ('Empty','Lowpass','Highpass','Bandpass','Empty','Peaking');
  Eff_Names: array[0..EFFECTS_TYPES_NUM-1] of string =
  ('Filter','Distortion','Reverb','Delay','Mod amp','Delay2','Filter');
  Eff_Types: array[0..EFFECTS_TYPES_NUM-1] of TEffectType =
  (TFilter,TDistort,TReverb,TDelay,TMod_Amp,TDelay2,TFilter);
  Eff_Commands: array[0..EFFECTS_TYPES_NUM-1] of string =
  (FILTER_COM,DISTORT_COM,REVERB_COM,DELAY_COM,MOD_AMP_COM,DELAY2_COM,
  FILTER_COM2);

  Eff_Names2: array[0..EFFECTS_TYPES_NUM-2] of string =
  ('Filter','Distortion','Reverb','Delay','Mod amp','Delay2');
             // !!!!!!!!!

  Sample_Speed, Sample_Tempo, Sample_Note_Len: integer;

  hash: integer;    // !!!!!!!!!

constructor THarmonizer.Create;
var
  i :integer;
begin
  inherited Create;
  Enabled:=False;
//  Harmonics_Mix_Coefs[0]:=1.0;  // Первая гармоника по умолчанию - 100%
  for i:=0 to HARMONICS_NUM-1 do
  begin
    Harmonics_Mix_Coefs[i]:=0.0;
    Harmonics_Freq_Coefs[i]:=i+1;
  end
end;

procedure THarmonizer.Clear;
var
  i :integer;
begin
  Enabled:=False;
//  Harmonics_Mix_Coefs[0]:=1.0;
  for i:=0 to HARMONICS_NUM-1 do
  begin
    Harmonics_Mix_Coefs[i]:=0.0;
    Harmonics_Freq_Coefs[i]:=i+1;
  end
end;

procedure THarmonizer.Load(var f: textfile; Osc_Num: integer);
var
  i,k :integer;
  s: string;
begin
  Enabled:=True;
  i:=0;
  while true do
  begin
    readln(f,s);
    if s=NULL_SYM then break;      //    конец коэффициентов
    Val(s,Harmonics_Freq_Coefs[i],k);
    Readln(f,Harmonics_Mix_Coefs[i]);
    inc(i);
  end;
end;

procedure THarmonizer.Save(var f: textfile; Osc_Num: integer);
var
  i :integer;
begin
  Writeln(f,HARM_COM);
  Writeln(f,Osc_Num);
  for i:=0 to HARMONICS_NUM-1 do
    if Harmonics_Mix_Coefs[i]>0 then
    begin
      writeln(f, Harmonics_Freq_Coefs[i]:10:11);
      writeln(f, Harmonics_Mix_Coefs[i]:10:11);
    end;
  writeln(f,NULL_SYM);
end;

constructor TOscillator.Create;
begin
  inherited Create;
  Oscill_Type:=0;
  Enabled:=False;
  Oscill_Mix_Coef:=1.0;
//  Oscill_Flanger_Coef:=PERCENT3 div 2;
  Oscill_Flanger_Coef:=PERCENT3 div 2;
  Oscill_Shape_Coef:=PERCENT*1;
end;

procedure TOscillator.Save(var f: textfile; Osc_Num: integer);
begin
  Writeln(f,SETUP_OSC_COM3);
  Writeln(f,Osc_Num);
  Writeln(f,Oscill_Type);
  Writeln(f, Oscill_Mix_Coef:10:11);
  Writeln(f,Oscill_Flanger_Coef);
  Writeln(f, Oscill_Shape_Coef);
end;

procedure TOscillator.Load(var f: textfile; Osc_Num: integer; Com_Type_Flg: byte);
begin
  Readln(f,Oscill_Type);
  Enabled:=True;
  Readln(f,Oscill_Mix_Coef);
  if Com_Type_Flg=1 then
    Readln(f,Oscill_Flanger_Coef);
  if Com_Type_Flg=2 then
  begin
    Readln(f,Oscill_Flanger_Coef);
    Readln(f,Oscill_Shape_Coef);
  end;
end;

procedure TOscillator.Clear;
begin
  Oscill_Type:=0;
  Enabled:=False;
  Oscill_Mix_Coef:=1.0;
  Oscill_Flanger_Coef:=PERCENT3 div 2;
  Oscill_Shape_Coef:=PERCENT*1;  
end;

constructor TEnvelope.Create;
begin
  inherited Create;
  Nodes_Num:=0;
{  Nodes_Values[1]:=0;
  Nodes_X_Coords[1]:=1;
  Nodes_Values[2]:=0;
  Nodes_X_Coords[2]:=0;}
end;

procedure TEnvelope.Save(var f: textfile);
begin
  if Nodes_Num>0 then
  begin
    Nodes_X_Coords[Nodes_Num]:=1.0;
    writeln(f, Nodes_Values[1]:10:11);
    for i:=2 to Nodes_Num do
    begin
      writeln(f, (Nodes_X_Coords[i]-Nodes_X_Coords[i-1]):10:11);
      writeln(f, Nodes_Values[i]:10:11);
    end;
    writeln(f,0); // dummy value
  end
  else
    begin              // Пишем нулевую огибающую
      Writeln(f, 0);
      Writeln(f, 1);
      Writeln(f, 0);
      Writeln(f, 0);
    end;
    Writeln(f,NULL_SYM);
end;

procedure TEnvelope.Load(var f: textfile);
var
  s: string;
  Cur_Len, Readed_Len: real;
  i: integer;
begin
  Nodes_Num:=0;
  Cur_Len:=0;

  while true do
  begin
    readln(f,s);
    if s=NULL_SYM then break;      //    конец огибающей
    inc(Nodes_Num);
    Val(s,Nodes_Values[Nodes_Num],i);
    Nodes_X_Coords[Nodes_Num]:=Cur_Len;
    Readln(f,Readed_Len);
    Cur_Len:=Cur_Len+Readed_Len;
  end;
end;

procedure TEnvelope.Clear;
begin
  Nodes_Num:=0;
end;

constructor TSequencer.Create;
begin
  inherited Create;
  Amp_Env:=TEnvelope.Create;
  Notes_Num:=0;
  Cur_Octave:=1;
  Seq_Notes_Repeat_Num:=1;
  Enabled:=False;
end;

procedure TSequencer.Save(var f: textfile; Channel_Num: integer);
var
  i: integer;
begin
  Writeln(f,SEQ_COM2);
  Writeln(f,Seq_Notes_Repeat_Num);
  Amp_Env.Save(f);
  for i:=0 to Notes_Num-1 do
    Writeln(f,Notes[i]);
  Writeln(f,NULL_SYM);
end;

procedure TSequencer.Load(var f: textfile; Channel_Num: integer; Com_Type_Flg: byte);
var
  i: integer;
  s: string;
begin
  Seq_Notes_Repeat_Num:=1;
  if Com_Type_Flg=1 then
    ReadLn(f,Seq_Notes_Repeat_Num);
  Amp_Env.Load(f);
  Notes_Num:=0;
  while true do
  begin
    ReadLn(f,s);
    if s=NULL_SYM then break;    // Больше нот нету
    Notes[Notes_Num]:=s;
    inc(Notes_Num);
  end;
  Enabled:=True;  
end;

procedure TSequencer.Clear;
begin
  Notes_Num:=0;
  Cur_Octave:=1;
  Amp_Env.Clear;
  Enabled:=False;
  Seq_Notes_Repeat_Num:=1;
end;

constructor TCust_Sound.Create;
begin
  inherited Create;
  Enabled:=False;  
  Amp_Env:=TEnvelope.Create;
  Freq_Env:=TEnvelope.Create;
end;

procedure TCust_Sound.Save(var f: textfile; Channel_Num: integer);
var
  i: integer;
begin
  Writeln(f,CUST_SOUND_COM);
  Amp_Env.Save(f);
  Freq_Env.Save(f);
end;

procedure TCust_Sound.Load(var f: textfile; Channel_Num: integer);
var
  i: integer;
  s: string;
begin
  Amp_Env.Load(f);
  Freq_Env.Load(f);
  Enabled:=True;  
end;

procedure TCust_Sound.Clear;
begin
  Enabled:=False;  
  Amp_Env.Clear;
  Freq_Env.Clear;
end;

constructor TWavData.Create;
begin
  inherited Create;
end;

procedure TWavData.Draw_Wav_Data(Canvas :TCanvas);
  var
    i : integer;
    Area_X_Size, Area_Y_Size : integer;
    Area_Start_X, Area_Center_Y : integer;
    X_Scale_Coef, Y_Scale_Coef :real;
    Prev_Cur_X_Int, Cur_X_Int :integer;
    Sample_Y  :integer;
    Cur_X     :real;
    Pos_Max_Sample_Y, Neg_Max_Sample_Y :integer;
    Wav_Data_Len: integer;
  begin
    Wav_Data_Len := sample_size div WAV_DATA_SIZE;  //т.к. SampleData - 16 bit

    Canvas.Pen.Color := clGreen;
    Canvas.Brush.Color := clBlack;
    Canvas.FillRect(Canvas.ClipRect);    // Очищаем экран

    if Wav_Data_Len=0 then exit;      // Если сэмпл - пустой, значит, облом

    with Canvas.ClipRect do
      begin
        Area_X_Size := Right-Left;
        Area_Y_Size := Bottom-Top;
        Area_Start_X := Left;
        Area_Center_Y := (Bottom-Top) div 2;

        X_Scale_Coef := Area_X_Size/Wav_Data_Len;
        Y_Scale_Coef := Area_Y_Size/MAX_SAMPLE_DATA_VALUE;

                                              // Рисуем
        Cur_X:=0;
        Prev_Cur_X_Int:=-1;
        Pos_Max_Sample_Y:=0;
        Neg_Max_Sample_Y:=0;

        for i:=1 to Wav_Data_Len do
        begin
          Cur_X_Int := Trunc(Cur_X);
          Sample_Y := Round(SmallInt(sample[i+$2e])*Y_Scale_Coef);

          if Prev_Cur_X_Int<>Cur_X_Int then    // Рисуем только если отличается
          begin
            Prev_Cur_X_Int:=Cur_X_Int;
            if Pos_Max_Sample_Y>Neg_Max_Sample_Y then
            begin
              Canvas.MoveTo(Cur_X_Int, Area_Center_Y+Pos_Max_Sample_Y);
              Canvas.LineTo(Cur_X_Int, Area_Center_Y+Neg_Max_Sample_Y);
            end;
            Pos_Max_Sample_Y:=0;
            Neg_Max_Sample_Y:=0;
          end
          else
            begin    // Иначе ищем макс и мин. значения
              if (Sample_Y>0) and (Pos_Max_Sample_Y<Sample_Y) then
                Pos_Max_Sample_Y:=Sample_Y;
              if (Sample_Y<0) and (Neg_Max_Sample_Y>Sample_Y) then
                Neg_Max_Sample_Y:=Sample_Y;
            end;
          Cur_X:=Cur_X+X_Scale_Coef;
        end;
      end;
  end;

constructor TEffect.Create(Name: string);
begin
  inherited Create;
  Eff_Name:=Name;
end;

constructor TFilter.Create(Name: string);
begin
  inherited Create(Name);
  Filt_Type:=0;
  Filt_Order:=1;
  Freq_Env:=TEnvelope.Create;
  Gain_Env:=TEnvelope.Create;
  Q_Env:=TEnvelope.Create;
end;


procedure TFilter.Save(var f: textfile);
begin
  Writeln(f,FILTER_COM2);
  Writeln(f,Filt_Type);
  Writeln(f,Filt_Order);
  Freq_Env.Save(f);
  Gain_Env.Save(f);
  Q_Env.Save(f);
end;

function Str_Hash(str: string):integer;forward;

procedure TFilter.Load(var f: textfile);
begin
  readln(f,Filt_Type);
  if hash=Str_Hash(FILTER_COM2) then
    readln(f,Filt_Order);
  Freq_Env.Load(f);
  Gain_Env.Load(f);
  Q_Env.Load(f);
end;

procedure TFilter.Filt_Ord_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
    Filt_Order:=Position;
end;

procedure TFilter.Filt_Type_Change(Sender: TObject);
begin
  with Sender as TComboBox do
    if ItemIndex>-1 then
      Filt_Type:=ItemIndex;
end;

procedure TFilter.Filt_Env_Change(Sender: TObject);
begin
  case (Sender as TControl).Tag of
    1:  with Self.Freq_Env do
        Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
        'Filter Freq Env');
    2:  with Self.Gain_Env do
        Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
        'Filter Gain Env');
    3:  with Self.Q_Env do
        Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
        'Filter Q Env');
  end;
end;

procedure TFilter.Edit(Parent_Contr: TGroupBox);
var
  i: integer;
  Filt_Type_Select: TComboBox;
  Btn: TButton;
  Scroll: TScrollBar;
begin
  Filt_Type_Select:=TComboBox.Create(Parent_Contr);
  Filt_Type_Select.Parent:=Parent_Contr;
  for i:=0 to FILT_TYPES_NUM-1 do // Инитим список типов фильтров
    Filt_Type_Select.Items.Add(Filt_Type_Names[i]);
  Filt_Type_Select.Width:=70;
  Filt_Type_Select.ItemIndex:=Filt_Type;
  Filt_Type_Select.OnChange:=Filt_Type_Change;
  Filt_Type_Select.Left:=FILT_TYPE_SEL_X;  
  Filt_Type_Select.Top:=FILT_TYPE_SEL_Y;
  Filt_Type_Select.ShowHint:=True;
  Filt_Type_Select.Hint:='Filter type select';
  Btn:=TButton.Create(Parent_Contr);
  Btn.Parent:=Parent_Contr;
  Btn.OnClick:=Filt_Env_Change;
  Btn.Tag:=1;     //  В таг - номер огибающей
  Btn.Caption:='Freq';
  Btn.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*0;
  Btn.Left:=FILT_BTN_X;
  Btn:=TButton.Create(Parent_Contr);
  Btn.Parent:=Parent_Contr;
  Btn.OnClick:=Filt_Env_Change;
  Btn.Tag:=2;     //  В таг - номер огибающей
  Btn.Caption:='Gain';
  Btn.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*1;
  Btn.Left:=FILT_BTN_X;
  Btn:=TButton.Create(Parent_Contr);
  Btn.Parent:=Parent_Contr;
  Btn.OnClick:=Filt_Env_Change;
  Btn.Tag:=3;     //  В таг - номер огибающей
  Btn.Caption:='Q';
  Btn.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*2;
  Btn.Left:=FILT_BTN_X;

  Scroll:=TScrollBar.Create(Parent_Contr);
  Scroll.Parent:=Parent_Contr;
  Scroll.Min:=1;  
  Scroll.Max:=MAX_FILT_ORD;
  Scroll.ShowHint:=True;
  Scroll.Hint:='Filter order';  
  Scroll.Position:=Filt_Order;
  Scroll.Top:=FILT_ORD_SCROLL_Y;
  Scroll.Left:=FILT_ORD_SCROLL_X;
  Scroll.OnChange:=Filt_Ord_Scroll_Change;
end;

constructor TDistort.Create(Name: string);
begin
  inherited Create(Name);
  Dist_Env:=TEnvelope.Create;
  Dry_Coef:=0;
  Wet_Coef:=100;
end;

procedure TDistort.Save(var f: textfile);
begin
  Writeln(f,DISTORT_COM);
  Writeln(f, Dry_Coef);
  Writeln(f, Wet_Coef);
  Dist_Env.Save(f);
end;

procedure TDistort.Load(var f: textfile);
begin
  Readln(f, Dry_Coef);
  Readln(f, Wet_Coef);
  Dist_Env.Load(f);
end;

procedure TDistort.Dist_Env_Change(Sender: TObject);
begin
  with Self.Dist_Env do
  Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
  'Dist Env');
end;

procedure TDistort.Mix_Coef_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
    case Tag of
     1:Dry_Coef:=Position;
     2:Wet_Coef:=Position;
    end;
end;

procedure TDistort.Edit;
var
  Mix_Coef_Scroll: TScrollBar;
  Btn: TButton;
begin
  Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
  Mix_Coef_Scroll.Parent:=Parent_Contr;
  Mix_Coef_Scroll.Tag:=1;
  Mix_Coef_Scroll.Position:=Dry_Coef;
  Mix_Coef_Scroll.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*0;
  Mix_Coef_Scroll.Left:=DIST_COEF_SCROLL_X;;
  Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
  Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
  Mix_Coef_Scroll.Parent:=Parent_Contr;
  Mix_Coef_Scroll.Tag:=2;
  Mix_Coef_Scroll.Position:=Wet_Coef;
  Mix_Coef_Scroll.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*1;
  Mix_Coef_Scroll.Left:=DIST_COEF_SCROLL_X;
  Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
  Btn:=TButton.Create(Parent_Contr);
  Btn.Parent:=Parent_Contr;
  Btn.OnClick:=Dist_Env_Change;
  Btn.Caption:='Distortion';
  Btn.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*2;
  Btn.Left:=FILT_BTN_X;

{  with Dist_Env do
    Env_Editor.Edit_Distort_Env(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
      'Distortion');}
end;

constructor TReverb.Create(Name: string);
begin
  inherited Create(Name);
  Dry_Coef:=100;
  for i:=1 to MAX_DELAYS_NUM do
  begin
    Delays[i]:=0;
    Mix_Coefs[i]:=0;
  end;
end;

procedure TReverb.Save(var f: textfile);
begin
  Writeln(f,REVERB_COM);
  writeln(f, Dry_Coef);  
  for i:=1 to MAX_DELAYS_NUM do
    if Mix_Coefs[i]>0 then
    begin
      writeln(f, Delays[i]);
      writeln(f, Mix_Coefs[i]);
    end;
  writeln(f,NULL_SYM);
end;

procedure TReverb.Load(var f: textfile);
var
  s: string;
  k: integer;
begin
  Readln(f,Dry_Coef);
  i:=1;
  while true do
  begin
    ReadLn(f,s);
    if s=NULL_SYM then break;    // Больше нету
    Val(s,Delays[i],k);
    Readln(f,Mix_Coefs[i]);
    inc(i);
  end;
end;

procedure TReverb.Mix_Coef_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
    case Tag of
     0:Dry_Coef:=Position;
     1..MAX_DELAYS_NUM:Mix_Coefs[Tag]:=Position;
    end;
end;

procedure TReverb.Delay_Change(Sender: TObject);
var
  s: string;
  i,k: integer;
begin
  with (Sender as TEdit) do
  begin
    Val(Text,i,k);
    if k=0 then
      Delays[Tag]:=i;
  end;
end;

procedure TReverb.Edit(Parent_Contr: TGroupBox);
var
  Mix_Coef_Scroll: TScrollBar;
  Delay_Input: TEdit;
  s: string;
begin
    Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
    Mix_Coef_Scroll.Parent:=Parent_Contr;
    Mix_Coef_Scroll.Tag:=0;
    Mix_Coef_Scroll.Top:=DELAY_Y+DELAY_Y_STEP*0;
    Mix_Coef_Scroll.Left:=DELAY_COEF_SCROLL_X;;
    Mix_Coef_Scroll.Position:=Dry_Coef;
    Mix_Coef_Scroll.ShowHint:=True;
    Mix_Coef_Scroll.Hint:='Dry signal mixer';
    Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
{    Mix_Coef_Scroll.Height:=10;
    Mix_Coef_Scroll.Width:=100;}
  for i:=1 to MAX_DELAYS_NUM_ do
  begin
    Delay_Input:=TEdit.Create(Parent_Contr);
    Delay_Input.Parent:=Parent_Contr;
    Delay_Input.Tag:=i;
    Delay_Input.Top:=DELAY_Y+DELAY_Y_STEP*i;
    Delay_Input.Left:=DELAY_VALUE_X;
    Str(Delays[i],s);
    Delay_Input.Text:=s;
    Delay_Input.OnChange:=Delay_Change;
//    Delay_Input.Position:=Mix_Coefs[i];
    Delay_Input.Height:=20;
    Delay_Input.Width:=30;

    Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
    Mix_Coef_Scroll.Parent:=Parent_Contr;
    Mix_Coef_Scroll.Tag:=i;
    Mix_Coef_Scroll.Top:=DELAY_Y+DELAY_Y_STEP*i;
    Mix_Coef_Scroll.Left:=DELAY_COEF_SCROLL_X;;
    Mix_Coef_Scroll.Position:=Mix_Coefs[i];
    Mix_Coef_Scroll.ShowHint:=True;
    Mix_Coef_Scroll.Hint:='Delayed signal mixer';
    Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
{    Mix_Coef_Scroll.Height:=10;
    Mix_Coef_Scroll.Width:=100;}
  end;
  Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
end;

constructor TDelay.Create(Name: string);
begin
  inherited Create(Name);
  Dry_Coef:=100;
  for i:=1 to MAX_DELAYS_NUM do
  begin
    Delays[i]:=0;
    Mix_Coefs[i]:=0;
  end;
end;

procedure TDelay.Save(var f: textfile);
begin
  Writeln(f,DELAY_COM);
  writeln(f, Dry_Coef);  
  for i:=1 to MAX_DELAYS_NUM do
    if Mix_Coefs[i]>0 then
    begin
      writeln(f, Delays[i]);
      writeln(f, Mix_Coefs[i]);
    end;
  writeln(f,NULL_SYM);
end;

procedure TDelay.Load(var f: textfile);
var
  s: string;
  k: integer;
begin
  Readln(f,Dry_Coef);
  i:=1;
  while true do
  begin
    ReadLn(f,s);
    if s=NULL_SYM then break;    // Больше нету
    Val(s,Delays[i],k);
    Readln(f,Mix_Coefs[i]);
    inc(i);
  end;
end;

procedure TDelay.Mix_Coef_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
    case Tag of
     0:Dry_Coef:=Position;
     1..MAX_DELAYS_NUM:Mix_Coefs[Tag]:=Position;
    end;
end;

procedure TDelay.Delay_Change(Sender: TObject);
var
  s: string;
  i,k: integer;
begin
  with (Sender as TEdit) do
  begin
    Val(Text,i,k);
    if k=0 then
      Delays[Tag]:=i;
  end;
end;

procedure TDelay.Edit(Parent_Contr: TGroupBox);
var
  Mix_Coef_Scroll: TScrollBar;
  Delay_Input: TEdit;
  s: string;
begin
    Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
    Mix_Coef_Scroll.Parent:=Parent_Contr;
    Mix_Coef_Scroll.Tag:=0;
    Mix_Coef_Scroll.Top:=DELAY_Y+DELAY_Y_STEP*0;
    Mix_Coef_Scroll.Left:=DELAY_COEF_SCROLL_X;;
    Mix_Coef_Scroll.Position:=Dry_Coef;
    Mix_Coef_Scroll.ShowHint:=True;
    Mix_Coef_Scroll.Hint:='Dry signal mixer';
    Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
{    Mix_Coef_Scroll.Height:=10;
    Mix_Coef_Scroll.Width:=100;}
  for i:=1 to MAX_DELAYS_NUM_ do
  begin
    Delay_Input:=TEdit.Create(Parent_Contr);
    Delay_Input.Parent:=Parent_Contr;
    Delay_Input.Tag:=i;
    Delay_Input.Top:=DELAY_Y+DELAY_Y_STEP*i;
    Delay_Input.Left:=DELAY_VALUE_X;
    Str(Delays[i],s);
    Delay_Input.Text:=s;
    Delay_Input.OnChange:=Delay_Change;
//    Delay_Input.Position:=Mix_Coefs[i];
    Delay_Input.Height:=20;
    Delay_Input.Width:=30;

    Mix_Coef_Scroll:=TScrollBar.Create(Parent_Contr);
    Mix_Coef_Scroll.Parent:=Parent_Contr;
    Mix_Coef_Scroll.Tag:=i;
    Mix_Coef_Scroll.Top:=DELAY_Y+DELAY_Y_STEP*i;
    Mix_Coef_Scroll.Left:=DELAY_COEF_SCROLL_X;;
    Mix_Coef_Scroll.Position:=Mix_Coefs[i];
    Mix_Coef_Scroll.ShowHint:=True;
    Mix_Coef_Scroll.Hint:='Delayed signal mixer';
    Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
{    Mix_Coef_Scroll.Height:=10;
    Mix_Coef_Scroll.Width:=100;}
  end;
  Mix_Coef_Scroll.OnChange:=Mix_Coef_Scroll_Change;
end;


constructor TMod_Amp.Create(Name: string);
begin
  inherited Create(Name);
  Amp_Env:=TEnvelope.Create;
end;

procedure TMod_Amp.Save(var f: textfile);
begin
  Writeln(f,MOD_AMP_COM);
  Amp_Env.Save(f);
end;

procedure TMod_Amp.Load(var f: textfile);
begin
  Amp_Env.Load(f);
end;

procedure TMod_Amp.Amp_Env_Change(Sender: TObject);
begin
  with Self.Amp_Env do
  Env_Editor.Edit_Envelope(Nodes_Num, Nodes_Values, Nodes_X_Coords,1,
  'Amplitude Envelope');
end;

procedure TMod_Amp.Edit(Parent_Contr: TGroupBox);
var
//  i: integer;
  Btn: TButton;
begin
  Btn:=TButton.Create(Parent_Contr);
  Btn.Parent:=Parent_Contr;
  Btn.OnClick:=Amp_Env_Change;
  Btn.Caption:='Amp Env';
  Btn.Top:=FILT_BTN_Y+FILT_BTN_Y_STEP*0;
  Btn.Left:=FILT_BTN_X;
end;

constructor TDelay2.Create(Name: string);
begin
  inherited Create(Name);
  Feedback:=10;
  Step:=2
end;

procedure TDelay2.Save(var f: textfile);
begin
  Writeln(f,DELAY2_COM);
  writeln(f, Feedback);
  writeln(f, Step);  
end;

procedure TDelay2.Load(var f: textfile);
var
  s: string;
  k: integer;
begin
  Readln(f,Feedback);
  Readln(f,Step);
end;

procedure TDelay2.Feedback_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
     Feedback:=Position;
end;

procedure TDelay2.Step_Scroll_Change(Sender: TObject);
begin
  with (Sender as TScrollBar) do
    Step:=Position;
end;

procedure TDelay2.Edit(Parent_Contr: TGroupBox);
var
  Scroll: TScrollBar;
  Step_Scroll: TScrollBar;
begin
    Scroll:=TScrollBar.Create(Parent_Contr);
    Scroll.Parent:=Parent_Contr;
    Scroll.Top:=DELAY2_Y;
    Scroll.Left:=DELAY2_COEF_SCROLL_X;
    Scroll.Position:=Feedback;
    Scroll.ShowHint:=True;
    Scroll.Hint:='Delay feedback';
    Scroll.OnChange:=Feedback_Scroll_Change;

    Scroll:=TScrollBar.Create(Parent_Contr);
    Scroll.Parent:=Parent_Contr;
    Scroll.Top:=DELAY2_Y+DELAY2_Y_STEP;
    Scroll.Left:=DELAY2_COEF_SCROLL_X;
    Scroll.Position:=Step;
    Scroll.ShowHint:=True;
    Scroll.Hint:='Delay step';
    Scroll.Min:=1;
    Scroll.Max:=10;
    Scroll.OnChange:=Step_Scroll_Change;
end;



