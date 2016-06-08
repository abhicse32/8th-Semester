; ModuleID = 'test3.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"a:%d, b:%d\0A\00", align 1

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 4
  %b = alloca i32, align 4
  store i32 12, i32* %a, align 4
  store i32* %a, i32** %p, align 4
  %0 = load i32*, i32** %p, align 4
  %1 = load i32, i32* %0, align 4
  %add = add nsw i32 %1, 12
  store i32 %add, i32* %b, align 4
  %2 = load i32*, i32** %p, align 4
  %incdec.ptr = getelementptr inbounds i32, i32* %2, i32 1
  store i32* %incdec.ptr, i32** %p, align 4
  %3 = load i32, i32* %2, align 4
  store i32* %b, i32** %p, align 4
  %4 = load i32*, i32** %p, align 4
  %5 = load i32, i32* %4, align 4
  %add1 = add nsw i32 %5, 23
  %6 = load i32*, i32** %p, align 4
  store i32 %add1, i32* %6, align 4
  %7 = load i32*, i32** %p, align 4
  %8 = load i32, i32* %7, align 4
  %9 = load i32, i32* %a, align 4
  %add2 = add nsw i32 %8, %9
  store i32 %add2, i32* %b, align 4
  %10 = load i32, i32* %a, align 4
  %11 = load i32, i32* %b, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @.str, i32 0, i32 0), i32 %10, i32 %11)
  ret i32 0
}

declare i32 @printf(i8*, ...) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (trunk 257509)"}
