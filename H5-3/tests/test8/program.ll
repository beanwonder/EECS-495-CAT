; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [21 x i8] c"Values: %ld %ld %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define i8** @function_that_complicates_everything(i32 %argc, i8** %par1, i8** %par2) #0 {
  %1 = icmp sgt i32 %argc, 5
  br i1 %1, label %2, label %3

; <label>:2                                       ; preds = %0
  br label %4

; <label>:3                                       ; preds = %0
  br label %4

; <label>:4                                       ; preds = %3, %2
  %.0 = phi i8** [ %par1, %2 ], [ %par2, %3 ]
  ret i8** %.0
}

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %d1 = alloca i8*, align 8
  %d2 = alloca i8*, align 8
  %1 = call i8* @CAT_create_signed_value(i64 5)
  store i8* %1, i8** %d1, align 8
  %2 = call i8* @CAT_create_signed_value(i64 7)
  store i8* %2, i8** %d2, align 8
  %3 = call i8** @function_that_complicates_everything(i32 %argc, i8** %d1, i8** %d2)
  %4 = load i8*, i8** %3, align 8
  %5 = call i64 @CAT_get_signed_value(i8* %4)
  %6 = load i8*, i8** %d1, align 8
  %7 = call i64 @CAT_get_signed_value(i8* %6)
  %8 = load i8*, i8** %d2, align 8
  %9 = call i64 @CAT_get_signed_value(i8* %8)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @.str, i32 0, i32 0), i64 %5, i64 %7, i64 %9)
  ret i32 0
}

declare i8* @CAT_create_signed_value(i64) #1

declare i64 @CAT_get_signed_value(i8*) #1

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
