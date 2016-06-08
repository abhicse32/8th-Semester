; ModuleID = 'test.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

@main.a = private unnamed_addr constant [4 x i32] [i32 1, i32 3, i32 4, i32 1], align 4

; Function Attrs: nounwind
define i32 @main() #0 {
entry:
  %retval = alloca i32, align 4
  %a = alloca [4 x i32], align 4
  %i = alloca i32, align 4
  %b = alloca [4 x i32], align 4
  store i32 0, i32* %retval, align 4
  %0 = bitcast [4 x i32]* %a to i8*
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %0, i8* bitcast ([4 x i32]* @main.a to i8*), i32 16, i32 4, i1 false)
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %1 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %1, 4
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32, i32* %i, align 4
  %arrayidx = getelementptr inbounds [4 x i32], [4 x i32]* %a, i32 0, i32 %2
  %3 = load i32, i32* %arrayidx, align 4
  %4 = load i32, i32* %i, align 4
  %arrayidx1 = getelementptr inbounds [4 x i32], [4 x i32]* %b, i32 0, i32 %4
  store i32 %3, i32* %arrayidx1, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %5 = load i32, i32* %i, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %6 = load i32, i32* %retval, align 4
  ret i32 %6
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture, i8* nocapture readonly, i32, i32, i1) #1

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (trunk 257509)"}
