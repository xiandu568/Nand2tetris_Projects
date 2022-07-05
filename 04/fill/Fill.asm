// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

   // 16384 -24575 为屏幕显示内存 24576 键盘输入寄存器

   // 存储地址
   @24575
   D=A

   @R0
   M=D

   @SCREEN
   D=A
   @R1
   M=D
(LOOP)
   @24576
   D=M
   @FILL
   D;JGT

   @CLEAR
   0;JMP
(FILL)
   // 判断屏幕是否已满
   @R0
   D=M
   @R1
   D=D-M
   @LOOP
   D;JLT

   @R1
   D=M 

   A=M   //把地址推入A
   M=-1

   @R1
   M=D+1
   

   @LOOP
   0;JMP

(CLEAR)
    // 判断屏幕是否已经空
    @SCREEN
    D=A 
    @R1
    D=D-M   
    @LOOP
    D;JGT 

    @R1
    D=M 
    A=M
    M=0
    @R1
    M=D-1
    @LOOP
    0;JMP





   

