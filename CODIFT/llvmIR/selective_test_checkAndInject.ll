; ModuleID = 'llvmIR/selective_test.ll'
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
  %codift_inject_temp33 = alloca i1, align 1
  %codift_inject_temp32 = alloca i32, align 4
  %codift_inject_temp31 = alloca i32, align 4
  %codift_inject_temp30 = alloca i32, align 4
  %codift_inject_temp29 = alloca i1, align 1
  %codift_inject_temp28 = alloca i32, align 4
  %codift_inject_temp27 = alloca i32, align 4
  %codift_inject_temp26 = alloca i32, align 4
  %codift_inject_temp25 = alloca i32, align 4
  %codift_inject_temp24 = alloca i32, align 4
  %codift_inject_temp23 = alloca i32, align 4
  %codift_inject_temp22 = alloca i32, align 4
  %codift_inject_temp21 = alloca i32, align 4
  %codift_inject_temp20 = alloca i32, align 4
  %codift_inject_temp19 = alloca i32, align 4
  %codift_inject_temp18 = alloca i32, align 4
  %codift_inject_temp16 = alloca i32, align 4
  %codift_inject_temp15 = alloca i32, align 4
  %codift_inject_temp14 = alloca i32, align 4
  %codift_inject_temp13 = alloca i32, align 4
  %codift_inject_temp12 = alloca i32, align 4
  %codift_inject_temp11 = alloca i32, align 4
  %codift_inject_temp10 = alloca i32, align 4
  %codift_inject_temp9 = alloca i32, align 4
  %codift_inject_temp8 = alloca i32, align 4
  %codift_inject_temp7 = alloca i32, align 4
  %codift_inject_temp6 = alloca i32, align 4
  %codift_inject_temp5 = alloca i32, align 4
  %codift_inject_temp4 = alloca i32, align 4
  %codift_inject_temp3 = alloca i32, align 4
  %codift_inject_temp2 = alloca i32, align 4
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
  %6 = alloca i32, align 4
  call void @ramWriteFunc(ptr %6, i32 0)
  %7 = alloca [5 x i32], align 16
  call void @ramWriteFunc(ptr %7, i32 0)
  %8 = alloca i32, align 4
  call void @ramWriteFunc(ptr %8, i32 0)
  %9 = alloca i32, align 4
  call void @ramWriteFunc(ptr %9, i32 0)
  call void @ramWriteFunc(ptr %1, i32 0)
  store i32 0, ptr %1, align 4
  call void @ramWriteFunc(ptr %2, i32 0)
  store i32 10, ptr %2, align 4
  call void @ramWriteFunc(ptr %3, i32 0)
  store i32 20, ptr %3, align 4
  %10 = call i32 @ramReadFunc(ptr %2)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %10)
  %11 = load i32, ptr %2, align 4
  store i32 %11, ptr %codift_inject_temp5, align 4
  store i32 %11, ptr %codift_inject_temp2, align 4
  %12 = call i32 @ramReadFunc(ptr %codift_inject_temp2)
  call void @ramWriteFunc(ptr %codift_inject_temp, i32 %12)
  store i32 %11, ptr %codift_inject_temp, align 4
  %13 = call i32 @ramReadFunc(ptr %3)
  call void @ramWriteFunc(ptr %codift_inject_temp3, i32 %13)
  %14 = load i32, ptr %3, align 4
  store i32 %14, ptr %codift_inject_temp6, align 4
  store i32 %14, ptr %codift_inject_temp4, align 4
  %15 = call i32 @ramReadFunc(ptr %codift_inject_temp4)
  call void @ramWriteFunc(ptr %codift_inject_temp3, i32 %15)
  store i32 %14, ptr %codift_inject_temp3, align 4
  %16 = call i32 @ramReadFunc(ptr %codift_inject_temp5)
  %17 = call i32 @ramReadFunc(ptr %codift_inject_temp6)
  %codift-merge = or i32 %16, %17
  call void @ramWriteFunc(ptr %codift_inject_temp7, i32 %codift-merge)
  %18 = add nsw i32 %11, %14
  store i32 %18, ptr %codift_inject_temp9, align 4
  store i32 %18, ptr %codift_inject_temp8, align 4
  %19 = call i32 @ramReadFunc(ptr %codift_inject_temp8)
  call void @ramWriteFunc(ptr %codift_inject_temp7, i32 %19)
  store i32 %18, ptr %codift_inject_temp7, align 4
  %20 = call i32 @ramReadFunc(ptr %codift_inject_temp9)
  call void @ramWriteFunc(ptr %4, i32 %20)
  store i32 %18, ptr %4, align 4
  %21 = call i32 @ramReadFunc(ptr @get_untrusted_input)
  call void @secExcFunc(i32 %21)
  %22 = call i32 @get_untrusted_input()
  store i32 %22, ptr %codift_inject_temp10, align 4
  %23 = call i32 @ramReadFunc(ptr %codift_inject_temp10)
  call void @ramWriteFunc(ptr %5, i32 %23)
  store i32 %22, ptr %5, align 4
  %24 = call i32 @ramReadFunc(ptr %4)
  call void @ramWriteFunc(ptr %codift_inject_temp11, i32 %24)
  %25 = load i32, ptr %4, align 4
  store i32 %25, ptr %codift_inject_temp15, align 4
  store i32 %25, ptr %codift_inject_temp12, align 4
  %26 = call i32 @ramReadFunc(ptr %codift_inject_temp12)
  call void @ramWriteFunc(ptr %codift_inject_temp11, i32 %26)
  store i32 %25, ptr %codift_inject_temp11, align 4
  %27 = call i32 @ramReadFunc(ptr %5)
  call void @ramWriteFunc(ptr %codift_inject_temp13, i32 %27)
  %28 = load i32, ptr %5, align 4
  store i32 %28, ptr %codift_inject_temp16, align 4
  store i32 %28, ptr %codift_inject_temp14, align 4
  %29 = call i32 @ramReadFunc(ptr %codift_inject_temp14)
  call void @ramWriteFunc(ptr %codift_inject_temp13, i32 %29)
  store i32 %28, ptr %codift_inject_temp13, align 4
  %30 = call i32 @ramReadFunc(ptr %codift_inject_temp15)
  %31 = call i32 @ramReadFunc(ptr %codift_inject_temp16)
  %codift-merge17 = or i32 %30, %31
  call void @ramWriteFunc(ptr %codift_inject_temp18, i32 %codift-merge17)
  %32 = add nsw i32 %25, %28
  store i32 %32, ptr %codift_inject_temp20, align 4
  store i32 %32, ptr %codift_inject_temp19, align 4
  %33 = call i32 @ramReadFunc(ptr %codift_inject_temp19)
  call void @ramWriteFunc(ptr %codift_inject_temp18, i32 %33)
  store i32 %32, ptr %codift_inject_temp18, align 4
  %34 = call i32 @ramReadFunc(ptr %codift_inject_temp20)
  call void @ramWriteFunc(ptr %6, i32 %34)
  store i32 %32, ptr %6, align 4
  %35 = call i32 @ramReadFunc(ptr %6)
  call void @ramWriteFunc(ptr %codift_inject_temp21, i32 %35)
  %36 = load i32, ptr %6, align 4
  store i32 %36, ptr %codift_inject_temp23, align 4
  store i32 %36, ptr %codift_inject_temp22, align 4
  %37 = call i32 @ramReadFunc(ptr %codift_inject_temp22)
  call void @ramWriteFunc(ptr %codift_inject_temp21, i32 %37)
  store i32 %36, ptr %codift_inject_temp21, align 4
  %38 = getelementptr inbounds [5 x i32], ptr %7, i64 0, i64 0
  %39 = call i32 @ramReadFunc(ptr %codift_inject_temp23)
  call void @ramWriteFunc(ptr %38, i32 %39)
  store i32 %36, ptr %38, align 16
  %40 = getelementptr inbounds [5 x i32], ptr %7, i64 0, i64 0
  %41 = call i32 @ramReadFunc(ptr %40)
  call void @ramWriteFunc(ptr %codift_inject_temp24, i32 %41)
  %42 = load i32, ptr %40, align 16
  store i32 %42, ptr %codift_inject_temp26, align 4
  store i32 %42, ptr %codift_inject_temp25, align 4
  %43 = call i32 @ramReadFunc(ptr %codift_inject_temp25)
  call void @ramWriteFunc(ptr %codift_inject_temp24, i32 %43)
  store i32 %42, ptr %codift_inject_temp24, align 4
  %44 = call i32 @ramReadFunc(ptr %codift_inject_temp26)
  call void @ramWriteFunc(ptr %8, i32 %44)
  store i32 %42, ptr %8, align 4
  %45 = call i32 @ramReadFunc(ptr %8)
  call void @ramWriteFunc(ptr %codift_inject_temp27, i32 %45)
  %46 = load i32, ptr %8, align 4
  store i32 %46, ptr %codift_inject_temp28, align 4
  %47 = call i32 @ramReadFunc(ptr %codift_inject_temp28)
  call void @ramWriteFunc(ptr %codift_inject_temp27, i32 %47)
  store i32 %46, ptr %codift_inject_temp27, align 4
  %48 = icmp sgt i32 %46, 100
  store i1 %48, ptr %codift_inject_temp29, align 1
  %codift_temp = alloca i1, align 1
  call void @ramWriteFunc(ptr %codift_temp, i32 0)
  %49 = call i32 @ramReadFunc(ptr %codift_inject_temp29)
  call void @ramWriteFunc(ptr %codift_temp, i32 %49)
  store i1 %48, ptr %codift_temp, align 1
  %50 = call i32 @ramReadFunc(ptr %codift_temp)
  call void @secExcFunc(i32 %50)
  br i1 %48, label %51, label %54

51:                                               ; preds = %0
  %52 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %52)
  %53 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %54

54:                                               ; preds = %51, %0
  %55 = call i32 @ramReadFunc(ptr @get_trusted_data)
  call void @secExcFunc(i32 %55)
  %56 = call i32 @get_trusted_data()
  store i32 %56, ptr %codift_inject_temp30, align 4
  %57 = call i32 @ramReadFunc(ptr %codift_inject_temp30)
  call void @ramWriteFunc(ptr %9, i32 %57)
  store i32 %56, ptr %9, align 4
  %58 = call i32 @ramReadFunc(ptr %9)
  call void @ramWriteFunc(ptr %codift_inject_temp31, i32 %58)
  %59 = load i32, ptr %9, align 4
  store i32 %59, ptr %codift_inject_temp32, align 4
  %60 = call i32 @ramReadFunc(ptr %codift_inject_temp32)
  call void @ramWriteFunc(ptr %codift_inject_temp31, i32 %60)
  store i32 %59, ptr %codift_inject_temp31, align 4
  %61 = icmp sgt i32 %59, 0
  store i1 %61, ptr %codift_inject_temp33, align 1
  %codift_temp1 = alloca i1, align 1
  call void @ramWriteFunc(ptr %codift_temp1, i32 0)
  %62 = call i32 @ramReadFunc(ptr %codift_inject_temp33)
  call void @ramWriteFunc(ptr %codift_temp1, i32 %62)
  store i1 %61, ptr %codift_temp1, align 1
  %63 = call i32 @ramReadFunc(ptr %codift_temp1)
  call void @secExcFunc(i32 %63)
  br i1 %61, label %64, label %67

64:                                               ; preds = %54
  %65 = call i32 @ramReadFunc(ptr @printf)
  call void @secExcFunc(i32 %65)
  %66 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  br label %67

67:                                               ; preds = %64, %54
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
