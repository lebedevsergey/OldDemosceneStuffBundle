// Copyright 2002 Sergey Lebedev
// Licensed under the Apache License, Version 2.0

// synth GUI

const
  MARKERS_SIZE = 8;

type
  TMarker=class(TLabel)
    Prev_Marker: TMarker;
    Next_Marker: TMarker;
    Marker_Scale: integer;
    procedure Marker_Del(Sender: TObject; Button:
                             TMouseButton; Shift:
                             TShiftState; X, Y: Integer);
    procedure Rescale(New_Scale: integer);
    constructor Create(AOwner: TComponent ;X,Y, Scale: integer);
    destructor  Destroy; override;
  end;

var
  First_Marker, Last_Marker: TMarker;
  Center_Y, Size_X, Size_Y :integer;


constructor TMarker.Create(AOwner: TComponent; X,Y, Scale: integer);
var
  MyRect: TRect;
begin
  inherited Create(AOwner);
  MyRect := BoundsRect;
  MyRect.Left:=X;
  MyRect.Top:=Y;
  BoundsRect:= MyRect;
  Color:=clGreen;
  Height:=MARKERS_SIZE;
  Width:=MARKERS_SIZE;
  DragMode:=dmAutomatic;
  Parent:=(AOwner as TWinControl);
  inc(Markers_Num);
  OnMouseDown:=Marker_Del;
  Marker_Scale:=Scale;
end;

destructor TMarker.Destroy;
begin
  Parent:=nil;
  inherited Destroy;
end;

procedure TMarker.Rescale(New_Scale: integer);
var
  Tmp_Y: integer;
begin
  Tmp_Y:=Top-Center_Y;
  Tmp_Y:=Round(Tmp_Y*Marker_Scale/New_Scale);
  Top:=Tmp_Y+Center_Y;
  Marker_Scale:=New_Scale;
end;

procedure Redraw_Envelope(Canvas: TCanvas);
var
  i: integer;
  Area_X_Size, Area_Y_Size : integer;
  Area_Start_X, Area_Center_Y, Area_End_X : integer;
  Area_Start_Y, Area_End_Y : integer;
  Grid_Subdiv_Start_X, Grid_Subdiv_X_Adder: integer;
  X1,Y1,X2,Y2 :integer;
  Sample_Y  :integer;
  Cur_X     :real;
  Pos_Max_Sample_Y, Neg_Max_Sample_Y :integer;
  Cur_Marker_Ptr, Next_Marker_Ptr: TMarker;
begin
  Canvas.Brush.Color := clBlack;
  Canvas.FillRect(Canvas.ClipRect);    // Очищаем экран

  if (First_Marker=nil) or (Last_Marker=nil) then exit;
    with Canvas.ClipRect do
    begin
      Area_X_Size := Right-Left;
      Area_Y_Size := Bottom-Top;
      Area_Start_X := Left;
      Area_End_X := Right;
      Area_Start_Y := Top;
      Area_End_Y := Bottom;      
      Area_Center_Y := (Bottom-Top) div 2;
                                              // Рисуем ось координат
      Canvas.Pen.Color := clSilver;
      Canvas.MoveTo(Area_Start_X,Area_Center_Y);
      Canvas.LineTo(Area_End_X,Area_Center_Y);
                                              // Рисуем сетку
      if Grid_Subdiv_Num>1 then
      begin
        Grid_Subdiv_X_Adder:=Area_X_Size div Grid_Subdiv_Num;
        Grid_Subdiv_Start_X:=Area_Start_X;
        for i:=1 to Grid_Subdiv_Num do
        begin
          Grid_Subdiv_Start_X:=Grid_Subdiv_Start_X+Grid_Subdiv_X_Adder;
          Canvas.MoveTo(Grid_Subdiv_Start_X,Area_Start_Y);
          Canvas.LineTo(Grid_Subdiv_Start_X,Area_End_Y);
        end;
      end;

      Canvas.Pen.Color := clGreen;
                                              // Рисуем огибающую
      Cur_Marker_Ptr:=First_Marker;
      while Cur_Marker_Ptr.Next_Marker<>nil do
      begin
        Cur_Marker_Ptr.RePaint;               // Перерисовываем маркеры
        Next_Marker_Ptr:=Cur_Marker_Ptr.Next_Marker;
        X1:=Cur_Marker_Ptr.BoundsRect.Right;
        Y1:=Cur_Marker_Ptr.BoundsRect.Top;
        X2:=Next_Marker_Ptr.BoundsRect.Left;
        Y2:=Next_Marker_Ptr.BoundsRect.Top;

        Canvas.MoveTo(X1,Y1);
        Canvas.LineTo(X2,Y2);
        Cur_Marker_Ptr:=Next_Marker_Ptr;
      end;
      Next_Marker_Ptr.RePaint;              // Перерисовываем
                                            // последний маркер
    end;
end;

procedure Insert_Marker(AOwner: TComponent; X,Y: integer);
var
  X1,X2 :integer;
  New_Marker: TMarker;
  Cur_Marker_Ptr, Next_Marker_Ptr: TMarker;
begin                                  // Ищем куда его вставить ;)
  Cur_Marker_Ptr:=First_Marker;
  while true do
  begin
    if Cur_Marker_Ptr.Next_Marker=nil then
      exit;                            // Не нашли :-(((
    Next_Marker_Ptr:=Cur_Marker_Ptr.Next_Marker;
    X1:=Cur_Marker_Ptr.BoundsRect.Right;
    X2:=Next_Marker_Ptr.BoundsRect.Left;
    if (X>X1) and (X<X2) then
      break;                           // Нашли
    Cur_Marker_Ptr:=Next_Marker_Ptr;
  end;
  New_Marker:=TMarker.Create(AOwner,X,Y,Scale_Y);
  New_Marker.Prev_Marker:=Cur_Marker_Ptr;
  New_Marker.Next_Marker:=Next_Marker_Ptr;
  Cur_Marker_Ptr.Next_Marker:=New_Marker;
  Next_Marker_Ptr.Prev_Marker:=New_Marker;
end;

procedure Delete_Marker(Marker: TMarker);
var
  Prev_Marker_, Next_Marker_: TMarker;
begin
  if (Marker<>nil) and (Marker.Prev_Marker<>nil) and
    (Marker.Next_Marker<>nil) then
  begin
    Prev_Marker_:=Marker.Prev_Marker;
    Next_Marker_:=Marker.Next_Marker;
    Prev_Marker_.Next_Marker:=Next_Marker_;
    Next_Marker_.Prev_Marker:=Prev_Marker_;
    dec(Markers_Num);
    Marker.Free;
    Marker:=nil;
  end;
end;

procedure Get_Markers_Values;
var
  Cur_Marker_Ptr: TMarker;
  i: integer;
begin

  if Markers_Num>0 then
  begin
                        // Складываем все обратно
    Cur_Marker_Ptr:=First_Marker;
    i:=1;
    while Cur_Marker_Ptr<>nil do
      begin
        Tmp_Env_Nodes_Values[i]:=Cur_Marker_Ptr.BoundsRect.Top;
        Tmp_Env_Nodes_X_Coords[i]:=Cur_Marker_Ptr.BoundsRect.Left;
        inc(i);
        Cur_Marker_Ptr:=Cur_Marker_Ptr.Next_Marker;
      end;
  end;
end;


procedure TMarker.Marker_Del(Sender: TObject; Button:
                             TMouseButton; Shift:
                             TShiftState; X, Y: Integer);
begin
  if Button=mbRight then
  begin
    Delete_Marker((Sender as TMarker));
    Env_Editor.Refresh;
  end
end;

procedure Rescale_Markers;
var
  Cur_Marker_Ptr, Next_Marker_Ptr: TMarker;
begin
  Cur_Marker_Ptr:=First_Marker;
  while Cur_Marker_Ptr<>nil do
  begin
    Cur_Marker_Ptr.Rescale(Scale_Y);               // rescale
    Cur_Marker_Ptr:=Cur_Marker_Ptr.Next_Marker;
  end;
end;
