; ModuleID = 'llvmIR/automaticDetecting_test.ll'
source_filename = "src/code/automaticDetecting_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [31 x i8] c"=== Direct Security Test ===\0A\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"TEST 1: Clean data\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"  Tag: %u\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"\0ATEST 2: Tainted data\0A\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %codift_inject_temp7 = alloca i32, align 4
  %codift_inject_temp6 = alloca i32, align 4
  %codift_inject_temp5 = alloca i32, align 4
  %codift_inject_temp4 = alloca i32, align 4
  %codift_inject_temp3 = alloca i32, align 4
  %codift_inject_temp2 = alloca i32, align 4
  %codift_inject_temp1 = alloca i32, align 4
  %codift_inject_temp = alloca i32, align 4
  %1 = alloca i32, align 4
  call void @ramWriteFunc(ptr %1, i32 0)
  %2 = alloca i32, align 4
  call void @ramWriteFunc(ptr %2, i32 0)
  %3 = alloca i32, align 4
  call void @ramWriteFunc(ptr %3, i32 0)
  %4 = alloca i32, align 4
  call void @ramWriteFunc(ptr %4, i32 0)
  %5 = alloca i32, align 4
  call void @ramWriteFunc(ptr %5, i32 0)
  store i32 0, ptr %1, align 4
  %6 = call i32 @ramReadFunc(ptr @codift_init)
  call void @secExcFunc(i32 %6)
  call void @codift_init()
  %7 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %7)
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %9 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %9)
  %10 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  store i32 42, ptr %2, align 4
  %11 = call i32 @ramReadFunc(ptr @ramReadFunc)
  call void @secExcFunc(i32 %11)
  %12 = call i32 @ramReadFunc(ptr noundef %2)
  store i32 %12, ptr %3, align 4
  %13 = call i32 @ramReadFunc(ptr %3)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %13)
  %14 = load i32, ptr %3, align 4
  store i32 %14, ptr %codift_inject_temp1, align 4
  %15 = call i32 @ramReadFunc(ptr %codift_inject_temp1)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %15)
  store i32 %14, ptr %codift_inject_temp, align 4
  %16 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %16)
  %17 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %14)
  %18 = call i32 @ramReadFunc(ptr %3)
  call void @ramWriteFunc(ptr %codift_inject_temp2, i32 %18)
  %19 = load i32, ptr %3, align 4
  store i32 %19, ptr %codift_inject_temp3, align 4
  %20 = call i32 @ramReadFunc(ptr %codift_inject_temp3)
  call void @ramWriteFunc(ptr %codift_inject_temp2, i32 %20)
  store i32 %19, ptr %codift_inject_temp2, align 4
  %21 = call i32 @ramReadFunc(ptr @secExcFunc)
  call void @secExcFunc(i32 %21)
  call void @secExcFunc(i32 noundef %19)
  %22 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %22)
  %23 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  %24 = call i32 @ramReadFunc(ptr @__isoc99_scanf)
  call void @secExcFunc(i32 %24)
  %25 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str.4, ptr noundef %4)
  call void @ramWriteFunc(ptr @.str.4, i32 1)
  call void @ramWriteFunc(ptr %4, i32 1)
  %26 = call i32 @ramReadFunc(ptr @ramReadFunc)
  call void @secExcFunc(i32 %26)
  %27 = call i32 @ramReadFunc(ptr noundef %4)
  store i32 %27, ptr %5, align 4
  %28 = call i32 @ramReadFunc(ptr %5)
  call void @ramWriteFunc(ptr %codift_inject_temp4, i32 %28)
  %29 = load i32, ptr %5, align 4
  store i32 %29, ptr %codift_inject_temp5, align 4
  %30 = call i32 @ramReadFunc(ptr %codift_inject_temp5)
  call void @ramWriteFunc(ptr %codift_inject_temp4, i32 %30)
  store i32 %29, ptr %codift_inject_temp4, align 4
  %31 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %31)
  %32 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %29)
  %33 = call i32 @ramReadFunc(ptr %5)
  call void @ramWriteFunc(ptr %codift_inject_temp6, i32 %33)
  %34 = load i32, ptr %5, align 4
  store i32 %34, ptr %codift_inject_temp7, align 4
  %35 = call i32 @ramReadFunc(ptr %codift_inject_temp7)
  call void @ramWriteFunc(ptr %codift_inject_temp6, i32 %35)
  store i32 %34, ptr %codift_inject_temp6, align 4
  %36 = call i32 @ramReadFunc(ptr @secExcFunc)
  call void @secExcFunc(i32 %36)
  call void @secExcFunc(i32 noundef %34)
  ret i32 0
}

declare void @codift_init() #1

declare i32 @printf(ptr noundef, ...) #1

declare i32 @ramReadFunc(ptr noundef) #1

declare void @secExcFunc(i32 noundef) #1

declare i32 @__isoc99_scanf(ptr noundef, ...) #1

declare void @ramWriteFunc(ptr, i32)

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"Ubuntu clang version 15.0.7"}
