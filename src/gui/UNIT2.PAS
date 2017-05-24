// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI window 2

unit Unit2;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, StdCtrls, ExtCtrls, MMSystem;

const
  SPAM_ENVELOPE_MAX_NODES_NUM = 30;
  INC_SCALE_KEY = '-';
  DEC_SCALE_KEY = '+';
  MIN_SCALE_Y = 1;
  MAX_SCALE_Y = 10;

type
  TEnv_Editor = class(TForm)
    procedure FormDragOver(Sender, Source: TObject; X, Y: Integer;
      State: TDragState; var Accept: Boolean);
    procedure FormPaint(Sender: TObject);
    procedure FormDblClick(Sender: TObject);
    procedure FormMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure FormShow(Sender: TObject);
    procedure FormHide(Sender: TObject);
    procedure FormKeyPress(Sender: TObject; var Key: Char);
  private
    { Private declarations }
  public
    { Public declarations }
  procedure Edit_Envelope(var Nodes_Num: integer;
                        var Nodes_Values: array of real;
                        var Nodes_X_Coords: array of real;
                        Grid_Subdiv_Num_: integer;
                        Env_Editor_Caption: string);
  end;


var
  Tmp_Env_Nodes_Values, Tmp_Env_Nodes_X_Coords:
           array[1..SPAM_ENVELOPE_MAX_NODES_NUM] of integer;
  Markers_Num, Tmp_Env_Nodes_Num: integer;
  Env_Editor: TEnv_Editor;
  Cur_Mouse_X, Cur_Mouse_Y: integer;
  Grid_Subdiv_Num: integer;
  Scale_Y: integer;

implementation

{$R *.DFM}

{$I env_edit.pas}

procedure TEnv_Editor.Edit_Envelope(var Nodes_Num: integer;
                        var Nodes_Values: array of real;
                        var Nodes_X_Coords: array of real;
                        Grid_Subdiv_Num_: integer;
                        Env_Editor_Caption: string);
var
  i: integer;
  Max_Abs_Val: real;

begin
  Scale_Y:=1;
  Left:=100;
  Top:=10;
  Height:=300;
  Width:=435;
  ClientHeight:=Height-30;
  ClientWidth:=Width-30;

  Center_Y:=ClientHeight div 2;
  Size_X:=ClientWidth-MARKERS_SIZE;
  Size_Y:=ClientHeight-MARKERS_SIZE;
  Grid_Subdiv_Num:=Grid_Subdiv_Num_;
  Caption:=Env_Editor_Caption;

  Tmp_Env_Nodes_Num:=Nodes_Num;
  if Nodes_Num>0 then
  begin
    Max_Abs_Val:=1;
    for i:=1 to Nodes_Num do   // Определяем удобный масштаб
      if Abs(Nodes_Values[i-1])>Max_Abs_Val then
        Max_Abs_Val:=Abs(Nodes_Values[i-1]);
    Scale_Y:=(Round(Max_Abs_Val+1));
    if (Scale_Y>MAX_SCALE_Y) then
      Scale_Y:=MAX_SCALE_Y;
      
    for i:=1 to Nodes_Num do
    begin                                     // Почему-то Delphi
           // при передаче массива, считает, что он будет
           // индексироваться с 0 :-()
      Tmp_Env_Nodes_Values[i]:=round(-Nodes_Values[i-1]*Center_Y/Scale_Y)+Center_Y;
      Tmp_Env_Nodes_X_Coords[i]:=round(Nodes_X_Coords[i-1]*Size_X);
    end;
  end;
  ShowModal;

  Nodes_Num:=Markers_Num;      // Пихаем все обратно
  if Nodes_Num>0 then
    for i:=1 to Nodes_Num do
    begin
      Nodes_Values[i-1]:=(Center_Y-Tmp_Env_Nodes_Values[i])/Center_Y*Scale_Y;
      Nodes_X_Coords[i-1]:=Tmp_Env_Nodes_X_Coords[i]/Size_X;
    end;

end;

procedure TEnv_Editor.FormDragOver(Sender, Source: TObject; X, Y: Integer;
  State: TDragState; var Accept: Boolean);
begin
  Accept:=True;
                                // Таcкаем маркер
    with Source as TMarker do
  begin                          // Ежели это ни первый ни последний маркер
    if (Source=First_Marker) then //(они должны быть строго по краям)
      SetBounds(0,Y,Width,Height)
    else
      if (Source=Last_Marker) then
        SetBounds((Sender as TEnv_Editor).ClientWidth-MARKERS_SIZE,Y,Width,Height)
      else
        begin
                // Проверим, что мы не выкатились за пределы окна
          if Y<0 then Y:=0;
          if Y>(Sender as TEnv_Editor).ClientHeight-MARKERS_SIZE then
            Y:=(Sender as TEnv_Editor).ClientHeight-MARKERS_SIZE;
               // Проверим, чтобы маркер не зашкалил за соседние
          if (Source as TMarker).Prev_Marker.BoundsRect.Right>X then
            X:=(Source as TMarker).Prev_Marker.BoundsRect.Right;
          if (Source as TMarker).Next_Marker.BoundsRect.Left<X then
            X:=(Source as TMarker).Next_Marker.BoundsRect.Left;

          SetBounds(X,Y,Width,Height);
        end;
  end;
  Redraw_Envelope(Canvas);
end;

procedure TEnv_Editor.FormPaint(Sender: TObject);
begin
  Redraw_Envelope(Self.Canvas);
end;

procedure TEnv_Editor.FormDblClick(Sender: TObject);
begin
  Insert_Marker(Self, Cur_Mouse_X,Cur_Mouse_Y);
  Redraw_Envelope(Self.Canvas);
end;

procedure TEnv_Editor.FormMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  Cur_Mouse_X:=X;
  Cur_Mouse_Y:=Y;
  inherited;
end;

procedure TEnv_Editor.FormShow(Sender: TObject);
var
  Center_Y: integer;
  Cur_Marker, Next_Marker_: TMarker;
  i: integer;
begin

  if Tmp_Env_Nodes_Num>0 then
  begin                             // Узлы уже есть
    Markers_Num:=0;
    First_Marker:=TMarker.Create(Env_Editor,
                  Tmp_Env_Nodes_X_Coords[1],
                  Tmp_Env_Nodes_Values[1],Scale_Y);
    Cur_Marker:=First_Marker;

    for i:=2 to Tmp_Env_Nodes_Num do
      begin
        Next_Marker_:=TMarker.Create(Env_Editor,
                      Tmp_Env_Nodes_X_Coords[i],
                      Tmp_Env_Nodes_Values[i],Scale_Y);
        Cur_Marker.Next_Marker:=Next_Marker_;
        Next_Marker_.Prev_Marker:=Cur_Marker;
        Cur_Marker:=Next_Marker_;
      end;
    Last_Marker:=Next_Marker_;
  end
  else
  begin                           //  Нет ни одного узла
    Center_Y:=ClientHeight div 2;   // Начинаем все сначала
    Markers_Num:=0;

    First_Marker:=TMarker.Create(Env_Editor,
                           0,Center_Y,Scale_Y);
    Last_Marker:=TMarker.Create(Env_Editor,
                ClientWidth-MARKERS_SIZE,Center_Y,Scale_Y);

    First_Marker.Next_Marker:=Last_Marker;
    Last_Marker.Prev_Marker:=First_Marker;
  end;
  Redraw_Envelope(Canvas);
end;

procedure TEnv_Editor.FormHide(Sender: TObject);
var
  Cur_Marker_Ptr,Next_Marker_Ptr: TMarker;
begin
  Get_Markers_Values;
                                  // Грохаем все маркеры
  Cur_Marker_Ptr:=First_Marker;
  while Cur_Marker_Ptr<>nil do
  begin
    Next_Marker_Ptr:=Cur_Marker_Ptr.Next_Marker;
    Cur_Marker_Ptr.Free;
    Cur_Marker_Ptr:=Next_Marker_Ptr;
  end;
end;

procedure TEnv_Editor.FormKeyPress(Sender: TObject; var Key: Char);
begin
  if (Key=INC_SCALE_KEY) and (Scale_Y<MAX_SCALE_Y) then
    Scale_Y:=Scale_Y+1;
  if (Key=DEC_SCALE_KEY) and (Scale_Y>MIN_SCALE_Y) then
    Scale_Y:=Scale_Y-1;

  Rescale_Markers;
  Redraw_Envelope(Canvas);

  Key:=#0;             // Обработали
end;

end.
