; ModuleID = 'program1.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [11 x i8] c"H1: Begin\0A\00", align 1
@.str.4 = private unnamed_addr constant [9 x i8] c"H1: End\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @CAT_execution() #0 {
  %d1 = alloca i8*, align 8
  %d2 = alloca i8*, align 8
  %d3 = alloca i8*, align 8
  %1 = call i8* @CAT_create_signed_value(i64 5)
  store i8* %1, i8** %d1, align 8
  %2 = load i8*, i8** %d1, align 8
  %3 = call i64 @CAT_get_signed_value(i8* %2)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %3)
  %5 = call i8* @CAT_create_signed_value(i64 8)
  store i8* %5, i8** %d2, align 8
  %6 = load i8*, i8** %d2, align 8
  %7 = call i64 @CAT_get_signed_value(i8* %6)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 %7)
  %9 = call i8* @CAT_create_signed_value(i64 0)
  store i8* %9, i8** %d3, align 8
  %10 = load i8*, i8** %d3, align 8
  %11 = load i8*, i8** %d1, align 8
  %12 = load i8*, i8** %d2, align 8
  call void @CAT_binary_add(i8* %10, i8* %11, i8* %12)
  %13 = load i8*, i8** %d3, align 8
  %14 = call i64 @CAT_get_signed_value(i8* %13)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.2, i32 0, i32 0), i64 %14)
  ret void
}

declare i8* @CAT_create_signed_value(i64) #1

declare i32 @printf(i8*, ...) #1

declare i64 @CAT_get_signed_value(i8*) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 8
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 8
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str.3, i32 0, i32 0))
  call void @CAT_execution()
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str.4, i32 0, i32 0))
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
