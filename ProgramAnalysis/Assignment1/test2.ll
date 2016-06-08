; ModuleID = 'test2.c'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"

@main.arr = private unnamed_addr constant [5 x i32] [i32 4, i32 -1, i32 5, i32 6, i32 -9], align 4
@.str = private unnamed_addr constant [9 x i8] c"sum: %d\0A\00", align 1

; Function Attrs: nounwind
define i32 @sumArray(i32* %arr, i32 %len) #0 {
entry:
  %arr.addr = alloca i32*, align 4
  %len.addr = alloca i32, align 4
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  store i32* %arr, i32** %arr.addr, align 4
  store i32 %len, i32* %len.addr, align 4
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %len.addr, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %2 = load i32, i32* %i, align 4
  %3 = load i32*, i32** %arr.addr, align 4
  %arrayidx = getelementptr inbounds i32, i32* %3, i32 %2
  %4 = load i32, i32* %arrayidx, align 4
  %5 = load i32, i32* %sum, align 4
  %add = add nsw i32 %5, %4
  store i32 %add, i32* %sum, align 4
  %6 = load i32, i32* %i, align 4
  %inc = add nsw i32 %6, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %7 = load i32, i32* %sum, align 4
  ret i32 %7
}

; Function Attrs: nounwind
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 4
  %arr = alloca [5 x i32], align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 4
  %0 = bitcast [5 x i32]* %arr to i8*
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %0, i8* bitcast ([5 x i32]* @main.arr to i8*), i32 20, i32 4, i1 false)
  %arraydecay = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  %call = call i32 @sumArray(i32* %arraydecay, i32 5)
  %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @.str, i32 0, i32 0), i32 %call)
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture, i8* nocapture readonly, i32, i32, i1) #1

declare i32 @printf(i8*, ...) #2

attributes #0 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { argmemonly nounwind }
attributes #2 = { "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+fxsr,+mmx,+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.8.0 (trunk 257509)"}
