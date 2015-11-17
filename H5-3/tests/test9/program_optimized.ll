; ModuleID = 'program_optimized.bc'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [24 x i8] c"H5: \09Value of d1 = %ld\0A\00", align 1

; Function Attrs: nounwind uwtable
define void @a_generic_C_function(i8* %d1, i32 %argc) #0 {
  %1 = icmp sgt i32 %argc, 10
  br i1 %1, label %2, label %4

; <label>:2                                       ; preds = %0
  %3 = call i8* @CAT_create_signed_value(i64 8)
  br label %4

; <label>:4                                       ; preds = %2, %0
  %.0 = phi i8* [ %3, %2 ], [ %d1, %0 ]
  %5 = call i64 @CAT_get_signed_value(i8* %.0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @.str, i32 0, i32 0), i64 %5)
  ret void
}

declare i8* @CAT_create_signed_value(i64) #1

declare i32 @printf(i8*, ...) #1

declare i64 @CAT_get_signed_value(i8*) #1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
  %1 = call i8* @CAT_create_signed_value(i64 5)
  call void @a_generic_C_function(i8* %1, i32 %argc)
  ret i32 0
}

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.0 (tags/RELEASE_370/final)"}
