; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [20 x i8] c"H1: \09Value 2 = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [19 x i8] c"H1: \09Result = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @CAT_execution(i8* %d1) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 8)
  call void @CAT_binary_add(i8* %d1, i8* %1, i8* %1)
  %2 = call i64 @CAT_get_signed_value(i8* %1)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([20 x i8], [20 x i8]* @.str, i32 0, i32 0), i64 %2)
  %4 = call i8* @CAT_create_signed_value(i64 0)
  call void @CAT_binary_add(i8* %4, i8* %d1, i8* %1)
  %5 = call i64 @CAT_get_signed_value(i8* %4)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i64 %5)
  ret void
}

declare i8* @CAT_create_signed_value(i64) #1

declare void @CAT_binary_add(i8*, i8*, i8*) #1

declare i32 @printf(i8*, ...) #1

declare i64 @CAT_get_signed_value(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 5)
  call void @CAT_execution(i8* %1)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
