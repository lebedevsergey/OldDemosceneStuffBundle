// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI utilities

procedure Toggle(var Value: boolean);
begin
  if Value then
    Value:=False
  else
    Value:=True;
end;

procedure Stop_Play_Wav;
  begin
    PlaySoundA('', 0, SND_PURGE);
  end;

procedure Clear_All_Variables;
begin
  Stop_Play_Wav;       // Затыкаем звук
  Sample_Speed:=DEFAULT_SAMPLE_SPEED;                 // Сбрасываем bpm len
  Sample_Tempo:=DEFAULT_SAMPLE_TEMPO;
  Sample_Note_Len:=DEFAULT_SAMPLE_NOTE_LEN;

  for i:=0 to OSCILL_NUM-1 do      // Чистим массив осцилляторов
    Oscillators[i].Clear;
  for i:=0 to CHANNELS_NUM-1 do      // Чистим массив секвенсеров
    Sequencers[i].Clear;
  for i:=0 to CUST_SOUND_NUM-1 do    // Чистим звуки
    Custom_Sounds[i].Clear;
  for i:=0 to OSCILL_NUM-1 do      // Чистим гармонайзеры
    Harmonizers[i].Clear;
  for i:=0 to Effects.Count-1 do   // Чистим эффекты
    TEffect(Effects.Items[i]).Free;
  Effects.Clear;
  Custom_Wave.Clear;        // Чистим custom wave
  Pitch_Speed:=DEFAULT_PITCH_SPEED;
  Attack_Len:=DEFAULT_ATTACK_LEN;
  Decay_Len:=DEFAULT_DECAY_LEN;
end;

procedure Play_Wav(Wav_Name :PChar; Loop_Flg: boolean);
  var
    Res: boolean;
  begin
    if Loop_Flg then
      Res:=PlaySoundA(@sample, 0, SND_MEMORY OR
                                  SND_ASYNC OR SND_LOOP)
    else
      Res:=PlaySoundA(@sample, 0, SND_MEMORY OR
                                  SND_ASYNC);
    if not(Res) then    //  ашипка
      Application.MessageBox('Cannot play sample', 'Synth', MB_OK);
  end;


function Num_To_Str(value: integer): string;
var
  s: string;
begin
  Str(value,s);
  Num_To_Str:=s;
end;


function Gen_Sample(Script_Name, Wav_Name: string): boolean;
  var
    f,Sample_Len :integer;
    Cfg_File : TextFile;
    f2: Textfile;
    f3: file of byte;
    s,s1: string;
  begin
    Gen_Sample:=True;

    Sample_Len:=Trunc(5*SAMPLE_RATE/(2*Sample_Tempo)*Sample_Speed*Sample_Note_Len
    *Sequencers[0].Seq_Notes_Repeat_Num);
    if Sample_Len>MAX_SAMPLE_SIZE_ then
    begin // Слишком длинный сэмпл
      Application.MessageBox('Too big sample :-(', 'Synth', MB_OK);
      Gen_Sample:=False;
      exit;
    end;

  AssignFile(f2,Script_Name);  // Читаем скрипт в память
  Reset(f2);
  s:='';
  while not(EOF(f2)) do
  begin
    Readln(f2,s1);
    s:=s+' '+s1;
  end;
  CloseFile(f2);
                               // Генерим сэмпл
  GenSample(@s,Length(s),@sample,sample_size);
                               // Пишем сэмпл в файл
  AssignFile(f3,Wav_Name);
  ReWrite(f3);
  BlockWrite(f3,sample,sample_size);
  Close(f3);
  end;

procedure Save_Script(Script_Name: string);
var
  i: integer;
  f: textfile;
begin
              // Открываем файл
  AssignFile(f,Script_Name);
  ReWrite(f);

  if Pitch_Speed<>DEFAULT_PITCH_SPEED then
  begin
    writeln(f,SET_PITCH_SPEED_COM);
    writeln(f,Pitch_Speed);
  end;
  if (Attack_Len<>DEFAULT_Attack_Len) or
  (Decay_Len<>DEFAULT_Decay_Len) then
  begin
    writeln(f,SET_A_D_COM);
    writeln(f,Attack_Len);
    writeln(f,Decay_Len);
  end;

  if (Sample_Speed>0) and
  (Sample_Tempo>0) and (Sample_Note_Len>0) then // Все значения корректны
  begin                                     // Пишем bpm
    //!!!!!!!!!!!!! Заплатка by желание manve
    writeln(f,LEN_MUL_COM);
    writeln(f,Sequencers[0].Seq_Notes_Repeat_Num);
    //!!!!!!!!!!!!! Заплатка by желание manve

    writeln(f,SET_BPM_SAMPLE_LEN_COM);
    writeln(f,Sample_Tempo);
    writeln(f,Sample_Speed);
    writeln(f,Sample_Note_Len);
  end;

  writeln(f,SET_CUST_WAVE_COM);
  writeln(f,0);  
  Custom_Wave.Save(f);        // Пишем custom wave

                               // Пишем гармонайзеры
  for i:=0 to OSCILL_NUM-1 do
    if Harmonizers[i].Enabled then
      Harmonizers[i].Save(f,i);
                               // Пишем осцилляторы
  for i:=0 to OSCILL_NUM-1 do
    if Oscillators[i].Enabled then
      Oscillators[i].Save(f,i);

  for i:=0 to CHANNELS_NUM-1 do    // Пишем секвенсоры
    if (Sequencers[i].Notes_Num>0) and (Sequencers[i].Enabled) then
      Sequencers[i].Save(f,i);

  for i:=0 to CUST_SOUND_NUM-1 do    // Пишем звуки
    if Custom_Sounds[i].Enabled then
      Custom_Sounds[i].Save(f,i);

//  if Filter.Enabled then           // Пишем эффекты
  for i:=0 to Effects.Count-1 do
    TEffect(Effects.Items[i]).Save(f);

                                  // Пишем команду End
  Write(f,END_COM);

  CloseFile(f);
end;

procedure Do_Save_Script;
var
  Wav_Filename, Script_Filename: string;
begin
  with Form1 do
  begin
    SaveDialog1.Filter := 'Script files|*.SCR';
    SaveDialog1.DefaultExt := 'SCR';
    SaveDialog1.Options := [ofOverwritePrompt];

    if not SaveDialog1.Execute then exit;     // Не хотят сохраняться

    Script_Filename:=ExtractFileName(SaveDialog1.Filename);
    Save_Script(Script_Filename);
                                            // Делаем имя .wav файла
    Wav_Filename:=Copy(Script_Filename,1, // такое же, как у скрипта
      Length(Script_Filename)-3)+'wav';
    Gen_Sample(Script_Filename, Wav_Filename);
  end;
end;

function Str_Hash(str: string):integer;
var
  i, len, hash: integer;
begin
  len:=Length(str);
  if len>0 then
  begin
    hash:=0;
    for i:=1 to len do
      hash:=hash+((Ord(str[i])*i) mod 10000);
  end
  else
    hash:=0;

  Str_Hash:=hash;
end;

procedure Load_Script(Script_Name: string);
var
  i, {hash,} tmp: integer;
  com: string;
  f: textfile;
  Tmp_Obj: TEffect;
begin

  Clear_All_Variables;

                             // Открываем файл
  AssignFile(f,Script_Name);
  Reset(f);

  while true do
  begin                       // Читаем команды
    Readln(f,com);
    hash:=Str_Hash(com);     // Хеш команды
    if hash=Str_Hash(END_COM) then
      break;
    if hash=Str_Hash(SETUP_OSC_COM) then
    begin
      Readln(f,i);
      Oscillators[i].Enabled:=True;
      Oscillators[i].Load(f,i,0);
    end;
    if hash=Str_Hash(SETUP_OSC_COM2) then
    begin
      Readln(f,i);
      Oscillators[i].Enabled:=True;
      Oscillators[i].Load(f,i,1);
    end;
    if hash=Str_Hash(SETUP_OSC_COM3) then
    begin
      Readln(f,i);
      Oscillators[i].Enabled:=True;
      Oscillators[i].Load(f,i,2);
    end;
    if hash=Str_Hash(SEQ_COM) then
      Sequencers[0].Load(f,0,0);
    if hash=Str_Hash(SEQ_COM2) then
      Sequencers[0].Load(f,0,1);

    if hash=Str_Hash(CUST_SOUND_COM) then
    begin
      Custom_Sounds[0].Load(f,0);
    end;

    if hash=Str_Hash(SET_BPM_SAMPLE_LEN_COM) then
    begin
      Readln(f,Sample_Tempo);
      Readln(f,Sample_Speed);
      Readln(f,Sample_Note_Len);
    end;

    if hash=Str_Hash(SET_PITCH_SPEED_COM) then
    begin
      readln(f,Pitch_Speed);
    end;
    if hash=Str_Hash(SET_A_D_COM) then
    begin
      readln(f,Attack_Len);
      readln(f,Decay_Len);
    end;

    for i:=Low(Eff_Commands) to High(Eff_Commands) do
      if hash=Str_Hash(Eff_Commands[i]) then
      begin
        Tmp_Obj:=Eff_Types[i].Create(Eff_Names[i]);
        Effects.Add(Tmp_Obj);
        Tmp_Obj.Load(f);
        break;
      end;

    if hash=Str_Hash(HARM_COM) then
    begin
      Readln(f,i);
      Harmonizers[i].Load(f,i);
    end;

    if hash=Str_Hash(SET_CUST_WAVE_COM) then
    begin
      readln(f,tmp);
      Custom_Wave.Load(f);
    end

  end;
                      // Закончили
  CloseFile(f);

  if (Sample_Speed>0) and
  (Sample_Tempo>0) and (Sample_Note_Len>0) then // Все значения корректны
  begin                                         // У нас есть bpm len
    Form1.Sample_Speed_Input.Position:=Sample_Speed;
    Form1.Sample_Tempo_Input.Position:=Sample_Tempo;
    Form1.Sample_Note_Len_Input.Position:=Sample_Note_Len;
    Form1.Sample_Speed_Show.Caption:=Num_To_Str(Sample_Speed);
    Form1.Sample_Tempo_Show.Caption:=Num_To_Str(Sample_Tempo);
    Form1.Sample_Note_Len_Show.Caption:=Num_To_Str(Sample_Note_Len);
  end
  else
  begin                                         // У нас нету bpm len
    Form1.Sample_Speed_Input.Position:=0;
    Form1.Sample_Tempo_Input.Position:=0;
    Form1.Sample_Note_Len_Input.Position:=0;
    Form1.Sample_Speed_Show.Caption:='';
    Form1.Sample_Tempo_Show.Caption:='';
    Form1.Sample_Note_Len_Show.Caption:='';
  end;
end;

procedure Do_Load_Script;
begin
  with Form1 do
  begin
    OpenDialog1.Filter := 'Script files|*.SCR';
    OpenDialog1.DefaultExt := 'SCR';
    OpenDialog1.Options := [ofFileMustExist];

    if not OpenDialog1.Execute then exit;     // Не хотят грузиться

    Load_Script(OpenDialog1.Filename);
    Form1.Caption:=SYNTH_NAME+'   '+OpenDialog1.Filename;
  end;
end;

procedure Do_Play(Loop_Flg: boolean);
begin
  Save_Script(TMP_SCRIPT_NAME);
  Stop_Play_Wav;
  if Gen_Sample(TMP_SCRIPT_NAME, TMP_SAMPLE_NAME) then
  begin
    Play_Wav(TMP_SAMPLE_NAME, Loop_Flg);
    Sample1.Draw_Wav_Data(Form1.Sample_View.Canvas);
  end;
{$IFDEF DELETE_TMP_SCRIPT}
  DeleteFile(TMP_SCRIPT_NAME);
{$ENDIF}
end;


procedure Toggle_Note_Sustain;
var
  Note_Name: string;
  Note_End_Letter: char;
begin
  with Form1 do
  begin
    if (Sequencer_Notes.Items.Count>0) and (Sequencer_Notes.ItemIndex>-1) then
      begin // Ежели ноты есть и какая-нить из них выделена, то все хорошо
        Note_Name:=Sequencer_Notes.Items.Strings[Sequencer_Notes.ItemIndex];
        if Note_Name=EMPTY_NOTE_NAME then exit;  // Пустая нота
        Note_End_Letter:=Note_Name[Length(Note_Name)];

        if Note_End_Letter=NOTE_PITCH_SYM then
            Note_Name[Length(Note_Name)]:=NOTE_SUS_PITCH_SYM;
        if Note_End_Letter=' ' then
            Note_Name[Length(Note_Name)]:=NOTE_SUSTAIN_SYM;
        if Note_End_Letter=NOTE_SUSTAIN_SYM then
            Note_Name[Length(Note_Name)]:=' ';
        if Note_End_Letter=NOTE_SUS_PITCH_SYM then
            Note_Name[Length(Note_Name)]:=NOTE_PITCH_SYM;

        Sequencer_Notes.Items.Strings[Sequencer_Notes.ItemIndex]:=Note_Name;
    end;
    end;
end;


procedure Toggle_Note_Pitch;
var
  Note_Name: string;
  Note_End_Letter: char;
begin
  with Form1 do
  begin
  if (Sequencer_Notes.Items.Count>0) and (Sequencer_Notes.ItemIndex>-1) then
    begin // Ежели ноты есть и какая-нить из них выделена, то все хорошо
      Note_Name:=Sequencer_Notes.Items.Strings[Sequencer_Notes.ItemIndex];
      if Note_Name=EMPTY_NOTE_NAME then exit;  // Пустая нота
      Note_End_Letter:=Note_Name[Length(Note_Name)];

      if Note_End_Letter=NOTE_SUS_PITCH_SYM then
          Note_Name[Length(Note_Name)]:=NOTE_SUSTAIN_SYM;
      if Note_End_Letter=NOTE_PITCH_SYM then
          Note_Name[Length(Note_Name)]:=' ';
      if Note_End_Letter=NOTE_SUSTAIN_SYM then
          Note_Name[Length(Note_Name)]:=NOTE_SUS_PITCH_SYM;
      if Note_End_Letter=' ' then
          Note_Name[Length(Note_Name)]:=NOTE_PITCH_SYM;
      Sequencer_Notes.Items.Strings[Sequencer_Notes.ItemIndex]:=Note_Name;
    end;
    end;
end;

procedure Update_Sequencer;
begin
  with Form1 do
  begin
    with Sequencers[0] do
    begin
      Notes_Num:=Sequencer_Notes.Items.Count;
      if Sequencer_Notes.Items.Count>0 then
        begin      // Ежели ноты есть, копируем их в секвенсор
          for i:=0 to Sequencer_Notes.Items.Count-1 do
            Notes[i]:=Sequencer_Notes.Items.Strings[i];
        end;
    end;
  end;
end;

procedure Read_Harmonizer(Harmonizer: THarmonizer;
                       Harm_Scrolls: array of TScrollBar);
var
  i: integer;
begin
  for i:=0 to HARMONICS_NUM-1 do
    Harm_Scrolls[i].Position:=100-Round(Harmonizer.Harmonics_Mix_Coefs[i]*100);
  Form1.Harm_Enable_Button.Checked:=Harmonizer.Enabled;
end;

procedure Write_Harmonizer(Harmonizer: THarmonizer;
                       Harm_Scrolls: array of TScrollBar);
var
  i: integer;
begin
  for i:=0 to HARMONICS_NUM-1 do
      Harmonizer.Harmonics_Mix_Coefs[i]:=(100-Harm_Scrolls[i].Position)/100;
  Harmonizer.Enabled:=Form1.Harm_Enable_Button.Checked;      
end;

