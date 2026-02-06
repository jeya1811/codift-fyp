; ModuleID = 'src/code/selective_test.c'
source_filename = "src/code/selective_test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [27 x i8] c"Access with tainted data!\0A\00", align 1
@.str.1 = private unnamed_addr constant [14 x i8] c"Clean access\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @get_untrusted_input() #0 {
  ret i32 200
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @get_trusted_data() #0 {
  ret i32 42
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca [5 x i32], align 16
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  store i32 10, ptr %2, align 4
  store i32 20, ptr %3, align 4
  %10 = load i32, ptr %2, align 4
  %11 = load i32, ptr %3, align 4
  %12 = add nsw i32 %10, %11
  store i32 %12, ptr %4, align 4
  %13 = call i32 @get_untrusted_input()
  store i32 %13, ptr %5, align 4
  %14 = load i32, ptr %4, align 4
  %15 = load i32, ptr %5, align 4
  %16 = add nsw i32 %14, %15
  store i32 %16, ptr %6, align 4
  %17 = load i32, ptr %6, align 4
  %18 = getelementptr inbounds [5 x i32], ptr %7, i64 0, i64 0
  store i32 %17, ptr %18, align 16
  %19 = getelementptr inbounds [5 x i32], ptr %7, i64 0, i64 0
  %20 = load i32, ptr %19, align 16
  store i32 %20, ptr %8, align 4
  %21 = load i32, ptr %8, align 4
  %22 = icmp sgt i32 %21, 100
  br i1 %22, label %23, label %25

23:                                               ; preds = %0
  %24 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %25

25:                                               ; preds = %23, %0
  %26 = call i32 @get_trusted_data()
  store i32 %26, ptr %9, align 4
  %27 = load i32, ptr %9, align 4
  %28 = icmp sgt i32 %27, 0
  br i1 %28, label %29, label %31

29:                                               ; preds = %25
  %30 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  br label %31

31:                                               ; preds = %29, %25
  ret i32 0
}

declare i32 @printf(ptr noundef, ...) #1

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
