; ModuleID = 'test1.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

@.str = private unnamed_addr constant [28 x i8] c"area of the triangle is:%f\0A\00", align 1
@.str.1 = private unnamed_addr constant [22 x i8] c"factorial of %d = %d\0A\00", align 1

; Function Attrs: nounwind
define void @area(float %param) #0 {
entry:
  %param.addr = alloca float, align 4
  %base = alloca float, align 4
  %area = alloca float, align 4
  store float %param, float* %param.addr, align 4
  store float 0x4037570A40000000, float* %base, align 4
  %0 = load float, float* %base, align 4
  %1 = load float, float* %param.addr, align 4
  %mul = fmul float %0, %1
  %div = fdiv float %mul, 2.000000e+00
  store float %div, float* %area, align 4
  %2 = load float, float* %area, align 4
  %conv = fpext float %2 to double
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @.str, i32 0, i32 0), double %conv)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind
define void @factorial(i32 %param) #0 {
entry:
  %param.addr = alloca i32, align 4
  %accumulator = alloca i32, align 4
  store i32 %param, i32* %param.addr, align 4
  store i32 1, i32* %accumulator, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %0 = load i32, i32* %param.addr, align 4
  %cmp = icmp sgt i32 %0, 1
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, i32* %param.addr, align 4
  %dec = add nsw i32 %1, -1
  store i32 %dec, i32* %param.addr, align 4
  %2 = load i32, i32* %accumulator, align 4
  %mul = mul nsw i32 %2, %1
  store i32 %mul, i32* %accumulator, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %3 = load i32, i32* %param.addr, align 4
  %4 = load i32, i32* %accumulator, align 4
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @.str.1, i32 0, i32 0), i32 %3, i32 %4)
  ret void
}

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  call void @area(float 0x3FF3BE76C0000000)
  call void @factorial(i32 6)
  ret i32 0
}

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (trunk 257509)"}
