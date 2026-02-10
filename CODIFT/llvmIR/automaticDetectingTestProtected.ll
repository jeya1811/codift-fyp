; ModuleID = 'llvmIR/automaticDetectingTest.ll'
source_filename = "src/code/automaticDetecting_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [31 x i8] c"=== Direct Security Test ===\0A\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"TEST 1: Clean data\0A\00", align 1
@.str.2 = private unnamed_addr constant [11 x i8] c"  Tag: %u\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"\0ATEST 2: Tainted data\0A\00", align 1
@.str.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @access(i32 noundef %0) #0 {
  %prop_tmp5 = alloca i32, align 4
  %prop_tmp4 = alloca i32, align 4
  %prop_tmp3 = alloca i32, align 4
  %prop_tmp2 = alloca i32, align 4
  %prop_tmp1 = alloca i32, align 4
  %prop_tmp = alloca i32, align 4
  store i32 %0, ptr %prop_tmp, align 4
  %2 = alloca i32, align 4
  %3 = call i8 @ramReadFunc(ptr %prop_tmp)
  call void @ramWriteFunc(ptr %2, i8 %3)
  store i32 %0, ptr %2, align 4
  %4 = call i8 @ramReadFunc(ptr %2)
  call void @ramWriteFunc(ptr %prop_tmp1, i8 %4)
  %5 = load i32, ptr %2, align 4
  store i32 %5, ptr %prop_tmp2, align 4
  store i32 %5, ptr %prop_tmp1, align 4
  %6 = call i8 @ramReadFunc(ptr %prop_tmp2)
  store i32 1, ptr %prop_tmp3, align 4
  %7 = call i8 @ramReadFunc(ptr %prop_tmp3)
  %8 = or i8 %6, %7
  call void @ramWriteFunc(ptr %prop_tmp4, i8 %8)
  %9 = add nsw i32 %5, 1
  store i32 %9, ptr %prop_tmp5, align 4
  store i32 %9, ptr %prop_tmp4, align 4
  %10 = call i8 @ramReadFunc(ptr %prop_tmp5)
  call void @ramWriteFunc(ptr %2, i8 %10)
  store i32 %9, ptr %2, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %prop_tmp9 = alloca i32, align 4
  %prop_tmp8 = alloca i32, align 4
  %prop_tmp7 = alloca i32, align 4
  %prop_tmp6 = alloca i32, align 4
  %prop_tmp5 = alloca i32, align 4
  %prop_tmp4 = alloca i32, align 4
  %prop_tmp3 = alloca i32, align 4
  %prop_tmp2 = alloca i32, align 4
  %prop_tmp1 = alloca i32, align 4
  %prop_tmp = alloca i32, align 4
  %chk_tmp = alloca i32, align 4
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  store i32 0, ptr %prop_tmp, align 4
  %6 = call i8 @ramReadFunc(ptr %prop_tmp)
  call void @ramWriteFunc(ptr %1, i8 %6)
  store i32 0, ptr %1, align 4
  call void @codift_init()
  %7 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  store i32 42, ptr %prop_tmp1, align 4
  %9 = call i8 @ramReadFunc(ptr %prop_tmp1)
  call void @ramWriteFunc(ptr %2, i8 %9)
  store i32 42, ptr %2, align 4
  %10 = call i32 @ramReadFunc(ptr noundef %2)
  store i32 %10, ptr %prop_tmp2, align 4
  %11 = call i8 @ramReadFunc(ptr %prop_tmp2)
  call void @ramWriteFunc(ptr %3, i8 %11)
  store i32 %10, ptr %3, align 4
  %12 = call i8 @ramReadFunc(ptr %3)
  call void @ramWriteFunc(ptr %prop_tmp3, i8 %12)
  %13 = load i32, ptr %3, align 4
  store i32 %13, ptr %prop_tmp3, align 4
  %14 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %13)
  %15 = call i8 @ramReadFunc(ptr %3)
  call void @ramWriteFunc(ptr %prop_tmp4, i8 %15)
  %16 = load i32, ptr %3, align 4
  store i32 %16, ptr %prop_tmp4, align 4
  call void @secExcFunc(i32 noundef %16)
  %17 = call i32 (ptr, ...) @printf(ptr noundef @.str.3)
  %18 = call i32 (ptr, ...) @__isoc99_scanf(ptr noundef @.str.4, ptr noundef %4)
  call void @ramWriteFunc(ptr @.str.4, i8 1)
  call void @ramWriteFunc(ptr %4, i8 1)
  %19 = call i8 @ramReadFunc(ptr %4)
  call void @ramWriteFunc(ptr %prop_tmp5, i8 %19)
  %20 = load i32, ptr %4, align 4
  store i32 %20, ptr %prop_tmp6, align 4
  store i32 %20, ptr %prop_tmp5, align 4
  %21 = call i8 @ramReadFunc(ptr %prop_tmp6)
  call void @ramWriteFunc(ptr %chk_tmp, i8 %21)
  store i32 %20, ptr %chk_tmp, align 4
  %22 = call i8 @ramReadFunc(ptr %chk_tmp)
  call void @secExcFunc(i8 %22)
  call void @access(i32 noundef %20)
  %23 = call i32 @ramReadFunc(ptr noundef %4)
  store i32 %23, ptr %prop_tmp7, align 4
  %24 = call i8 @ramReadFunc(ptr %prop_tmp7)
  call void @ramWriteFunc(ptr %5, i8 %24)
  store i32 %23, ptr %5, align 4
  %25 = call i8 @ramReadFunc(ptr %5)
  call void @ramWriteFunc(ptr %prop_tmp8, i8 %25)
  %26 = load i32, ptr %5, align 4
  store i32 %26, ptr %prop_tmp8, align 4
  %27 = call i32 (ptr, ...) @printf(ptr noundef @.str.2, i32 noundef %26)
  %28 = call i8 @ramReadFunc(ptr %5)
  call void @ramWriteFunc(ptr %prop_tmp9, i8 %28)
  %29 = load i32, ptr %5, align 4
  store i32 %29, ptr %prop_tmp9, align 4
  call void @secExcFunc(i32 noundef %29)
  ret i32 0
}

declare void @codift_init() #1

declare i32 @printf(ptr noundef, ...) #1

declare i32 @ramReadFunc(ptr noundef) #1

declare void @secExcFunc(i32 noundef) #1

declare i32 @__isoc99_scanf(ptr noundef, ...) #1

declare void @ramWriteFunc(ptr, i8)

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
