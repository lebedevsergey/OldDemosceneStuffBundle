// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI main module

program SYNT_NEW;

uses
  Forms,
  Unit2 in 'Unit2.pas' {Env_Editor},
  Unit1 in 'Unit1.pas' {Form1};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(TEnv_Editor, Env_Editor);
  Application.Run;
end.
