; ModuleID = 'program_optimized.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [14 x i8] c"H1: \09Y = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [20 x i8] c"H1: \09Value 1 = %ld\0A\00", align 1
@.str.3 = private unnamed_addr constant [15 x i8] c"Result 2 = %d\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @another_execution(i32 %userInput, i8* %y) #0 {
  %1 = call i64 @CAT_get_signed_value(i8* %y)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @.str, i32 0, i32 0), i64 %1)
  call void @CAT_binary_add(i8* %y, i8* %y, i8* %y)
  %3 = call i64 @CAT_get_signed_value(i8* %y)
  %4 = trunc i64 %3 to i32
  ret i32 %4
}

declare i32 @printf(i8*, ...) #1

declare i64 @CAT_get_signed_value(i8*) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

; Function Attrs: nounwind uwtable
define void @CAT_execution(i32 %userInput, i8* %d1, i32 %userInput2) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 8)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 8)
  %3 = icmp sgt i32 %userInput, 10
  br i1 %3, label %4, label %6

; <label>:4                                       ; preds = %0
  %5 = call i8* @CAT_create_signed_value(i64 8)
  call void @CAT_binary_add(i8* %d1, i8* %5, i8* %5)
  br label %6

; <label>:6                                       ; preds = %4, %0
  %d2.0 = phi i8* [ %5, %4 ], [ %1, %0 ]
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 8)
  %8 = call i64 @CAT_get_signed_value(i8* %d1)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.2, i32 0, i32 0), i64 %8)
  %10 = icmp sgt i32 %userInput2, 0
  br i1 %10, label %11, label %13

; <label>:11                                      ; preds = %6
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str.1, i32 0, i32 0), i64 8)
  br label %13

; <label>:13                                      ; preds = %11, %6
  ret void
}

declare i8* @CAT_create_signed_value(i64) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 8)
  %2 = add nsw i32 %argc, 1
  call void @CAT_execution(i32 %argc, i8* %1, i32 %2)
  %3 = call i32 @another_execution(i32 %argc, i8* %1)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str.3, i32 0, i32 0), i32 %3)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
