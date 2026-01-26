; ModuleID = 'llvmIR/simple_test.ll'
source_filename = "src/code/simple_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [6 x i8] c"hello\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %codift_inject_temp2 = alloca i1, align 1
  %codift_inject_temp1 = alloca i32, align 4
  %codift_inject_temp = alloca i32, align 4
  %1 = alloca i32, align 4
  call void @ramWriteFunc(ptr %1, i32 0)
  %2 = alloca i32, align 4
  call void @ramWriteFunc(ptr %2, i32 0)
  call void @ramWriteFunc(ptr %1, i32 0)
  store i32 0, ptr %1, align 4
  call void @ramWriteFunc(ptr %2, i32 0)
  store i32 10, ptr %2, align 4
  %3 = call i32 @ramReadFunc(ptr %2)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %3)
  %4 = load i32, ptr %2, align 4
  store i32 %4, ptr %codift_inject_temp1, align 4
  %5 = call i32 @ramReadFunc(ptr %codift_inject_temp1)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %5)
  store i32 %4, ptr %codift_inject_temp, align 4
  %6 = icmp sgt i32 %4, 5
  store i1 %6, ptr %codift_inject_temp2, align 1
  %codift_temp = alloca i1, align 1
  call void @ramWriteFunc(ptr %codift_temp, i32 0)
  %7 = call i32 @ramReadFunc(ptr %codift_inject_temp2)
  call void @ramWriteFunc(ptr %codift_temp, i32 %7)
  store i1 %6, ptr %codift_temp, align 1
  %8 = call i32 @ramReadFunc(ptr %codift_temp)
  call void @secExcFunc(i32 %8)
  br i1 %6, label %9, label %12

9:                                                ; preds = %0
  %10 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %10)
  %11 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %12

12:                                               ; preds = %9, %0
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

declare i32 @ramReadFunc(ptr)

declare void @secExcFunc(i32)

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
