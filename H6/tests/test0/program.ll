; ModuleID = 'program.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [17 x i8] c"H1: \09X    = %ld\0A\00", align 1
@.str.1 = private unnamed_addr constant [17 x i8] c"H1: \09Ref1 = %ld\0A\00", align 1
@.str.2 = private unnamed_addr constant [17 x i8] c"H1: \09Ref2 = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = call noalias i8* @malloc(i64 8) #3
  %2 = bitcast i8* %1 to i8**
  %3 = call noalias i8* @malloc(i64 8) #3
  %4 = bitcast i8* %3 to i8**
  %5 = call i8* @CAT_create_signed_value(i64 8)
  store i8* %5, i8** %2, align 8
  %6 = load i8*, i8** %2, align 8
  store i8* %6, i8** %4, align 8
  %7 = call i64 @CAT_get_signed_value(i8* %5)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str, i32 0, i32 0), i64 %7)
  %9 = load i8*, i8** %2, align 8
  %10 = call i64 @CAT_get_signed_value(i8* %9)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.1, i32 0, i32 0), i64 %10)
  %12 = load i8*, i8** %4, align 8
  %13 = call i64 @CAT_get_signed_value(i8* %12)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @.str.2, i32 0, i32 0), i64 %13)
  call void @CAT_binary_add(i8* %5, i8* %5, i8* %5)
  %15 = bitcast i8** %2 to i8*
  call void @free(i8* %15) #3
  %16 = bitcast i8** %4 to i8*
  call void @free(i8* %16) #3
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

declare i8* @CAT_create_signed_value(i64) #2

declare i32 @printf(i8*, ...) #2

declare i64 @CAT_get_signed_value(i8*) #2

declare void @CAT_binary_add(i8*, i8*, i8*) #2

; Function Attrs: nounwind
declare void @free(i8*) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
